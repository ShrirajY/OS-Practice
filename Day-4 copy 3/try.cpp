#include <iostream>
#include <cstdio>
// #include <sys/stat.h>
#include <fcntl.h>
// #include <unistd.h>

int main() {
    const char *dir_name = "E://Current Work//OS Practice//AllFiles//my_folder";
    const char *file_name = "E://Current Work//OS Practice//AllFiles//my_folder//my_file.txt";

    // Create directory
    if (mkdir(dir_name) == -1) {
        perror("mkdir");
        return 1;
    }

    // Create and write to a file
    FILE *file = fopen(file_name, "w");
    if (file == nullptr) {
        perror("fopen");
        return 1;
    }

    fprintf(file, "Hello, World!\n");
    fclose(file);

    std::cout << "Folder and file created successfully.\n";
    return 0;
}
