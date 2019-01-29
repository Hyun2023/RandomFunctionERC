#include "iRRAM.h"
#include "iRRAM/core.h"
#include "functional"
using namespace iRRAM;



REAL join(REAL a, REAL b)
{
  REAL  m = (a+b)/2;
  REAL  e = abs(a-b);
  m.adderror(e.vsize);
  return m;
}

REAL slow_power2 (int p )
{
  REAL x = 1;
  for (int i =0; i<(-p); i++)
  {
    x = x  / 2;
  }
  return x;
}


REAL glue (REAL c, REAL x, REAL y)
{
  int p = ACTUAL_STACK.actual_prec;
  REAL pre = slow_power2(p);
  if (choose (abs(c) < pre, abs(c) > pre / 2) == 1)
    return join (x, y);
  else
    if (c > 0)
      return y;
    else
      return x;
}


// expects f(x) = g(x),
// return h := lambda y => x>y -> g(x) | else -> f(x)
std::function <REAL(REAL)> join (REAL x, std::function<REAL(REAL)> f, std::function<REAL(REAL)> g)
{
  return ([=](REAL y)->REAL { return glue (y-x, f(y), g(y)); });

}


void compute() {

  std::function <REAL(REAL)> f = ([=](REAL y)->REAL { return REAL(0); });
  std::function <REAL(REAL)> g = ([=](REAL y)->REAL { return y; });
  std::function <REAL(REAL)> h = join (REAL(0), f, g);
  cout << h(-0.05) <<"\n";
}