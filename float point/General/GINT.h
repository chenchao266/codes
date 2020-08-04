// 本模板是第3章<通用整型运算>实现的一部分,支持任意长度的有符号整型
// 酒泉卫星发射中心技术部  刘纯根  2007-7

#ifndef __LCG_CPP_CGINT_2005_11_5__
#define __LCG_CPP_CGINT_2005_11_5__

#include"GUINT.h"

template< class _TYPE, int _COUNT >
class CGINT
{
	public:	// 构造函数
		CGINT();
		CGINT( int n );
		CGINT( const CGUINT& b );
		CGINT( const char* str );

	public:	// 特殊函数

		// 支持int转换
		CGINT  operator=( int n );
		operator int();

		// 支持CGUINT转换
		operator CGUINT();
		CGINT operator=( const CGUINT& b );

		// 符号操作
		friend _TYPE GetSign( const CGINT& a );

	public:	// 友函数,重载操作符+,-,*,/,>>,<<,&,|,^,~
		
		// 基本运算
		friend CGINT operator+( const CGINT& a, const CGINT& b );
		friend CGINT operator-( const CGINT& a, const CGINT& b );
		friend CGINT operator*( const CGINT& a, const CGINT& b );
		friend CGINT operator/( const CGINT& a, const CGINT& b );
		friend CGINT operator%( const CGINT& a, const CGINT& b );

		CGINT operator-();

		// 移位操作
		friend CGINT operator>>( const CGINT& a, int bits );
		friend CGINT operator<<( const CGINT& a, int bits );

		// 位操作
		friend CGINT operator&( const CGINT& a, const CGINT& b );
		friend CGINT operator|( const CGINT& a, const CGINT& b );
		friend CGINT operator^( const CGINT& a, const CGINT& b );
		friend CGINT operator~( const CGINT& a );
		
		// 逻辑操作
		friend int operator> ( const CGINT& a, const CGINT& b );
		friend int operator>=( const CGINT& a, const CGINT& b );
		friend int operator< ( const CGINT& a, const CGINT& b );
		friend int operator<=( const CGINT& a, const CGINT& b );
		friend int operator==( const CGINT& a, const CGINT& b );
		friend int operator!=( const CGINT& a, const CGINT& b );
		friend int operator! ( const CGINT& b );

	public:
		CGUINT<_TYPE,_COUNT> m_uint;
};

//-----------------------------------------------------------------------------
//                           构造函数与一些特殊函数
//-----------------------------------------------------------------------------

// 缺省构造函数
template< class _TYPE, int _COUNT >
CGINT<_TYPE,_COUNT>::CGINT()
{
}

template< class _TYPE, int _COUNT >
CGINT<_TYPE,_COUNT>::CGINT( const CGUINT<_TYPE,_COUNT>& b )
{
	m_uint = b;
}

// 从int构造CGINT
template< class _TYPE, int _COUNT >
CGINT<_TYPE,_COUNT>::CGINT( int n )
{
	CGUINT<_TYPE,_COUNT> One( 1 );

	if( n >= 0 )
		m_uint = n;
	else
	{
		m_uint = -n;
		m_uint = ~m_uint + One;
	}
}

template< class _TYPE, int _COUNT >
CGINT<_TYPE,_COUNT>::CGINT( const char* str )
{
	m_uint = str;
}


// 将CGINT转换为int
template< class _TYPE, int _COUNT >
CGINT<_TYPE,_COUNT>::operator int()
{
	int n;
	CGINT<_TYPE,_COUNT> Zero;
	CGUINT<_TYPE,_COUNT> One( 1 );

	if( *this >= Zero )
		n = m_uint;
	else
	{
		n = ~m_uint + One;
		n = -n;
	}

	return n;
}

// 赋值函数,由int创建CGINT
template< class _TYPE, int _COUNT >
CGINT<_TYPE,_COUNT> CGINT<_TYPE,_COUNT>::operator=( int n )
{
	CGINT<_TYPE,_COUNT> c( n );
	*this = c;
	return *this;
}

// 赋值函数,由CGUINT创建CGINT
template< class _TYPE, int _COUNT >
CGINT<_TYPE,_COUNT> CGINT<_TYPE,_COUNT>::operator=( const CGUINT<_TYPE,_COUNT>& b )
{
	m_uint = b;
	return *this;
}

// 转换函数,返回CGUINT
template< class _TYPE, int _COUNT >
CGINT<_TYPE,_COUNT>::operator CGUINT<_TYPE,_COUNT>()
{
	return m_uint;
}

template< class _TYPE, int _COUNT >
_TYPE GetSign( const CGINT<_TYPE,_COUNT>& a )
{
	return a.m_uint.m_data[_COUNT-1]&(_TYPE)_TYPE_HIGHT_BIT;
}

//-----------------------------------------------------------------------------
//                           +,-,*,/
//-----------------------------------------------------------------------------

// CGINT + CGINT
// 其实只需一条指令即可完成计算,但需要进行溢出检测
template< class _TYPE, int _COUNT >
CGINT<_TYPE,_COUNT> operator+( const CGINT<_TYPE,_COUNT>& a, const CGINT<_TYPE,_COUNT>& b )
{
	_TYPE sa, sb, sc;
	CGINT<_TYPE,_COUNT> c;
	unsigned int nErrors;

	sa = GetSign( a );
	sb = GetSign( b );

	nErrors = Exception( 0, 0 );
	c.m_uint = a.m_uint + b.m_uint;
	Exception( nErrors, ERROR_ALL );

	sc = GetSign( c );

	// ++-,--+
	if( ( sa & sb & !sc ) || ( !sa && !sb && sc ) )
		Exception( ERROR_ADD_CARRAY, 0 );

	return c;
}

// CGINT - CGINT
template< class _TYPE, int _COUNT >
CGINT<_TYPE,_COUNT> operator-( const CGINT<_TYPE,_COUNT>& a, const CGINT<_TYPE,_COUNT>& b )
{
	CGINT<_TYPE,_COUNT> One(1);
	return a + ~b + One;
}

// -CGINT
template< class _TYPE, int _COUNT >
CGINT<_TYPE,_COUNT> CGINT<_TYPE,_COUNT>::operator-()
{
	CGUINT<_TYPE,_COUNT> One( 1 );
	return ~m_uint + One;
}

// CGINT * CGINT
template< class _TYPE, int _COUNT >
CGINT<_TYPE,_COUNT> operator*( const CGINT<_TYPE,_COUNT>& a, const CGINT<_TYPE,_COUNT>& b )
{
	_TYPE sa, sb;
	CGINT<_TYPE,_COUNT> c, d;
	CGUINT<_TYPE,_COUNT> One( 1 );

	if( sa = GetSign( a ) )
		c.m_uint = ~a.m_uint + One;
	else
		c = a;

	if( sb = GetSign( b ) )
		d.m_uint = ~b.m_uint + One;
	else
		d = b;

	c.m_uint = c.m_uint * d.m_uint;

	// 高端部分被抛弃
//	if( Exception( 0, 0 ) & ERROR_MUL_OVERFLOW, 0 )
//		Exception( ERROR_MUL_OVERFLOW, 0 );

	// 溢出
	if( GetSign( c ) )
		Exception( ERROR_MUL_OVERFLOW, 0 );

	if( sa^sb )
		c.m_uint = ~c.m_uint + One;

	return c;
}

// CGINT / CGINT
template< class _TYPE, int _COUNT >
CGINT<_TYPE,_COUNT> operator/( const CGINT<_TYPE,_COUNT>& a, const CGINT<_TYPE,_COUNT>& b )
{
	_TYPE sa, sb;
	CGINT<_TYPE,_COUNT> c, d;
	CGUINT<_TYPE,_COUNT> One( 1 ), Zero;

	if( sa = GetSign( a ) )
		c.m_uint = ~a.m_uint + One;
	else
		c = a;

	if( sb = GetSign( b ) )
		d.m_uint = ~b.m_uint + One;
	else
		d = b;

	// 被0除
	if( d.m_uint == Zero )
	{
		Exception( ERROR_DIV_BYZERO, 0 );
		return Zero;
	}

	c.m_uint = c.m_uint / d.m_uint;

	// 有余数
//	if( Exception( 0, 0 ) & ERROR_DIV_PRECISION )
//		Exception( ERROR_DIV_PRECISION, 0 );

	if( sa^sb )
		c.m_uint = ~c.m_uint + One;

	return c;
}

template< class _TYPE, int _COUNT >
CGINT<_TYPE,_COUNT> operator%( const CGINT<_TYPE,_COUNT>& a, const CGINT<_TYPE,_COUNT>& b )
{
	return a - b*( a/b );
}

//-----------------------------------------------------------------------------
//                           >>,<<
//-----------------------------------------------------------------------------

// 需要进行符号扩展
template< class _TYPE, int _COUNT >
CGINT<_TYPE,_COUNT> operator>>( const CGINT<_TYPE,_COUNT>& a, int bits )
{
	CGUINT<_TYPE,_COUNT> mask, One( 1 ), c, Zero;

	c = a.m_uint;

	if( bits == 0 )
		return a;

	if( bits < 0 )
		return a << -bits;

	if( bits >= _SIZE*8 )
		return Zero;

	if( bits == 0 )
		return a;

	c = c >> bits;
	
	if( GetSign( a ) )
	{
		// 负数右移,扩展符号
		mask = One;
		mask = mask << int( _SIZE*8 - bits );
		mask = mask - One;
		mask = ~mask;

		c = c | mask;
	}

	return c;
}

template< class _TYPE, int _COUNT >
CGINT<_TYPE,_COUNT> operator<<( const CGINT<_TYPE,_COUNT>& a, int bits )
{
	if( bits < 0 )
		return a >> -bits;

	return a.m_uint << bits;
}


//-----------------------------------------------------------------------------
//                           &,|,^,,~
//-----------------------------------------------------------------------------
template< class _TYPE, int _COUNT >
CGINT<_TYPE,_COUNT> operator&( const CGINT<_TYPE,_COUNT>& a, const CGINT<_TYPE,_COUNT>& b )
{
	return a.m_uint & b.m_uint;
}

template< class _TYPE, int _COUNT >
CGINT<_TYPE,_COUNT> operator|( const CGINT<_TYPE,_COUNT>& a, const CGINT<_TYPE,_COUNT>& b )
{
	return a.m_uint | b.m_uint;
}

template< class _TYPE, int _COUNT >
CGINT<_TYPE,_COUNT> operator^( const CGINT<_TYPE,_COUNT>& a, const CGINT<_TYPE,_COUNT>& b )
{
	return a.m_uint ^ b.m_uint;
}

template< class _TYPE, int _COUNT >
CGINT<_TYPE,_COUNT> operator~( const CGINT<_TYPE,_COUNT>& a )
{
	return ~a.m_uint;
}

//-----------------------------------------------------------------------------
//                           >,>=,<,<=,==,!=,!
//-----------------------------------------------------------------------------
template< class _TYPE, int _COUNT >
int operator>( const CGINT<_TYPE,_COUNT>& a, const CGINT<_TYPE,_COUNT>& b )
{
	_TYPE sa, sb;
	CGINT<_TYPE,_COUNT> c, d;
	CGUINT<_TYPE,_COUNT> One( 1 );

	if( sa = GetSign( a ) )
		c.m_uint = ~a.m_uint + One;
	else
		c = a;

	if( sb = GetSign( b ) )
		d.m_uint = ~b.m_uint + One;
	else
		d = b;

	if( sa )
	{
		if( sb )
			return c.m_uint < d.m_uint;
		else
			return 0;
	}
	else
	{
		if( sb )
			return 1;
		else
			return c.m_uint > d.m_uint;
	}
}

template< class _TYPE, int _COUNT >
int operator<( const CGINT<_TYPE,_COUNT>& a, const CGINT<_TYPE,_COUNT>& b )
{
	_TYPE sa, sb;
	CGINT<_TYPE,_COUNT> c, d;
	CGUINT<_TYPE,_COUNT> One( 1 );

	if( sa = GetSign( a ) )
		c.m_uint = ~a.m_uint + One;
	else
		c = a;

	if( sb = GetSign( b ) )
		d.m_uint = ~b.m_uint + One;
	else
		d = b;

	if( sa )
	{
		if( sb )
			return c.m_uint > d.m_uint;
		else
			return 1;
	}
	else
	{
		if( sb )
			return 0;
		else
			return c.m_uint < d.m_uint;
	}
}

template< class _TYPE, int _COUNT >
int operator>=( const CGINT<_TYPE,_COUNT>& a, const CGINT<_TYPE,_COUNT>& b )
{
	return ( a > b ) || ( a == b );
}

template< class _TYPE, int _COUNT >
int operator<=( const CGINT<_TYPE,_COUNT>& a, const CGINT<_TYPE,_COUNT>& b )
{
	return ( a < b ) || ( a == b );	
}

template< class _TYPE, int _COUNT >
int operator==( const CGINT<_TYPE,_COUNT>& a, const CGINT<_TYPE,_COUNT>& b )
{
	return a.m_uint == b.m_uint;
}

template< class _TYPE, int _COUNT >
int operator!=( const CGINT<_TYPE,_COUNT>& a, const CGINT<_TYPE,_COUNT>& b )
{
	return a.m_uint != b.m_uint;
}

template< class _TYPE, int _COUNT >
int operator!( const CGINT<_TYPE,_COUNT>& b )
{
	return !b.m_uint;
}


//-----------------------------------------------------------------------------
//                           IntToStr,IntFromStr
//-----------------------------------------------------------------------------

template< class _TYPE, int _COUNT >
int IntFromStr( CGINT<_TYPE,_COUNT>& n, const char* str = 0 )
{
	return IntFromStr( *(CGUINT<_TYPE,_COUNT>*)&n, str );
}

template< class _TYPE, int _COUNT >
const char* IntToStr( const CGINT<_TYPE,_COUNT>& n, int base = 10, char prefix = 0, 
					  int digits = 0, int bCase = 1, int* pCnt = 0 )
{
	static char str[_SIZE*8+32];
	char *p;
	int i;
	CGUINT<_TYPE,_COUNT> m, One( 1 );

	m = n.m_uint;

	i = 0;
	if( GetSign( n ) )
	{
		str[0]= '-';
		i	  = 1;
		m	  = ~m + One;
	}

	p = (char*)IntToStr( m, base, prefix, digits, bCase, pCnt );
	while( *p )
	{
		str[i++] = *p;
		p++;
	}

	str[i] = '\0';

	if( pCnt != 0 )
		*pCnt = i;

	return str;
}

#endif