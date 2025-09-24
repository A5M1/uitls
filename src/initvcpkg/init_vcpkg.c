//x86_64-w64-mingw32-gcc -o vcpkg-init.exe init_vcpkg.c  -Os -s -nostdlib -lkernel32 -lshell32 -Wl,-e,wmainCRTStartup
#include <windows.h>
#include <shellapi.h>
#include <iostream>
#include <string>
#include <vector>

static DWORD wlen(const wchar_t *s) {
    DWORD n = 0;
    while (s[n]) n++;
    return n;
}

static void wcpy(wchar_t *dst, const wchar_t *src) {
    while (*src) {
        *dst++ = *src++;
    }
    *dst = 0;
}

static DWORD wcat(wchar_t *dst, const wchar_t *src) {
    DWORD off = wlen(dst);
    wcpy(dst + off, src);
    return off + wlen(src);
}

static void writestr(HANDLE h, const wchar_t *s) {
    if (h == INVALID_HANDLE_VALUE || s == NULL) {
        return;
    }
    DWORD written;
    WriteFile(h, s, wlen(s) * sizeof(wchar_t), &written, NULL);
}

static size_t mstrlen(const char *s) {
    size_t len = 0;
    while (s[len]) {
        len++;
    }
    return len;
}

static void writestrA(HANDLE h, const char *s) {
    DWORD written;
    WriteFile(h, s, mstrlen(s), &written, NULL);
}

#ifdef _M_X64
__attribute__((naked)) void ___chkstk_ms(void) {
    __asm__ volatile(
        "mov %rsp,%rax\n"
        "sub $0x20,%rax\n"
        "test %rax,%rax\n"
        "ret\n"
    );
}
#endif

int wmainCRTStartup(void) {
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE hStdErr = GetStdHandle(STD_ERROR_HANDLE);

    wchar_t vcpkgPath[MAX_PATH];
    wchar_t cmd[MAX_PATH + 128];
    wchar_t hash_w[64] = {0};
    char hash_a[64] = {0};

    DWORD len = GetEnvironmentVariableW(L"VCPKG_ROOT", vcpkgPath, MAX_PATH);
    if (len == 0 || len >= MAX_PATH) {
        writestrA(hStdErr, "ERROR: Environment variable VCPKG_ROOT is not set or is too long.\r\n");
        return 1;
    }

    wcpy(cmd, L"git -C \"");
    wcat(cmd, vcpkgPath);
    wcat(cmd, L"\" rev-parse HEAD");

    HANDLE hPipeRead, hPipeWrite;
    SECURITY_ATTRIBUTES sa = {sizeof(sa), NULL, TRUE};
    if (!CreatePipe(&hPipeRead, &hPipeWrite, &sa, 0)) {
        writestrA(hStdErr, "ERROR: Failed to create pipe.\r\n");
        return 1;
    }

    STARTUPINFOW si = {sizeof(si)};
    PROCESS_INFORMATION pi = {0};
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdOutput = hPipeWrite;
    si.hStdError = hPipeWrite;

    if (!CreateProcessW(NULL, cmd, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        CloseHandle(hPipeWrite);
        CloseHandle(hPipeRead);
        writestrA(hStdErr, "ERROR: Failed to run git command.\r\n");
        return 1;
    }

    CloseHandle(hPipeWrite);

    DWORD bytesRead = 0;
    if (!ReadFile(hPipeRead, hash_a, sizeof(hash_a) - 1, &bytesRead, NULL) || bytesRead == 0) {
        CloseHandle(hPipeRead);
        writestrA(hStdErr, "ERROR: Failed to read Git hash.\r\n");
        return 1;
    }

    hash_a[bytesRead] = 0;
    MultiByteToWideChar(CP_UTF8, 0, hash_a, -1, hash_w, sizeof(hash_w) / sizeof(wchar_t));

    CloseHandle(hPipeRead);

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    for (DWORD i = 0; i < wlen(hash_w); i++) {
        if (hash_w[i] == L'\r' || hash_w[i] == L'\n') {
            hash_w[i] = 0;
            break;
        }
    }

    HANDLE hFile = CreateFileW(L"vcpkg.json", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        writestrA(hStdErr, "ERROR: Failed to create vcpkg.json.\r\n");
        return 1;
    }

    wchar_t line[1024];
    wcpy(line, L"{\r\n  \"name\": \"my-project\",\r\n  \"version-string\": \"1.0.0\",\r\n  \"builtin-baseline\": \"");
    wcat(line, hash_w);
    wcat(line, L"\",\r\n  \"dependencies\": [\r\n");
    writestr(hFile, line);

    int argc;
    wchar_t **argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    if (argv) {
        for (int i = 1; i < argc; i++) {
            wcpy(line, L"    \"");
            wcat(line, argv[i]);
            wcat(line, L"\"");
            if (i + 1 < argc) {
                wcat(line, L",");
            }
            wcat(line, L"\r\n");
            writestr(hFile, line);
        }
        LocalFree(argv);
    }

    writestr(hFile, L"  ]\r\n}\r\n");
    CloseHandle(hFile);

    writestr(hStdOut, L"vcpkg.json created successfully.\r\n");

    return 0;
}
