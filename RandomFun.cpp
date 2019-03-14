
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

static REAL glue(REAL c, REAL x, REAL y)
{
    int p = state->ACTUAL_STACK.actual_prec/2;
    single_valued code;
    REAL pre = scale(REAL(1), p);
    if (choose(abs(c) < pre, abs(c) > pre / 2) == 1)
        return join(x, y);
    else
    {
        if (c > 0)
            return y;
        else
            return x;
    }
}

typedef std::function<REAL(REAL)> f_RR;

// expects f(x) = g(x),
// return h := lambda y => x>y -> g(x) | else -> f(x)
static f_RR join(REAL x, f_RR f, f_RR g)
{
    //cout<<f(x)<<" "<<g(x)<<" \n";
    return [=](REAL y) -> REAL { return glue(y-x, f(y), g(y)); };
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

static REAL chauder(REAL x, int n, INTEGER k)
{
    if (k==0) return 0;
    REAL max = scale(REAL(1), (n-1)/2);
    if ((n-1) % 2 != 0)
	max *= sqrt(REAL(2));
    REAL first  = scale(REAL(k-1), -n);
    REAL second = scale(REAL(k)  , -n);
    REAL third  = scale(REAL(k+1), -n);
    auto chauderL = [=](REAL t) -> REAL { return max*(t-first); };
    auto chauderR = [=](REAL t) -> REAL { return max*(third-t); };
    // if (x == first)
    //     return 0;
    // if (x < first )
    //     { return 0;}
    // if (x < second)
    //     { return chauderL(x);}
    // if (x < third)
    //     {return chauderR(x);}
    // return 0;
    
    auto f = join(third, join(second, join(first, zero, chauderL), chauderR), zero);
    return f(x);
}

namespace {
class Wiener
{
    private:
        std::map< std::pair<INTEGER,INTEGER>, REAL > X;
        std::mt19937 gen;
        REAL X_0;
        int m;

    public:
        Wiener(unsigned int seed = random_device{}())
        : gen(seed)
        {
            m = 0;
            X_0 = gaussian_real({(unsigned)gen(), (unsigned)gen()});
        }

        REAL wienerApprox(REAL t,int pBound)
        {
            // int p = state->ACTUAL_STACK.actual_prec;
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
                REAL y=scale(t, i); // *REAL(Ipow2(i));// power(INTEGER(2),REAL(i));
                // std::cout<<"oh my god is it real?\n";
                // cout << y << "\n";
                INTEGER k=y.as_INTEGER();
                // std::cout<<"save me lord\n";
                // cout << k <<"hahahahahahaha\n";
                if (X.count(std::make_pair(i,k-1)) == 0)
                    X[std::make_pair(i,k-1)] = gaussian_real({(unsigned)gen(), (unsigned)gen()});
                if (X.count(std::make_pair(i,k)) == 0)
                    X[std::make_pair(i,k)] = gaussian_real({(unsigned)gen(), (unsigned)gen()});
                if (X.count(std::make_pair(i,k+1)) == 0)
                    X[std::make_pair(i,k+1)] = gaussian_real({(unsigned)gen(), (unsigned)gen()});
                
                val += X[std::make_pair(i,k-1)]*chauder(t,i,k+1);
            
                val += X[std::make_pair(i,k-1)]*chauder(t,i,k-1);
        
                val += X[std::make_pair(i,k  )]*chauder(t,i,k);

                // RATIONAL first = RATIONAL(k, Ipow2(i));
                // if (t == first)
                //     return -1;
                // cout << t << "\n";
                // cout << REAL(first) <<"\n";
                // std::cout <<i<<"we are good";

            }
            sizetype err;
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
    cout << setRwidth(20);
    for(int i=0;i<=100;i++)
    {
        REAL j = REAL(i)/100;
        cout << j
             << "," << w1.wienerApprox(j, -100)
             << "," << w2.wienerApprox(j, -100)
             << "," << w3.wienerApprox(j, -100)
             << "\n";
    }
}
