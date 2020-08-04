#ifndef GTRANSTEST_HPP
#define GTRANSTEST_HPP
#include "gpermutation.hpp"
#include "transparam.hpp"
namespace GeneralTranslocation{
//tr :all translocations sequence
//wrt:write file
//bf :brute force
int gbfEnumTrans(GeneralPermutation& permut,GeneralPermutation& rpermut, double& usetime, bool wrt);
int gtestEnumTrans(GeneralPermutation& permut, double& usetime, bool wrt);
void gtestEnumTransTree(GeneralPermutation& permut, double& usetime, bool wrt);
void gtestPermut(GeneralPermutation& permut, GeneralPermutation& rpermut,bool tr,bool wrt,bool bf,double& enumtime,double& bftime,double& trtime);

void grandomTest(bool tr,bool wrt,bool bf, double& enumtime,double& bftime,double& trtime);
void grandomTest(bool tr,bool wrt,bool bf);
void grandomEnumTestALL(int num);

void gstaticTest(bool tr,bool wrt,bool bf, double& enumtime,double& bftime,double& trtime);
void gstaticTest(bool tr,bool wrt,bool bf);

void gstaticTest2(bool tr,bool wrt,bool bf, double& enumtime,double& bftime,double& trtime);
void gstaticTest2(bool tr,bool wrt,bool bf);

void ginputTest(bool tr,bool wrt,bool bf, double& enumtime,double& bftime,double& trtime);
void ginputTest(bool tr,bool wrt,bool bf);


}//namespace
#endif


