/*
Name:
Copyright:
Author: chenchao266
Date: 18-08-09 21:17
Description:
*/

#ifndef INCLUDE_HUGENUMBER_HPP
#define INCLUDE_HUGENUMBER_HPP

#include<iostream>

//#include"SizeType.hpp"
#ifdef _MSC_VER

typedef __int16 int16_t;
typedef unsigned __int16 uint16_t;
typedef __int32 int32_t;
typedef unsigned __int32 uint32_t;
typedef __int64 int64_t;
typedef unsigned __int32 uint64_t;

#else
#include <stdint.h>
#endif

#define  BIT5MASK    0x0000001f
#define  BIT32MASK   0xffffffff
#define  BITSHIFT    5
#define  BITLENGTH   32
#define  BIT32HIGHT	 0x80000000
//class ostream;
//32bit
class HugeNumber
{

public:
	enum Sign{ PLUS, MINUS };
	//HugeNumber();
	HugeNumber(const HugeNumber& lhg);
	HugeNumber(const char* s);
	HugeNumber(uint32_t value=0, int scale=0, Sign sign=PLUS);
	HugeNumber(uint32_t value, size_t size, int  scale, Sign sign=PLUS);
	~HugeNumber(){ delete[] m_value;}
	class ExceptionFormat
	{
	public:
		ExceptionFormat(int radix){ std::cout<<"error in format!"<<radix<<" number" <<std::endl; } //
	};

	HugeNumber& operator +=(const HugeNumber& lhg);
	HugeNumber& operator -=(const HugeNumber& lhg);
	HugeNumber& operator *=(const HugeNumber& lhg);
	HugeNumber& operator /=(const HugeNumber& lhg);
	HugeNumber& operator <<=(size_t shift);
	HugeNumber& operator >>=(size_t shift);
	HugeNumber& operator -();

	HugeNumber& operator =(const HugeNumber& lhg);
	HugeNumber operator =( uint32_t n );

	friend bool operator <(const HugeNumber& lhg, const HugeNumber& rhg);
	friend bool operator >(const HugeNumber& lhg, const HugeNumber& rhg);
	friend bool operator ==(const HugeNumber& lhg, const HugeNumber& rhg);
	friend bool operator <=(const HugeNumber& lhg, const HugeNumber& rhg);
	friend bool operator >=(const HugeNumber& lhg, const HugeNumber& rhg);
	friend bool operator !=(const HugeNumber& lhg, const HugeNumber& rhg);
	friend bool operator !(const HugeNumber& lhg);
	friend std::ostream& operator <<(std::ostream& os, const HugeNumber& lhg);
	friend const HugeNumber operator +(const HugeNumber& hgnum, const HugeNumber& rhg);
	friend const HugeNumber operator -(const HugeNumber& lhg, const HugeNumber& rhg);
	friend const HugeNumber operator *(const HugeNumber& lhg, const HugeNumber& rhg);
	friend const HugeNumber operator /(const HugeNumber& lhg, const HugeNumber& rhg);
	friend const HugeNumber div(const HugeNumber& lhg, const HugeNumber& rhg);
	friend const HugeNumber operator %(const HugeNumber& lhg, const HugeNumber& rhg);
	friend const HugeNumber operator <<(const HugeNumber& lhg, size_t shift);
	friend const HugeNumber operator >>(const HugeNumber& lhg, size_t shift);
	friend const HugeNumber operator &(const HugeNumber& lhg, const HugeNumber& rhg);
	friend const HugeNumber operator |(const HugeNumber& lhg, const HugeNumber& rhg);
	friend const HugeNumber operator ^(const HugeNumber& lhg, const HugeNumber& rhg);
	friend const HugeNumber operator ~( const HugeNumber& lhg );
	friend const HugeNumber operator -(const HugeNumber& lhg);

private:
	void RegularizeScale();//??
	void Shrink();
	int  GetHighestLength()const;
	int  InputString(const char* s);
	void GetLength( int radix);
	void GetSize();
	unsigned int Hex2(const char a);
	void FillValue(const char* s,int radix, size_t prefix, size_t strPeriod, size_t allLength);
	void AddPositive(const HugeNumber& lhg);
	void SubtractPositive(const HugeNumber& lhg);
	friend void AddPositive(const HugeNumber& lhg, const HugeNumber& rhg, HugeNumber& hg);
	friend void SubtractPositive(const HugeNumber& lhg, const HugeNumber& rhg, HugeNumber& hg);
	friend void LeftShift(const HugeNumber& lhg,HugeNumber& hg,size_t shift);
	friend void RightShift(const HugeNumber& lhg,HugeNumber& hg,size_t shift);

	friend void Shift_Multiple( HugeNumber& lhg, HugeNumber& rhg, int bLeft );
	friend void Shift_Multiple_M( HugeNumber& lhg, HugeNumber& rhg, int bLeft, int bits );
	friend int GetHighestBit( const HugeNumber& lhg );
	friend HugeNumber DivKernel( const HugeNumber& lhg, const HugeNumber& rhg );

	uint32_t * m_value;
	size_t m_size;// non 0 length
	size_t m_length;//length of m_value
	int m_scale;//32n
	Sign m_sign;

	static const int S_prefix[17];
};

////////////////////////////////////////

#endif /* INCLUDE_HUGENUMBER_HPP */
