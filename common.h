
/************************************************
class derived from AtomOperation is a harware part;
class Connect connect two AtomOperations;
modules run at the same time 
************************************************/
#ifndef __COMMON_H__
#define __COMMON_H__

#include<cassert>
#include<functional>
#include<algorithm>
#include<cstring>
#include<vector>
#include<iostream>
#define TEST
#define IN
#define OUT
#define WORD_TYPE  int


#define EXECUTE for_each(regs.begin(),regs.end(),[](Operation* e){e->Process();});\
                for_each(wires.begin(),wires.end(),[](Connect e){\
                    e._set_data();});  
#define SET_ALIAS(src,alias) auto &alias = src;              


/**
* @brief the base class of all hardware modules
*/
class Operation {
#ifndef TEST
protected:
#else
public:
#endif
    WORD_TYPE *fan_out{nullptr}; ///< palce the result of this operation
    WORD_TYPE count_fan_out;      ///< total of fan_out
    WORD_TYPE *fan_in{nullptr};  ///< palce of operands and controls of this module
    WORD_TYPE count_fan_in;       ///< total of fan_in
//    WORD_TYPE current_connect_in=-1; ///< the current 
//    WORD_TYPE current_connect_out=-1;
    bool is_register{true};
public:
    Operation(WORD_TYPE a,WORD_TYPE b)
    {
        fan_in = new WORD_TYPE[a];
        fan_out = new WORD_TYPE[b];
        count_fan_in = a;
        count_fan_out = b;
        memset(fan_in,0,a*sizeof(WORD_TYPE));
        memset(fan_out,0,b*sizeof(WORD_TYPE));
    }
   const bool& GetType() const
    {
        return is_register;
    }
    const WORD_TYPE& Output(WORD_TYPE index)const
    {
        assert(count_fan_out > index);
        return fan_out[index];
    }
    void Input(WORD_TYPE index, WORD_TYPE value)
    {
        assert(count_fan_in>index);
        fan_in[index]=value;
    }
/*  
    WORD_TYPE& GetCurrentIn()
    {
        return current_connect_in;
    }
    WORD_TYPE & GetCurrentOut()
    {
        return current_connect_out;
    }
 */
    const WORD_TYPE& GetCountFanIn()const
    {
        return count_fan_in;
    }
    const WORD_TYPE& GetCountFanOut()const
    {
        return count_fan_out;
    }
//    virtual void Initialize()=0;
    virtual void Process(){} 
};

class Connect {
#ifndef TEST
protected:
#else
public:
#endif
    Operation *m1;
    Operation *m2;
    WORD_TYPE num_port_in;
    WORD_TYPE num_port_out;
public:
    void _set_data()
    {    
         m2->fan_in[num_port_in]=m1->fan_out[num_port_out];
         if(!m2->GetType()) m2->Process();
    }
    void operator()(Operation *n1,Operation *n2,const WORD_TYPE &num_out,const WORD_TYPE &num_in)
    {   
        m1=n1;
        m2=n2;
        num_port_in=num_in;
        num_port_out=num_out;
 //       num_port_out=++m1->GetCurrentOut();
 //       num_port_in=++m2->GetCurrentIn();
        assert(m1->GetCountFanOut()>num_port_out);
        assert(m2->GetCountFanIn()>num_port_in);
    }

};
using Module = std::vector<Operation*>;
//using FunctionUnitSet = std::vector<AtomOperation*>;
using Wires = std::vector<Connect>;
using Wire = Connect;

/*

static void execute(Module regs,Wires wires)
{
    for_each(regs.begin(),regs.end(),[](Operation* e){e->Process();});
}

#define NEW_EXECUTE task_queue.push(std::bind(execute,regs,wires));

static void execute(Module regs,Wires wires)
{
    for_each(regs.begin(),regs.end(),[](Operation* e){e->Process();});
}


using Task = std::function<void()>;
*/
void Print()
{

}
template<typename First,typename ...Args>
void Print(First p1,Args... args)
{
    std::cout<<p1<<std::endl;
    Print(args...);
}
template<typename P>
void Print(std::vector<P> p)
{
        for(auto e:p)
            std::cout<<e<<std::endl;
}
template<typename T>
void Print(T *t,int i)
{       

        std::function<int(int)> f= std::bind(&Operation::Output,t,std::placeholders::_1);
        //const int&(Operation::*f)(int) const = &Operation::Output;
        for(int index = 0;index<i;++index)
           // std::cout<<(t->* f)(index)<<std::endl;
           std::cout<<f(index)<<std::endl;
}

template<typename... T>
class Size
{
};

template<typename T>
class Size<T>
{  public:
    static const int value{sizeof(T)};
};
template<typename T,typename... Args>
class Size<T,Args...>
{  public:
    static const int value = sizeof(T) +  Size<Args...>::value;
};



#endif