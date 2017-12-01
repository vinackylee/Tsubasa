#ifndef __PE_ARRAY_H__
#define __PE_ARRAY_H__

#include "pe.h"
#include <vector>
#include <thread>
#include <mutex>
#include <queue>
#include<iostream>
using namespace std;

class PeArray : public Operation
{
  private:
    std::vector<std::vector<PE>> pe_array;
    int row;
    int column;
    Wires wire;
    /*std::queue<Task> task_queue;

    std::thread  process_impl;
    std::mutex  lock_task_queue;
    std::condition_variable cond;
    void run_task()
    {
        while(true)
        {   
            Task task;
            std::unique_lock<std::mutex> lock{lock_task_queue};
            cond.wait(lock,[this]{return !this->task_queue.empty()});
            task  = std::move(task_queue.front());
            task_queue.pop();
        }
    }*/
  public:
    PeArray(int a, int b);
    virtual void Process() override;
};
PeArray::PeArray(int a, int b) : Operation(a + b + 1, 2 * a)
{
    row = a;
    column = b;
    for (int i = 0; i < a; i++)
    {
        std::vector<PE> temp;
        for (int j = 0; j < b; j++)
        {
            temp.emplace_back();
        }
        pe_array.push_back(temp);
    }
    int num = 8 * a * b - 4 * a - 4 * b;
    for (int i = 0; i < num; i++)
    {
        wire.emplace_back();
    }
    int index = 0;
    for (int i = 0; i < a; i++)
    {
        for (int j = 0; j < b; j++)
        {
            if (i > 0)
            {
                wire[index++](&pe_array[i][j], &pe_array[i - 1][j], 0, 4);
                wire[index++](&pe_array[i][j], &pe_array[i - 1][j], 4, 8);
            }
            if (i < a - 1)
            {
                wire[index++](&pe_array[i][j], &pe_array[i + 1][j], 1, 3);
                wire[index++](&pe_array[i][j], &pe_array[i + 1][j], 5, 7);
            }
            if (j > 0)
            {
                wire[index++](&pe_array[i][j], &pe_array[i][j - 1], 2, 6);
                wire[index++](&pe_array[i][j], &pe_array[i][j - 1], 6, 10);
            }
            if (j < b - 1)
            {
                wire[index++](&pe_array[i][j], &pe_array[i][j + 1], 3, 5);
                wire[index++](&pe_array[i][j], &pe_array[i][j + 1], 7, 9);
            }
        }
    }
}

void PeArray::Process()
{
    for(int i=0;i<row;i++)
    {
        for(int j=0;j<column;j++)
        {
            pe_array[i][j].Input(0,fan_in[i]);
            pe_array[i][j].Input(1,fan_in[row+j]);
            pe_array[i][j].Input(2,fan_in[row+column]);
            pe_array[i][j].Process();
        }
    }
    for(int i=0;i<row;i++)
    {
        fan_out[2*i]=pe_array[i][column-1].Output(3);
        fan_out[2*i+1]=pe_array[i][column-1].Output(7);
    }
}

#endif