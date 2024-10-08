#ifndef _FILE_HPP
#define _FILE_HPP

#include "InodesFile.hpp"
#include "Buffer.hpp"

#include<iostream>

class Files{
    private:
        char *FileName;
        Inode *inode;
        BufVec *bufvec;
    public:
        Files(char *_Name)
        {
            
            FileName=(char *)malloc(sizeof(char)*30);
            strcpy(FileName,_Name);
            inode=(Inode *)malloc(sizeof(inode));
            inode=IPool.AllocatInode();
            bufvec=new BufVec();
            std::cout<<"Created File with FileName: "<<FileName<<" and with Inode No: "<<inode->inodeNo<<std::endl;
        }

        void Info()
        {
            std::cout<<std::endl;
            std::cout<<"File's Name is : "<<FileName<<std::endl;
            std::cout<<"Allocated Buffers are: ";
            this->showBufHead();
            std::cout<<std::endl;
            std::cout<<"Allocated inode is"<<inode->inodeNo<<std::endl;
        }

        void Write(char *space)
        {
            std::cout<<"writing into File"<<std::endl;
            bufvec->write(space);
        }

        void show()
        {
            bufvec->show();
        }

        void showInode()
        {
            std::cout<<inode->inodeNo<<std::endl;
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
            std::cout<<"Copying "<<this->FileName<<" Into "<<other->FileName<<std::endl;
            this->bufvec->copy(other->bufvec);
        }

        void link(Files *other)
        {
            std::cout<<"Linking "<<this->FileName<<" With "<<other->FileName<<std::endl;
            this->bufvec->clear();
            this->bufvec=other->bufvec;
        }

        void writeToFile()
        {
            
        }
};

#endif