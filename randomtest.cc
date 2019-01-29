#include "iRRAM.h"
#include "irram-random.h"
using namespace iRRAM;

void compute()
{
    for(int i=100;i<=150;i++)
        cout<<setRwidth(i)<<uniform_real()<<std::endl;
}