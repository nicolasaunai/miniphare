#include "utilityphare.h"




double factorial(int64 n)
{
    double r=1.;

    for(int32 i=2;i<=n;i++)
        r *= i;

    return r;
}

int64 binomial(int64 n, int64 k)
{
    int64 coef=1;

    if(n<0 || k<0 || k>n)
        coef = 0 ;
    else
        coef = factorial(n) / factorial(k) / factorial(n-k) ;

    return coef;
}

int64 ipow(int base, unsigned exp)
{
    auto result = 1;
    for(unsigned i=0 ; i<exp; ++i) result *= base ;

    return result;
}

int64 trinomial(int64 n, int64 k)
{
    int64 coef=0;

    if(n<0)
    {
        return trinomial(-n, k);
    } else
    {
        for (int j = 0; j < n+1; j++)
        {
            coef += ipow(-1, j) * binomial(n, j) * binomial(2*n-2*j, n-k-j) ;
        }
    }

    return coef;
}


