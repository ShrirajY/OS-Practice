#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

int main() {
    const char *dir_name = "my_folder";
    const char *file_name = "my_folder/my_file.txt";

    // Create a directory
    if (mkdir(dir_name, 0700) == -1) {
        perror("mkdir");
        exit(EXIT_FAILURE);
    }

    // Create and write to a file within the directory
    FILE *file = fopen(file_name, "w");
    if (file == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "Hello, World!\n");
    fclose(file);

    printf("Folder and file created successfully.\n");

    return 0;
}
