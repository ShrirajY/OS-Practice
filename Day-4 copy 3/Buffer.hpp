#ifndef _BUFFER_HPP
#define _BUFFER_HPP

#include <iostream>
#include <cstring>
#include <fstream>
#define FREE 0
#define LOCKED 1
#define MAXBUFSIZE 50

struct Buffer
{
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

class BufferFList
{
private:
    Buffer *Fhead;
    Buffer *Ftail;
    int NoBuffers;

public:
    BufferFList()
    {
        Fhead = (Buffer *)malloc(sizeof(Buffer));
        Fhead->BufHeader = -1;
        Fhead->FListPrev = Fhead;
        Fhead->FListNext = Fhead;
        NoBuffers = 0;
        Ftail = Fhead;
    }

    void AddBuffer(Buffer *temp)
    {
        Ftail->FListNext = temp;
        temp->FListPrev = Ftail;
        temp->FListNext = Fhead;
        Fhead->FListPrev = temp;
        Ftail = temp;
        NoBuffers++;
        std::cout << "Added in FList: " << Ftail->BufHeader << std::endl;
    }

    Buffer *AllocateBufferFList()
    {
        Buffer *temp = Fhead->FListNext;
        Fhead->FListNext = temp->FListNext;
        temp->FListNext->FListPrev = Fhead;
        temp->FListNext = NULL;
        temp->FListPrev = NULL;
        NoBuffers--;
        temp->status = LOCKED;
        if (NoBuffers == 0)
        {
            Fhead = (Buffer *)realloc(Fhead, sizeof(Buffer));
            Fhead->BufHeader = -1;
            Fhead->FListPrev = Fhead;
            Fhead->FListNext = Fhead;
            NoBuffers = 0;
            Ftail = Fhead;
        }
        return temp;
    }

    int NoBuffersShow()
    {
        return NoBuffers;
    }

    void show()
    {
        Buffer *temp = Fhead->FListNext;
        while (temp != Fhead)
        {
            std::cout << temp->BufHeader << "\t";
            temp = temp->FListNext;
        }
        std::cout << std::endl;
    }
};

BufferFList BFList;
class BufferPool : public BufferFList
{
private:
    Buffer *head;
    int NoBuffer;
    Buffer *rememberedBuffer;

public:
    BufferPool()
    {
        head = Create(0);
        rememberedBuffer = head;
        Buffer *prev = head;
        for (int i = 1; i < 1500; i++)
        {
            Buffer *temp = Create(i);
            prev->poolNext = temp;
            temp->poolPrev = prev;
            prev = temp;
        }
        AddBuffersToList();
    }

    Buffer *Create(int i)
    {
        Buffer *temp = (Buffer *)malloc(sizeof(Buffer));
        temp->BufHeader = i;
        temp->status = FREE;
        temp->FListNext = NULL;
        temp->FListPrev = NULL;
        temp->poolNext = NULL;
        temp->poolPrev = NULL;
        temp->VecNext = NULL;
        temp->VecPrev = NULL;
        temp->Bsize = 0;
        temp->space = (char *)malloc(sizeof(char) * 60);
        return temp;
    }

    Buffer *AllocatBuffer()
    {
        Buffer *temp = BFList.AllocateBufferFList();
        if (BFList.NoBuffersShow() == 0)
        {
            rememberedBuffer = temp->poolNext;
            AddBuffersToList();
        }
        return temp;
    }

    void AddBuffersToList()
    {
        int target = 0;
        Buffer *temp = rememberedBuffer;
        BFList.AddBuffer(temp);
        temp = temp->poolNext;
        std::cout << "\nCame Here again\n";
        while (target != 3 && temp != rememberedBuffer)
        {
            if (temp->status == FREE)
            {
                std::cout << "Added" << std::endl;
                BFList.AddBuffer(temp);
                target++;
                NoBuffer++;
            }
            temp = temp->poolNext;
        }
    }
};

void WriteToBuffer(Buffer *b, char ch)
{
    if (b->Bsize < MAXBUFSIZE)
    {
        b->space[b->Bsize++] = ch;
    }
}

BufferPool BPool;
class BufVec
{
private:
    Buffer *tail;
    int NoBuffers;
    Buffer *CurrBuff;
    

public:
    bool LINKFLAG;
    Buffer *head;
    Buffer *CurrBuffWrite;
    int BufTapeWrite;
    char *FileNameBufVec;
    std::fstream fileStream;
    char *openFile;
    BufVec *other;
    BufVec(char *_Name)
    {
        head = (Buffer *)malloc(sizeof(Buffer));
        head->BufHeader = -1;
        head->VecNext = head;
        head->VecPrev = head;
        tail = head;
        NoBuffers = 0;
        CurrBuff = head;
        LINKFLAG = false;
        FileNameBufVec = (char *)malloc(sizeof(char) * 200);
        strcpy(FileNameBufVec,_Name);

        char *FOLDER_PATH = strdup("E://Current Work//OS Practice//AllFiles//");
        openFile = (char *)malloc(sizeof(char) * 200);
        sprintf(openFile, "%s%s", FOLDER_PATH, FileNameBufVec);
        fileStream.open(openFile, std::ios::out | std::ios::app);
        if (!fileStream)
        {
            std::cerr << "Failed to open file: " << FileNameBufVec << std::endl;
        }
    }

    void insert(Buffer *a)
    {
        if (LINKFLAG == false)
        {
            if (head->BufHeader == -1)
            {
                head = a;
                head->VecNext = head;
                head->VecPrev = head;
                tail = head;
                NoBuffers++;
                CurrBuffWrite = head;
                BufTapeWrite = 0;
                return;
            }
            tail->VecNext = a;
            a->VecPrev = tail;
            a->VecNext = head;
            head->VecPrev = a;
            tail = a;
            NoBuffers++;
            return;
        }
        else
        {
            head = a;
            tail = head->VecPrev;
            LINKFLAG = false;
            return;
        }
    }

    void remove(Buffer *a)
    {

    }

    void write(char *str)
    {
        if (head->BufHeader == -1)
        {
            this->insert(BPool.AllocatBuffer());
            CurrBuff = head;
        }
        for (int i = 0; i < strlen(str); i++)
        {
            if (CurrBuff->Bsize >= MAXBUFSIZE)
            {
                CurrBuff->space[CurrBuff->Bsize] = '\0';
                if (CurrBuff->VecNext == head)
                {
                    Buffer *temp = BPool.AllocatBuffer();
                    this->insert(temp);
                }
                CurrBuff = CurrBuff->VecNext;
            }
            WriteToBuffer(CurrBuff, str[i]);
        }
        CurrBuff->space[CurrBuff->Bsize] = '\0';
        this->WriteToFile();
        if(LINKFLAG==true)
        {
            other->WriteToFile();
        }
    }

    void show()
    {
        Buffer *temp = head;
        // std::cout<<std::endl;
        for (int i = 1; i <= NoBuffers; i++)
        {
            std::cout << temp->space;
            temp = temp->VecNext;
        }
    }

    void ShowBufHeaders()
    {
        Buffer *temp = head;
        for (int i = 1; i <= NoBuffers; i++)
        {
            std::cout << temp->BufHeader << "\t";
            temp = temp->VecNext;
        }
        std::cout << std::endl;
    }

    void clear()
    {
        Buffer *temp = head;
        Buffer *next;
        for (int i = 1; i <= NoBuffers; i++)
        {
            next = temp->VecNext;
            temp->VecNext = NULL;
            temp->VecPrev = NULL;
            temp->space = strdup("");
            temp->Bsize = 0;
            temp->status = FREE;
        }
        head = (Buffer *)malloc(sizeof(Buffer));
        head->BufHeader = -1;
        head->VecNext = head;
        head->VecPrev = head;
        tail = head;
        NoBuffers = 0;
        CurrBuff = head;
        CurrBuffWrite = head;
        BufTapeWrite = 0;
    }

    int NoBuffersRet()
    {
        return this->NoBuffers;
    }

    void copy(BufVec *other)
    {
        std::ofstream fileStream2(openFile, std::ofstream::trunc);
        fileStream2.close();
        std::cout << "File Truncated First" << std::endl;
        this->clear();
        Buffer *temp = other->head;
        for (int i = 1; i <= other->NoBuffersRet(); i++)
        {
            this->write(temp->space);
            temp = temp->VecNext;
        }
        this->WriteToFile();
        if(LINKFLAG==true)
        {
            other->WriteToFile();
        }
    }

    void link(BufVec *other)
    {
        this->clear();
        this->LINKFLAG = true;
        this->insert(other->head);
        this->LINKFLAG = false;
        this->NoBuffers = other->NoBuffers;
    }

    BufVec* operator=(BufVec *other)
    {
        this->head=other->head;
        this->NoBuffers=other->NoBuffers;
        this->CurrBuff=other->CurrBuff;
        std::cout<<"Flow here came"<<std::endl;
        return this;
    }

    void AssignOperator(BufVec* other)
    {
        this->CurrBuff=other->CurrBuff;
        this->head=other->head;
        this->NoBuffers=other->NoBuffers;
        this->tail=other->tail;
        this->BufTapeWrite=other->BufTapeWrite;
        this->CurrBuffWrite=other->CurrBuffWrite;
        this->FileNameBufVec=other->FileNameBufVec;
        this->LINKFLAG=other->LINKFLAG;
        this->openFile=other->openFile;
        this->other=other->other;
        std::cout<<"Shwoing Content"<<std::endl;
        this->show();
    }

    void WriteToFile()
    {
        if (fileStream.is_open())
        {
            Buffer *temp = this->CurrBuffWrite;
            if (temp == nullptr)
            {
                std::cerr << "No buffers to write." << std::endl;
                return;
            }

            do
            {
                // Debug: print the content of each buffer
                std::cout << "Writing buffer: " << temp->space << std::endl;

                if (temp != this->CurrBuffWrite)
                {
                    fileStream.write(temp->space, temp->Bsize);
                }

                else
                {
                    for (size_t i = this->BufTapeWrite; i < temp->Bsize; ++i)
                    {
                        // fputc(temp->space[i], fileStream); // Write one character at a time
                        fileStream.put(temp->space[i]);
                    }
                }
                // Move to the next buffer
                temp = temp->VecNext;

            } while (temp != this->head); // Iterate until we circle back to head

            this->CurrBuffWrite = temp->VecPrev;
            this->BufTapeWrite = temp->VecPrev->Bsize;
            fileStream.flush(); // Ensure all data is written to the file
        }
        else
        {
            std::cerr << "File stream is not open." << std::endl;
        }
    }

    void WriteToFileOther(BufVec *other)
    {
        if (other->fileStream.is_open())
        {
            Buffer *temp = other->CurrBuffWrite;
            if (temp == nullptr)
            {
                std::cerr << "No buffers to write." << std::endl;
                return;
            }

            do
            {
                // Debug: print the content of each buffer
                std::cout << "Writing buffer: " << temp->space << std::endl;

                if (temp != other->CurrBuffWrite)
                {
                    other->fileStream.write(temp->space, temp->Bsize);
                }
                else
                {
                    for (size_t i = other->BufTapeWrite; i < temp->Bsize; ++i)
                    {
                        // fputc(temp->space[i], fileStream); // Write one character at a time
                        other->fileStream.put(temp->space[i]);
                    }
                }
                // Move to the next buffer
                temp = temp->VecNext;

            } while (temp != other->head); // Iterate until we circle back to head

            other->CurrBuffWrite = temp->VecPrev;
            other->BufTapeWrite = temp->VecPrev->Bsize;
            other->fileStream.flush(); // Ensure all data is written to the file
        }
        else
        {
            std::cerr << "File stream is not open." << std::endl;
        }
    }
};
#endif