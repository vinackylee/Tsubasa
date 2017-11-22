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
        _set_data();
        fan_out[0]=fan_in[0]*fan_in[1];
    }
};
class Add:public AtomOperation
{
public:
    Add():AtomOperation(2,1)
    {
        type=MUL;
    }
    virtual void Process() override
    {
        _set_data();
        fan_out[0]=fan_in[0]+fan_in[1];
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
        _set_data();
        if(attached_func_unit!=nullptr)
            attached_func_unit->Process();

        for(int i=0;i<count_fan_out;i++)
        {
                
                if(attached_func_unit!=nullptr)
                    fan_out[i] = attached_func_unit->Output(0);
                else
                    fan_out[i] = fan_in[0];
        }

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
     Modules modules;
     Lines   lines;
public:
    PE():AtomOperation(2,1)
    { 
         modules.push_back(new Register(1,1));
         modules.push_back(new Register(1,1));
         modules.push_back(new Register(1,1));
         modules.push_back(new Register(1,1));
         modules.push_back(new Register(1,1));
         modules.push_back(new Register(1,2));
         AtomOperation *mul = new Mul;
         AtomOperation *add = new Add;
         AtomOperation *bobble = new Bobble;

         modules[2]->SetFuncunit(mul);
         modules[3]->SetFuncunit(bobble);
         modules[5]->SetFuncunit(add);

         lines = new Line[9];
         lines[0](modules[0],modules[2]->GetFuncunit());
         lines[1](modules[1],modules[2]->GetFuncunit());
         lines[2](modules[2]->GetFuncunit(),modules[2]);
         lines[3](modules[2],modules[3]->GetFuncunit());
         lines[4](modules[3]->GetFuncunit(),modules[3]);
         lines[5](modules[3],modules[5]->GetFuncunit());
         lines[6](modules[4],modules[5]->GetFuncunit());
         lines[7](modules[5]->GetFuncunit(),modules[5]);
         lines[8](modules[5],modules[4]);
    }
    virtual void Process()
    {
        modules[0]->Input(0,fan_in[0]);
        modules[1]->Input(0,fan_in[1]);
        EXECUTE;
        fan_out[0]= modules[5]->GetFuncunit()->Output(0);
        cout<<modules[3]->Output(0)<<' '<<modules[5]->GetFuncunit()->fan_in[0]<<' '<<modules[5]->GetFuncunit()->fan_in[1]
        <<' '<<modules[5]->Output(0)<<' '<<modules[4]->fan_in[0]<<' '<<modules[4]->Output(0)<<std::endl;
    }
};

int main()
{  
    PE *pe=new PE;
    for(int i=1;i<10;i++)
    {
        pe->Input(0,i);
        pe->Input(1,i);
        pe->Process();

    }
   cout<<pe->Output(0)<<endl;    
    return 0;
}