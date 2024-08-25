#pragma once
#ifndef _MAIN_MEMORY_HPP
#define _MAIN_MEMORY_HPP

#include<iostream>
#include<string.h>
#include<vector>
#include "BufferHeader.hpp"
//Buffer States/Inode States
#define FREE 0
#define UNLOCKED 1
#define LOCKED 2
#define BUSY 3

//Extra definitions
#define INODE int
#define TAPE_HEAD int
#define FILENAME char*
#define TotalBuffers 3000
#define BufferSpace 50

typedef struct _Buffer{
    char space[BufferSpace];
    int Flag=FREE;
}Buffer;


class Buffer{
    public:
        char *space;
        int Flag;
        int BufferHeader;
        int BufferTape=0;
        Buffer()
        {
            space=(char *)malloc(sizeof(char)*BufferSpace);
            BufferHeader=bh.AllocateBuffer();
            Flag=FREE;
        }

        Buffer(Buffer& b)
        {
            space=(char *)malloc(sizeof(char)*BufferSpace); /*Shallow Copying avoids pointing to same address*/
            std::cout<<"Buffer Created through shallow copying"<<std::endl;
            Flag=FREE;
            strcpy(this->space,b.space);
            Flag=b.Flag;
        }

        Buffer& operator=(Buffer other)
        {
            strcpy(this->space,other.space); /*Shallow Copying avoids pointing to same address*/
            this->Flag=other.Flag;
            std::cout<<"Buffer copied to another buffer using shallow copying"<<std::endl;
            this->BufferHeader=other.BufferHeader;
            return *this;
        }

        void copy(Buffer *other)
        {
            std::cout<<"Buffer copied to another buffer using Deep copying"<<std::endl;
            this->space=other->space;
            this->Flag=other->Flag;
        }

        void info()
        {
            std::cout<<"Buffer's Header number is "<<BufferHeader<<std::endl;
            std::cout<<"Buffer's text is "<<space<<std::endl;
            std::cout<<"Buffer's current flag is "<<Flag<<std::endl;
        }

        void Write(char *str)
        {
            if(strlen(str)>50)
            {
                std::cout<<"Buffer out of bound"<<std::endl;
            }
            strcpy(this->space,str);
            return;
        }

        void WriteCh(char ch)
        {
            space[BufferTape++]=ch;
            return;
        }
};

struct Node{
    Buffer *b;
    Node *next;
    Node *prev;
};
typedef struct _MainMemory{
    Buffer buffers[TotalBuffers]; /*Total Memory= 50*60=3000 Bytes*/
    int OccBuffers=0; /*Total Occupied Buffers*/
    TAPE_HEAD tHead=0; /*Next Free Block*/
}MainMemory;

typedef struct _File{
    FILENAME Filename; /*File name*/
    std::vector <Buffer> FileBuffers; /*Buffers associatedwith this file*/
}File;
class BufferTable{
    private:
        Node *head;
        Node *tail;
        int totalB=0;
        void **Table[4];
    public:
        BufferTable()
        {
            std::cout<<"Freelist created"<<std::endl;
            head=(Node *)malloc(sizeof(Node));
            head->b=NULL;
            head->next=head;
            head->prev=head;
            tail=head;
            totalB=0;
            for(int i=0;i<4;i++)
            {
                Table[i]=(void **)malloc(sizeof(void *)*totalB);
            }
        }

        void AddBuffer(Buffer *bf)
        {
            Node *temp=(Node *)malloc(sizeof(Node));
            temp->b=bf;
            temp->next=head;
            head->prev=temp;
            tail->next=temp;
            temp->prev=tail;
            tail=temp;
            totalB++;
            for(int i=0;i<4;i++)
            {
                Table[i]=(void **)realloc(Table[i],sizeof(void *)*totalB);
            }
            Table[0][totalB-1]=(void *)(&bf->BufferHeader);
            Table[1][totalB-1]=(void *)(&bf->BufferTape);
            Table[2][totalB-1]=(void *)(&bf->Flag);
            Table[3][totalB-1]=(void *)bf->space;
        }

        Buffer RemBuffer(int i)
        {
            Node *temp=head->next;
            Node *prev;
            while(temp->b->BufferHeader!=i)
            {
                prev=temp;
                temp=temp->next;
            }
            prev->next=temp->next;
            temp->next->prev=prev;
            totalB--;
            for(int i=0;i<4;i++)
            {
                Table[i]=(void **)realloc(Table[i],sizeof(void *)*totalB);
            }
            free(temp);
        }

        void TableShow()
        {
            std::cout<<"BufferHeader\tBufferTape\tBufferFlag\tSpace\n";
            for(int i=0;i<totalB;i++)
            {
                std::cout<<*((int *)Table[0][i])<<"\t";
                std::cout<<"\t"<<*((int *)Table[1][i])<<"\t";
                std::cout<<"\t"<<*((int *)Table[2][i])<<"\t";
                std::cout<<"\t"<<((char *)Table[3][i])<<"\t";
                std::cout<<std::endl;
            }
        }
};
BufferTable bt;

typedef struct _CacheNode{
    int val;
    struct _CacheNode *next;
    struct _CacheNode *prev;
}CacheNode;

class BufferCache{
    private:
        CacheNode *head;
        CacheNode *tail;
        int totalNodes;
        int hit;
        int miss;
        int size;
    public:
        BufferCache()
        {
            size=6;
            totalNodes=0;
            head=(CacheNode *)malloc(sizeof(CacheNode));
            head->val=0;
            head->next=head;
            head->prev=head;
            tail=head;
            hit=0;
            miss=0;
        }

        void insert(int a)
        {
            if(totalNodes<6)
            {
                CacheNode *temp=(CacheNode *)malloc(sizeof(CacheNode));
                temp->val=a;
                tail->next=temp;
                temp->next=head;
                temp->prev=tail;
                head->prev=temp;
                tail=temp;
                size++;
                return;
            }

            else
            {
                CacheNode *search=head->next;
                while(search!=head)
                {
                    if(search->val==a)
                    {
                        this->remove(a);
                        this->insert(a);
                        return;
                    }
                    else
                    {
                        search=search->next;
                    }
                }

                this->remove(head->next->val);
                this->insert(a);
            }
        }

        int remove(int a)
        {
            CacheNode *temp=head->next;
            CacheNode *prev=head;
            while(temp!=head)
            {
                if(temp->val==a)
                {
                    prev->next=temp->next;
                    temp->next->prev=prev;
                    free(temp);
                    temp=NULL;
                    size--;
                    return 1;
                }
            }
            std::cout<<"Buffer Not Found"<<std::endl;
            return -1;
        }   
};

class BufferVector{
    public:
        Buffer** vec;
        int size;
    public:
        BufferVector()
        {
            vec=(Buffer **)malloc(sizeof(Buffer *)*0);
            std::cout<<"Buffer vector created"<<std::endl;
            size=0;
        }
        void pushBack(Buffer *b)
        {
            vec=(Buffer **)realloc(vec,sizeof(Buffer *)*(size+1));
            if(vec==NULL)
            {
                std::cout<<"Problem in allocating memory"<<std::endl;
                exit(-1);
            }
            bt.AddBuffer(b);
            size+=1;
            vec[size-1]=b;
        }

        void clear()
        {
            for(int i=0;i<size;i++)
            {
                bh.AddBuffer(vec[i]->BufferHeader);
                vec[i]=(Buffer *)realloc(vec[i],0);
                vec[i]=NULL;
            }
            std::cout<<"Buffer vector has been cleared"<<std::endl;
            vec=(Buffer **)realloc(vec,0);
            vec=NULL;
            size=0;
        }

        Buffer popBack(int n)
        {
            Buffer *temp=vec[n];
            for(int i=n+1;i<size;i++)
            {
                vec[i-1]=vec[i];
            }
            vec=(Buffer **)realloc(vec,sizeof(Buffer)*(size-1));
            size--;
        }

        Buffer* operator[](int i)
        {
            return vec[i];
        }
        int Size()
        {
            return size;
        }
};

class Files{
    public:
        FILENAME Filename;
        BufferVector bufVec;
        Files(char *name)
        {
            Filename=(FILENAME)malloc(sizeof(char)*30);
            strcpy(Filename,name);
            std::cout<<"File created named "<<Filename<<std::endl;
        }

        void copy(Files other)
        {
            bufVec.clear();
            std::cout<<"File makes copy of "<<other.Filename<<std::endl;
            for(int i=0;i<other.bufVec.Size();i++)
            {
                Buffer *temp=new Buffer(*(other.bufVec.vec[i]));
                bufVec.pushBack(temp);
            }
        }

        void cat()
        {
            for(int i=0;i<bufVec.size;i++)
            {
                std::cout<<bufVec.vec[i]->space<<std::endl;
            }
        }

        void BufferInfo()
        {
            for(int i=0;i<bufVec.Size();i++)
            {
                bufVec[i]->info();
                std::cout<<std::endl;
            }
        }

        void Write(char *FullStr)
        {
            int a=(strlen(FullStr)/47)+1;
            Buffer **b;
            b=(Buffer **)malloc(sizeof(Buffer *)*a);
            for(int i=0;i<a;i++)
            {
                b[i]=new Buffer();
                bufVec.pushBack(b[i]);
            }   

            int temp=0;

            for(int i=0;i<strlen(FullStr);i++)
            {
                bufVec[temp]->WriteCh(FullStr[i]);
                if((i+1)%47==0)
                {   
                    temp++;
                }
            }   
        }

        void Free()
        {
            bufVec.clear();
        }
};

MainMemory MM;
#endif