#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include"common.h"

class Register:public Operation
{
    // fan_in  is value in wire which is connected to this register from others
    // fan_out is the actual value of this  register 
public:
    Register(int b):Operation(1,b)
    {  
    }
    virtual void Process() override
    {   
        for(int i=0;i<count_fan_out;i++)
        {
            fan_out[i] = fan_in[0];
        }
    }
};

class Ttrigger:public Operation
{
public:
    Ttrigger():Operation(0,1)
    {

    }
    virtual void Process() override
    {
        fan_out[0]=1-fan_out[0];
    }
};

template<short cycle>
class Allocator:public Register
{
public:
    Allocator():Register(1)
    {

    }
};

/**
* @brief an allocator
* 2 input,the first one is data_in,the another is signal of control 
*/
template<>
class Allocator<1>:public Operation
{
public:
/**
* @brief initialize an allocator
* @param b output number
*/
    Allocator(int b):Operation(2,b)
    {
        is_register=false;
    }
    virtual void Process() override
    {
        assert(fan_in[1]<count_fan_out);
        fan_out[fan_in[1]]=fan_in[0];
    }
};

template<short cycle>
class Selector:public Register
{
public:
    Selector():Register(1)
    {

    }
};
template<>
class Selector<1>:public Operation
{
public:
    Selector(int a):Operation(a+1,1)
    {
        is_register=false;
    }
    virtual void Process() override
    {
        assert(fan_in[count_fan_in-1]<count_fan_in-1);
        fan_out[0]=fan_in[fan_in[count_fan_in-1]];
    }
};

template<short cycle>
class Mul:public Register
{
public:
    Mul():Register(1)
    {

    }
};
template<>
class Mul<1>:public Operation
{
public:
    Mul():Operation(2,1)
    {
        is_register=false;
    }
    virtual void Process() override
    {
        fan_out[0]=fan_in[0]*fan_in[1];
    }
};
template<short cycle>
class Add:public Register
{
public:
    Add():Register(1)
    {

    }
};
template<>
class Add<1>:public Operation
{
public:
    Add():Operation(2,1)
    {
        is_register=false;
    }
    virtual void Process() override
    {
        fan_out[0]=fan_in[0]+fan_in[1];
    }
};


#endif