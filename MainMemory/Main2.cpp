#include<iostream>
#include<string.h>
#include<vector>
#define ACTIVE 1
#define DELETE 2
#define NOT_ACTIVE 0
#define INODE int
#define TAPE_HEAD int
#define FILENAME char*
#define TotalBlocks 60
#define BlockSpace 50

typedef struct _Block{
    char space[BlockSpace];
    int flag=NOT_ACTIVE;
}Block;

typedef struct _MainMemory{
    Block blocks[TotalBlocks];
    int OccBlocks=0;
    TAPE_HEAD thead=0;
}MainMemory;

typedef struct _File{
    FILENAME FileName;
    std::vector<Block> blks;
    char Permission[3];
}File;

MainMemory MM;

int getBlk()
{
    MM.OccBlocks++;
    return MM.thead++;
}

int write(char *text)
{
    int blk=getBlk();
    MM.blocks[blk].flag=ACTIVE;
    strcpy(MM.blocks[blk].space,text);
    return 1;
}

int main()
{
    write(strdup("This is the text"));
    write(strdup("Second Text"));
    std::cout<<MM.blocks[0].space<<std::endl;
    std::cout<<MM.blocks[1].space<<std::endl;
    return 0;
}