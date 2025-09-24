#include "config.h"
#include <stdio.h>

char BUILD_TOOLS_ROOT[MAX_PATH_LENGTH];
char VCToolsInstallDir[MAX_PATH_LENGTH];
char WindowsSDKDir[MAX_PATH_LENGTH];
char INCLUDE_PATH[MAX_PATH_BUFFER];
char LIB_PATH[MAX_PATH_BUFFER];
char BUILD_TOOLS_BIN[MAX_PATH_BUFFER];

void init_paths(void) {
    if (!GetCurrentDirectory(MAX_PATH_LENGTH, BUILD_TOOLS_ROOT)) {
        BUILD_TOOLS_ROOT[0] = '\0';
        return;
    }

    snprintf(VCToolsInstallDir, sizeof(VCToolsInstallDir),
             "%s\\VC\\Tools\\MSVC\\%s", BUILD_TOOLS_ROOT, MSVC_VERSION);

    snprintf(WindowsSDKDir, sizeof(WindowsSDKDir),
             "%s\\Windows Kits\\10", BUILD_TOOLS_ROOT);

    snprintf(INCLUDE_PATH, sizeof(INCLUDE_PATH),
             "%s\\include;"
             "%s\\Include\\%s\\ucrt;"
             "%s\\Include\\%s\\shared;"
             "%s\\Include\\%s\\um;"
             "%s\\Include\\%s\\winrt;"
             "%s\\Include\\%s\\cppwinrt",
             VCToolsInstallDir,
             WindowsSDKDir, WINDOWS_SDK_VERSION,
             WindowsSDKDir, WINDOWS_SDK_VERSION,
             WindowsSDKDir, WINDOWS_SDK_VERSION,
             WindowsSDKDir, WINDOWS_SDK_VERSION,
             WindowsSDKDir, WINDOWS_SDK_VERSION);

    snprintf(LIB_PATH, sizeof(LIB_PATH),
             "%s\\lib\\%s;"
             "%s\\Lib\\%s\\ucrt\\%s;"
             "%s\\Lib\\%s\\um\\%s",
             VCToolsInstallDir, ARCH,
             WindowsSDKDir, WINDOWS_SDK_VERSION, ARCH,
             WindowsSDKDir, WINDOWS_SDK_VERSION, ARCH);

    snprintf(BUILD_TOOLS_BIN, sizeof(BUILD_TOOLS_BIN),
             "%s\\bin\\Host%s\\%s;"
             "%s\\bin\\%s\\%s;"
             "%s\\bin\\%s\\%s\\ucrt",
             VCToolsInstallDir, HOST_ARCH, ARCH,
             WindowsSDKDir, WINDOWS_SDK_VERSION, ARCH,
             WindowsSDKDir, WINDOWS_SDK_VERSION, ARCH);
}
