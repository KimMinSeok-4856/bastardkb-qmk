using System;
using System.IO;
using System.Collections.Generic;
using System.Diagnostics;
using System.Management;
using System.Runtime.InteropServices;
using System.Security.Principal;
using System.Windows.Forms;
using Microsoft.Win32.SafeHandles;

namespace CharybdisFlasher {
    class Program {
        [DllImport("kernel32.dll", SetLastError = true, CharSet = CharSet.Auto)]
        static extern SafeFileHandle CreateFile(
            string lpFileName,
            uint dwDesiredAccess,
            uint dwShareMode,
            IntPtr lpSecurityAttributes,
            uint dwCreationDisposition,
            uint dwFlagsAndAttributes,
            IntPtr hTemplateFile);

        [DllImport("kernel32.dll", ExactSpelling = true, SetLastError = true)]
        static extern bool DeviceIoControl(
            SafeFileHandle hDevice,
            uint dwIoControlCode,
            IntPtr lpInBuffer,
            uint nInBufferSize,
            IntPtr lpOutBuffer,
            uint nOutBufferSize,
            out uint lpBytesReturned,
            IntPtr lpOverlapped);

        [DllImport("kernel32.dll", SetLastError = true)]
        static extern bool WriteFile(
            SafeFileHandle hFile,
            byte[] lpBuffer,
            uint nNumberOfBytesToWrite,
            out uint lpNumberOfBytesWritten,
            IntPtr lpOverlapped);

        [DllImport("kernel32.dll", SetLastError = true)]
        static extern bool SetFilePointerEx(
            SafeFileHandle hFile,
            long liDistanceToMove,
            out long lpNewFilePointer,
            uint dwMoveMethod);

        const uint GENERIC_READ = 0x80000000;
        const uint GENERIC_WRITE = 0x40000000;
        const uint FILE_SHARE_READ = 0x00000001;
        const uint FILE_SHARE_WRITE = 0x00000002;
        const uint OPEN_EXISTING = 3;
        const uint FSCTL_LOCK_VOLUME = 0x00090018;
        const uint FSCTL_DISMOUNT_VOLUME = 0x00090020;

        [STAThread]
        static void Main(string[] args) {
            try {
                Console.OutputEncoding = System.Text.Encoding.UTF8;
            } catch {}

            // 관리자 권한 확인 및 자동 승격 요청
            if (!IsAdministrator()) {
                try {
                    ProcessStartInfo proc = new ProcessStartInfo();
                    proc.UseShellExecute = true;
                    proc.WorkingDirectory = Environment.CurrentDirectory;
                    proc.FileName = Process.GetCurrentProcess().MainModule.FileName;
                    if (args != null && args.Length > 0) {
                        proc.Arguments = "\"" + string.Join("\" \"", args) + "\"";
                    }
                    proc.Verb = "runas";
                    Process.Start(proc);
                    return;
                } catch {
                    // 사용자가 UAC 거부 시 현재 세션에서 계속 시도
                }
            }

            Console.Title = "Charybdis Direct Firmware Flasher";
            Console.ForegroundColor = ConsoleColor.Cyan;
            Console.WriteLine("=================================================");
            Console.WriteLine("    Charybdis Mini RP2040 펌웨어 직접 플래셔     ");
            Console.WriteLine("=================================================");
            Console.ResetColor();

            // 1. UF2 펌웨어 파일 선택
            string uf2Path = SelectUf2File(args);
            if (string.IsNullOrEmpty(uf2Path) || !File.Exists(uf2Path)) {
                Console.WriteLine("\n선택된 파일이 없습니다. 프로그램을 종료합니다.");
                WaitForKey();
                return;
            }

            byte[] uf2Bytes = null;
            try {
                uf2Bytes = File.ReadAllBytes(uf2Path);
            } catch (Exception ex) {
                Console.ForegroundColor = ConsoleColor.Red;
                Console.WriteLine("[오류] 파일 읽기 실패: " + ex.Message);
                Console.ResetColor();
                WaitForKey();
                return;
            }

            Console.ForegroundColor = ConsoleColor.Green;
            Console.WriteLine("\n[선택 완료] {0}", Path.GetFileName(uf2Path));
            Console.WriteLine("    파일 크기: {0:N0} bytes ({1} blocks)", uf2Bytes.Length, uf2Bytes.Length / 512);
            Console.WriteLine("    전체 경로: {0}", uf2Path);
            Console.ResetColor();

            // 2. RPI-RP2 장치 검색 (연결될 때까지 재시도 가능)
            int diskNumber = -1;
            string driveLetter = null;

            while (true) {
                Console.WriteLine("\n[2] 키보드(RPI-RP2) 부트로더 장치 검색 중...");
                if (FindRp2Device(out diskNumber, out driveLetter)) {
                    Console.ForegroundColor = ConsoleColor.Green;
                    Console.WriteLine("    -> 연결 감지: 디스크 PhysicalDrive{0} (드라이브 {1})", 
                        diskNumber, driveLetter != null ? driveLetter : "N/A");
                    Console.ResetColor();
                    break;
                }

                Console.ForegroundColor = ConsoleColor.Yellow;
                Console.WriteLine("[안내] RPI-RP2 부트로더 장치가 감지되지 않았습니다.");
                Console.WriteLine("       1) 키보드의 BOOT(리셋) 버튼을 누른 채로 USB를 꽂아주세요.");
                Console.WriteLine("       2) 또는 이미 꽂혀있다면 리셋 버튼을 눌러 부트로더 모드로 진입하세요.");
                Console.ResetColor();
                Console.Write("\n준비 후 Enter를 누르면 다시 검색합니다 (Q 입력 시 종료): ");
                string retry = Console.ReadLine();
                if (retry != null && retry.Trim().Equals("Q", StringComparison.OrdinalIgnoreCase)) {
                    return;
                }
            }

            // 3. 물리 섹터 레벨 직접 플래싱
            Console.WriteLine("\n[3] 물리 섹터 레벨 직접 플래싱 시작...");
            string volPath = driveLetter != null ? @"\\.\" + driveLetter : null;
            string diskPath = @"\\.\PhysicalDrive" + diskNumber;

            SafeFileHandle hVol = null;
            uint bytesReturned;
            if (volPath != null) {
                hVol = CreateFile(volPath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, IntPtr.Zero, OPEN_EXISTING, 0, IntPtr.Zero);
                if (!hVol.IsInvalid) {
                    DeviceIoControl(hVol, FSCTL_LOCK_VOLUME, IntPtr.Zero, 0, IntPtr.Zero, 0, out bytesReturned, IntPtr.Zero);
                    DeviceIoControl(hVol, FSCTL_DISMOUNT_VOLUME, IntPtr.Zero, 0, IntPtr.Zero, 0, out bytesReturned, IntPtr.Zero);
                }
            }

            using (SafeFileHandle hDisk = CreateFile(diskPath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, IntPtr.Zero, OPEN_EXISTING, 0, IntPtr.Zero)) {
                if (hDisk.IsInvalid) {
                    int err = Marshal.GetLastWin32Error();
                    Console.ForegroundColor = ConsoleColor.Red;
                    Console.WriteLine("[오류] 물리 디스크 열기 실패 (오류 코드: {0})", err);
                    Console.WriteLine("       '관리자 권한으로 실행' 상태인지 확인해 주세요.");
                    Console.ResetColor();
                } else {
                    long newPos;
                    SetFilePointerEx(hDisk, 512 * 100, out newPos, 0); // sector 100

                    uint written = 0;
                    if (WriteFile(hDisk, uf2Bytes, (uint)uf2Bytes.Length, out written, IntPtr.Zero)) {
                        Console.ForegroundColor = ConsoleColor.Cyan;
                        Console.WriteLine("\n=================================================");
                        Console.WriteLine("  성공! 총 {0:N0} bytes (100%) 플래싱 완료!", written);
                        Console.WriteLine("  키보드가 새 펌웨어로 자동 재부팅됩니다.");
                        Console.WriteLine("=================================================");
                        Console.ResetColor();
                    } else {
                        int err = Marshal.GetLastWin32Error();
                        Console.ForegroundColor = ConsoleColor.Red;
                        Console.WriteLine("[오류] 쓰기 실패 (오류 코드: {0})", err);
                        Console.ResetColor();
                    }
                }
            }

            if (hVol != null && !hVol.IsInvalid) {
                hVol.Dispose();
            }

            Console.WriteLine("\n아무 키나 누르면 종료합니다 (3초 후 자동 종료)...");
            WaitForKeyOrTimeout(3000);
        }

        static bool IsAdministrator() {
            try {
                using (WindowsIdentity identity = WindowsIdentity.GetCurrent()) {
                    WindowsPrincipal principal = new WindowsPrincipal(identity);
                    return principal.IsInRole(WindowsBuiltInRole.Administrator);
                }
            } catch {
                return false;
            }
        }

        static string SelectUf2File(string[] args) {
            // 명령줄 인자 또는 드래그 앤 드롭으로 전달된 파일 확인
            if (args != null && args.Length > 0 && !string.IsNullOrEmpty(args[0])) {
                string directArg = args[0].Trim('"', '\'');
                if (File.Exists(directArg) && directArg.EndsWith(".uf2", StringComparison.OrdinalIgnoreCase)) {
                    Console.WriteLine("[안내] 전달된 펌웨어 파일 사용: " + Path.GetFileName(directArg));
                    return directArg;
                }
            }

            List<string> uf2Files = ScanForUf2Files();

            while (true) {
                Console.WriteLine("\n[1] 플래싱할 UF2 펌웨어 파일을 선택하세요:");
                Console.WriteLine("-----------------------------------------------------------------");

                if (uf2Files.Count > 0) {
                    for (int i = 0; i < uf2Files.Count; i++) {
                        FileInfo fi = new FileInfo(uf2Files[i]);
                        string tag = (i == 0) ? " ★ [최신 빌드 / 기본값]" : "";
                        Console.WriteLine(string.Format("  [{0}] {1}{2}", i + 1, fi.Name, tag));
                        Console.ForegroundColor = ConsoleColor.DarkGray;
                        Console.WriteLine(string.Format("      수정일: {0:yyyy-MM-dd HH:mm:ss} | 크기: {1:N0} bytes",
                            fi.LastWriteTime, fi.Length));
                        Console.WriteLine(string.Format("      위치: {0}", fi.DirectoryName));
                        Console.ResetColor();
                    }
                } else {
                    Console.ForegroundColor = ConsoleColor.Yellow;
                    Console.WriteLine("  (바탕화면 및 실행 폴더에서 .uf2 파일을 찾지 못했습니다)");
                    Console.ResetColor();
                }

                Console.WriteLine("  [F] 파일 탐색기 창 열기 (직접 파일 찾아보기)");
                Console.WriteLine("  [Q] 프로그램 종료");
                Console.WriteLine("-----------------------------------------------------------------");

                if (uf2Files.Count > 0) {
                    Console.Write(string.Format("번호 선택 [1~{0}, F] (Enter 누르면 1번 자동 선택): ", uf2Files.Count));
                } else {
                    Console.Write("선택 [F/Q]: ");
                }

                string input = Console.ReadLine();
                if (input != null) input = input.Trim();

                // 엔터 입력 시: 최신 파일 자동 선택
                if (string.IsNullOrEmpty(input)) {
                    if (uf2Files.Count > 0) {
                        return uf2Files[0];
                    }
                    continue;
                }

                if (input.Equals("Q", StringComparison.OrdinalIgnoreCase)) {
                    return null;
                }

                if (input.Equals("F", StringComparison.OrdinalIgnoreCase)) {
                    string picked = OpenFilePicker();
                    if (!string.IsNullOrEmpty(picked) && File.Exists(picked)) {
                        return picked;
                    }
                    Console.ForegroundColor = ConsoleColor.Yellow;
                    Console.WriteLine("\n파일 선택이 취소되었습니다. 다시 선택해 주세요.");
                    Console.ResetColor();
                    continue;
                }

                int choice;
                if (int.TryParse(input, out choice) && choice >= 1 && choice <= uf2Files.Count) {
                    return uf2Files[choice - 1];
                }

                // 파일 경로를 직접 입력/붙여넣기 한 경우 처리
                string manualPath = input.Trim('"', '\'');
                if (File.Exists(manualPath) && manualPath.EndsWith(".uf2", StringComparison.OrdinalIgnoreCase)) {
                    return Path.GetFullPath(manualPath);
                }

                Console.ForegroundColor = ConsoleColor.Yellow;
                Console.WriteLine("\n올바른 번호 또는 'F'를 입력해 주세요.");
                Console.ResetColor();
            }
        }

        static List<string> ScanForUf2Files() {
            List<string> result = new List<string>();
            HashSet<string> seen = new HashSet<string>(StringComparer.OrdinalIgnoreCase);

            string desktop = Environment.GetFolderPath(Environment.SpecialFolder.Desktop);
            string exeDir = AppDomain.CurrentDomain.BaseDirectory;
            string downloads = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.UserProfile), "Downloads");

            string[] searchDirs = new string[] { desktop, exeDir, downloads };

            foreach (string dir in searchDirs) {
                try {
                    if (!string.IsNullOrEmpty(dir) && Directory.Exists(dir)) {
                        string[] files = Directory.GetFiles(dir, "*.uf2", SearchOption.TopDirectoryOnly);
                        foreach (string f in files) {
                            string fullPath = Path.GetFullPath(f);
                            if (seen.Add(fullPath)) {
                                result.Add(fullPath);
                            }
                        }
                    }
                } catch {}
            }

            result.Sort(delegate(string a, string b) {
                try {
                    return File.GetLastWriteTime(b).CompareTo(File.GetLastWriteTime(a));
                } catch {
                    return 0;
                }
            });

            return result;
        }

        static string OpenFilePicker() {
            try {
                using (OpenFileDialog ofd = new OpenFileDialog()) {
                    ofd.Title = "플래싱할 UF2 펌웨어 파일을 선택하세요";
                    ofd.Filter = "UF2 펌웨어 파일 (*.uf2)|*.uf2|모든 파일 (*.*)|*.*";
                    ofd.InitialDirectory = Environment.GetFolderPath(Environment.SpecialFolder.Desktop);
                    ofd.CheckFileExists = true;
                    ofd.Multiselect = false;
                    ofd.RestoreDirectory = true;
                    if (ofd.ShowDialog() == DialogResult.OK) {
                        return ofd.FileName;
                    }
                }
            } catch (Exception ex) {
                Console.ForegroundColor = ConsoleColor.Red;
                Console.WriteLine("[대화상자 오류] " + ex.Message);
                Console.ResetColor();
            }
            return null;
        }

        static bool FindRp2Device(out int diskNumber, out string driveLetter) {
            diskNumber = -1;
            driveLetter = null;

            try {
                using (ManagementObjectSearcher searcher = new ManagementObjectSearcher(
                    "SELECT DeviceID, Caption FROM Win32_DiskDrive WHERE Model LIKE '%RP2%' OR Caption LIKE '%RP2%'")) {
                    foreach (ManagementObject disk in searcher.Get()) {
                        object devIdObj = disk["DeviceID"];
                        string devId = devIdObj != null ? devIdObj.ToString() : null;
                        if (!string.IsNullOrEmpty(devId) && devId.ToUpper().Contains("PHYSICALDRIVE")) {
                            string numStr = devId.Substring(devId.ToUpper().IndexOf("PHYSICALDRIVE") + 13);
                            int.TryParse(numStr, out diskNumber);
                        }
                    }
                }

                foreach (DriveInfo drive in DriveInfo.GetDrives()) {
                    try {
                        if (drive.IsReady && (drive.VolumeLabel == "RPI-RP2" || drive.DriveType == DriveType.Removable)) {
                            if (File.Exists(Path.Combine(drive.RootDirectory.FullName, "INFO_UF2.TXT"))) {
                                driveLetter = drive.Name.TrimEnd('\\');
                                break;
                            }
                        }
                    } catch {}
                }
            } catch (Exception ex) {
                Console.WriteLine("    WMI 조회 경고: " + ex.Message);
            }

            if (diskNumber == -1 && driveLetter != null) {
                try {
                    using (ManagementObjectSearcher searcher = new ManagementObjectSearcher(string.Format(
                        "ASSOCIATORS OF {{Win32_LogicalDisk.DeviceID='{0}'}} WHERE AssocClass=Win32_LogicalDiskToPartition", driveLetter))) {
                        foreach (ManagementObject partition in searcher.Get()) {
                            using (ManagementObjectSearcher diskSearcher = new ManagementObjectSearcher(string.Format(
                                "ASSOCIATORS OF {{Win32_DiskPartition.DeviceID='{0}'}} WHERE AssocClass=Win32_DiskDriveToDiskPartition", partition["DeviceID"]))) {
                                foreach (ManagementObject disk in diskSearcher.Get()) {
                                    object devIdObj = disk["DeviceID"];
                                    string devId = devIdObj != null ? devIdObj.ToString() : null;
                                    if (!string.IsNullOrEmpty(devId) && devId.ToUpper().Contains("PHYSICALDRIVE")) {
                                        string numStr = devId.Substring(devId.ToUpper().IndexOf("PHYSICALDRIVE") + 13);
                                        int.TryParse(numStr, out diskNumber);
                                    }
                                }
                            }
                        }
                    }
                } catch {}
            }

            return diskNumber != -1;
        }

        static void WaitForKey() {
            try {
                if (Environment.UserInteractive && !Console.IsInputRedirected) {
                    Console.WriteLine("\n아무 키나 누르면 종료합니다...");
                    Console.ReadKey(true);
                }
            } catch {}
        }

        static void WaitForKeyOrTimeout(int timeoutMs) {
            try {
                if (Environment.UserInteractive && !Console.IsInputRedirected) {
                    int waited = 0;
                    while (waited < timeoutMs) {
                        if (Console.KeyAvailable) {
                            Console.ReadKey(true);
                            return;
                        }
                        System.Threading.Thread.Sleep(100);
                        waited += 100;
                    }
                } else {
                    System.Threading.Thread.Sleep(timeoutMs);
                }
            } catch {
                System.Threading.Thread.Sleep(timeoutMs);
            }
        }
    }
}
