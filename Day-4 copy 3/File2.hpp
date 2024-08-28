#ifndef _FIL2_HPP
#define _FILE2_HPP

#include "InodesFile.hpp"
#include "Buffer.hpp"
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdio>
// #include <sys/stat.h>
#include <fcntl.h>
class Files
{
private:
    char *FileName;
    Inode *inode;
    BufVec *bufvec;
    // std::fstream fileStream; // File stream for appending content
    char *openFile;
    char FileType;
    int permission;
    Files **Children;
    Files *Parent;
    int childrenNo;
public:
friend class BufVec;
    Files(char *_Name, char _FT, Files *_parent)
    {
        FileName = (char *)malloc(sizeof(char) * 30);
        strcpy(FileName, _Name);
        inode = (Inode *)malloc(sizeof(Inode)); // Fixed typo here
        inode = IPool.AllocatInode();
        
        Parent=_parent;
        // Open file stream in append mode, create if it doesn't exist
        char *FOLDER_PATH = strdup("E://Current Work//OS Practice//AllFiles//");
        openFile = (char *)malloc(sizeof(char) * 200);
        
        if(Parent==NULL)
        {
            std::cout<<"Creating BufVec\n";
            sprintf(openFile,"E://Current Work//OS Practice//AllFiles//%s",FileName);
            std::cout<<openFile;
            bufvec=new BufVec(openFile,_FT,false);
        }
        else
        {
            sprintf(openFile,"%s//%s",Parent->openFile,this->FileName);
            bufvec=new BufVec(openFile,_FT,Parent);
            Parent->AddChildren(this);
        }

        this->FileType=_FT;
        permission=777;
        Children=(Files **)malloc(sizeof(Files *)*0);
        childrenNo=0;
    }

    ~Files()
    {
        // if (fileStream.is_open())
        // {
        //     fileStream.close();
        // }
        // free(FileName);
        // delete inode;
        // delete bufvec;
    }

    void Info()
    {
        std::cout << "File's Name is : " << FileName << std::endl;
        std::cout << "Allocated Buffers are: ";
        this->showBufHead();
        std::cout << std::endl;
        std::cout << "Allocated inode is " << inode->inodeNo << std::endl;
    }

    void Write(const char *space)
    {
        bufvec->write(const_cast<char *>(space));
    }

    void show()
    {
        bufvec->show();
    }

    void showInode()
    {
        std::cout << inode->inodeNo << std::endl;
    }

    void showBufHead()
    {
        bufvec->ShowBufHeaders();
    }

    void clear()
    {
        bufvec->clear();
    }

    void copy(Files *other)
    {

        this->bufvec->copy(other->bufvec);
    }

    void link(Files *other)
    {
        this->bufvec->clear();
        this->bufvec = other->bufvec;
        // this->bufvec->AssignOperator(other->bufvec);
        // this->bufvec->LINKFLAG=true;
        // this->bufvec->other=other->bufvec;
        // other->bufvec->LINKFLAG=true;
        // other->bufvec->other=this->bufvec;


        std::ifstream src(other->openFile, std::ios::binary);
        std::ofstream dest(this->openFile, std::ios::binary);

        if (!src || !dest)
        {
            std::cerr << "Error opening file for reading or writing." << std::endl;
        }

        dest << src.rdbuf();
    }

    /*void WriteToFile()
    {
        if (fileStream.is_open())
        {
            Buffer *temp = bufvec->CurrBuffWrite;
            if (temp == nullptr)
            {
                std::cerr << "No buffers to write." << std::endl;
                return;
            }

            do
            {
                // Debug: print the content of each buffer
                std::cout << "Writing buffer: " << temp->space << std::endl;

                if (temp != bufvec->CurrBuffWrite)
                {
                    fileStream.write(temp->space, temp->Bsize);
                }
                else
                {
                    for (size_t i = bufvec->BufTapeWrite; i < temp->Bsize; ++i)
                    {
                        // fputc(temp->space[i], fileStream); // Write one character at a time
                        fileStream.put(temp->space[i]);
                    }
                }
                // Move to the next buffer
                temp = temp->VecNext;

            } while (temp != bufvec->head); // Iterate until we circle back to head

            bufvec->CurrBuffWrite = temp->VecPrev;
            bufvec->BufTapeWrite = temp->VecPrev->Bsize;
            fileStream.flush(); // Ensure all data is written to the file
        }
        else
        {
            std::cerr << "File stream is not open." << std::endl;
        }
    }*/

    void AddChildren(Files *child)
    {
        Children=(Files **)realloc(Children,sizeof(Files *)*(childrenNo+1));
        Children[childrenNo]=child;
        childrenNo+=1;
    }
};

#endif // _FILE_HPP
