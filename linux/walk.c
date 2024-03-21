// This program recursively prints out all sub directories and files in this directory

#include <dirent.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/dir.h>
#include <sys/stat.h>
#define MAXPATH 4096

uint64_t total_size = 0;
uint64_t total_files = 0;
uint64_t total_directories = 0;
uint64_t total_hlinks = 0;

void walk(const char *path)
{
    DIR *dir = opendir(path);
    struct dirent *dptr;
    char pathbuffer[MAXPATH];
    struct stat st;
    if (!dir)
    {
        perror(path);
        return;
    }
    while ((dptr = readdir(dir)) != NULL)
    {
        if (strcmp(dptr->d_name, "..") == 0)
            continue;
        if (strcmp(dptr->d_name, ".") == 0)
        {
            printf("%s\n", path);
            continue;
        }
        if (path[strlen(path) - 1] == '/')
            snprintf(pathbuffer, MAXPATH - 1, "%s%s", path, dptr->d_name);
        else
            snprintf(pathbuffer, MAXPATH - 1, "%s/%s", path, dptr->d_name);
        if (lstat(pathbuffer, &st) == -1)
        {
            perror(pathbuffer);
            continue;
        }
        printf("%s\n", pathbuffer);
        if (S_ISLNK(st.st_mode))
        {
            // Ignore links to avoid endless loops
            total_hlinks++;
            continue;
        }
        if (S_ISDIR(st.st_mode))
        {
            total_directories++;
            walk(pathbuffer);
        }
        else
        {
            total_files++;
            total_size += st.st_size;
        }
    }
    closedir(dir);
}

int main(int argc, char *argv[])
{
    const char *current_dir = ".";
    if (argc == 2)
    {
        current_dir = argv[1];
    }
    if (argc > 2)
    {
        fprintf(stderr, "Usage: walk [directory]\n");
        exit(1);
    }
    walk(current_dir);
    printf("Total directories: %u\n", total_directories);
    printf("Total hard links: %u\n", total_hlinks);
    printf("Total size: %u\n", total_size);
    printf("Total size (in GB): %f\n", total_size / 1000000000.0);
    printf("Total files: %u\n", total_files);

    fprintf(stderr,"Total directories: %u\n", total_directories);
    fprintf(stderr,"Total hard links: %u\n", total_hlinks);
    fprintf(stderr,"Total size: %u\n", total_size);
    fprintf(stderr,"Total size (in GB): %f\n", total_size / 1000000000.0);
    fprintf(stderr,"Total files: %u\n", total_files);
}