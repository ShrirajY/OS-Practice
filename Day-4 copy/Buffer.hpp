#ifndef _BUFFER_HPP
#define _BUFFER_HPP

#include<iostream>
#include<cstring>
#define FREE 0
#define LOCKED 1
#define MAXBUFSIZE 10

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
            std::cout<<"Created Free buffer's List"<<std::endl;
        }

        void AddBuffer(Buffer *temp)
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
            temp->status=LOCKED;
            std::cout<<"Buffer allocated with bufheader "<<temp->BufHeader<<" and with status LOCKED"<<std::endl;
            if(NoBuffers==0)
            {
                Fhead=(Buffer *)realloc(Fhead,sizeof(Buffer));
                Fhead->BufHeader=-1;
                Fhead->FListPrev=Fhead;
                Fhead->FListNext=Fhead;
                NoBuffers=0;
                Ftail=Fhead;
            }
            return temp;
        }

        int NoBuffersShow()
        {
            return NoBuffers;
        }

        void show()
        {
            Buffer *temp=Fhead->FListNext;
            std::cout<<"Buffers Currently Present in free list are"<<std::endl;
            while(temp!=Fhead)
            {
                std::cout<<temp->BufHeader<<"\t";
                temp=temp->FListNext;
            }
            std::cout<<std::endl;
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
            std::cout<<"Created Buffer Pool with inherited BFList"<<std::endl;
            head=Create(0);
            rememberedBuffer=head;
            Buffer *prev=head;
            for(int i=1;i<1500;i++)
            {
                Buffer* temp=Create(i);
                prev->poolNext=temp;
                temp->poolPrev=prev;
                prev=temp;
            }
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
                rememberedBuffer=temp->poolNext;
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
            while(target!=3 && temp!=rememberedBuffer)
            {
                if(temp->status==FREE)
                {
                    BFList.AddBuffer(temp);
                    target++;
                    NoBuffer++;
                }
                temp=temp->poolNext;
            }
            std::cout<<"Buffers are added to FList"<<std::endl;
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
        bool LINKFLAG;
    public:
        BufVec()
        {
            std::cout<<"BufVec Created"<<std::endl;
            head=(Buffer *)malloc(sizeof(Buffer));
            head->BufHeader=-1;
            head->VecNext=head;
            head->VecPrev=head;
            tail=head;
            NoBuffers=0;
            CurrBuff=head;
            LINKFLAG=false;
        }

        void insert(Buffer *a)
        {
            std::cout<<"Buffer added to BufVec"<<std::endl;
            if(LINKFLAG==false)
            {
                if(head->BufHeader==-1)
                {
                    head=a;
                    head->VecNext=head;
                    head->VecPrev=head;
                    tail=head;
                    NoBuffers++;
                    return;
                }
                tail->VecNext=a;
                a->VecPrev=tail;
                a->VecNext=head;
                head->VecPrev=a;
                tail=a;
                NoBuffers++;
                return;
            }
            else
            {
                head=a;
                tail=head->VecPrev;
                LINKFLAG=false;
                return;
            }
        }

        void remove(Buffer *a)
        {

        }

        void write(char *str)
        {
            std::cout<<"Buffer getting written"<<std::endl;
            if(head->BufHeader==-1)
            {
                this->insert(BPool.AllocatBuffer());
                CurrBuff=head;
            }
            for(int i=0;i<strlen(str);i++)
            {
                if(CurrBuff->Bsize>=MAXBUFSIZE)
                {
                    CurrBuff->space[CurrBuff->Bsize]='\0';
                    if(CurrBuff->VecNext==head)
                    {
                        Buffer *temp=BPool.AllocatBuffer();
                        this->insert(temp);
                    }
                    CurrBuff=CurrBuff->VecNext;
                }
                WriteToBuffer(CurrBuff,str[i]);
            }
            CurrBuff->space[CurrBuff->Bsize]='\0';
        }

        void show()
        {
            Buffer *temp=head;
            // std::cout<<std::endl;
            std::cout<<"Text present in File is"<<std::endl;
            for(int i=1;i<=NoBuffers;i++)
            {
                std::cout<<temp->space;
                temp=temp->VecNext;
            }
            std::cout<<std::endl;
        }

        void ShowBufHeaders()
        {
            Buffer *temp=head;
            std::cout<<"Showing Buffers present in BufVec"<<std::endl;
            for(int i=1;i<=NoBuffers;i++)
            {
                std::cout<<temp->BufHeader<<"\t";
                temp=temp->VecNext;
            }
            std::cout<<std::endl;
        }

        void clear()
        {
            Buffer *temp=head;
            Buffer *next;
            std::cout<<"Clearing Buffer"<<std::endl;
            for(int i=1;i<=NoBuffers;i++)
            {
                next=temp->VecNext;
                temp->VecNext=NULL;
                temp->VecPrev=NULL;
                temp->space=strdup("");
                temp->Bsize=0;
                temp->status=FREE;
            }
            head=(Buffer *)malloc(sizeof(Buffer));
            head->BufHeader=-1;
            head->VecNext=head;
            head->VecPrev=head;
            tail=head;
            NoBuffers=0;
            CurrBuff=head;
        }

        int NoBuffersRet()
        {
            return this->NoBuffers;
        }

        void copy(BufVec *other)
        {
            this->clear();
            Buffer *temp=other->head;
            std::cout<<"Copying another BufVec into this"<<std::endl;
            for(int i=1;i<=other->NoBuffersRet();i++)
            {
                this->write(temp->space);
                temp=temp->VecNext;
            }
        }

        void link(BufVec *other)
        {
            std::cout<<"Linking another BufVec into this"<<std::endl;
            this->clear();
            this->LINKFLAG=true;
            this->insert(other->head);
            this->LINKFLAG=false;
            this->NoBuffers=other->NoBuffers;
        }
};
#endif