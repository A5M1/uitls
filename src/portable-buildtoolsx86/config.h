#pragma once

#include <windows.h>

#define ENABLE_LOGGING     0   
#define ENABLE_DEBUG_LOG   0   
#define MAX_PATH_LENGTH 2048
#define MAX_PATH_BUFFER 4096
#define MSVC_VERSION        "14.44.35207"
#define WINDOWS_SDK_VERSION "10.0.26100.0"
#define ARCH      "x86"
#define HOST_ARCH "x64"

extern char BUILD_TOOLS_ROOT[MAX_PATH_LENGTH];
extern char VCToolsInstallDir[MAX_PATH_LENGTH];
extern char WindowsSDKDir[MAX_PATH_LENGTH];
extern char INCLUDE_PATH[MAX_PATH_BUFFER];
extern char LIB_PATH[MAX_PATH_BUFFER];
extern char BUILD_TOOLS_BIN[MAX_PATH_BUFFER];

void init_paths(void);
