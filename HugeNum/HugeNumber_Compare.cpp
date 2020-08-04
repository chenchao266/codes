
#include<cmath>
#include<cstdlib>
#include<fstream>
#include<ctime>
#include<iomanip>
#include<cctype>
//using namespace std;
#include"HugeNumber.hpp"
bool operator<(const HugeNumber& lhg, const HugeNumber& rhg)
{

    if ((lhg.m_sign==HugeNumber::PLUS)&&(rhg.m_sign==HugeNumber::MINUS))
        return false;
    else if ((lhg.m_sign==HugeNumber::MINUS)&&(rhg.m_sign==HugeNumber::PLUS))
        return true;
    if((lhg.m_size==0)&&(rhg.m_size>0))
        return true;
    else if((lhg.m_size>=0)&&(rhg.m_size==0))
        return false;
    size_t ln1=((lhg.m_size-1)<<BITSHIFT)+lhg.GetHighestLength()+lhg.m_scale;
    size_t ln2=((rhg.m_size-1)<<BITSHIFT)+rhg.GetHighestLength()+rhg.m_scale;
    if(ln1>ln2)
        return false;
    else if(ln1<ln2)
        return true;

    //ln1==ln2
    size_t i,j;

    for( i=lhg.m_size-1,j=rhg.m_size-1; (i!=0)&&(j!=0);  --i,--j){

        if(lhg.m_value[i]>rhg.m_value[j]) return false;
        else if(lhg.m_value[i]<rhg.m_value[j]) return true;
    }
    if(lhg.m_value[i]>rhg.m_value[j]) return false;
    else if(lhg.m_value[i]<rhg.m_value[j]) return true;
    //==
    if(j>0){
        size_t m=0;
        for(;m<=j;++m){
            if(rhg.m_value[j-m]!=0)
                break;
        }
        if(m>j)
            return false;
        else
            return true;
    }
    else {//i>=0
         return false;
    }

}

bool operator>(const HugeNumber& lhg, const HugeNumber& rhg)
{

    if ((lhg.m_sign==HugeNumber::PLUS)&&(rhg.m_sign==HugeNumber::MINUS))
        return true;
    else if ((lhg.m_sign==HugeNumber::MINUS)&&(rhg.m_sign==HugeNumber::PLUS))
        return false;
    if((lhg.m_size==0)&&(rhg.m_size>=0))
        return false;
    else if((lhg.m_size>0)&&(rhg.m_size==0))
        return true;
    size_t ln1=((lhg.m_size-1)<<BITSHIFT)+lhg.GetHighestLength()+lhg.m_scale;
    size_t ln2=((rhg.m_size-1)<<BITSHIFT)+rhg.GetHighestLength()+rhg.m_scale;
    if(ln1>ln2)
        return true;
    else if(ln1<ln2)
        return false;

    //ln1==ln2
    size_t i,j;
    for( i=lhg.m_size-1,j=rhg.m_size-1; (i!=0)&&(j!=0);  --i,--j){

        if(lhg.m_value[i]>rhg.m_value[j]) return true;
        else if(lhg.m_value[i]<rhg.m_value[j]) return false;
    }
    if(lhg.m_value[i]>rhg.m_value[j]) return true;
    else if(lhg.m_value[i]<rhg.m_value[j]) return false;
    //==
    if(i>0){
        size_t m=0;
        for(;m<=i;++m){
            if(lhg.m_value[i-m]!=0)
                break;
        }
        if(m>i)
            return false;
        else
            return true;
    }
    else {//j>=0
         return false;
    }

}

bool operator==(const HugeNumber& lhg, const HugeNumber& rhg)
{

    if (lhg.m_sign!=rhg.m_sign)
        return false;
    if(((lhg.m_size==0)&&(rhg.m_size!=0))||((lhg.m_size!=0)&&(rhg.m_size==0)))
        return false;
    else if(((lhg.m_size==0)&&(rhg.m_size==0)))
        return true;
    size_t ln1=((lhg.m_size-1)<<BITSHIFT)+lhg.GetHighestLength()+lhg.m_scale;
    size_t ln2=((rhg.m_size-1)<<BITSHIFT)+rhg.GetHighestLength()+rhg.m_scale;
    if(ln1!=ln2)
        return false;

    //ln1==ln2
    size_t i,j;
    for( i=lhg.m_size-1,j=rhg.m_size-1; (i!=0)&&(j!=0);  --i,--j){

        if(lhg.m_value[i]!=rhg.m_value[j])
            return false;
    }
    if(lhg.m_value[i]!=rhg.m_value[j])
        return false;
    //==
    if(i>0){
        size_t m=0;
        for(;m<=i;++m){
            if(lhg.m_value[i-m]!=0)
                return false;
        }

    }
    else
    if(j>0){
        size_t m=0;
        for(;m<=j;++m){
            if(lhg.m_value[j-m]!=0)
                return false;
        }

    }
    return true;

}

bool operator>=(const HugeNumber& lhg, const HugeNumber& rhg)
{
    return !(lhg<rhg);
}

bool operator<=(const HugeNumber& lhg, const HugeNumber& rhg)
{
    return !(lhg>rhg);
}

bool operator!=(const HugeNumber& lhg, const HugeNumber& rhg)
{
    return !(lhg==rhg);
}

