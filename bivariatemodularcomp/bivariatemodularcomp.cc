#include "bivariatemodularcomp.h"
#include <cmath>
#include "mat_ZZX.h"
#include "mat_ZZ_pX.h"
#include <NTL/tools.h>
#include <time.h>
using namespace std;
using namespace NTL;

ZZ_pX pow (const ZZ_pX& f, unsigned long p){
  if (p == 0)
    return ZZ_pX(1);
  ZZ_pX result{f};
  for (unsigned long i = 1; i < p; i++){
    result *= f;
  }
  return result;
}

void BivariateModularComp::init (const ZZX &f_new, const Vec<long> &type_new, unsigned long prec_new){
	f = f_new;
	type = type_new;
	prec = prec_new;
	sqrtP = ceil(sqrt(type.length()));
  ZZX running = ZZX{1};
  for (int i = 0; i < sqrtP; i++){
    fs.append(running);
    running *= f;
    trunc(running,prec);
  }
  F = running;
  conv(f_field, f);
  conv(F_field, F);
  conv(fs_field, fs);
  for (int i = 0; i < type.length(); i++) totalVars += type[i] + 1;
  create_rhs_matrix(B,fs_field);
  initialized = true;
}

BivariateModularComp::BivariateModularComp(){}

BivariateModularComp::BivariateModularComp(const ZZX& f, const Vec<long> &type, unsigned long prec) {
  init(f,type,prec);
}

Vec<ZZ_pX> BivariateModularComp::create_lhs_list (const Vec<ZZ_p> &v){
  Vec<ZZ_pX> result;
  long acc = 0;
  for (int i = 0; i < type.length(); i++){
    ZZ_pX p;
    for (int j = 0; j < type[i]+1; j++)
      SetCoeff(p,j,v[acc+j]);
    acc += type[i]+1;
    result.append(p);
  }
  return result;
}

void BivariateModularComp::create_lhs_matrix(Mat<ZZ_pX> &A,const Vec<ZZ_pX> &v){
  A.SetDims(ceil((type.length()*1.0)/sqrtP),sqrtP);
  for (int i = 0; i < A.NumRows(); i++){
    for (int j = 0; j < A.NumCols(); j++){
      if(A.NumCols() * i + j < v.length()){
        A.put(i,j,v[A.NumCols() * i + j]);
      }
    }
  }
}

void BivariateModularComp::create_rhs_matrix(Mat<ZZ_pX> &B,const Vec<ZZ_pX> &v){
  B.SetDims(v.length(),type.length());
  for (long i = 0; i < B.NumRows(); i++){
    long acc = 0;
    for (long j = 0; j < B.NumCols(); j++){
      ZZ_pX p;
      for (int s = 0; s < type[j]+1; s++){
        SetCoeff(p,s,coeff(v[i],acc+s));
      }
      acc += type[j]+1;
      B.put(i,j,p);
    }
  }
}

void BivariateModularComp::deslice(Mat<ZZ_pX> &D,const Mat<ZZ_pX> &C){
  D.SetDims(C.NumRows(),1);
  for (int i = 0; i < C.NumRows(); i++){
    ZZ_pX v;
    v.SetLength(prec);
    long acc = 0;
    for (int j = 0; j < C.NumCols(); j++){
      auto &p = C[i][j];
      for (int s = 0; s < deg(p)+1; s++)
        if(acc+s < prec)
       	  v[acc + s] += coeff(p,s);
        else break;
      acc += type[j]+1;
    }
    D.put(i,0,v);
  }
}

Vec<ZZ_p> BivariateModularComp::mult_Horners(const Vec<ZZ_p> &rhs){
  if (!initialized) throw "must init first";
  auto lhs = create_lhs_list(rhs);
  Vec<ZZ_p> v;
  ZZ_pX result{lhs[lhs.length()-1]};
  for (long i = lhs.length()-2; i >= 0; i--){
    result = trunc((result * f_field),prec) + lhs[i];
  }
  for (long i = 0; i < prec; i++)
    v.append(coeff(result,i));
  return v;
}

Vec<ZZ_p> BivariateModularComp::mult(const Vec<ZZ_p> &rhs){
  if (!initialized) throw "must init first";
  Mat<ZZ_pX> A;
  create_lhs_matrix(A,create_lhs_list(rhs));
  mul(A,A,B);
  ZZ_pX p;
  Mat<ZZ_pX> B;
  deslice(B,A);
  p += B[B.NumRows()-1][0];
  for (long i = B.NumRows()-2; i >= 0; i--){
    p = trunc(p*F_field,prec) + B[i][0];
  }
  Vec<ZZ_p> v;
  for (int i = 0; i < prec; i++)
    v.append(coeff(p,i));
  return v;
}




