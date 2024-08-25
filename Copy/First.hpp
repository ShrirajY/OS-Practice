#ifndef _FIRST_HPP
#define _FIRST_HPP

class First{
    public:
    virtual int ShowFirst()=0;
    virtual int SecondShow()=0;

    static First* Create();
};

#endif