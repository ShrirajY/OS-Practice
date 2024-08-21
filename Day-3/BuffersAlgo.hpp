#pragma once
#ifndef _BUFFER_ALGO_HPP
#define _BUFFER_ALGO_HPP

#include "MainMemory.hpp"

struct Node{
    Buffer *b;
    Node *next;
    Node *prev;
};

class FreeListB{
    private:
        Node *head;
        Node *tail;
        int totalB;
        
    public:
        FreeListB()
        {
            std::cout<<"Freelist created"<<std::endl;
            head=(Node *)malloc(sizeof(Node));
            tail=(Node *)malloc(sizeof(Node));
            head->b=NULL;
            head->next=head;
            head->prev=head;
            tail=head;
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
        }

        Buffer RemBuffer()
        {
            Node *temp=head->next;
            Buffer bf=*(temp->b);
            head->next=temp->next;
            temp->next->prev=head;
            free(temp);
            return bf;
        }

        void show()
        {
            Node *temp=head->next;
            while(temp!=head)
            {
                std::cout<<temp->b->BufferHeader<<"\t";
                temp=temp->next;
            }
        }
};

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
            tail=(Node *)malloc(sizeof(Node));
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
#endif