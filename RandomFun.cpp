
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
    else if (c > 0)
        return y;
    else
        return x;
}

typedef std::function<REAL(REAL)> f_RR;

// expects f(x) = g(x),
// return h := lambda y => x>y -> g(x) | else -> f(x)
static f_RR join(REAL x, f_RR f, f_RR g)
{
    return [=](REAL y) -> REAL { return glue(y-x, f(y), g(y)); };
}


static REAL zero(REAL x)
{
    return 0;
}

/*
 *            second
 *              |
 * M ->         +
 *             / \
 *            /   \
 *           /     \
 *          /       \
 * 0 -> ---+         +----
 *         |         |
 *       first     third
 *
 * M = max*(second-first)
 *   = max*(third-second)
 *   = max*(2^-n)
 *   = 2^((n-1)/2)*2^-n
 *   = 2^((n-1)/2-n)
 *   = 2^(-(n+1)/2)
 *
 * second = k*(2^-n)
 * max    = 2^((n-1)/2)
 * n      > 0
 */
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

    auto f = join(third, join(second, join(first, zero, chauderL), chauderR), zero);
    return f(x);
}

namespace {
class Wiener
{
    private:
        std::vector<std::map<INTEGER,REAL>> X;
        std::mt19937 gen;
        const REAL X_0;

        REAL gr() { return gaussian_real({(unsigned)gen(), (unsigned)gen()}); }

    public:
        Wiener(unsigned int seed = random_device{}())
        : gen(seed)
        , X_0(gr())
        {}

        REAL wienerApprox(REAL t,int pBound)
        {
            int p = -100;
            /*
            p = state->ACTUAL_STACK.actual_prec;
            p = p / 40;
            p = pBound;*/
            int required_idx = max(-4 * p + 13, -pBound);
            REAL val = X_0 * t;
            X.resize(required_idx);
            for (int i=1; i<=required_idx; i++)
            {
                auto &Xi = X[i-1];
                INTEGER k = scale(t, i).as_INTEGER();

                if (Xi.find(k-1) == Xi.end())
                    Xi[k-1] = gr();
                if (Xi.find(k  ) == Xi.end())
                    Xi[k  ] = gr();
                if (Xi.find(k+1) == Xi.end())
                    Xi[k+1] = gr();

                val += Xi[k-1] * chauder(t,i,k-1);
                val += Xi[k  ] * chauder(t,i,k  );
                val += Xi[k+1] * chauder(t,i,k+1);
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
