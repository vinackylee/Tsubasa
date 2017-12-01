#ifndef __PE_H__
#define __PE_H__

#include"component.h"

class PE:public Operation
{
private:
     Module regs;
     Wires wires;
     Module func_units;
public:
/**
* @brief  0-a,1-b,2-control,3-up0,4-down0,5-left0,6-right0,7-up1,8-down1,9-left1,10-right1\n
*         0-up0,1-down0,2-left0,3-right0,4-up1,5-down1,6-left1,7-right1\n
*         for fan_in[2], fan_in[2]/100 is the signal for allocator_dst, (fan_in[2]/10)%10 is the signal for selector_share1,\n
          (fan_in[2]%10) is the signal for selector_share2
*/
    PE():Operation(11,8)
    { 
        regs.push_back(new Register(1));
        regs.push_back(new Register(1));
        regs.push_back(new Mul<2>);
        regs.push_back(new Register(1));
        regs.push_back(new Register(1));
        regs.push_back(new Register(2));
        regs.push_back(new Register(2));
        regs.push_back(new Register(1));
        regs.push_back(new Register(1));
        regs.push_back(new Ttrigger()); //control the allocator_dst and selector_acc
        
        SET_ALIAS(regs[0],mul_src_reg1)
        SET_ALIAS(regs[1],mul_src_reg2)
        SET_ALIAS(regs[2],mul_cycle_1)
        SET_ALIAS(regs[3],mul_dst_reg)
        SET_ALIAS(regs[4],add_acc_reg)
        SET_ALIAS(regs[5],add_dst_reg1)
        SET_ALIAS(regs[6],add_dst_reg2)
        SET_ALIAS(regs[7],pes_share_reg1)
        SET_ALIAS(regs[8],pes_share_reg2)
        SET_ALIAS(regs[9],t_trigger)

        Mul<1>* mul=new Mul<1>;
        Add<1>* add=new Add<1>;
        Allocator<1>* allocator_dst=new Allocator<1>(2);
        Selector<1>* selector_acc=new Selector<1>(2);
        Selector<1>* selector_share1=new Selector<1>(5);
        Selector<1>* selector_share2=new Selector<1>(5);

        func_units.push_back(mul);
        func_units.push_back(add);
        func_units.push_back(allocator_dst);
        func_units.push_back(selector_acc);
        func_units.push_back(selector_share1);
        func_units.push_back(selector_share2);

        for(int i=0;i<18;i++)
        {
            wires.emplace_back();
        }
        wires[0](mul_src_reg1,mul,0,0);
        wires[1](mul_src_reg2,mul,0,1);
        wires[2](mul,mul_cycle_1,0,0);
        wires[3](mul_cycle_1,mul_dst_reg,0,0);
        wires[4](mul_dst_reg,add,0,0);
        wires[5](add_acc_reg,add,0,1);
        wires[6](add,allocator_dst,0,0);
        wires[7](allocator_dst,add_dst_reg1,0,0);
        wires[8](allocator_dst,add_dst_reg2,1,0);
        wires[9](add_dst_reg2,selector_acc,0,0);
        wires[10](add_dst_reg1,selector_acc,0,1);
        wires[11](selector_acc,add_acc_reg,0,0);
        wires[12](add_dst_reg1,selector_share1,1,0);
        wires[13](add_dst_reg2,selector_share2,1,0);
        wires[14](selector_share1,pes_share_reg1,0,0);
        wires[15](selector_share2,pes_share_reg2,0,0);
        wires[16](t_trigger,allocator_dst,0,1);
        wires[17](t_trigger,selector_acc,0,2);
    }
    virtual void Process()
    {
#ifdef TEST
        assert(fan_in[2]/100==0||fan_in[2]/100==1);
        assert((fan_in[2]/10)%10<5);
        assert((fan_in[2]%10)<5);
#endif
        regs[0]->Input(0,fan_in[0]);
        regs[1]->Input(0,fan_in[1]);
       // func_units[2]->Input(1,fan_in[2]/100);
       // func_units[3]->Input(2,fan_in[2]/100);
        func_units[4]->Input(1,fan_in[3]);
        func_units[4]->Input(2,fan_in[4]);
        func_units[4]->Input(3,fan_in[5]);
        func_units[4]->Input(4,fan_in[6]);
        func_units[5]->Input(1,fan_in[7]);
        func_units[5]->Input(2,fan_in[8]);
        func_units[5]->Input(3,fan_in[9]);
        func_units[5]->Input(4,fan_in[10]);
        func_units[4]->Input(5,(fan_in[2]/10)%10);
        func_units[5]->Input(5,fan_in[2]%10);

        EXECUTE;

        fan_out[0] = regs[7]->Output(0);
        fan_out[1] = regs[7]->Output(0);
        fan_out[2] = regs[7]->Output(0);
        fan_out[3] = regs[7]->Output(0);
        fan_out[4] = regs[8]->Output(0);
        fan_out[5] = regs[8]->Output(0);
        fan_out[6] = regs[8]->Output(0);
        fan_out[7] = regs[8]->Output(0);
 /*     
        for(int i=0;i<5;i++)
            cout<<regs[i]->fan_in[0]<<'-'<<regs[i]->fan_out[0]<<' ';
        cout<<endl;
 */     

 //       std::vector<int> d{1,2,3,45,6};
 //        CPrint<int>c(d);
 //        Print(12,3,43,4);
     }
};

#endif