#ifndef _BUFFER_HEADER_HPP
#define _BUFFER_HEADER_HPP
#include<vector>

class BufferHeader{
    private:
        std::vector<int> ArrayHeader[4];
        int CurrentBuffer;
    public:
        BufferHeader()
        {
            CurrentBuffer=0;
            for(int i=0;i<750;i=i+4)
            {
                for(int j=0;j<4;j++)
                {
                    ArrayHeader[j].push_back((i*4)+j);
                }
            }
        }

        int AllocateBuffer()
        {
            for(int i=0;i<4;i++)
            {
                if(ArrayHeader[(CurrentBuffer+i)%4].size()!=0)
                {
                    int a=ArrayHeader[(CurrentBuffer+i)%4][ArrayHeader[(CurrentBuffer+i)%4].size()-1];
                    ArrayHeader[(CurrentBuffer+i)%4].pop_back();
                    CurrentBuffer=(((CurrentBuffer+i)%4)+1)%4;
                    return a;
                }
            }
            return -1;
        }

        void AddBuffer(int b)
        {
            ArrayHeader[b%4].push_back(b);
            std::cout<<"Buffer added to "<<b%4<<" numbered "<<b<<std::endl;
            return;
        }
};



BufferHeader bh;
#endif