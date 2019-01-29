#include "iRRAM/lib.h"
#include "iRRAM/core.h"
#include "iRRAM.h"

#include "include/random-real.h"
#include "include/random-complex.h"
using namespace iRRAM;

INTEGER i2power(int n)
{
    INTEGER r(1);
    INTEGER t(2);
    for(int i=0;i<n;i++)
    {
        r=r*t;
    }
	return r;
}

REAL test_approx(int prec,const REAL& x)
{
	REAL N=1/i2power(-1*prec);
	switch(choose(x<N,x>-1*N))
	{
		case 1:
			return 0;
		case 2:
			return x;
	}
}

void compute()
{
	cout<<limit(test_approx,REAL(0.001));
}