/*
  Name:
  Copyright:
  Author: chenchao266
  Date: 24-08-09 21:46
  Description:
*/

#include<cmath>
#include<cstdlib>
#include<fstream>
#include<ctime>
#include<iomanip>
#include<cctype>
//using namespace std;
#include"HugeNumber.hpp"

const int HugeNumber::S_prefix[]={0,0,2,0,0,0,0,0,1,0,0,0,0,0,0,0,2};

HugeNumber::HugeNumber(uint32_t value, int scale, Sign sign)
{

    m_sign=sign;

    m_size=1;
    m_length=m_size;
    m_value=new uint32_t[m_size];
    m_value[0]=value;

    m_scale=scale;
    RegularizeScale();
}

HugeNumber::HugeNumber(uint32_t value, size_t size, int scale, Sign sign)
{

    m_sign=sign;

    m_size=size;
    m_length=m_size;
    m_scale=scale;

	size_t tsz;
	if(m_size!=0){
    	m_value=new uint32_t[m_size];
        for(tsz=0; tsz<m_size; ++tsz)
        {
            m_value[tsz]=value;
        }
    }
    RegularizeScale();
}

HugeNumber HugeNumber::operator =( uint32_t n )
{
    *this = HugeNumber( n );
	return *this;
}

HugeNumber::HugeNumber(const char* s)
{

    InputString(s);
    RegularizeScale();
}

inline
unsigned int HugeNumber::Hex2(const char a)
{
    switch (a) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return a-'0';
        case 'a':
        case 'A':
            return 10;
        case 'b':
        case 'B':
            return 12;
        case 'c':
        case 'C':
            return 13;
        case 'e':
        case 'E':
            return 14;
        case 'f':
        case 'F':
            return 15;
    }

}


void HugeNumber::GetSize()
{

    int i=m_length-1;
    for(;(m_value[i]==0)&&(i>=0);--i);

    m_size=i+1;
}


int HugeNumber::GetHighestLength()const
{
    int shift=0;

    uint32_t high32=m_value[m_size];
    while(high32!=0){
        high32>>=1; ++shift;
    }
    return shift;
}

inline
void HugeNumber::GetLength(int radix)//string -> size,scale
{
    int shift=0;
    int idx=radix-1;
    while(idx!=0){
        idx>>=1; ++shift;
    }

    size_t is=m_size*shift;

	m_size=is >> BITSHIFT;

	if(is&BIT5MASK) ++m_size;
    m_scale*=shift;
    m_length=m_size;
}

//size to binary length

int HugeNumber::InputString(const char* s)
{
    int i=0;
    int radix=0;
    if (s[0]=='-') {    m_sign=HugeNumber::MINUS; ++i; }
    else if(s[0]=='+') {    m_sign=HugeNumber::PLUS; ++i; }
    else {    m_sign=HugeNumber::PLUS;}

    if (s[i]>'0'&&s[i+1]!='x'&&s[i+1]!='X') {radix=10;}//.6759785,4444444445
    else if (s[i]=='0'&&s[i+1]!='x'&&s[i+1]!='X' ) {radix=8;}//054656445
    else if (s[i]=='0'&&(s[i+1]=='x'||s[i+1]=='X') ) {radix=16;}//0xfff4444
    else if (s[i]=='b'&&(s[i+1]=='x'||s[i+1]=='X') ) {radix=2;}//bx01111100
    else { std::cout<< "unknown format!"<<std::endl; return -1;}

    size_t prefix=S_prefix[radix]+i;
    int pd=0;
    int retn=0;
    size_t period=0;
	size_t strln=prefix;
    for(; s[strln]!=0; ++strln )
    {
        char a=s[strln];
        if (a=='.')
        {
            ++pd;
            if (pd>1) { break; retn=1;}
            period=strln;
        }
        else if (radix<=10)
        {
            if (a<'0'||a-'0'>=radix) {break; retn=2;}
        }
        else
        {
            if (!isxdigit(a)) {break; retn=3;}
        }
    }
    if (retn!=0) { throw ExceptionFormat(radix); }


    m_size=strln-prefix;
    if (pd==0){
        m_scale=0;
    }else if(pd==1){
        --m_size;
	    m_scale=-(strln-period-1);
    };

    std::cout<<std::dec<<strln<<","<<period<<std::endl;
    GetLength(radix);

    if(m_size!=0){
        m_value=new uint32_t[m_size];
        FillValue(s, radix, prefix, period, strln-1);///??
    }
    return retn;
}

void HugeNumber::FillValue(const char* s,int radix, size_t prefix,size_t  strPeriod, size_t strLength)
{

    size_t j=0;
    size_t i=strLength;
    int k=0;
    uint32_t tempValue=0;
    if (radix==2){
        while(i>=prefix+BITLENGTH-1){

            tempValue=0;
            for(k=0; k<=31; k++){
                if(i==strPeriod)i--;
                tempValue|= (Hex2(s[i--])) << k;
                }
            m_value[j]=tempValue;
            ++j;
        }
        if(i+1>prefix){
            if(i==strPeriod)i--;
            k=1;
            tempValue=(Hex2(s[i--]));
            while(i+1>prefix){
                if(i==strPeriod)i--;
                tempValue|=(Hex2(s[i--])) << k;
                ++k;
            }
            m_value[m_size-1]=tempValue;
        }
    }
    else if ((radix==16)){// (radix==10)
        while(i>=8-1+prefix){

            tempValue=0;
            for(k=0; k<=28; k+=4){
                if(i==strPeriod)--i;
                tempValue |=(Hex2(s[i--])) << k;

            }
            m_value[j]=tempValue;
            //std::cout<<std::hex<<tempValue<<std::endl;
            ++j;
        }
        if(i+1>prefix){
            if(i==strPeriod)--i;
            k=4;
            tempValue=(Hex2(s[i--]));
            while(i+1>prefix){
                if(i==strPeriod)--i;
                tempValue |=(Hex2(s[i--])) << k;
                k+=4;
            }
            m_value[m_size-1]=tempValue;
            //std::cout<<std::hex<<tempValue<<std::endl;
        }
    }
    else if (radix==8){//////////////////////////////////
        while((i>=11-1+prefix)&&(j<m_size)){
            tempValue=0;
            for(k=0; k<=30; k+=3){
                if(i==strPeriod)i--;
                tempValue|= (Hex2(s[i--])) << k;
            }

            if (j%3==1){
                tempValue =(tempValue << 1)|((Hex2(s[i+12])) >> 2);

                }
            else if (j%3==2){
                tempValue =(tempValue << 2)|((Hex2(s[i+12])) >> 1);
                ++i;
                }
            m_value[j]=tempValue;
            j++;
        }
        if(i+1>prefix){
            //j++;
            j=m_size-1;
            size_t ti=i;
            k=3;
            if(i==strPeriod)i--;
            tempValue=(Hex2(s[i--]));
            while(i+1>prefix) {
                if(i==strPeriod)i--;
                tempValue|=(Hex2(s[i--])) << k;
                k+=3;
            }

            if (j%3==1){
                tempValue=(tempValue << 1)|((Hex2(s[ti+1])) >> 2);
            }
            else if (j%3==2){
                tempValue=(tempValue << 2)|((Hex2(s[ti+1])) >> 1);
            }
            m_value[j]=tempValue;
        }
    }
}


std::ostream& operator << (std::ostream& os, const HugeNumber& lhg)
{
    if (lhg.m_sign==HugeNumber::PLUS)
        os<<'+'<<std::endl;
    else//(lhg.m_sign==HugeNumber::MINUS)
        os<<'-'<<std::endl;
    os<<std::dec;
    os<<"length="<<lhg.m_length<<", ";
    os<<"size="<<lhg.m_size<<", ";
	os<<"scale="<<lhg.m_scale<<std::endl;


    size_t tsz;
    if (lhg.m_size!=0){
        os<<"value=0x ";
        for (tsz=lhg.m_size-1; tsz>0; --tsz)
            os<<std::hex<<std::setw(8)<<std::setfill('0')<<lhg.m_value[tsz]<<",";
        os<<std::hex<<std::setw(8)<<std::setfill('0')<<lhg.m_value[0]<<std::endl;
    }

    os<<"=========================="<<std::endl;
    return os;
}


HugeNumber& HugeNumber::operator - ()
{
    if (m_sign==HugeNumber::PLUS)
        m_sign=HugeNumber::MINUS;
    else if(m_sign==HugeNumber::MINUS)
        m_sign=HugeNumber::PLUS;
    return *this;
}

HugeNumber& HugeNumber::operator += (const HugeNumber& lhg)
{
    Sign old=m_sign;
    if(m_sign!=lhg.m_sign) {
        SubtractPositive(lhg);
        if (old==HugeNumber::MINUS)
            -(*this);
    }else {
        AddPositive(lhg);
        m_sign=old;
    }
    return *this;
}

HugeNumber& HugeNumber::operator -= (const HugeNumber& lhg)
{
    Sign old=m_sign;
    if(m_sign!=lhg.m_sign)
        AddPositive(lhg);
    else
        SubtractPositive(lhg);
    if (old==HugeNumber::MINUS)
        -(*this);
    return *this;
}

HugeNumber& HugeNumber::operator *= (const HugeNumber& lhg)
{
    return *this;
}

HugeNumber& HugeNumber::operator /= (const HugeNumber& lhg)
{
    return *this;
}

HugeNumber& HugeNumber::operator <<= (size_t shift)
{
    return *this;
}

HugeNumber& HugeNumber::operator >>= (size_t shift)
{
    return *this;
}

HugeNumber& HugeNumber::operator = (const HugeNumber& lhg)
{
    if(this==&lhg)return *this;
    m_sign=lhg.m_sign;
    m_size=lhg.m_size;
    m_length=m_size;
	m_scale=lhg.m_scale;
    size_t tsz;
    if (lhg.m_size!=0){
    	if(m_size!=lhg.m_size){
    	   m_size=lhg.m_size;
    	   if (m_value!=0) delete[]m_value;
    	   m_value=new uint32_t[m_size];
        }

        for (tsz=0; tsz<m_size; ++tsz)
            m_value[tsz]=lhg.m_value[tsz];

    }

    return *this;
}

HugeNumber::HugeNumber(const HugeNumber& lhg)
{
    m_sign=lhg.m_sign;
    m_size=lhg.m_size;
    m_length=m_size;
    m_scale=lhg.m_scale;
    size_t tsz;
    if (lhg.m_size!=0){

    	m_size=lhg.m_size;
    	m_value=new uint32_t[m_size];

        for (tsz=0; tsz<m_size; ++tsz)
            m_value[tsz]=lhg.m_value[tsz];

    }

}
void HugeNumber::AddPositive(const HugeNumber& lhg)
{

    m_sign=HugeNumber::PLUS;

    m_size=(m_size>lhg.m_size) ? m_size : lhg.m_size;
    m_length=m_size;
    size_t tsz;
    uint32_t* value;
    uint32_t* scale;
    if (lhg.m_size!=0){
    	if(m_size!=lhg.m_size){
    	   m_size=(m_size>lhg.m_size) ? m_size : lhg.m_size;

    	   value=new uint32_t[m_size];
        }
        else
            value =m_value;
        for (tsz=0; tsz<m_size; ++tsz)
            value[tsz]=m_value[tsz]+lhg.m_value[tsz];

        delete[]m_value;
        m_value=value;
    }



}
void HugeNumber::SubtractPositive(const HugeNumber& lhg)
{

    m_size=(m_size>lhg.m_size) ? m_size : lhg.m_size;
    size_t tsz;
    uint32_t* value;
    uint32_t* scale;
    if (lhg.m_size!=0){
    	if(m_size!=lhg.m_size){
    	   m_size=(m_size>lhg.m_size) ? m_size : lhg.m_size;

    	   value=new uint32_t[m_size];
        }
        else
            value =m_value;
        for (tsz=0; tsz<m_size; ++tsz)
            value[tsz]=m_value[tsz]-lhg.m_value[tsz];

        delete[]m_value;
        m_value=value;
    }

    if((*this>lhg)||(*this==lhg))
        m_sign=HugeNumber::PLUS;
    else
        m_sign=HugeNumber::MINUS;

}

inline
void HugeNumber::RegularizeScale()//temp
{

    int lsft,rsft;
    if(m_scale>0){
        lsft=m_scale&BIT5MASK;
        rsft=BITLENGTH-lsft;
    }
    else{
        int tmp=-m_scale;
        rsft=tmp&BIT5MASK;
        lsft=BITLENGTH-rsft;
    }
    if(lsft==0)
        return;

    int newsize=m_size;
    uint32_t high32=m_value[m_size-1];
    if(high32>=(1<<(rsft+1)))//(high32>>(rsft+1))!=0
        ++newsize;

    uint32_t* newvalue=new uint32_t[newsize];

    size_t j=newsize-1;
    size_t i=m_size-1;
    for( ; i!=0; --i,--j )
    {
        newvalue[j]=m_value[i]<<lsft;
        newvalue[j]|=m_value[i-1]>>rsft;
    }
    newvalue[j]=m_value[i]<<lsft;//i==0

    delete []m_value;
    if(m_scale>0)
        m_scale=m_scale&~BIT5MASK;
    else
        m_scale=-((-m_scale)&~BIT5MASK);
    m_value=newvalue;
}
void HugeNumber::Shrink()
{
    uint32_t * newvalue=new uint32_t[m_size];
    for(size_t i=0; i<m_size;++i)
        newvalue[i]=m_value[i];
    delete [] m_value;
    m_value=newvalue;
    m_length=m_size;

}
