/*
  Name:
  Copyright:
  Author: chenchao266
  Date: 20-08-09 14:06
  Description:
*/

#ifndef INCLUDE_SIZETYPE_HPP
#define INCLUDE_SIZETYPE_HPP

//#define LITTLE_ENDIAN

//#if defined(LITTLE_ENDIAN)
//#define LEFT_SHIFT <<
//#define RIGHT_SHIFT >>
//#elif defined(BIG_ENDIAN)
//#define LEFT_SHIFT >>
//#define RIGHT_SHIFT <<
//#endif
typedef  unsigned int uint32_t;
//typedef  unsigned long size_t;

template <uint32_t _N>
struct SizeType{
    uint32_t m_size[_N];

    SizeType() {   for (uint32_t i=0; i<_N; ++i) m_size[i]=0;}
    SizeType(uint32_t i) {    SizeType(); m_size[0]=i;}
    SizeType& operator += (const SizeType& st);
    SizeType& operator -= (const SizeType& st);
    SizeType& operator *= (const SizeType& st);
    SizeType& operator /= (const SizeType& st);
    SizeType& operator = (const SizeType& st);
    SizeType& operator <<= (uint32_t shift);
    SizeType& operator >>= (uint32_t shift);
    friend bool operator < (const SizeType& st, const SizeType& st2);
    friend bool operator > (const SizeType& st, const SizeType& st2);
    friend bool operator == (const SizeType& st, const SizeType& st2);
    friend bool operator <= (const SizeType& st, const SizeType& st2);
    friend bool operator >= (const SizeType& st, const SizeType& st2);
    friend bool operator != (const SizeType& st, const SizeType& st2);
};

template <uint32_t _N>
SizeType<_N>& SizeType<_N>::operator+=(const SizeType<_N>& st)
{
    for(uint32_t i=0; i<_N; i++)
    {
        m_size[i]+=st.m_size[i];
    }
}

template <uint32_t _N>
SizeType<_N>& SizeType<_N>::operator-=(const SizeType<_N>& st)
{
    for(uint32_t i=0; i<_N; i++)
    {
        m_size[i]-=st.m_size[i];
    }
}

template <uint32_t _N>
SizeType<_N>& SizeType<_N>::operator*=(const SizeType<_N>& st)
{
    for(uint32_t i=0; i<_N; i++)
    {
        m_size[i]*=st.m_size[i];
    }
}

template <uint32_t _N>
SizeType<_N>& SizeType<_N>::operator/=(const SizeType<_N>& st)
{
    for(uint32_t i=0; i<_N; i++)
    {
        m_size[i]/=st.m_size[i];
    }
}

template <uint32_t _N>
SizeType<_N>& SizeType<_N>::operator<<=(uint32_t shift)
{
    for(uint32_t i=0; i<_N; i++)
    {
        m_size[i]<<=shift;
    }
}

template <uint32_t _N>
SizeType<_N>& SizeType<_N>::operator>>=(uint32_t shift)
{
    for(uint32_t i=0; i<_N; i++)
    {
        m_size[i]>>=shift;
    }
}
template <uint32_t _N>
bool operator<(const SizeType<_N>& st, const SizeType<_N>& st2)
{
    //*this-=st;
    for(uint32_t i=0; i<_N; i++)
    {
        if(st.m_size[i]>st2.m_size[i]) return false;
    }
    return true;
}

template <uint32_t _N>
bool  operator>(const SizeType<_N>& st, const SizeType<_N>& st2)
{
    for(uint32_t i=0; i<_N; i++)
    {
        if(st.m_size[i]<st2.m_size[i]) return false;
    }
    return true;
}

template <uint32_t _N>
bool  operator==(const SizeType<_N>& st, const SizeType<_N>& st2)
{
    for(uint32_t i=0; i<_N; i++)
    {
        if(st.m_size[i]!=st2.m_size[i]) return false;
    }
    return true;
}


typedef SizeType<1> Size_32t;
typedef SizeType<2> Size_64t;
typedef SizeType<4> Size_128t;

#endif /* INCLUDE_SIZETYPE_HPP */
