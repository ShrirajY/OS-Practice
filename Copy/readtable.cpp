#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Function to trim leading and trailing spaces
char* trimSpaces(char* str) {
    // Trim leading spaces
    while (isspace((unsigned char)*str)) str++;

    // Trim trailing spaces
    char* end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    *(end + 1) = '\0';

    return str;
}

// Function to count the number of columns in the file
int countColumns(const char* filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return -1;
    }

    char line[1024];
    int cols = 0;

    if (fgets(line, sizeof(line), file)) {
        // Count the number of columns based on the '|' character
        char *ptr = line;
        while (*ptr) {
            if (*ptr == '|') cols++;
            ptr++;
        }
    }

    fclose(file);
    return cols - 1; // Subtract one because the last column will not have a trailing '|'
}

// Function to read the table from file and convert it to a 2D array
int** readTable(const char* filename, int *rows, int *cols) {
    *cols = countColumns(filename);
    if (*cols < 1) {
        return NULL; // Error or empty file
    }

    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return NULL;
    }

    // Temporary storage for rows
    int** table = NULL;
    int rowCount = 0;
    char line[1024];

    // Read the file line by line
    while (fgets(line, sizeof(line), file)) {
        // Skip lines that are empty or do not contain data
        if (strlen(line) < 2) continue;

        // Allocate memory for the new row
        table = (int **)realloc(table, (rowCount + 1) * sizeof(int *));
        table[rowCount] = (int *)malloc(*cols * sizeof(int));

        // Debug: Print the line being processed
        printf("Debug: Reading line: %s", line);

        // Parse the line
        char *ptr = line;
        int colCount = 0;

        while (*ptr) {
            // Skip leading '|'
            while (*ptr == '|') ptr++;

            // Read number and trim spaces
            char buffer[32];
            int k = 0;

            // Extract characters until '|' or end of line
            while (*ptr && *ptr != '|' && !isspace((unsigned char)*ptr)) {
                buffer[k++] = *ptr++;
            }
            buffer[k] = '\0';

            // Skip any remaining whitespace
            while (isspace((unsigned char)*ptr)) ptr++;

            // Debug: Print value parsed
            printf("Debug: Parsing value: '%s'\n", buffer);

            // Ensure buffer is not empty
            if (strlen(buffer) > 0) {
                if (colCount < *cols) {
                    table[rowCount][colCount] = atoi(trimSpaces(buffer));
                    colCount++;
                }
            }
        }

        if (colCount != *cols) {
            fprintf(stderr, "Column count mismatch in row %d. Expected %d but found %d.\n", rowCount + 1, *cols, colCount);
            fclose(file);
            for (int i = 0; i <= rowCount; i++) {
                free(table[i]);
            }
            free(table);
            return NULL;
        }

        rowCount++;
    }

    fclose(file);
    *rows = rowCount;
    return table;
}

// Function to print the 2D array with proper formatting
void printTable(int** table, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("|%8d ", table[i][j]);
        }
        printf("|\n");
    }
}

int main() {
    const char* filename = "table.txt";
    int rows, cols;
    int** table = readTable(filename, &rows, &cols);

    if (table) {
        printf("Table read from file:\n");
        printTable(table, rows, cols);

        // Free allocated memory
        for (int i = 0; i < rows; i++) {
            free(table[i]);
        }
        free(table);
    } else {
        printf("Failed to read the table from the file.\n");
    }

    return 0;
}
