#include <NTL/lzz_pX.h>
#include <NTL/vector.h>
#include <NTL/matrix.h>
#include <NTL/mat_lzz_p.h>
#include <assert.h>

#include "vec_lzz_p_extra.h"
#include "lzz_p_toeplitz_like.h"

NTL_CLIENT



/*------------------------------------------------------------*/
/* initializes a zz_pX_Multipoint                             */
/* check takes an extra argument, not used here               */
/*------------------------------------------------------------*/
void check(int opt){

  zz_p::FFTInit(0);

  long alpha = 5;
  for (long m = 50; m < 100; m++)
    for (long n = m-5; n < m+5; n++){
      Mat<zz_p> G, H;
      random(G, m, alpha);
      random(H, n, alpha);
      lzz_p_toeplitz_like T(G, H);
    }
}  

int main(int argc, char ** argv){
  int opt = 0;
  if (argc > 1)
    opt = atoi(argv[1]);
  check(opt);
  return 0;
}
