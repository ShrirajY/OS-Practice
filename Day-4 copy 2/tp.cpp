#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // Ensure the correct number of arguments are provided
    

    const char *sourceFilename = "tp.webp";
    const char *destinationFilename = "dest.webp";

    // Open the source file in binary read mode
    FILE *sourceFile = fopen(sourceFilename, "rb");
    if (sourceFile == NULL) {
        perror("Error opening source file");
        return EXIT_FAILURE;
    }

    // Get the size of the source file
    fseek(sourceFile, 0, SEEK_END);
    long fileSize = ftell(sourceFile);
    fseek(sourceFile, 0, SEEK_SET);

    // Allocate memory to hold the file contents
    printf("%d",fileSize);
    char *buffer = (char *)malloc(fileSize);
    if (buffer == NULL) {
        perror("Memory allocation failed");
        fclose(sourceFile);
        return EXIT_FAILURE;
    }

    // Read the entire file into the buffer
    size_t bytesRead = fread(buffer, 1, fileSize, sourceFile);
    if (bytesRead != fileSize) {
        perror("Error reading from source file");
        free(buffer);
        fclose(sourceFile);
        return EXIT_FAILURE;
    }

    // Close the source file
    fclose(sourceFile);

    // Open the destination file in binary write mode
    FILE *destinationFile = fopen(destinationFilename, "wb");
    if (destinationFile == NULL) {
        perror("Error opening destination file");
        free(buffer);
        return EXIT_FAILURE;
    }

    // Write the buffer to the destination file
    size_t bytesWritten = fwrite(buffer, 1, fileSize, destinationFile);
    if (bytesWritten != fileSize) {
        perror("Error writing to destination file");
        free(buffer);
        fclose(destinationFile);
        return EXIT_FAILURE;
    }

    // Close the destination file and free the buffer
    fclose(destinationFile);
    free(buffer);

    printf("File copied successfully.\n");
    return EXIT_SUCCESS;
}
