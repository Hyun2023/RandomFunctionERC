
#include "iRRAM/lib.h"
#include "iRRAM/core.h"
#include "iRRAM.h"

#include "include/random-real.h"
#include "include/random-complex.h"

#include<vector>
#include<algorithm>
#include<chrono>
#include<random>
using namespace iRRAM;


class Wiener
{
    private:
        std::vector<std::pair<REAL,REAL>> paramList;
    public:
        Wiener()
        {
            paramList.push_back(std::make_pair(REAL(0),REAL(0)));
        }

        REAL getBoundary()
        {
            return paramList.back().second;
        }

        void printParamList()
        {
            for(auto it=paramList.begin();it!=paramList.end();++it)
            {
                cout<<setRwidth(20)<<(*it).first<<"  ,  "<<(*it).second<<"\n";
            }
        }

        REAL eval(REAL t)
        {
            REAL A,B,t_2,t_1;
            REAL m,var,res;
            bool flag=true;
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
                    flag=false;
                    break;
                }
            }
            if (flag)
            {
                res=gaussian_real(0,sqrt(t-paramList.back().first));
                paramList.push_back(std::make_pair(t,res));
            }
            else
            {
                m=A+(B-A)/(t_2-t_1)*(t-t_1);
                var=(t_2-t)*(t-t_1)/(t_2-t_1);
                res=gaussian_real(m,sqrt(var));
                paramList.insert(it,std::make_pair(t,res));
            }
            
            
            return res;
        }
};

void shuffle(std::vector<REAL> vec)
{

}


void compute()
{
    Wiener w1;
    // cout<<setRwidth(20)<<REAL(0)<<","<<REAL(0)<<"\n";
    std::vector<REAL> pList;
    // pList.push_back(REAL(0));
    for(int i=1;i<100;i++)
    {
        pList.push_back(uniform_real());
        // cout<<setRwidth(20)<<REAL(i)/REAL(10)<<","<<w1.eval(REAL(i)/REAL(10))<<"\n";
        // REAL t=w1.eval(REAL(i)/REAL(10));
    }

    cout<<"-------------------------------------------------------\n";
    for(REAL p:pList)
    {
        // cout<<setRwidth(20)<<p<<"\n";
        // cout<<setRwidth(20)<<p<<","<<w1.eval(p)<<"\n";
        w1.eval(p);
    }
    // for(REAL p:pList)
    // {
    //     cout<<setRwidth(20)<<p<<","<<w1.eval(p)<<"\n";
    // }
    // cout<<"-------------------------------------------------------\n";
    // cout<<setRwidth(20)<<REAL(1)<<","<<w1.eval(REAL(1))<<"\n";
    w1.printParamList();
}