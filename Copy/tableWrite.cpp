#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to manage the table
typedef struct {
    int **data;
    int *columnWidths;
    int rows;
    int cols;
    int WrittenRow;
} Table;

// Function to create a new table
Table* createTable(int cols) {
    Table *table = (Table *)malloc(sizeof(Table));
    table->cols = cols;
    table->rows = 0;
    table->data = NULL;
    table->columnWidths = (int *)calloc(cols, sizeof(int));
    table->WrittenRow = 0;
    return table;
}

// Function to add a row to the table
void addRow(Table *table, int *row) {
    table->rows++;
    table->data = (int **)realloc(table->data, table->rows * sizeof(int *));
    table->data[table->rows - 1] = (int *)malloc(table->cols * sizeof(int));
    memcpy(table->data[table->rows - 1], row, table->cols * sizeof(int));

    // Update column widths
    for (int j = 0; j < table->cols; ++j) {
        int numDigits = snprintf(NULL, 0, "%d", row[j]);
        if (numDigits > table->columnWidths[j]) {
            table->columnWidths[j] = numDigits;
        }
    }
}

// Function to append rows to the file
void appendToFile(const char *filename, Table *table) {
    FILE *file = fopen(filename, "a"); // Open file in append mode

    if (!file) {
        perror("Error opening file");
        return;
    }

    // Total width for each column including border: 8 characters for data + 1 border on each side
    int totalWidth = 8 + 2; // 8 for data and 2 for borders

    for (int i = table->WrittenRow; i < table->rows; ++i) {
        for (int j = 0; j < table->cols; ++j) {
            // Print each number with a total width of 8 characters for data
            fprintf(file, "|%*d ", totalWidth - 1, table->data[i][j]);
        }
        fprintf(file, "|\n"); // End of row with a border on the right
    }
    table->WrittenRow = table->rows;
    fclose(file);
}

// Function to free the table memory
void freeTable(Table *table) {
    for (int i = 0; i < table->rows; ++i) {
        free(table->data[i]);
    }
    free(table->data);
    free(table->columnWidths);
    free(table);
}

int main() {
    Table *table = createTable(4); // Initialize table with 4 columns

    // Add initial rows
    int row1[] = {1, 23, 456, 7890};
    int row2[] = {12, 345, 6789, 0};
    int row3[] = {123, 4567, 89, 101112};

    addRow(table, row1);
    addRow(table, row2);
    addRow(table, row3);
    appendToFile("table.txt", table);

    // Append new rows
    int row4[] = {987, 654, 321, 100};
    int row5[] = {54321, 678, 9, 10};
    int row6[] = {54321, 678, 9, 10};

    addRow(table, row4);
    addRow(table, row5);
    addRow(table, row6);
    appendToFile("table.txt", table);

    printf("New rows have been appended to table.txt\n");

    // Free the allocated memory
    freeTable(table);

    return 0;
}
