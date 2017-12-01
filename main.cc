#include"pe_array.h"


int A[20][20],B[20][20];

int main()
{  
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            A[i][j]=1;
            B[i][j]=1;
        }
    }
    PeArray *pe_array = new PeArray(4,4);
    for(int i=0;i<11;i++)
    {
        pe_array->Input(0,A[0+i%2][i/2]);
        pe_array->Input(1,A[2+i%2][i/2]);
        pe_array->Input(2,A[4+i%2][i/2]);
        pe_array->Input(3,A[6+i%2][i/2]);
        pe_array->Input(4,B[0+i%2][i/2]);
        pe_array->Input(5,B[2+i%2][i/2]);
        pe_array->Input(6,B[4+i%2][i/2]);
        pe_array->Input(7,B[6+i%2][i/2]);
        pe_array->Input(8,(i%2)*100);
        pe_array->Process();
    }
    Print(pe_array,8);
    Print(Size<int,char,double>::value);
    return 0;
}