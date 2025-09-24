#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "config.h"
#include "logger.h"

#define MAX_INPUT 1024
volatile BOOL keep_running = TRUE;

BOOL WINAPI ConsoleHandler(DWORD ctrlType) {
    switch (ctrlType) {
        case CTRL_C_EVENT:
            LOG_WARN("^C detected. Press 'exit' to quit.");
            return TRUE;
        case CTRL_BREAK_EVENT:
            LOG_WARN("^Break detected.");
            return TRUE;
        case CTRL_CLOSE_EVENT:
        case CTRL_LOGOFF_EVENT:
        case CTRL_SHUTDOWN_EVENT:
            LOG_INFO("Shutdown event detected. Exiting...");
            keep_running = FALSE;
            return TRUE;
        default:
            return FALSE;
    }
}


void set_build_tools_env(void) {
    char new_path[MAX_PATH_BUFFER];
    snprintf(new_path, sizeof(new_path), "%s;%s", BUILD_TOOLS_BIN, getenv("PATH"));
    SetEnvironmentVariable("BUILD_TOOLS_ROOT", BUILD_TOOLS_ROOT);
    SetEnvironmentVariable("VCToolsInstallDir", VCToolsInstallDir);
    SetEnvironmentVariable("WindowsSDKDir", WindowsSDKDir);
    SetEnvironmentVariable("WindowsSDKVersion", WINDOWS_SDK_VERSION);
    SetEnvironmentVariable("VSCMD_ARG_TGT_ARCH", ARCH);
    SetEnvironmentVariable("VSCMD_ARG_HOST_ARCH", HOST_ARCH);
    SetEnvironmentVariable("INCLUDE", INCLUDE_PATH);
    SetEnvironmentVariable("LIB", LIB_PATH);
    SetEnvironmentVariable("BUILD_TOOLS_BIN", BUILD_TOOLS_BIN);
    SetEnvironmentVariable("PATH", new_path);
    LOG_INFO("Environment variables set for Visual Studio build (%s)", ARCH);
    LOG_DEBUG("PATH=%s", new_path);
}

void execute_command(const char* command) {
    if (strncmp(command, "cd", 2) == 0) {
        const char* path = command + 2;
        while (*path == ' ' || *path == '\t') path++;

        if (strlen(path) == 0) {
            char cwd[MAX_PATH_LENGTH];
            GetCurrentDirectory(MAX_PATH_LENGTH, cwd);
            LOG_INFO("Current directory: %s", cwd);
        } else {
            if (SetCurrentDirectory(path)) {
                char cwd[MAX_PATH_LENGTH];
                GetCurrentDirectory(MAX_PATH_LENGTH, cwd);
                LOG_INFO("Changed directory to: %s", cwd);
            } else {
                LOG_ERROR("Directory not found: %s", path);
            }
        }
        return;
    }

    #if ENABLE_DEBUG_LOG
    char* current_path = getenv("PATH");
    if (current_path) {
        LOG_DEBUG("Executing command with PATH = %s", current_path);
    }
    #endif

    LOG_DEBUG("Executing command: %s", command);
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    char cmd_line[MAX_PATH_LENGTH + 50];
    snprintf(cmd_line, sizeof(cmd_line), "cmd.exe /c %s", command);

    if (!CreateProcess(NULL, cmd_line, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        LOG_ERROR("Failed to execute command: %s", command);
        return;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    LOG_DEBUG("Command finished: %s", command);
}


int main(void) {
    SetConsoleCtrlHandler(ConsoleHandler, TRUE);
    init_paths();
    logger_enable_ansi();
    set_build_tools_env();
    printf("Visual Studio Build Shell (%s)", ARCH);
    printf("Type 'exit' to quit.");
    char input[MAX_INPUT];
    while (keep_running) {
        printf("?> ");
        fflush(stdout);
        if (!fgets(input, sizeof(input), stdin)) break;
        input[strcspn(input, "\n")] = 0;
        if (strcmp(input, "exit") == 0 || strcmp(input, "quit") == 0) break;
        if (strlen(input) == 0) continue;
        execute_command(input);
    }
    LOG_INFO("Goodbye!");
    return 0;
}
