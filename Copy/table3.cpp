#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Define constants for data types
#define TYPE_INT 1
#define TYPE_CHAR 2
#define TYPE_STRING 3

// Structure to manage the table
typedef struct
{
    void ***data;         // 2D array of void pointers to handle multiple data types
    int *columnWidths;
    int rows;
    int cols;
    int *typeList;        // Array to store the type of each column
    int WrittenRow;
} Table;

class TableCls
{
private:
    Table *tableStruct;   // Rename to avoid conflict

public:
    TableCls(int cols, int *typeList)
    {
        tableStruct = (Table *)malloc(sizeof(Table));
        tableStruct->cols = cols;
        tableStruct->rows = 0;
        tableStruct->data = NULL;
        tableStruct->columnWidths = (int *)calloc(cols, sizeof(int));
        tableStruct->typeList = (int *)malloc(cols * sizeof(int));
        memcpy(tableStruct->typeList, typeList, cols * sizeof(int));
        tableStruct->WrittenRow = 0;
    }

    ~TableCls()
    {
        freeTable();
    }

    void addRow(void **row)
    {
        tableStruct->rows++;
        tableStruct->data = (void ***)realloc(tableStruct->data, tableStruct->rows * sizeof(void **));
        tableStruct->data[tableStruct->rows - 1] = (void **)malloc(tableStruct->cols * sizeof(void *));

        for (int j = 0; j < tableStruct->cols; ++j)
        {
            switch (tableStruct->typeList[j])
            {
            case TYPE_INT:
                tableStruct->data[tableStruct->rows - 1][j] = malloc(sizeof(int));
                memcpy(tableStruct->data[tableStruct->rows - 1][j], row[j], sizeof(int));
                break;
            case TYPE_CHAR:
                tableStruct->data[tableStruct->rows - 1][j] = malloc(sizeof(char));
                memcpy(tableStruct->data[tableStruct->rows - 1][j], row[j], sizeof(char));
                break;
            case TYPE_STRING:
                tableStruct->data[tableStruct->rows - 1][j] = strdup((char *)row[j]);
                break;
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

        for (int i = tableStruct->WrittenRow; i < tableStruct->rows; ++i)
        {
            for (int j = 0; j < tableStruct->cols; ++j)
            {
                switch (tableStruct->typeList[j])
                {
                case TYPE_INT:
                    fprintf(file, "|%*d ", totalWidth - 1, *(int *)tableStruct->data[i][j]);
                    break;
                case TYPE_CHAR:
                    fprintf(file, "|%*c ", totalWidth - 1, *(char *)tableStruct->data[i][j]);
                    break;
                case TYPE_STRING:
                    fprintf(file, "|%-*s ", totalWidth - 1, (char *)tableStruct->data[i][j]);
                    break;
                }
            }
            fprintf(file, "|\n"); // End of row with a border on the right
        }
        tableStruct->WrittenRow = tableStruct->rows;
        fclose(file);
    }

    void freeTable()
    {
        for (int i = 0; i < tableStruct->rows; ++i)
        {
            for (int j = 0; j < tableStruct->cols; ++j)
            {
                switch (tableStruct->typeList[j])
                {
                case TYPE_INT:
                case TYPE_CHAR:
                    free(tableStruct->data[i][j]);
                    break;
                case TYPE_STRING:
                    free(tableStruct->data[i][j]);
                    break;
                }
            }
            free(tableStruct->data[i]);
        }
        free(tableStruct->data);
        free(tableStruct->columnWidths);
        free(tableStruct->typeList);
        free(tableStruct);
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

    void **readTable(const char *filename, int *rows, int *cols)
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
    void ***tableData = NULL;
    int rowCount = 0;
    char line[1024];

    // Read the file line by line
    while (fgets(line, sizeof(line), file))
    {
        // Skip lines that are empty or do not contain data
        if (strlen(line) < 2)
            continue;

        // Allocate memory for the new row
        tableData = (void ***)realloc(tableData, (rowCount + 1) * sizeof(void **));
        tableData[rowCount] = (void **)malloc(*cols * sizeof(void *));

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

            // Read value and trim spaces
            char buffer[1024];
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
                    switch (tableStruct->typeList[colCount])
                    {
                    case TYPE_INT:
                        tableData[rowCount][colCount] = malloc(sizeof(int));
                        *(int *)tableData[rowCount][colCount] = atoi(trimSpaces(buffer));
                        break;
                    case TYPE_CHAR:
                        tableData[rowCount][colCount] = malloc(sizeof(char));
                        *(char *)tableData[rowCount][colCount] = trimSpaces(buffer)[0];
                        break;
                    case TYPE_STRING:
                        tableData[rowCount][colCount] = strdup(trimSpaces(buffer));
                        break;
                    }
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
                for (int j = 0; j < *cols; j++)
                {
                    if (tableStruct->typeList[j] == TYPE_STRING)
                        free(tableData[i][j]);
                    free(tableData[i][j]);
                }
                free(tableData[i]);
            }
            free(tableData);
            return NULL;
        }

        rowCount++;
    }

    fclose(file);
    *rows = rowCount;
    return (void **)tableData;
}

    void printTable(void **tableData, int rows, int cols)
    {
        int totalWidth = 8; // Width for each column (adjust as necessary)

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                switch (tableStruct->typeList[j])
                {
                case TYPE_INT:
                    printf("|%*d ", totalWidth, *(int *)((void **)tableData[i])[j]);
                    break;
                case TYPE_CHAR:
                    printf("|%*c ", totalWidth, *(char *)((void **)tableData[i])[j]);
                    break;
                case TYPE_STRING:
                    printf("|%-*s ", totalWidth, (char *)((void **)tableData[i])[j]);
                    break;
                }
            }
            printf("|\n");
        }
    }
};

int main()
{
    int typeList[] = {TYPE_INT, TYPE_CHAR, TYPE_STRING}; // Define types for columns

    TableCls t(3, typeList); // Initialize table with 3 columns

    // Add initial rows
    int row1[] = {123, 'A', (int)"Hello"};
    int row2[] = {456, 'B', (int)"World"};
    void *r1[] = { &row1[0], &row1[1], (void *)row1[2] };
    void *r2[] = { &row2[0], &row2[1], (void *)row2[2] };

    t.addRow(r1);
    t.addRow(r2);

    
    t.appendToFile("table.txt");

    // Read and print the table from file
    int rows, cols;
    void **readTableData = t.readTable("table.txt", &rows, &cols);

    if (readTableData)
    {
        printf("Table read from file:\n");
        t.printTable(readTableData, rows, cols);

        // Free allocated memory
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                if (typeList[j] == TYPE_STRING)
                    free(((char **)readTableData[i])[j]);
            }
            free(readTableData[i]);
        }
        free(readTableData);
    }
    else
    {
        printf("Failed to read the table from the file.\n");
    }

    return 0;
}
