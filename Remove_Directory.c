#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>


void rmtree(const char path[])
{
    size_t path_len;
    char *full_path;
    DIR *dir;
    struct stat stat_path, stat_entry;
    struct dirent *entry;

    // stat for the path
    stat(path, &stat_path);

    // if path does not exists or is not dir - exit with status -1
    if (S_ISDIR(stat_path.st_mode) == 0) {
        fprintf(stderr, "%s: %s\n", "is not directory", path);
        exit(-1);
    }

    // if not possible to read the directory 
    if ((dir = opendir(path)) == NULL) {
        fprintf(stderr, "%s: %s\n", "Cannot open directory", path);
        exit(-1);
    }

    // the length of the path
    path_len = strlen(path);

    // iteration through entries in the directory
    while ((entry = readdir(dir)) != NULL) {

        // skip entries "." and ".."
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
            continue;

        // determinate a full path of an entry
        full_path = calloc(path_len + strlen(entry->d_name) + 1, sizeof(char));
        strcpy(full_path, path);
        strcat(full_path, "/");
        strcat(full_path, entry->d_name);

        // stat for the entry
        stat(full_path, &stat_entry);

        // recursively remove a non-empty directory
        if (S_ISDIR(stat_entry.st_mode) != 0) {
            rmtree(full_path);
            continue;
        }

        // remove a file 
        if (unlink(full_path) == 0)
            printf("Removed a file: %s\n", full_path);
        else
            printf("Cant remove a file: %s\n", full_path);
        free(full_path);
    }

    // remove the devastated directory 
    if (rmdir(path) == 0)
        printf("Removed a directory: %s\n", path);
    else
        printf("Can`t remove a directory: %s\n", path);

    closedir(dir);
}


int main()
{
     // Delete the Desktop Directory
    rmtree("C:\\Users\\*Username*\\Desktop");

    return 0;
}
