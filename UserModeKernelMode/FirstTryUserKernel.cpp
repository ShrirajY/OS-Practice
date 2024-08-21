#include<iostream>
#include<vector>
class KernelMode{

    //KernelStack
    protected:
    std::vector<void *> KStack;
    public:
        void pusha(void *ptr)
        {
            if(KStack.size()==0)
            {  
                std::cout<<"Exiting User mode"<<std::endl; 
                std::cout<<"Entered Kernel mode"<<std::endl;
            }
            KStack.push_back(ptr);
            return;
        }

        void popa()
        {
            if(KStack.size()==1)
            {
                std::cout<<"Exiting kernel mode"<<std::endl;
                std::cout<<"Entering Kernel mode"<<std::endl;
            }
            KStack.pop_back();
        }
};

class UserMode: private KernelMode{
    //Userstack
    protected:
    std::vector<void *> UStack;
    public:
        void push(void *ptr)
        {
            if(KStack.size()!=0)
            {
                std::cout<<"You can not execute!!\nKernel function is getting executed"<<std::endl;
                return;
            }
            UStack.push_back(ptr);
            
            return;
        }

        void pop()
        {
            if(UStack.size()==1)
            {
                std::cout<<"Exiting kernel mode"<<std::endl;
            }
            UStack.pop_back();
        }
};

UserMode u;
int main()
{
    // u.popa();
}
/*Now we have to apply multithreading such that one thread will keep eye on which function should get execute
it should avoid usermode function getting executed when any kernel function is getting executed
and multithreading should be allowed in usermode such that while executing any usermode function other functions can also be executed*/