#include"common.h"
#include<iostream>
using namespace std;

class Mul:public AtomOperation
{
public:
    Mul():AtomOperation(2,1)
    {
        type=MUL;
    }
    virtual void Process() override
    {
        fan_out[0]=fan_in[0]*fan_in[1];
        _set_data();
    }
};
class Add:public AtomOperation
{
public:
    Add():AtomOperation(2,1)
    {
        type=ADD;
    }
    virtual void Process() override
    {
        fan_out[0]=fan_in[0]+fan_in[1];
        _set_data();
    }
};
class Register:public AtomOperation
{
public:

    Register(int a,int b):AtomOperation(a,b)
    {  
        type = REGISTER;
    }
    virtual void Process() override
    {   
        for(int i=0;i<count_fan_out;i++)
        {
            fan_out[i] = fan_in[0];
        }
        _set_data();
    }
};
class Bobble:public Register
{
public:
    Bobble():Register(1,1)
    {
        type=BOBBLE;
    }
};



class PE:public AtomOperation
{
private:
     RegisterSet reg_set;
     FunctionUnitSet fun_set;
     Wires wires;
public:
    PE():AtomOperation(2,1)
    { 
        reg_set.push_back(new Register(1,1));
        reg_set.push_back(new Register(1,1));
        reg_set.push_back(new Register(1,1));
        reg_set.push_back(new Register(1,1));
        reg_set.push_back(new Register(1,1));
        reg_set.push_back(new Register(1,2));

        fun_set.push_back(new Mul);
        fun_set.push_back(new Add);

        wires=new Wire[8];
        wires[0](reg_set[0],fun_set[0]);
        wires[1](reg_set[1],fun_set[0]);
        wires[2](fun_set[0],reg_set[2]);
        wires[3](reg_set[2],reg_set[3]);
        wires[4](reg_set[3],fun_set[1]);
        wires[5](reg_set[4],fun_set[1]);
        wires[6](fun_set[1],reg_set[5]);
        wires[7](reg_set[5],reg_set[4]);
    }
    virtual void Process()
    {
        reg_set[0]->Input(0,fan_in[0]);
        reg_set[1]->Input(0,fan_in[1]);
        EXECUTE;
        fan_out[0]= reg_set[5]->Output(1);
        /*
        cout<<modules[3]->Output(0)<<' '<<modules[4]->GetFuncunit()->fan_in[0]<<' '<<modules[4]->GetFuncunit()->fan_in[1]
        <<' '<<modules[4]->Output(0)<<' '<<modules[5]->fan_in[0]<<' '<<modules[5]->Output(0)<<std::endl;
   */ }
};

int main()
{  
    PE *pe=new PE;
    for(int i=1;i<50;i++)
    {
        pe->Input(0,i);
        pe->Input(1,i);
        pe->Process();
         cout<<pe->Output(0)<<endl; 
    }
     
    return 0;
}