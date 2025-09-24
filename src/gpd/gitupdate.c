#include <stdlib.h>
#include <stdio.h>
int main() {
    if (system("git checkout --orphan new-branch") != 0) {
        perror("Failed to create new branch");
        return 1;
    }
    if (system("git add .") != 0) {
        perror("Failed to add files");
        return 1;
    }
    if (system("git commit -m \"Wiped\"") != 0) {
        perror("Failed to commit changes");
        return 1;
    }
    if (system("git branch -D main") != 0) {
        perror("Failed to delete main branch");
        return 1;
    }
    if (system("git branch -m main") != 0) {
        perror("Failed to rename branch to main");
        return 1;
    }
    if (system("git push --force origin main") != 0) {
        perror("Failed to push changes to remote");
        return 1;
    }
    printf("Successfully updated the branch.\n");
    return 0;
}
