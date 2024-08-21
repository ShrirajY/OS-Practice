#ifndef _INODE_HPP
#define _INODE_HPP
#include<iostream>
#include<vector>

#define FREE 0
#define Locked 1

struct Inode{
    int inodeNo;
    int status;
    Inode *poolNext;
    Inode *poolPrev;
    Inode *FListNext;
    Inode *FListPrev;
    Inode *CacheNext;
    Inode *CachePrev;
};

class InodeFList{
    private:
        Inode *Fhead;
        Inode *Ftail;
        int NoInodes;
    public:
        InodeFList()
        {
            Fhead=(Inode *)malloc(sizeof(Inode));
            Fhead->inodeNo=-1;
            Fhead->FListPrev=Fhead;
            Fhead->FListNext=Fhead;
            NoInodes=0;
            Ftail=Fhead;
        }

        void *AddInode(Inode *temp)
        {
            Ftail->FListNext=temp;
            temp->FListPrev=Ftail;
            temp->FListNext=Fhead;
            Fhead->FListPrev=temp;
            Ftail=temp;
            NoInodes++;
        }

        Inode *AllocateInodeFList()
        {
            Inode *temp=Fhead->FListNext;
            Fhead->FListNext=temp->FListNext;
            temp->FListNext->FListPrev=Fhead;
            temp->FListNext=NULL;
            temp->FListPrev=NULL;
            NoInodes--;
            return temp;
        }

        int NoInodesShow()
        {
            return NoInodes;
        }

        void show()
        {
            Inode *temp=Fhead->FListNext;
            while(temp!=Fhead)
            {
                std::cout<<temp->inodeNo<<std::endl;
                temp=temp->FListNext;
            }
            free(temp);
            temp=NULL;
        }
};
InodeFList IFList;
class InodePool:public InodeFList{
    private:
        Inode *head;
        int NoInode;
        Inode* rememberedInode;
    public:
        InodePool()
        {
            head=Create(0);
            rememberedInode=head;
            Inode *prev=head;
            for(int i=1;i<150;i++)
            {
                Inode* temp=Create(i);
                prev->poolNext=temp;
                temp->poolPrev=prev;
                prev=temp;
                free(temp);
                temp=NULL;
            }
            free(prev);
            AddInodesToList();
        }

        Inode* Create(int i)
        {
            Inode *temp=(Inode *)malloc(sizeof(Inode));
            temp->inodeNo=i;
            temp->status=FREE;
            temp->FListNext=NULL;
            temp->FListPrev=NULL;
            temp->poolNext=NULL;
            temp->poolPrev=NULL;
            temp->CacheNext=NULL;
            temp->CachePrev=NULL;
            return temp;
        }

        

        Inode* AllocatInode()
        {
            Inode *temp= IFList.AllocateInodeFList();
            if(IFList.NoInodesShow()==0)
            {
                AddInodesToList();
            }
            return temp;
        }

        void AddInodesToList()
        {
            int target=0;
            Inode *temp=rememberedInode;
            IFList.AddInode(temp);
            temp=temp->poolNext;
            while(target!=30 && temp!=rememberedInode)
            {
                if(temp->status==FREE)
                {
                    IFList.AddInode(temp);
                    target++;
                    NoInode++;
                }
                temp=temp->poolNext;
            }
            free(temp);
        }
};
InodePool IPool;

class InodeCache{
    private:
        Inode *Chead;
        Inode *Ctail;
        int size;
        int max;
    public:
        InodeCache()
        {
            max=8;
            size=0;
            Chead=(Inode *)malloc(sizeof(Inode));
            Chead->inodeNo=-1;
            Chead->CacheNext=Chead;
            Chead->CachePrev=Chead;
            Ctail=Chead;
        }

        void insert(Inode *a)
        {
            Inode *temp=Chead->CacheNext;
            int flag=0;
            while(temp!=Chead)
            {
                if(temp->inodeNo==a->inodeNo)
                {
                    a=this->remove(a);
                    flag=2;
                }
                temp=temp->CacheNext;
            }
            if(flag!=2)
            {
                this->remove(Chead->CacheNext);
            }
            Ctail->CacheNext=a;
            a->CachePrev=Ctail;
            a->CacheNext=Chead;
            Chead->CacheNext=a;
            Ctail=a;
            free(temp);
            temp=NULL;
            size++;
            return;
        }

        Inode* remove(Inode *a)
        {
            Inode *temp=Chead->CacheNext;
            Inode *prev=Chead;
            while(temp!=Chead)
            {
                if(temp->inodeNo==a->inodeNo)
                {
                    prev->CacheNext=temp->CacheNext;
                    temp->CacheNext->CachePrev=prev;
                    size--;
                    return temp;
                }
                temp=temp->CacheNext;
            }
            std::cout<<"Inode Not Found"<<std::endl;
            return NULL;
        }
};

InodeCache ICache;
#endif