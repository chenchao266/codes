#ifndef __LCG_CPP_CGINT_2005_11_5__
#define __LCG_CPP_CGINT_2005_11_5__

#include"CGUINT.h"

template< class _TYPE, int _COUNT >
class CGINT
{
	public:	// ���캯��
		CGINT();
		CGINT( int n );
		CGINT( const CGUINT& b );
		CGINT( const char* str );

	public:	// ���⺯��

		// ֧��intת��
		CGINT  operator=( int n );
		operator int();

		// ֧��CGUINTת��
		operator CGUINT();
		CGINT operator=( const CGUINT& b );

		// ���Ų���
		friend _TYPE GetSign( const CGINT& a );

	public:	// �Ѻ���,���ز�����+,-,*,/,>>,<<,&,|,^,~
		
		// ��������
		friend CGINT operator+( const CGINT& a, const CGINT& b );
		friend CGINT operator-( const CGINT& a, const CGINT& b );
		friend CGINT operator*( const CGINT& a, const CGINT& b );
		friend CGINT operator/( const CGINT& a, const CGINT& b );
		friend CGINT operator%( const CGINT& a, const CGINT& b );

		CGINT operator-();

		// ��λ����
		friend CGINT operator>>( const CGINT& a, int bits );
		friend CGINT operator<<( const CGINT& a, int bits );

		// λ����
		friend CGINT operator&( const CGINT& a, const CGINT& b );
		friend CGINT operator|( const CGINT& a, const CGINT& b );
		friend CGINT operator^( const CGINT& a, const CGINT& b );
		friend CGINT operator~( const CGINT& a );
		
		// �߼�����
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
//                           ���캯����һЩ���⺯��
//-----------------------------------------------------------------------------

// ȱʡ���캯��
template< class _TYPE, int _COUNT >
CGINT<_TYPE,_COUNT>::CGINT()
{
}

template< class _TYPE, int _COUNT >
CGINT<_TYPE,_COUNT>::CGINT( const CGUINT<_TYPE,_COUNT>& b )
{
	m_uint = b;
}

// ��int����CGINT
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


// ��CGINTת��Ϊint
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

// ��ֵ����,��int����CGINT
template< class _TYPE, int _COUNT >
CGINT<_TYPE,_COUNT> CGINT<_TYPE,_COUNT>::operator=( int n )
{
	CGINT<_TYPE,_COUNT> c( n );
	*this = c;
	return *this;
}

// ��ֵ����,��CGUINT����CGINT
template< class _TYPE, int _COUNT >
CGINT<_TYPE,_COUNT> CGINT<_TYPE,_COUNT>::operator=( const CGUINT<_TYPE,_COUNT>& b )
{
	m_uint = b;
	return *this;
}

// ת������,����CGUINT
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
// ��ʵֻ��һ��ָ�����ɼ���,����Ҫ����������
template< class _TYPE, int _COUNT >
CGINT<_TYPE,_COUNT> operator+( const CGINT<_TYPE,_COUNT>& a, const CGINT<_TYPE,_COUNT>& b )
{
	_TYPE sa, sb;
	CGINT<_TYPE,_COUNT> c, d;
	CGUINT<_TYPE,_COUNT> One( 1 );

	c = a;
	if( sa = GetSign( a ) )
		c.m_uint = ~c.m_uint + One;

	d  = b;
	if( sb = GetSign( b ) )
		d.m_uint = ~d.m_uint + One;

	if( sa == sb )
	{
		// ͬ��,-x-y => -(x+y), x+y
		c.m_uint = c.m_uint + d.m_uint;

		// ����Ƿ����
		if( GetSign( c ) )
			Exception( ERROR_ADD_CARRAY, 0 );

		// ��������
		if( sa )
			c.m_uint = ~c.m_uint + One;
	}
	else
	{
		// ���,ת��Ϊ����,���������������������
		if( sa )	// -x+y => y-x;
			c.m_uint = d.m_uint - c.m_uint;
		else		// x+(-y) => x-y
			c.m_uint = c.m_uint - d.m_uint;
	}

	return c;
}

// CGINT - CGINT
template< class _TYPE, int _COUNT >
CGINT<_TYPE,_COUNT> operator-( const CGINT<_TYPE,_COUNT>& a, const CGINT<_TYPE,_COUNT>& b )
{

	_TYPE sa, sb;
	CGINT<_TYPE,_COUNT> c, d;
	CGUINT<_TYPE,_COUNT> One( 1 );

	c  = a;
	if( sa = GetSign( a ) )
		c.m_uint = ~c.m_uint + One;

	d  = b;
	if( sb = GetSign( b ) )
		d.m_uint = ~d.m_uint + One;

	if( sa == sb )
	{
		// ͬ��,���������������������
		if( sa )	// -x-(-y) => y-x;
			c.m_uint = d.m_uint - c.m_uint;
		else		// x-y
			c.m_uint = c.m_uint - d.m_uint;
	}
	else
	{
		// ���,ת��Ϊ�ӷ�,x-(-y) => x+y, -x-y => -(x+y)
		c.m_uint = c.m_uint + d.m_uint;

		// ����Ƿ����
		if( GetSign( c ) )
			Exception( ERROR_ADD_CARRAY, 0 );
		
		// ��������
		if( sa )
			c.m_uint = ~c.m_uint + One;
	}

	return c;
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

	c = a, d = b;

	if( sa = GetSign( a ) )
		c.m_uint = ~c.m_uint + One;

	if( sb = GetSign( b ) )
		d.m_uint = ~d.m_uint + One;

	c.m_uint = c.m_uint * d.m_uint;

	// �߶˲��ֱ�����
	if( Exception( 0, 0 ) & ERROR_MUL_OVERFLOW, 0 )
		Exception( ERROR_MUL_OVERFLOW, 0 );

	// ���
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

	c = a, d = b;

	if( sa = GetSign( a ) )
		c.m_uint = ~c.m_uint + One;

	if( sb = GetSign( b ) )
		d.m_uint = ~d.m_uint + One;

	// ��0��
	if( d.m_uint == Zero )
	{
		Exception( ERROR_DIV_BYZERO, 0 );
		return Zero;
	}

	c.m_uint = c.m_uint / d.m_uint;

	// ������
	if( Exception( 0, 0 ) & ERROR_DIV_PRECISION )
		Exception( ERROR_DIV_PRECISION, 0 );

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

// ��Ҫ���з�����չ
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
		// ��������,��չ����
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

	c = a, d = b;

	if( sa = GetSign( a ) )
		c.m_uint = ~c.m_uint + One;

	if( sb = GetSign( b ) )
		d.m_uint = ~d.m_uint + One;

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

	c = a, d = b;

	if( sa = GetSign( a ) )
		c.m_uint = ~c.m_uint + One;

	if( sb = GetSign( b ) )
		d.m_uint = ~d.m_uint + One;

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

