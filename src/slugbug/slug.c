#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define SLUG_LEN 6
void get_random_slug(char *slug, int length) {
    const char *chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (int i = 0; i < length; i++) {
        slug[i] = chars[rand() % (int)strlen(chars)];
    }
    slug[length] = '\0';
}
void make_new_filename(const char *filename, char *newname) {
    const char *ext = strrchr(filename, '.');
    if (!ext) ext = filename + strlen(filename); 
    int name_len = (int)(ext - filename);
    char slug[SLUG_LEN + 1];
    get_random_slug(slug, SLUG_LEN);
    strncpy(newname, filename, name_len);
    newname[name_len] = '\0';
    strcat(newname, "-");
    strcat(newname, slug);
    strcat(newname, ext);
}
int main(void) {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind;
    srand((unsigned int)time(NULL));
    hFind = FindFirstFile("*.*", &findFileData);
    if (hFind == INVALID_HANDLE_VALUE) {
        printf("No files found.\n");
        return 1;
    }
    do {
        if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            char newname[MAX_PATH];
            make_new_filename(findFileData.cFileName, newname);
            if (MoveFile(findFileData.cFileName, newname)) {
                printf("Renamed '%s' -> '%s'\n", findFileData.cFileName, newname);
            } else {
                printf("Failed to rename '%s' (error %lu)\n", findFileData.cFileName, GetLastError());
            }
        }
    } while (FindNextFile(hFind, &findFileData) != 0);
    FindClose(hFind);
    return 0;
}
