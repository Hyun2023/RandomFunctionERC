
#include <vector>
#include <functional>
#include <map>

#include <iRRAM.h>

#include <random-real.h>

using namespace iRRAM;
using namespace random;

static REAL join(REAL a, REAL b)
{
    REAL  m = (a+b)/2;
    REAL  e = abs(a-b);
    m.adderror(e.vsize);
    return m;
}

static REAL slow_power2 (int p )
{
    REAL x = 1;
    for (int i =0; i<(-p); i++)
    {
        x = x  / 2;
    }
    return x;
}


static REAL glue(REAL c, REAL x, REAL y)
{
    int p = ACTUAL_STACK.actual_prec/2;
    single_valued code;
    REAL pre = slow_power2(p);
    if (choose (abs(c) < pre, abs(c) > pre / 2) == 1)
{        return join (x, y); 
}    else
    {       
        if (c > 0)
            { return y;}
        else
            {return x;}
    }
}


// expects f(x) = g(x),
// return h := lambda y => x>y -> g(x) | else -> f(x)
static std::function <REAL(REAL)> join (REAL x, std::function<REAL(REAL)> f, std::function<REAL(REAL)> g)
{
    //cout<<f(x)<<" "<<g(x)<<" \n";
    return ([=](REAL y)->REAL { return glue (y-x, f(y), g(y)); });
//     return ([=](REAL y)->REAL { 
//         cout << x <<" "<<y<<"\n";
//         cout <<"start comparing\n";
//         if (y > x)
//         {
//             cout <<"end comparing\n";
//             return g(y);
//         }
//         else
//         {
//             cout <<"end comparing\n";
//             return f(y);
//         }       
//  });
}


static REAL zero(REAL x)
{
    return 0;
}

static INTEGER Ipow2(INTEGER X)
{
    INTEGER Y = 1;
    for (INTEGER K = X; K >0; K = K - 1)
        Y = Y * 2;
    return Y;
}

static REAL chauder(REAL x, int n, INTEGER k)
{
    if(k==0) return 0;
    REAL max=  power(REAL(2),REAL(n-1)/REAL(2));
    RATIONAL first = RATIONAL(k-1, Ipow2(n));
    REAL second = REAL(RATIONAL(k, Ipow2(n)));
    REAL third = REAL(RATIONAL(k+1, Ipow2(n)));
    auto chauderL=[=](REAL t)  ->REAL{return  max*(t-first);};
    auto chauderR=[=](REAL t)  ->REAL{return  max*(third-t);};
    // if (x == first)
    //     return 0;
    // if (x < first )
    //     { return 0;}
    // if (x < second)
    //     { return chauderL(x);}
    // if (x < third)
    //     {return chauderR(x);}
    // return 0;
    
    
    auto f= join(third, join(  second, join(first,zero,chauderL) , chauderR   ),zero);
    return f(x);
}

namespace {
class Wiener
{
    private:
        std::map< std::pair<INTEGER,INTEGER>, REAL > X;
        REAL X_0;
        int m;
    public:
        Wiener()
        {
            m=0;
            X_0=gaussian_real();
        }
        REAL wienerApprox(REAL t,int pBound)
        {
            sizetype err;
            // int p = ACTUAL_STACK.actual_prec;
            // p=p/40;
            int p=-100;
            int required_idx=-4*p+13;
            if(4*p+13<-1*pBound) required_idx=-1*pBound;
            REAL val=X_0*t;
            // std::cout<<required_idx<<"\n";
            for(int i=1;i<=required_idx;i++)
            {   
                // std::cout << i << "\n";
                //cout<<i<<"\n";
                // std::cout<<"????l?\n";
                REAL y=t*REAL(Ipow2(INTEGER(i)));// power(INTEGER(2),REAL(i));
                // std::cout<<"oh my god is it real?\n";
                // cout << y << "\n";
                INTEGER k=y.as_INTEGER();
                // std::cout<<"save me lord\n";
                // cout << k <<"hahahahahahaha\n";
                if(X.count(std::make_pair(i,k-1)) ==0) X[std::make_pair(i,k-1)]=gaussian_real();
                if(X.count(std::make_pair(i,k)) ==0) X[std::make_pair(i,k)]=gaussian_real();
                if(X.count(std::make_pair(i,k+1)) ==0) X[std::make_pair(i,k+1)]=gaussian_real();
                
                val+=X[std::make_pair(i,k-1)]*chauder(t,i,k+1);
            
                val+=X[std::make_pair(i,k-1)]*chauder(t,i,k-1);
        
                val+=X[std::make_pair(i,k)]*chauder(t,i,k);

                // RATIONAL first = RATIONAL(k, Ipow2(i));
                // if (t == first)
                //     return -1;
                // cout << t << "\n";
                // cout << REAL(first) <<"\n";
                // std::cout <<i<<"we are good";

            }
            sizetype_set(err,1,p);
            val.adderror(err);
            return val;
        }
        
};
}


void compute()
{
    Wiener w1,w2,w3;
    // cout<<chauder(REAL(0),1,0);
    //cout<<setRwidth(20)<<w.wienerApprox(REAL(1)/REAL(100))<<"\n";
    for(int i=0;i<=100;i++)
    {
        cout<<setRwidth(20)<<REAL(i)/REAL(100)<<","<<w1.wienerApprox(REAL(i)/REAL(100),-100)<<","<<w2.wienerApprox(REAL(i)/REAL(100),-100)<<","<<w3.wienerApprox(REAL(i)/REAL(100),-100)<<"\n";
    }
}
