#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Structure to manage the table
typedef struct
{
    int **data;
    int *columnWidths;
    int rows;
    int cols;
    int WrittenRow;
} Table;

class TableCls
{
private:
    Table *table;
    int rows;
    int cols;

public:
    TableCls(int cols)
    {
        table = (Table *)malloc(sizeof(Table));
        table->cols = cols;
        table->rows = 0;
        table->data = NULL;
        table->columnWidths = (int *)calloc(cols, sizeof(int));
        table->WrittenRow = 0;
    }

    void addRow(int *row)
    {
        table->rows++;
        table->data = (int **)realloc(table->data, table->rows * sizeof(int *));
        table->data[table->rows - 1] = (int *)malloc(table->cols * sizeof(int));
        memcpy(table->data[table->rows - 1], row, table->cols * sizeof(int));

        // Update column widths
        for (int j = 0; j < table->cols; ++j)
        {
            int numDigits = snprintf(NULL, 0, "%d", row[j]);
            if (numDigits > table->columnWidths[j])
            {
                table->columnWidths[j] = numDigits;
            }
        }
    }

    void appendToFile(const char *filename)
    {
        FILE *file = fopen(filename, "a"); // Open file in append mode

        if (!file)
        {
            perror("Error opening file");
            return;
        }

        // Total width for each column including border: 8 characters for data + 1 border on each side
        int totalWidth = 8 + 2; // 8 for data and 2 for borders

        for (int i = table->WrittenRow; i < table->rows; ++i)
        {
            for (int j = 0; j < table->cols; ++j)
            {
                // Print each number with a total width of 8 characters for data
                fprintf(file, "|%*d ", totalWidth - 1, table->data[i][j]);
            }
            fprintf(file, "|\n"); // End of row with a border on the right
        }
        table->WrittenRow = table->rows;
        fclose(file);
    }

    void freeTable()
    {
        for (int i = 0; i < table->rows; ++i)
        {
            free(table->data[i]);
        }
        free(table->data);
        free(table->columnWidths);
        free(table);
    }

    char *trimSpaces(char *str)
    {
        // Trim leading spaces
        while (isspace((unsigned char)*str))
            str++;

        // Trim trailing spaces
        char *end = str + strlen(str) - 1;
        while (end > str && isspace((unsigned char)*end))
            end--;
        *(end + 1) = '\0';

        return str;
    }

    int countColumns(const char *filename)
    {
        FILE *file = fopen(filename, "r");
        if (!file)
        {
            perror("Error opening file");
            return -1;
        }

        char line[1024];
        int cols = 0;

        if (fgets(line, sizeof(line), file))
        {
            // Count the number of columns based on the '|' character
            char *ptr = line;
            while (*ptr)
            {
                if (*ptr == '|')
                    cols++;
                ptr++;
            }
        }

        fclose(file);
        return cols - 1; // Subtract one because the last column will not have a trailing '|'
    }

    int **readTable(const char *filename, int *rows, int *cols)
    {
        *cols = countColumns(filename);
        if (*cols < 1)
        {
            return NULL; // Error or empty file
        }

        FILE *file = fopen(filename, "r");
        if (!file)
        {
            perror("Error opening file");
            return NULL;
        }

        // Temporary storage for rows
        int **table = NULL;
        int rowCount = 0;
        char line[1024];

        // Read the file line by line
        while (fgets(line, sizeof(line), file))
        {
            // Skip lines that are empty or do not contain data
            if (strlen(line) < 2)
                continue;

            // Allocate memory for the new row
            table = (int **)realloc(table, (rowCount + 1) * sizeof(int *));
            table[rowCount] = (int *)malloc(*cols * sizeof(int));

            // Debug: Print the line being processed
            printf("Debug: Reading line: %s", line);

            // Parse the line
            char *ptr = line;
            int colCount = 0;

            while (*ptr)
            {
                // Skip leading '|'
                while (*ptr == '|')
                    ptr++;

                // Read number and trim spaces
                char buffer[32];
                int k = 0;

                // Extract characters until '|' or end of line
                while (*ptr && *ptr != '|' && !isspace((unsigned char)*ptr))
                {
                    buffer[k++] = *ptr++;
                }
                buffer[k] = '\0';

                // Skip any remaining whitespace
                while (isspace((unsigned char)*ptr))
                    ptr++;

                // Debug: Print value parsed
                printf("Debug: Parsing value: '%s'\n", buffer);

                // Ensure buffer is not empty
                if (strlen(buffer) > 0)
                {
                    if (colCount < *cols)
                    {
                        table[rowCount][colCount] = atoi(trimSpaces(buffer));
                        colCount++;
                    }
                }
            }

            if (colCount != *cols)
            {
                fprintf(stderr, "Column count mismatch in row %d. Expected %d but found %d.\n", rowCount + 1, *cols, colCount);
                fclose(file);
                for (int i = 0; i <= rowCount; i++)
                {
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

    void printTable(int **table, int rows, int cols)
    {
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                printf("|%8d ", table[i][j]);
            }
            printf("|\n");
        }
    }
};


int main()
{
    //Table *table = createTable(4); // Initialize table with 4 columns
    TableCls t(4);
    // Add initial rows
    int row1[] = {1, 23, 456, 7890};
    int row2[] = {12, 345, 6789, 0};
    int row3[] = {123, 4567, 89, 101112};

    t.addRow(row1);
    t.addRow(row2);
    t.addRow(row3);
    t.appendToFile("table.txt");

    // Append new rows
    int row4[] = {987, 654, 321, 100};
    int row5[] = {54321, 678, 9, 10};
    int row6[] = {54321, 678, 9, 10};

    t.addRow(row4);
    t.addRow(row5);
    t.addRow(row6);
    t.appendToFile("table.txt");

    printf("New rows have been appended to table.txt\n");

    // Free the allocated memory
    t.freeTable();

    // Reading and printing the table from file
    int rows, cols;
    int** readTableData = t.readTable("table.txt", &rows, &cols);

    if (readTableData) {
        printf("Table read from file:\n");
        t.printTable(readTableData, rows, cols);

        // Free allocated memory
        for (int i = 0; i < rows; i++) {
            free(readTableData[i]);
        }
        free(readTableData);
    } else {
        printf("Failed to read the table from the file.\n");
    }
}