#ifndef TRANSTEST_HPP
#define TRANSTEST_HPP
#include "permutation.hpp"

namespace Translocation{
//tr :all translocations sequence
//wrt:write file
//bf :brute force
int bfEnumTrans(Permutation& permut, double& usetime, bool wrt);
int testEnumTrans(Permutation& permut, double& usetime, bool wrt);
void testEnumTransTree(Permutation& permut, double& usetime, bool wrt);
void testPermut(Permutation& permut,bool tr,bool wrt,bool bf,double& enumtime,double& bftime,double& trtime);

void randomTest(bool tr,bool wrt,bool bf, double& enumtime,double& bftime,double& trtime);
void randomTest(bool tr,bool wrt,bool bf);
void randomEnumTestALL(int num);
void randomMedianTest(int num,int a, bool wrt);
void randomMedianNMTest(int num,int a, bool wrt);
void randomHMedianTest(int num,int a,  bool wrt);
void randomMHTest(int num,int a,  bool wrt);

void staticTest(bool tr,bool wrt,bool bf, double& enumtime,double& bftime,double& trtime);
void staticTest(bool tr,bool wrt,bool bf);

void staticTest2(bool tr,bool wrt,bool bf, double& enumtime,double& bftime,double& trtime);
void staticTest2(bool tr,bool wrt,bool bf);
void staticMedianTest(bool wrt);
void staticHMedianTest(bool wrt);

void inputTest(bool tr,bool wrt,bool bf, double& enumtime,double& bftime,double& trtime);
void inputTest(bool tr,bool wrt,bool bf);
void inputFileTest(const char * filename, bool wrt );//median
}//namespace
#endif

