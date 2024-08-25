#include<iostream>
#include "Second.hpp"

int Second::ShowFirst(){
    std::cout<<"This is First Show"<<std::endl;
}

int Second::SecondShow(){
    std::cout<<"This is second show"<<std::endl;
}

First* First::Create()
{
    return new Second();
}

int main()
{
    First *newClass=First::Create();
    newClass->SecondShow();
    newClass->ShowFirst();
}