#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int FileCount=0;
char **MainMemory;
char **FileNames;
int **Blocks;
int counts[6]={0,0,0,0,0,0};
int create(char *name)
{
    FileCount++;
    FileNames[FileCount-1]=(char *)malloc(sizeof(char)*30);
    FileNames[FileCount-1]=strdup(name);
    Blocks[FileCount-1]=(int **)malloc(sizeof(int)*6);
    return FileCount-1;
}

int* MALLOC(size_t size)
{
    
}
int main()
{
    int FileCount=0;
    char **MainMemory=(char **)malloc(sizeof(char *)*30);
    char **FileNames=(char **)malloc(sizeof(char *)*6);
    int **Blocks=(int **)malloc(sizeof(int)*6);
    int counts[6]={0,0,0,0,0,0};
    for(int i=0;i<6;i++)
    {
        FileNames[i]=(char *)malloc(sizeof(char )*30);
        Blocks[i]=(int *)malloc(sizeof(int)*5);
    }

    for(int i=0;i<30;i++)
    {
        MainMemory[i]=(char *)malloc(sizeof(char)*35);
    }
    FileNames[0]=strdup("First File");
    FileCount++;
    char *text=(char *)malloc(sizeof(char)*30);
    text=strdup("This is\n So it should be fine\n");
    MainMemory[0]=text;
    Blocks[0][0]=0;
    counts[0]++;

    FileNames[1]=strdup("Second File");
    FileCount++;
    char *text2=(char *)malloc(sizeof(char)*30);
    text2=strdup("SecondFIle\nshould be clean\n");
    MainMemory[1]=text2;
    Blocks[1][0]=1;
    counts[1]++;

    for(int i=0;i<FileCount;i++)
    {
        printf("File name is: %s\n\n",FileNames[i]);
        for(int j=0;j<counts[i];j++)
        {
            printf("Corresponding block no is %d\n\n",Blocks[i][j]);
            printf("Corresponding text for this block is\n\n%s\n",MainMemory[Blocks[i][j]]);
        }
    }
}