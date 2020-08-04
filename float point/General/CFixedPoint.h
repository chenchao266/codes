#ifndef __LCG_FIXED_POINT_2005_11_10__
#define __LCG_FIXED_POINT_2005_11_10__

#include"CGUINT.h"
#include"MathLib.h"

#define _INT_SIGN_BIT ( CGUINT<_TYPE,_INT_COUNT>(1) << int( _INT_COUNT*sizeof(_TYPE)*8 - 1 ) )

template<class _TYPE,int _INT_COUNT,int _FRACTION_COUNT>
class CFixedPoint
{
	public:	// 构造函数
		CFixedPoint();
		CFixedPoint( int n );
		CFixedPoint( int n, int m );
		CFixedPoint( const char *str );

	public:	
		// 处理int的函数
		CFixedPoint operator=( int n );
		operator int();

		friend CFixedPoint FractionExp( const CFixedPoint& x, int& exp );
		friend CFixedPoint Integer( const CFixedPoint& x );
		friend CFixedPoint Abs( const CFixedPoint& x );
		friend CFixedPoint Scale( const CFixedPoint& x, int exp );

		static CFixedPoint QNaN();
		static CFixedPoint Zero( int sign );
		static CFixedPoint Infinite( int sign );

		friend int GetSign( const CFixedPoint& x );
		friend void SetSign( CFixedPoint& x );
		friend void ClrSign( CFixedPoint& x );

		friend int Class( const CFixedPoint& x  );

	public:	// 友函数,重载操作符+,-,*,/,>>,<<,&,|,^,~
		// 基本运算
		friend CFixedPoint operator+( const CFixedPoint& a, const CFixedPoint& b );
		friend CFixedPoint operator-( const CFixedPoint& a, const CFixedPoint& b );
		friend CFixedPoint operator*( const CFixedPoint& a, const CFixedPoint& b );
		friend CFixedPoint operator/( const CFixedPoint& a, const CFixedPoint& b );

		CFixedPoint  operator-();

		// 逻辑操作
		friend int operator> ( const CFixedPoint& a, const CFixedPoint& b );
		friend int operator>=( const CFixedPoint& a, const CFixedPoint& b );
		friend int operator< ( const CFixedPoint& a, const CFixedPoint& b );
		friend int operator<=( const CFixedPoint& a, const CFixedPoint& b );
		friend int operator==( const CFixedPoint& a, const CFixedPoint& b );
		friend int operator!=( const CFixedPoint& a, const CFixedPoint& b );
		friend int operator! ( const CFixedPoint& b );

	private:
		CGUINT<_TYPE,_FRACTION_COUNT>	m_nFraction;
		CGUINT<_TYPE,_INT_COUNT>		m_nInt;
};


//-----------------------------------------------------------------------------
//                           构造函数与一些特殊函数
//-----------------------------------------------------------------------------

// 缺省构造函数
template<class _TYPE,int _INT_COUNT,int _FRACTION_COUNT>
CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT>::CFixedPoint()
{
	m_nInt		= 0;
	m_nFraction = 0;
}

// 从int构造CFixedPoint
template<class _TYPE,int _INT_COUNT,int _FRACTION_COUNT>
CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT>::CFixedPoint( int n )
{
	m_nFraction = 0;

	if( n >= 0 )
		m_nInt = n;
	else
	{
		m_nInt = -n;
		SetSign( *this );
	}
}

// 从分数构造CFixedPoint
template<class _TYPE,int _INT_COUNT,int _FRACTION_COUNT>
CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT>::CFixedPoint( int n, int m )
{
	CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT> x( n ), y( m );
	*this = x/y;
}

// 从字符串构造CFixedPoint
template<class _TYPE,int _INT_COUNT,int _FRACTION_COUNT>
CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT>::CFixedPoint( const char *str )
{
	DataFromStr( *this, str );
}

// 从int得到CFixedPoint
template<class _TYPE,int _INT_COUNT,int _FRACTION_COUNT>
CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT>
CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT>::operator=( int n )
{
	CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT> x( n );
	*this = x;
	return *this;
}

template<class _TYPE,int _INT_COUNT,int _FRACTION_COUNT>
CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT>
Scale( const CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT>& x, int exp )
{
	CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT> y;
	CGUINT<_TYPE,_INT_COUNT+_FRACTION_COUNT> *py;
	int sy;

	y  = x;
	py = (CGUINT<_TYPE,_INT_COUNT+_FRACTION_COUNT>*)&y;

	sy = GetSign( y );
	ClrSign( y );

	if( exp >= 0 )
		*py = *py << exp;
	else
		*py = *py >> -exp;

	if( sy )
		SetSign( y );

	return y;
}

template<class _TYPE,int _INT_COUNT,int _FRACTION_COUNT>
CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT>
Abs( const CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT>& x )
{
	CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT> y;

	if( GetSign( x ) )
	{
		y = x;
		ClrSign( y );
		return y;
	}

	return x;
}

template<class _TYPE,int _INT_COUNT,int _FRACTION_COUNT>
CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT>
FractionExp( const CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT>& x, int& exp )
{
	CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT> y, Zero;
	CGUINT<_TYPE,_INT_COUNT+_FRACTION_COUNT> *py;
	int sy;

	y   = x;
	exp = 0;
	py  = (CGUINT<_TYPE,_INT_COUNT+_FRACTION_COUNT>*)&y;
	sy  = GetSign( y );

	ClrSign( y );

	if( y == Zero )
		return Zero;

	while( y.m_nInt > 1 )
	{
		*py = *py >> 1;
		exp++;
	}

	while( y.m_nInt < 1 )
	{
		*py = *py << 1;
		exp--;
	}

	return y;
}

// 转换为int
template<class _TYPE,int _INT_COUNT,int _FRACTION_COUNT>
CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT>
Integer( const CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT>& x )
{
	CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT> nt;

	nt = x;
	nt.m_nFraction = 0;

	return nt;
}

// 转换为int
template<class _TYPE,int _INT_COUNT,int _FRACTION_COUNT>
CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT>::operator int()
{
	int n;
	CGUINT<_TYPE,_INT_COUNT> nt;

	if( GetSign( *this ) )
	{
		// 清除符号
		nt = m_nInt & ( ~_INT_SIGN_BIT );

		n  = nt;
		n  =-n;
	}
	else
		n  = m_nInt;

	return n;
}

// 获取符号,1是-,0是+
template<class _TYPE,int _INT_COUNT,int _FRACTION_COUNT>
int GetSign( const CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT>& x )
{
	CGUINT<_TYPE,_INT_COUNT> Sign, Zero;

	Sign = x.m_nInt & _INT_SIGN_BIT;

	if( Sign != Zero )
		return 1;
	else
		return 0;
}

// 设置符号位
template<class _TYPE,int _INT_COUNT,int _FRACTION_COUNT>
void SetSign( CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT>& x )
{
	x.m_nInt = x.m_nInt | _INT_SIGN_BIT;
}

//清除符号位
template<class _TYPE,int _INT_COUNT,int _FRACTION_COUNT>
void ClrSign( CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT>& x )
{
	x.m_nInt = x.m_nInt & ( ~_INT_SIGN_BIT );
}

//-----------------------------------------------------------------------------
//                           +,-,*,/
//-----------------------------------------------------------------------------


// -CFixedPoint
template<class _TYPE,int _INT_COUNT,int _FRACTION_COUNT>
CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT> 
CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT>::operator-()
{
	CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT> x;

	x = *this;

	if( GetSign( x ) )
		ClrSign( x );
	else
		SetSign( x );
	
	return x;
}

// +
template<class _TYPE,int _INT_COUNT,int _FRACTION_COUNT>
CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT> 
operator+( const CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT>& x, 
		   const CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT>& y )
{
	unsigned int sx, sy;
	CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT> a, b, c;
	CGUINT<_TYPE,_INT_COUNT+_FRACTION_COUNT> *pa, *pb, *pc, One( 1 );

	a = x, b = y;
	
	// 提取符号
	sx = GetSign( x );
	ClrSign( a );
	pa = (CGUINT<_TYPE,_INT_COUNT+_FRACTION_COUNT>*)&a;

	sy = GetSign( y );
	ClrSign( b );
	pb = (CGUINT<_TYPE,_INT_COUNT+_FRACTION_COUNT>*)&b;

	pc = (CGUINT<_TYPE,_INT_COUNT+_FRACTION_COUNT>*)&c;

	if( sx == sy )
	{
		// 同号,x+y,-x+(-y)=-(x+y)
		*pc = *pa + *pb;

		// 符号位被修改,发现溢出
		if( GetSign( c ) )
		{
			Exception( ERROR_ADD_CARRAY, 0 );
			c = CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT>::Infinite( sx );
		}

		if( sx )
			SetSign( c );
	}
	else
	{
		// 异号
		if( sx )	// -x+y
			*pc = *pb - *pa;
		else		// +x-y
			*pc = *pa - *pb;

		// 负值需要调整符号
		if( GetSign( c ) )
		{
			*pc = ~(*pc) + One;
			SetSign( c );
		}
	}
	
	return c;
}

// -
template<class _TYPE,int _INT_COUNT,int _FRACTION_COUNT>
CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT> 
operator-( const CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT>& x,
		   const CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT>& y )
{
	unsigned int sx, sy;
	CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT> a, b, c;
	CGUINT<_TYPE,_INT_COUNT+_FRACTION_COUNT> *pa, *pb, *pc, One( 1 );

	a = x, b = y;
	
	// 提取符号
	sx = GetSign( x );
	ClrSign( a );
	pa = (CGUINT<_TYPE,_INT_COUNT+_FRACTION_COUNT>*)&a;

	sy = GetSign( y );
	ClrSign( b );
	pb = (CGUINT<_TYPE,_INT_COUNT+_FRACTION_COUNT>*)&b;

	pc = (CGUINT<_TYPE,_INT_COUNT+_FRACTION_COUNT>*)&c;

	if( sx == sy )
	{
		// 同号
		if( sx )	// -x-(-y) => y-x
			*pc = *pb - *pa;
		else		// x-y
			*pc = *pa - *pb;

		// 负值需要调整符号
		if( GetSign( c ) )
		{
			*pc = ~(*pc) + One;
			SetSign( c );
		}
	}
	else
	{
		// 异号,	// -x-y => -(x+y), x-(-y) => x+y
		*pc = *pa + *pb;

		// 符号位被修改,发现溢出
		if( GetSign( c ) )
		{
			Exception( ERROR_ADD_CARRAY, 0 );
			c = CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT>::Infinite( sx );
		}

		if( sx )
			SetSign( c );
	}
	
	return c;
}

// *
template<class _TYPE,int _INT_COUNT,int _FRACTION_COUNT>
CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT> 
operator*( const CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT>& x, 
		   const CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT>& y )
{
	// 积内存布局:m_nFraction,m_nFraction.m_nInt,m_nInt
	// 小数部分分界点位于2倍m_nFraction处

	unsigned int sx, sy;
	CGUINT<_TYPE,(_FRACTION_COUNT+_INT_COUNT)*2> a, b, c;
	CGUINT<_TYPE,_INT_COUNT>	 *pi, Zeroi;	//  整数部分
	CGUINT<_TYPE,_FRACTION_COUNT>*pf, Zerof;	//  小数部分
	CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT> xx, yy, z;
	
	// 提取符号
	xx = x;
	sx = GetSign( x );
	ClrSign( xx );

	yy = y;
	sy = GetSign( y );
	ClrSign( yy );

	// 转化为整型
	*(CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT>*)&a = xx;
	*(CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT>*)&b = yy;

	c = a*b;

	pf = (CGUINT<_TYPE,_FRACTION_COUNT>*)&c;
	pi = (CGUINT<_TYPE,_INT_COUNT>*)&pf[2];

	// 低位有舍弃
	if( pf[0] != Zerof )
		Exception( ERROR_MUL_PRECISION, 0 );

	z.m_nInt	  = pi[0];
	z.m_nFraction = pf[1];

	// 高位有舍弃
	if( pi[1] != Zeroi )
	{
		Exception( ERROR_MUL_OVERFLOW, 0 );
		z = CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT>::Infinite( sx^sy );
	}

	if( sx^sy )
		SetSign( z );
	
	return z;
}


// /
template<class _TYPE,int _INT_COUNT,int _FRACTION_COUNT>
CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT> 
operator/( const CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT>& x, 
		   const CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT>& y )
{
	// 商内存布局:m_nFraction m_nInt.m_nFraction
	unsigned int sx, sy;
	CGUINT<_TYPE,_FRACTION_COUNT*2+_INT_COUNT> a, b, c;
	CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT> xx, yy, z, Zero;

	// 提取符号
	xx = x;
	sx = GetSign( x );
	ClrSign( xx );

	yy = y;
	sy = GetSign( y );
	ClrSign( yy );

	if( yy == Zero )
	{
		Exception( ERROR_DIV_BYZERO, 0 );

		if( xx == Zero )
			return CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT>::QNaN();
		else
			return CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT>::Infinite( sx^sy );
	}

	// 转化为整型
	*(CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT>*)&a = xx;
	*(CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT>*)&b = yy;

	// 移动一个m_nFraction长度
	a = a << int( _FRACTION_COUNT*sizeof(_TYPE)*8 );
	c = a/b;

	// 检测低位舍弃
	if( Exception( 0, 0 ) & ERROR_DIV_PRECISION )
		Exception( ERROR_DIV_PRECISION, 0 );

	z = *(CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT>*)&c;

	if( sx^sy )
		SetSign( z );
	
	return z;
}

//-----------------------------------------------------------------------------
//                           >,>=,<,<=,==,!=
//-----------------------------------------------------------------------------

// >
template<class _TYPE,int _INT_COUNT,int _FRACTION_COUNT>
int 
operator>( const CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT>& x, 
		   const CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT>& y )
{
	unsigned int sx, sy;
	CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT> a, b;
	CGUINT<_TYPE,_INT_COUNT+_FRACTION_COUNT> *pa, *pb, Zero;

	a = x, b = y;
	
	sx = GetSign( x );
	ClrSign( a );
	pa = (CGUINT<_TYPE,_INT_COUNT+_FRACTION_COUNT>*)&a;

	sy = GetSign( y );
	ClrSign( b );
	pb = (CGUINT<_TYPE,_INT_COUNT+_FRACTION_COUNT>*)&b;

	// +0,-0
	if( *pa == Zero && *pb == Zero )
		return 0;

	if( sx == sy )
	{
		if( sx )	// --
			return *pa < *pb;
		else		// ++
			return *pa > *pb;
	}
	else
	{
		if( sx )	// -+
			return 0;
		else		// +-
			return 1;
	}
}

// <
template<class _TYPE,int _INT_COUNT,int _FRACTION_COUNT>
int 
operator<( const CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT>& x, 
		   const CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT>& y )
{
	unsigned int sx, sy;
	CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT> a, b, c;
	CGUINT<_TYPE,_INT_COUNT+_FRACTION_COUNT> *pa, *pb, Zero;

	a = x, b = y;
	
	sx = GetSign( x );
	ClrSign( a );
	pa = (CGUINT<_TYPE,_INT_COUNT+_FRACTION_COUNT>*)&a;

	sy = GetSign( y );
	ClrSign( b );
	pb = (CGUINT<_TYPE,_INT_COUNT+_FRACTION_COUNT>*)&b;

	// +0,-0
	if( *pa == Zero && *pb == Zero )
		return 0;

	if( sx == sy )
	{
		if( sx )	// --
			return *pa > *pb;
		else		// ++
			return *pa < *pb;
	}
	else
	{
		if( sx )	// -+
			return 1;
		else		// +-
			return 0;
	}
}

// ==
template<class _TYPE,int _INT_COUNT,int _FRACTION_COUNT>
int 
operator==( const CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT>& x, 
		    const CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT>& y )
{
	unsigned int sx, sy;
	CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT> a, b, c;
	CGUINT<_TYPE,_INT_COUNT+_FRACTION_COUNT> *pa, *pb, Zero;

	a = x, b = y;
	
	sx = GetSign( x );
	ClrSign( a );
	pa = (CGUINT<_TYPE,_INT_COUNT+_FRACTION_COUNT>*)&a;

	sy = GetSign( y );
	ClrSign( b );
	pb = (CGUINT<_TYPE,_INT_COUNT+_FRACTION_COUNT>*)&b;

	// +0,-0
	if( *pa == Zero && *pb == Zero )
		return 1;

	if( ( sx == sy ) && ( *pa == *pb ) )
		return 1;

	return 0;
}

// !=
template<class _TYPE,int _INT_COUNT,int _FRACTION_COUNT>
int 
operator!=( const CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT>& x, 
		    const CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT>& y )
{
	unsigned int sx, sy;
	CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT> a, b, c;
	CGUINT<_TYPE,_INT_COUNT+_FRACTION_COUNT> *pa, *pb, Zero;

	a = x, b = y;
	
	sx = GetSign( x );
	ClrSign( a );
	pa = (CGUINT<_TYPE,_INT_COUNT+_FRACTION_COUNT>*)&a;

	sy = GetSign( y );
	ClrSign( b );
	pb = (CGUINT<_TYPE,_INT_COUNT+_FRACTION_COUNT>*)&b;

	// -0,+0
	if( *pa == Zero && *pb == Zero )
		return 0;

	if( ( sx != sy ) || ( *pa != *pb ) )
		return 1;

	return 0;
}

// >=
template<class _TYPE,int _INT_COUNT,int _FRACTION_COUNT>
int 
operator>=( const CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT>& x, 
		    const CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT>& y )
{
	return ( x > y ) || ( x == y );
}


// <=
template<class _TYPE,int _INT_COUNT,int _FRACTION_COUNT>
int 
operator<=( const CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT>& x, 
		    const CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT>& y )
{
	return ( x < y ) || ( x == y );
}

//-----------------------------------------------------------------------------
//                           特殊函数
//-----------------------------------------------------------------------------

template<class _TYPE,int _INT_COUNT,int _FRACTION_COUNT>
CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT>
CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT>::QNaN()
{
	CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT> x;
	return x;
}

template<class _TYPE,int _INT_COUNT,int _FRACTION_COUNT>
CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT>
CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT>::Zero( int sign )
{
	CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT> x;
	if( sign )
		SetSign( x );
	return x;
}

template<class _TYPE,int _INT_COUNT,int _FRACTION_COUNT>
CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT>
CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT>::Infinite( int sign )
{
	CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT> x;

	x.m_nInt	  = ~x.m_nInt;
	x.m_nFraction = ~x.m_nFraction;
	
	if( sign == 0 )
		ClrSign( x );

	return x;
}

// class
template<class _TYPE,int _INT_COUNT,int _FRACTION_COUNT>
int Class( const CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT>& x  )
{
	int sx;
	CFixedPoint<_TYPE,_INT_COUNT,_FRACTION_COUNT> Zero;

	sx = GetSign( x );

	if( x == Zero )
		return sx == 0 ? CLASS_PZ : CLASS_NZ;

	return sx == 0 ? CLASS_PN : CLASS_NN;
}

#endif