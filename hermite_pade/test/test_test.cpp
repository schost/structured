#include "hermite_pade.h"

int main(){
  ZZX f;
  long N = 5;
  for (long i = 1; i <=N; i++)
    SetCoeff(f,i-1,i);
  //cout << f << endl;
  Vec<long> type;
  type.append(1);
  type.append(2);
  type.append(0);
  hermite_pade hp(f,type,N);
  Vec<Vec<ZZ>> v;
  hp.find_rand_sol(v);
  cout << v << endl;
}