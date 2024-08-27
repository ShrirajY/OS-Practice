#ifndef _FIL2_HPP
#define _FILE2_HPP

#include "InodesFile.hpp"
#include "Buffer.hpp"
#include <iostream>
#include <fstream>
#include <cstring>

class Files
{
private:
    char *FileName;
    Inode *inode;
    BufVec *bufvec;
    std::fstream fileStream; // File stream for appending content
    char *openFile;

public:
    Files(char *_Name)
    {
        FileName = (char *)malloc(sizeof(char) * 30);
        strcpy(FileName, _Name);
        inode = (Inode *)malloc(sizeof(Inode)); // Fixed typo here
        inode = IPool.AllocatInode();
        bufvec = new BufVec(_Name);
        
        // Open file stream in append mode, create if it doesn't exist
        char *FOLDER_PATH = strdup("E://Current Work//OS Practice//AllFiles//");
        openFile = (char *)malloc(sizeof(char) * 200);
        sprintf(openFile, "%s%s", FOLDER_PATH, FileName);
        fileStream.open(openFile, std::ios::out | std::ios::app);
        if (!fileStream)
        {
            std::cerr << "Failed to open file: " << FileName << std::endl;
        }
    }

    ~Files()
    {
        if (fileStream.is_open())
        {
            fileStream.close();
        }
        free(FileName);
        delete inode;
        delete bufvec;
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
        this->WriteToFile();
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
        std::ofstream fileStream2(openFile, std::ofstream::trunc);
        fileStream2.close();
        std::cout << "File Truncated First" << std::endl;
        this->bufvec->copy(other->bufvec);
        this->WriteToFile();
    }

    void link(Files *other)
    {
        this->bufvec->clear();
        this->bufvec = other->bufvec;
    }

    void WriteToFile()
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
    }
};

#endif // _FILE_HPP
