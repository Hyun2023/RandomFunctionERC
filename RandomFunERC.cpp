
#include "iRRAM/lib.h"
#include "iRRAM/core.h"
#include "iRRAM.h"

#include "include/random-real.h"
#include "include/random-complex.h"

#include<vector>
#include<functional>
#include<map>
using namespace iRRAM;


class Wiener
{
    private:
        std::vector<std::pair<REAL,REAL>> paramList;
    public:
        Wiener()
        {
            paramList.push_back(std::make_pair(REAL(0),REAL(0)));
            paramList.push_back(std::make_pair(REAL(1),gaussian_real()));
        }

        REAL getBoundary()
        {
            return paramList.back().second;
        }

        void printParamList()
        {
            for(auto it=paramList.begin();it!=paramList.end();++it)
            {
                cout<<setRwidth(20)<<(*it).first<<"\n"<<(*it).second<<"\n";
            }
        }

        REAL eval(REAL t)
        {
            REAL A,B,t_2,t_1;
            auto it=paramList.begin();
            for(;it!=paramList.end();it++)
            {
                if(t<(*it).first)
                {
                    t_2=(*it).first;
                    B=(*it).second;
                    it--;
                    t_1=(*it).first;
                    A=(*it).second;
                    it++;
                    break;
                }
            }
            REAL m=A+(B-A)/(t_2-t_1)*(t-t_1);
            REAL var=(t_2-t)*(t-t_1)/(t_2-t_1);
            cout<<t_1<<" , "<<t_2<<" , "<<A<<" , "<<B<<"\n";
            REAL res = var*var*gaussian_real()+m;
            paramList.insert(it,std::make_pair(t,res));
            return res;
        }
};


void compute()
{
    Wiener w1;
    // cout<<setRwidth(20)<<REAL(0)<<","<<REAL(0)<<"\n";
    for(int i=1;i<10;i++)
    {
        // cout<<setRwidth(20)<<REAL(i)/REAL(10)<<","<<w1.eval(REAL(i)/REAL(10))<<"\n";
        REAL t=w1.eval(REAL(i)/REAL(10));
    }
    // cout<<setRwidth(20)<<REAL(1)<<","<<w1.getBoundary()<<"\n";
}