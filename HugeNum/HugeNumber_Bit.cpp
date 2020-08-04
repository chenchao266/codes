
#include<cmath>
#include<cstdlib>
#include<fstream>
#include<ctime>
#include<iomanip>
#include<cctype>
//using namespace std;
#include"HugeNumber.hpp"
inline
void LeftShift(const HugeNumber& lhg,HugeNumber& hg,size_t shift)
{
    int lsft=shift&BIT5MASK;
    int rsft=BITLENGTH-lsft;

    size_t j=hg.m_size-1;
    size_t i=lhg.m_size-1;
    for( ; i!=0; --i,--j )
    {
        hg.m_value[j]=lhg.m_value[i]<<lsft;
        hg.m_value[j]|=lhg.m_value[i-1]>>rsft;
    }
    hg.m_value[j]=lhg.m_value[i]<<lsft;//i==0
    for( --j; j!=0; --j )
    {
        hg.m_value[j]=0;
    }
    hg.m_value[j]=0;//j==0
}
const HugeNumber operator<<(const HugeNumber& lhg, size_t shift)
{
    //m_scale unchange
    HugeNumber hg;
    hg.m_sign=lhg.m_sign;
    hg.m_scale=lhg.m_scale;
    int num=0;
    uint32_t high32=lhg.m_value[lhg.m_size-1];
    while(high32!=0){
        high32>>=1; ++num;
    }

    size_t ln=num+shift;
    hg.m_size=lhg.m_size-1+(ln>>BITSHIFT);
    if(ln&BIT5MASK)
        ++hg.m_size;

    hg.m_value=new uint32_t[hg.m_size];

    LeftShift(lhg,hg,shift);
    hg.m_length=hg.m_size;
    return hg;
}
void RightShift(const HugeNumber& lhg,HugeNumber& hg,size_t shift)
{
    size_t rsft=shift&BIT5MASK;
    size_t lsft=BITLENGTH-rsft;
    size_t j=hg.m_size-1;
    size_t i=lhg.m_size-1;
    hg.m_value[j]=lhg.m_value[i]>>rsft;

    for( --i,--j; j!=0; --i,--j )
    {
        hg.m_value[j]=lhg.m_value[i]>>rsft;
        hg.m_value[j]|=lhg.m_value[i+1]<<lsft;
    }
    hg.m_value[j]=lhg.m_value[i]>>rsft;//j==0
    hg.m_value[j]|=lhg.m_value[i+1]<<lsft;
}

const HugeNumber operator>>(const HugeNumber& lhg, size_t shift)
{
    //m_scale unchange
    HugeNumber hg;
    hg.m_sign=lhg.m_sign;
    hg.m_scale=lhg.m_scale;
    int num=0;
    uint32_t high32=lhg.m_value[lhg.m_size-1];
    while(high32!=0){
        high32>>=1; ++num;
    }

    size_t ln=0;
    if(num<shift){
        ln=shift-num;
        hg.m_size=lhg.m_size-(ln>>BITSHIFT);
        if(ln&BIT5MASK)
            --hg.m_size;
    }else{//==?
        hg.m_size=lhg.m_size;
    }
    hg.m_value=new uint32_t[hg.m_size];
    RightShift(lhg,hg,shift);
    hg.m_length=hg.m_size;
    return hg;

}

const HugeNumber operator&(const HugeNumber& lhg, const HugeNumber& rhg)
{
    HugeNumber hg;
    return hg;
}

const HugeNumber operator|(const HugeNumber& lhg, const HugeNumber& rhg)
{
    HugeNumber hg;
    return hg;
}

const HugeNumber operator^(const HugeNumber& lhg, const HugeNumber& rhg)
{
    HugeNumber hg;
    return hg;
}

const HugeNumber operator~( const HugeNumber& lhg )
{
	HugeNumber c;

	for( int i=0; i<lhg.m_size; i++ )
		c.m_value[i] = ~lhg.m_value[i];
	return c;
}

const HugeNumber operator-(const HugeNumber& lhg)
{
    HugeNumber hg;
    return hg;
}
