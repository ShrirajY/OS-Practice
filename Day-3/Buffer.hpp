#ifndef _BUFFER_HPP
#define _BIFFER_HPP

#include<iostream>

#define FREE 0
#define LOCKED 1
#define MAXBUFSIZE 58

struct Buffer{
    int BufHeader;
    char *space;
    int status;
    int Bsize;
    Buffer *poolNext;
    Buffer *poolPrev;
    Buffer *FListNext;
    Buffer *FListPrev;
    Buffer *VecNext;
    Buffer *VecPrev;
};

class BufferFList{
    private:
        Buffer *Fhead;
        Buffer *Ftail;
        int NoBuffers;
    public:
        BufferFList()
        {
            Fhead=(Buffer *)malloc(sizeof(Buffer));
            Fhead->BufHeader=-1;
            Fhead->FListPrev=Fhead;
            Fhead->FListNext=Fhead;
            NoBuffers=0;
            Ftail=Fhead;
        }

        void *AddBuffer(Buffer *temp)
        {
            Ftail->FListNext=temp;
            temp->FListPrev=Ftail;
            temp->FListNext=Fhead;
            Fhead->FListPrev=temp;
            Ftail=temp;
            NoBuffers++;
        }

        Buffer *AllocateBufferFList()
        {
            Buffer *temp=Fhead->FListNext;
            Fhead->FListNext=temp->FListNext;
            temp->FListNext->FListPrev=Fhead;
            temp->FListNext=NULL;
            temp->FListPrev=NULL;
            NoBuffers--;
            return temp;
        }

        int NoBuffersShow()
        {
            return NoBuffers;
        }

        void show()
        {
            Buffer *temp=Fhead->FListNext;
            while(temp!=Fhead)
            {
                std::cout<<temp->BufHeader<<std::endl;
                temp=temp->FListNext;
            }
            free(temp);
            temp=NULL;
        }
};

BufferFList BFList;
class BufferPool:public BufferFList{
    private:
        Buffer *head;
        int NoBuffer;
        Buffer* rememberedBuffer;
    public:
        BufferPool()
        {
            head=Create(0);
            rememberedBuffer=head;
            Buffer *prev=head;
            for(int i=1;i<1500;i++)
            {
                Buffer* temp=Create(i);
                prev->poolNext=temp;
                temp->poolPrev=prev;
                prev=temp;
                free(temp);
                temp=NULL;
            }
            free(prev);
            AddBuffersToList();
        }

        Buffer* Create(int i)
        {
            Buffer *temp=(Buffer *)malloc(sizeof(Buffer));
            temp->BufHeader=i;
            temp->status=FREE;
            temp->FListNext=NULL;
            temp->FListPrev=NULL;
            temp->poolNext=NULL;
            temp->poolPrev=NULL;
            temp->VecNext=NULL;
            temp->VecPrev=NULL;
            temp->Bsize=0;
            temp->space=(char *)malloc(sizeof(char)*60);
            return temp;
        }

        

        Buffer* AllocatBuffer()
        {
            Buffer *temp= BFList.AllocateBufferFList();
            if(BFList.NoBuffersShow()==0)
            {
                AddBuffersToList();
            }
            return temp;
        }

        void AddBuffersToList()
        {
            int target=0;
            Buffer *temp=rememberedBuffer;
            BFList.AddBuffer(temp);
            temp=temp->poolNext;
            while(target!=60 && temp!=rememberedBuffer)
            {
                if(temp->status==FREE)
                {
                    BFList.AddBuffer(temp);
                    target++;
                    NoBuffer++;
                }
                temp=temp->poolNext;
            }
            free(temp);
        }
};

void WriteToBuffer(Buffer *b,char ch)
{
    if(b->Bsize<MAXBUFSIZE)
    {
        b->space[b->Bsize++]=ch;
    }
}

BufferPool BPool;
class BufVec{
    private:
        Buffer *head;
        Buffer *tail;
        int NoBuffers;
        Buffer *CurrBuff;
    public:
        BufVec()
        {
            head=(Buffer *)malloc(sizeof(Buffer));
            head->BufHeader=-1;
            head->VecNext=head;
            head->VecPrev=head;
            tail=head;
            NoBuffers=0;
            CurrBuff=head;
        }

        void insert(Buffer *a)
        {
            if(head->BufHeader==-1)
            {
                head=a;
                return;
            }
            tail->VecNext=a;
            a->VecPrev=tail;
            a->VecNext=head;
            head->VecPrev=a;
            NoBuffers++;
        }

        void remove(Buffer *a)
        {

        }

        void write(char *str)
        {
            if(head->BufHeader==-1)
            {
                this->insert(BPool.AllocatBuffer());
            }
            for(int i=0;i<strlen(str);i++)
            {
                if(CurrBuff->Bsize>=MAXBUFSIZE)
                {
                    if(CurrBuff->VecNext==head)
                    {
                        Buffer *temp=BPool.AllocatBuffer();
                        this->insert(temp);
                    }
                    CurrBuff=CurrBuff->VecNext;
                }
                WriteToBuffer(CurrBuff,str[i]);
            }
        }
};
#endif