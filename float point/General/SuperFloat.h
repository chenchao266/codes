// ��ģ���ǵ�6��<ͨ�ø�������>ʵ�ֵ�һ����,֧�����ⳤ�ȵĸ�����
// ��ʽ�������Զ�IEEE��չ˫���ȸ�ʽ������
// ��Ȫ���Ƿ������ļ�����  ������  2007-7

#ifndef __LCG_SUPER_FLOAT_POINT_2005_11_20__
#define __LCG_SUPER_FLOAT_POINT_2005_11_20__

#include"GUINT.h"
#include"MathLib.h"

#define FRACTION_BITS	(sizeof(_FTYPE)*_FCOUNT*8)
#define SUPER_EXP_OFF	( ((_PTYPE)-1)>>2 )

// CSuperFloat����������,���������1!
#pragma pack( push, 1 )

template< class _FTYPE, int _FCOUNT, class _PTYPE >
class CSuperFloat
{
	public:
		// IEEE�����ʽ
		typedef struct
		{
			// ע��,�������ֱ���������һ��,��˶�����1
			CGUINT<_FTYPE,_FCOUNT> m_nFraction;
			_PTYPE m_nExp  : sizeof(_PTYPE)*8 - 1;
			_PTYPE m_nSign : 1;
		} _DATA;
	
		// չ����ʽ,���2λ������λ,������С��λ
		typedef struct
		{
			CGUINT<_FTYPE,_FCOUNT> d;
		} _BUF;

		// �˷���չ,���4λ������,������С��
		typedef struct
		{
			CGUINT<_FTYPE,_FCOUNT*2> d;
		} _MUL_BUF;

	public:	// ���캯��
		CSuperFloat();
		CSuperFloat( int n );
		CSuperFloat( int n, int m );
		CSuperFloat( const char *str );

	public:	// ���⺯��

		CSuperFloat operator=( int n );
		operator int();

		// IEEE��ʽչ����ָ�
		friend void BufExpand( const CSuperFloat& x, _BUF& buf, int& exp, int& sign );
		friend CSuperFloat BufRestore( _BUF& buf, int exp, int sign );
		friend CSuperFloat MulBufRestore( _MUL_BUF& buf, int exp, int sign );

		friend CSuperFloat Abs( const CSuperFloat& x );
		friend CSuperFloat Integer( const CSuperFloat& x );
		friend CSuperFloat Scale( const CSuperFloat&, int n );
		friend CSuperFloat FractionExp( const CSuperFloat&, int& exp );

		// ����ֵ
		static CSuperFloat Zero( int sign );		// +0.0,-0.0
		static CSuperFloat QNaN();					// QNaN
		static CSuperFloat Infinite( int sign );	// +INF,-INF

	public:	// IEEE���麯��

		friend int Class( const CSuperFloat& x );
	
	public:	// �Ѻ���,���ز�����+,-,*,/
		friend CSuperFloat operator+( const CSuperFloat& x, const CSuperFloat& y );
		friend CSuperFloat Add( const CSuperFloat& x, const CSuperFloat& y );

		friend CSuperFloat operator-( const CSuperFloat& x, const CSuperFloat& y );
		friend CSuperFloat Sub( const CSuperFloat& x, const CSuperFloat& y );

		friend CSuperFloat operator*( const CSuperFloat& x, const CSuperFloat& y );
		friend CSuperFloat operator/( const CSuperFloat& x, const CSuperFloat& y );

		CSuperFloat  operator-();

		// �߼�����
		friend int operator> ( const CSuperFloat& x, const CSuperFloat& y );
		friend int operator>=( const CSuperFloat& x, const CSuperFloat& y );
		friend int operator< ( const CSuperFloat& x, const CSuperFloat& y );
		friend int operator<=( const CSuperFloat& x, const CSuperFloat& y );
		friend int operator==( const CSuperFloat& x, const CSuperFloat& y );
		friend int operator!=( const CSuperFloat& x, const CSuperFloat& y );
		friend int operator! ( const CSuperFloat& y );

//	private:
	public:
		_DATA m_data;
};

#pragma pack( pop )

//-----------------------------------------------------------------------------
//                           ���캯����һЩ���⺯��
//-----------------------------------------------------------------------------

// ȱʡ���캯��,0.0
template<class _FTYPE,int _FCOUNT,class _PTYPE>
CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>::CSuperFloat()
{
	m_data.m_nSign	   = 0;
	m_data.m_nExp	   = 0;
}

// ��int����CSuperFloat
// ����㷨�����˸����ʽ�ĺ���
template<class _FTYPE,int _FCOUNT,class _PTYPE>
CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>::CSuperFloat( int n )
{
	int exp, sign;
	_BUF buf;

	if( n < 0 )
	{
		sign = 1;
		n	 = -n;
	}
	else
		sign = 0;

	// ����FRACTION_BITS - 2
	buf.d = n;
	exp = FRACTION_BITS - 2;

	if( n != 0 )
		*this = BufRestore( buf, exp, sign );
	else
		*this = Zero( 0 );
}

// �ɷ�������CSuperFloat
template<class _FTYPE,int _FCOUNT,class _PTYPE>
CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>::CSuperFloat( int n, int m )
{
	CSuperFloat<_FTYPE, _FCOUNT,_PTYPE> cn( n ), cm( m );
	*this = cn/cm;
}

// ��int�õ�CSuperFloat
template<class _FTYPE,int _FCOUNT,class _PTYPE>
CSuperFloat<_FTYPE, _FCOUNT,_PTYPE> CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>::
operator=( int n )
{
	CSuperFloat<_FTYPE, _FCOUNT,_PTYPE> x( n );
	*this = x;
	return *this;
}

template<class _FTYPE,int _FCOUNT,class _PTYPE>
CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>::CSuperFloat( const char *str )
{
	DataFromStr( *this, str );
}

// ��������ת��Ϊ����,�ض�ȡ��
/*
template<class _FTYPE,int _FCOUNT,class _PTYPE>
CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>::operator int()
{
	int sign, n, ns, exp;
	CSuperFloat<_FTYPE, _FCOUNT,_PTYPE> x, Two( 2 ), Zero;
	CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>::_BUF buf;
	CGUINT<_FTYPE,_FCOUNT> IntBit(1);

	x = *this;

	// �������
	if( x < Zero )
	{
		sign = 1;
		x = -x;
	}
	else
		sign = 0;

	IntBit = IntBit << int( FRACTION_BITS - 2 );
	IntBit = ~IntBit;

	n = 0;
	while( x.m_data.m_nExp >= SUPER_EXP_OFF )
	{
		n += ( 1 << int( x.m_data.m_nExp - SUPER_EXP_OFF ) );

		// ����λ�ı�,�������
		if( n < 0 )
			return 0;

		// �����Ӧλ
		BufExpand( x, buf, exp, ns );
		buf.d = buf.d & IntBit;
		x = BufRestore( buf, exp, ns );
	}

	if( sign )
		n = -n;
	
	return n;
}
*/

template<class _FTYPE,int _FCOUNT,class _PTYPE>
CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>::operator int()
{
	int sign, ns, exp, k;
	CSuperFloat<_FTYPE, _FCOUNT,_PTYPE> x, Zero;
	CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>::_BUF buf;
	CGUINT<_FTYPE,_FCOUNT> n;

	x = *this;

	// �������
	if( x < Zero )
	{
		sign = 1;
		x = -x;
	}
	else
		sign = 0;

	if( x.m_data.m_nExp >= SUPER_EXP_OFF )
	{
		k = x.m_data.m_nExp - SUPER_EXP_OFF;
		BufExpand( x, buf, exp, ns );
		n = *(CGUINT<_FTYPE,_FCOUNT>*)&buf;

		if( k >= FRACTION_BITS - 2 )
			n = n << int( k - FRACTION_BITS - 2 );
		else
			n = n >> int( FRACTION_BITS - 2 - k );
	}

	if( sign )
		n = ~n + 1;
	
	return n;
}

template<class _FTYPE,int _FCOUNT,class _PTYPE> 
CSuperFloat<_FTYPE, _FCOUNT,_PTYPE> 
Abs( const CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>& x )
{
	CSuperFloat<_FTYPE, _FCOUNT,_PTYPE> y;

	if( x.m_data.m_nSign )
	{
		y = x;
		y.m_data.m_nSign = 0;
		return y;
	}
	else
		return x;
}

// ��������ת��Ϊ����,�ض�ȡ��
template<class _FTYPE,int _FCOUNT,class _PTYPE> 
CSuperFloat<_FTYPE, _FCOUNT,_PTYPE> 
Integer( const CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>& x )
{
	int k;
	CSuperFloat<_FTYPE, _FCOUNT,_PTYPE> xx, Zero;

	xx = x;

	k = int( xx.m_data.m_nExp - SUPER_EXP_OFF );
	if( k >= 0 )
	{
		// ����������

		// �Ǹ�����
		if( k >= FRACTION_BITS - 1 )
			return xx;
		else
		{
			xx.m_data.m_nFraction = xx.m_data.m_nFraction >> int( FRACTION_BITS - k - 1 );
			xx.m_data.m_nFraction = xx.m_data.m_nFraction << int( FRACTION_BITS - k - 1 );
		}
	}
	else
		return Zero;

	return xx;
}

template<class _FTYPE,int _FCOUNT,class _PTYPE> 
CSuperFloat<_FTYPE, _FCOUNT,_PTYPE> 
FractionExp( const CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>& x, int& exp )
{
	CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>::_BUF buf;
	int sign;

	BufExpand( x, buf, exp, sign );
	return BufRestore( buf, 0, sign );
}


//-----------------------------------------------------------------------------
//                           +,-,*,/
//-----------------------------------------------------------------------------

// +
template<class _FTYPE,int _FCOUNT,class _PTYPE>
CSuperFloat<_FTYPE, _FCOUNT,_PTYPE> 
operator+( const CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>& x, 
		   const CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>& y )
{
	CSuperFloat<_FTYPE, _FCOUNT,_PTYPE> xx, yy;

	xx = x, yy = y;

	// ͬ�����
	if( xx.m_data.m_nSign == yy.m_data.m_nSign )
		return Add( xx, yy );

	// ������ת���������

	// x+(-y) => x-y
	if( xx.m_data.m_nSign == 0 )
	{
		yy.m_data.m_nSign = 0;
		return Sub( xx, yy );
	}

	// -x+y => y-x
	xx.m_data.m_nSign = 0;
	return Sub( yy, xx );
}

// -
template<class _FTYPE,int _FCOUNT,class _PTYPE>
CSuperFloat<_FTYPE, _FCOUNT,_PTYPE> 
operator-( const CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>& x, 
		   const CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>& y )
{
	CSuperFloat<_FTYPE, _FCOUNT,_PTYPE> xx, yy;

	xx = x, yy = y;

	// ������ת��Ϊͬ�����
	if( xx.m_data.m_nSign != yy.m_data.m_nSign )
	{
		// x-(-y)=x+y, -x-y=-x+(-y)
		if( xx.m_data.m_nSign == 0 )
			yy.m_data.m_nSign = 0;
		else
			yy.m_data.m_nSign = 1;
		
		return Add( xx, yy );
	}

	// ͬ�����

	// �������,-x-(-y)=y-x 
	if( xx.m_data.m_nSign == 1 )
	{
		yy.m_data.m_nSign = 0;
		xx.m_data.m_nSign = 0;
		return Sub( yy, xx );
	}

	// �������,x-y
	return Sub( xx, yy );
}

// -
template<class _FTYPE,int _FCOUNT,class _PTYPE>
CSuperFloat<_FTYPE, _FCOUNT,_PTYPE> CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>::operator-()
{
	CSuperFloat<_FTYPE, _FCOUNT,_PTYPE> x;
	
	x = *this;
	x.m_data.m_nSign = !x.m_data.m_nSign;

	return x;
}

// �ӷ������ں�,x,y�ķ��ű�����ͬ
template<class _FTYPE,int _FCOUNT,class _PTYPE>
CSuperFloat<_FTYPE, _FCOUNT,_PTYPE> 
Add( const CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>& x, 
	 const CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>& y )
{
	CSuperFloat<_FTYPE,_FCOUNT,_PTYPE>::_BUF a_buf, b_buf, temp;
	int xe, ye, re, xclass, yclass, offbits, sign;

	// [1]����ֵ����
	xclass = Class( x );
	yclass = Class( y );

	// NaN,#I
	if( xclass == CLASS_QNAN || xclass == CLASS_SNAN ||
		yclass == CLASS_QNAN || yclass == CLASS_SNAN )
	{
		Exception( EXCEPTION_INVALID, 0 );
		return CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>::QNaN();
	}

	// Infinite+Infinte,x,y������ͬ
	if( xclass == CLASS_PINF || xclass == CLASS_NINF ||
		yclass == CLASS_PINF || yclass == CLASS_NINF )
	{
		return CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>::Infinite( x.m_data.m_nSign );
	}

	// 0+?
	if( xclass == CLASS_PZ || xclass == CLASS_NZ )
		return y;

	// ?+0
	if( yclass == CLASS_PZ || yclass == CLASS_NZ )
		return x;

	// [2]���ò���������,׼������.ֻ�������������淶����������
	BufExpand( x, a_buf, xe, sign );
	BufExpand( y, b_buf, ye, sign );
	
	re		= xe;
	offbits = xe - ye;

	// ����
	if( xe < ye )
	{
		temp	= a_buf;
		a_buf	= b_buf;
		b_buf	= temp;

		re		= ye;
		offbits	= ye - xe;
	}

	// [3]����
	if( offbits <= ( FRACTION_BITS - 1 ) )
	{
		b_buf.d = b_buf.d >> offbits;
		a_buf.d = a_buf.d + b_buf.d;
	}

	// [4]ת��Ϊ�����ȸ�ʽ
	return BufRestore( a_buf, re, sign );
}

// ���������ں�,x,y�ķ��ű���Ϊ����
template<class _FTYPE,int _FCOUNT,class _PTYPE>
CSuperFloat<_FTYPE, _FCOUNT,_PTYPE> 
Sub( const CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>& x, 
	 const CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>& y )
{
	CSuperFloat<_FTYPE, _FCOUNT,_PTYPE> xx, yy;
	CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>::_BUF a_buf, b_buf, temp;
	int xe, ye, re, xclass, yclass, offbits, sign;

	xx = x, yy = y;

	// [1]����ֵ����
	xclass = Class( x );
	yclass = Class( y );

	// NaN,#I
	if( xclass == CLASS_QNAN || xclass == CLASS_SNAN ||
		yclass == CLASS_QNAN || yclass == CLASS_SNAN )
	{
		Exception( EXCEPTION_INVALID, 0 );
		return CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>::QNaN();
	}

	// Infinite-Infinite,#I
	if( xclass == CLASS_PINF || xclass == CLASS_NINF ||
		yclass == CLASS_PINF || yclass == CLASS_NINF )
	{
		Exception( EXCEPTION_INVALID, 0 );
		return CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>::QNaN();
	}

	// Infinite-?
	if( xclass == CLASS_PINF || xclass == CLASS_NINF )
		return xx;

	// ?-Infinite
	if( yclass == CLASS_PINF || yclass == CLASS_NINF )
	{
		yy.m_data.m_nSign = 1;
		return yy;
	}

	// 0.0-?
	if( xclass == CLASS_PZ || xclass == CLASS_NZ )
	{
		yy.m_data.m_nSign = 1;
		return yy;
	}

	// ?-0.0
	if( yclass == CLASS_PZ || yclass == CLASS_NZ )
		return xx;

	// [2]���ò���������,׼������.ֻ�������������淶����������
	BufExpand( x, a_buf, xe, sign );
	BufExpand( y, b_buf, ye, sign );

	re		= xe;
	offbits = xe - ye;
	sign	= 0;

	// ����
	if( ( xe < ye ) || ( ( xe == ye ) && ( a_buf.d < b_buf.d ) ) )
	{
		temp	= a_buf;
		a_buf	= b_buf;
		b_buf	= temp;

		re		= ye;
		offbits	= ye - xe;
		sign	= 1;
	}

	// [3]����,���������ֻ�������������淶��
	if( offbits <= ( FRACTION_BITS - 1 ) )
	{
		b_buf.d = b_buf.d >> offbits;
		a_buf.d = a_buf.d - b_buf.d;
	}

	// [4]ת��Ϊ�����ȸ�ʽ
	return BufRestore( a_buf, re, sign );
}

// �˷�����
template<class _FTYPE,int _FCOUNT,class _PTYPE>
CSuperFloat<_FTYPE, _FCOUNT,_PTYPE> 
operator*( const CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>& x, 
		   const CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>& y )
{
	CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>::_MUL_BUF c_buf;
	CGUINT<_FTYPE, _FCOUNT*2> a_buf, b_buf;
	
	int xclass, yclass, sign, xe, ye;

	sign = (int)(x.m_data.m_nSign^y.m_data.m_nSign);

	// [1]����ֵ����
	xclass = Class( x );
	yclass = Class( y );

	// NaN,#I
	if( xclass == CLASS_QNAN || xclass == CLASS_SNAN ||
		yclass == CLASS_QNAN || yclass == CLASS_SNAN )
	{
		Exception( EXCEPTION_INVALID, 0 );
		return CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>::QNaN();
	}

	// Infinite*0,#I
	if( ( xclass == CLASS_PINF || xclass == CLASS_NINF ) &&
		( yclass == CLASS_PZ   || yclass == CLASS_NZ ) )
	{
		Exception( EXCEPTION_INVALID, 0 );
		return CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>::QNaN(  );
	}

	// 0*infinite,#I
	if(	( xclass == CLASS_PZ   || xclass == CLASS_NZ   ) &&
		( yclass == CLASS_PINF || yclass == CLASS_NINF ) )
	{
		Exception( EXCEPTION_INVALID, 0 );
		return CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>::QNaN(  );
	}

	// INF*?
	if( xclass == CLASS_PINF || xclass == CLASS_NINF ||
		yclass == CLASS_PINF || yclass == CLASS_NINF )
	{
		return CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>::Infinite( sign );
	}

	// 0*?
	if( xclass == CLASS_PZ || xclass == CLASS_NZ ||
		yclass == CLASS_PZ || yclass == CLASS_NZ )
	{
		return CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>::Zero( sign );
	}

	// ���ò���������,׼������
	BufExpand( x, *(CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>::_BUF*)&a_buf, xe, sign );
	BufExpand( y, *(CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>::_BUF*)&b_buf, ye, sign );

	sign = (int)( x.m_data.m_nSign^y.m_data.m_nSign );

	// �˷�����,ע������!
	*(CGUINT<_FTYPE, _FCOUNT*2>*)&c_buf = a_buf * b_buf;

	// ת��Ϊ��׼��ʽ
	return MulBufRestore( c_buf, xe + ye, sign );
}


// �˷�����
template<class _FTYPE,int _FCOUNT,class _PTYPE>
CSuperFloat<_FTYPE, _FCOUNT,_PTYPE> 
operator/( const CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>& x, 
		   const CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>& y )
{
	// ���������ʽ
	// ����_BUF����,���һ��������λ,�͵�һ����С������
	CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>::_BUF a_buf, b_buf;
	CGUINT<_FTYPE,_FCOUNT*2> xx, yy;
	
	// ��ǰָ��
	int xe, ye, xclass, yclass, sign;

	sign = int( x.m_data.m_nSign^y.m_data.m_nSign );
	
	// [1]����ֵ����
	xclass = Class( x );
	yclass = Class( y );

	// NaN,#I
	if( xclass == CLASS_QNAN || xclass == CLASS_SNAN ||
		yclass == CLASS_QNAN || yclass == CLASS_SNAN )
	{
		Exception( EXCEPTION_INVALID, 0 );
		return CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>::QNaN();
	}

	// Infinite/Infinite,#I
	if( ( xclass == CLASS_PINF || xclass == CLASS_NINF ) &&
		( yclass == CLASS_PINF || yclass == CLASS_NINF ) )
	{
		Exception( EXCEPTION_INVALID, 0 );
		return CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>::QNaN();
	}

	// 0/0,#I
	if(	( xclass == CLASS_PZ || xclass == CLASS_NZ   ) &&
		( yclass == CLASS_PZ || yclass == CLASS_NZ ) )
	{
		Exception( EXCEPTION_INVALID, 0 );
		return CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>::QNaN();
	}

	// ?/0,#Z
	if(	yclass == CLASS_PZ || yclass == CLASS_NZ )
	{
		Exception( EXCEPTION_DIVIDEDBYZERO, 0 );
		return CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>::Infinite( sign );
	}

	// Infinte/?
	if( xclass == CLASS_PINF || xclass == CLASS_NINF )
	{
		return CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>::Infinite( sign );
	}

	// ?/Infinite
	if( yclass == CLASS_PINF || yclass == CLASS_NINF )
	{
		return CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>::Zero( sign );
	}

	// 0/?
	if( xclass == CLASS_PZ || xclass == CLASS_NZ )
	{
		return CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>::Zero( sign );
	}

	// [2]���ò���������,׼������
	BufExpand( x, a_buf, xe, sign );
	BufExpand( y, b_buf, ye, sign );

	// [3]��������
	*(CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>::_BUF*)&xx = a_buf;
	*(CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>::_BUF*)&yy = b_buf;

	// �ƶ�,��������-����λ
	xx = xx << int( FRACTION_BITS -2 );

	xx = xx/yy;
	a_buf.d = *(CGUINT<_FTYPE,_FCOUNT>*)&xx;

	// ����
	sign = x.m_data.m_nSign^y.m_data.m_nSign;
		
	return BufRestore( a_buf, xe - ye, sign );
}


//-----------------------------------------------------------------------------
//                           >,>=,<,<=,==,!=
//-----------------------------------------------------------------------------

// >
template<class _FTYPE,int _FCOUNT,class _PTYPE>
int operator>( const CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>& x, const CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>& y )
{
	switch( Class( x - y ) )
	{
	case CLASS_PINF:
	case CLASS_PN:
	case CLASS_PD:
		return 1;

	// ���Ǹ���,0,NaN,#I
	default:
		return 0;
	}
}

// >=
template<class _FTYPE,int _FCOUNT,class _PTYPE>
int operator>=( const CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>& x, const CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>& y )
{
	// INF == INF,#I
	if( IsInfinite( x ) && IsInfinite( y ) && x.m_data.m_nSign == y.m_data.m_nSign )
		return 1;

	switch( Class( x - y ) )
	{
	case CLASS_PINF:
	case CLASS_PN:
	case CLASS_PD:
	case CLASS_PZ:
	case CLASS_NZ:
		return 1;

	// ����,NaN,#I
	default:
		return 0;
	}
}

// ==
template<class _FTYPE,int _FCOUNT,class _PTYPE>
int operator==( const CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>& x, const CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>& y )
{
	// INF == INF
	if( IsInfinite( x ) && IsInfinite( y ) && x.m_data.m_nSign == y.m_data.m_nSign )
		return 1;

	switch( Class( x - y ) )
	{
	case CLASS_PZ:
	case CLASS_NZ:
		return 1;

	// ���0,NaN
	default:
		return 0;
	}
}

// <=
template<class _FTYPE,int _FCOUNT,class _PTYPE>
int operator<=( const CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>& x, const CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>& y )
{
	// INF == INF
	if( IsInfinite( x ) && IsInfinite( y ) && x.m_data.m_nSign == y.m_data.m_nSign )
		return 1;

	switch( Class( x - y ) )
	{
	case CLASS_NINF:
	case CLASS_NN:
	case CLASS_ND:
	case CLASS_PZ:
	case CLASS_NZ:
		return 1;

	// ������,NaN
	default:
		return 0;
	}
}

// !=
template<class _FTYPE,int _FCOUNT,class _PTYPE>
int operator!=( const CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>& x, const CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>& y )
{
	// INF == INF
	if( IsInfinite( x ) && IsInfinite( y ) && x.m_data.m_nSign == y.m_data.m_nSign )
		return 0;

	switch( Class( x - y ) )
	{
	case CLASS_PZ:
	case CLASS_NZ:
		return 0;

	// NaN����1
	default:
		return 1;
	}
}

// <
template<class _FTYPE,int _FCOUNT,class _PTYPE>
int operator<( const CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>& x, const CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>& y )
{
	switch( Class( x - y ) )
	{
	case CLASS_NINF:
	case CLASS_NN:
	case CLASS_ND:
		return 1;

	// ������,NaN,#I
	default:
		return 0;
	}
}

//-----------------------------------------------------------------------------
//                           ��������
//-----------------------------------------------------------------------------
template<class _FTYPE,int _FCOUNT,class _PTYPE>
CSuperFloat<_FTYPE, _FCOUNT,_PTYPE> CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>::
Infinite( int sign )
{
	CSuperFloat<_FTYPE, _FCOUNT,_PTYPE> rv;

	rv.m_data.m_nSign     = sign;
	rv.m_data.m_nExp      = -1;
	rv.m_data.m_nFraction = 0;

	return rv;
}

template<class _FTYPE,int _FCOUNT,class _PTYPE>
CSuperFloat<_FTYPE, _FCOUNT,_PTYPE> CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>::
Zero( int sign )
{
	CSuperFloat<_FTYPE, _FCOUNT,_PTYPE> rv;

	rv.m_data.m_nSign     = sign;
	rv.m_data.m_nExp      = 0;
	rv.m_data.m_nFraction = 0;

	return rv;
}


template<class _FTYPE,int _FCOUNT,class _PTYPE>
CSuperFloat<_FTYPE, _FCOUNT,_PTYPE> CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>::
QNaN()
{
	CSuperFloat<_FTYPE, _FCOUNT,_PTYPE> rv;
	CGUINT<_FTYPE,_FCOUNT> HighBit(3);

	HighBit = HighBit << int( FRACTION_BITS - 2 );

	rv.m_data.m_nSign     =  0;
	rv.m_data.m_nExp      = -1;
	rv.m_data.m_nFraction = HighBit;

	return rv;
}

template<class _FTYPE,int _FCOUNT,class _PTYPE>
void BufExpand( const CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>& x, 
			    CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>::_BUF& buf, int& exp, int& sign )
{
	int xclass = Class( x );
	CGUINT<_FTYPE,_FCOUNT> IntBit(3), Zero;
	IntBit  = IntBit  << int( sizeof(_FTYPE)*_FCOUNT*8 - 2 );

	// û������λ!
	exp  = x.m_data.m_nExp;
	exp -= (int)SUPER_EXP_OFF;

	buf.d = ( x.m_data.m_nFraction >> 1 );	// ����������2λ
	sign = x.m_data.m_nSign;

	if( xclass == CLASS_PD || xclass == CLASS_ND )
	{
		// ���������淶��,��ʱbuf���淶,ָ��Ҳ��Ҫ����
		while( ( ( buf.d & IntBit ) == Zero ) && ( buf.d != Zero ) )
		{
			buf.d = buf.d << 1;
			exp--;
		}
	}
}

template<class _FTYPE,int _FCOUNT,class _PTYPE>
CSuperFloat<_FTYPE, _FCOUNT,_PTYPE> 
BufRestore( CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>::_BUF& buf, int exp, int sign )
{
	CSuperFloat<_FTYPE, _FCOUNT,_PTYPE> rv;

	CGUINT<_FTYPE,_FCOUNT> IntBit(3), HighBit(1), Zero;

	// ��������λ
	IntBit  = IntBit  << int( sizeof(_FTYPE)*_FCOUNT*8 - 2 );
	HighBit = HighBit << int( sizeof(_FTYPE)*_FCOUNT*8 - 1 );

	// ���÷���ֵ
	rv.m_data.m_nSign = sign;

	// +-0.0
	if( buf.d == Zero )
		return rv;

	// Ѱ������λ1
	while( ( ( buf.d & IntBit ) == Zero ) && ( buf.d != Zero ) )
	{
		if( -exp >= (int)SUPER_EXP_OFF )
			break;

		buf.d = buf.d << 1;
		exp--;
	}

	// ע��,�����ҵ�1,Ҳ������Ϊ����û���ҵ�

	// �������,ת��ΪDenormals
	if( ( -exp >= (int)SUPER_EXP_OFF ) && ( buf.d != Zero ) )
	{
		Exception( EXCEPTION_UNDERFLOW, 0 );
		while( ( -exp > (int)SUPER_EXP_OFF ) && ( buf.d != Zero ) )
		{
			buf.d = buf.d >> 1;
			exp++;
		}

		// ����С����DenormalҲ�޷���ʾ,ת��Ϊ0
		exp = (int)SUPER_EXP_OFF;
		exp = -exp;
	}

	// ��֤����λ��1,����
	while( ( buf.d & HighBit ) != Zero )
	{
		if( exp >= int( SUPER_EXP_OFF + 1 ) )
			break;

		buf.d = buf.d >> 1;
		exp++;
	}

	// ������
	if( exp >= int(SUPER_EXP_OFF + 1 ) )
	{
		Exception( EXCEPTION_OVERFLOW, 0 );
		return CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>::Infinite( sign );
	}
	
	// ���÷���ֵ
	rv.m_data.m_nExp      = exp + (int)SUPER_EXP_OFF;
	rv.m_data.m_nFraction = buf.d << 1;

	return rv;
}

template<class _FTYPE,int _FCOUNT,class _PTYPE>
CSuperFloat<_FTYPE, _FCOUNT,_PTYPE> 
MulBufRestore( CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>::_MUL_BUF& buf, int exp, int sign )
{
	CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>::_BUF s_buf;
	CGUINT<_FTYPE,_FCOUNT*2> IntBit(7), Zero;
	CGUINT<_FTYPE,_FCOUNT> *pbuf;

	// 3���߶�����λ������0
	IntBit = IntBit << int( FRACTION_BITS*2 - 3 );

	while( ( buf.d & IntBit ) != Zero )
	{
		if( exp >= int( SUPER_EXP_OFF + 1 ) )
		{
			Exception( EXCEPTION_OVERFLOW, 0 );
			return CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>::Infinite( sign );
		}

		buf.d = buf.d >> 1;
		exp++;
	}

	// ���2��
	buf.d = buf.d << 2;

	pbuf = (CGUINT<_FTYPE,_FCOUNT>*)&buf.d;
	s_buf.d = pbuf[1];

	return BufRestore( s_buf, exp, sign );
}

// 2**n
template<class _FTYPE,int _FCOUNT,class _PTYPE>
CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>
Scale( const CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>& x, int n )
{
	int exp, sign;
	CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>::_BUF buf;

	BufExpand( x, buf, exp, sign );
	return BufRestore( buf, exp+n, sign );
}

//-----------------------------------------------------------------------------
//                           IEEE���麯��
//-----------------------------------------------------------------------------

// class
template<class _FTYPE,int _FCOUNT,class _PTYPE>
int Class( const CSuperFloat<_FTYPE, _FCOUNT,_PTYPE>& x )
{
	int exp;
	
	// ����ָ��
	exp  = int( x.m_data.m_nExp );
	exp -= (int)SUPER_EXP_OFF;

	CGUINT<_FTYPE,_FCOUNT> HighBit(1), Zero;

	HighBit = HighBit << int( FRACTION_BITS - 2 );

	if( -exp == (int)SUPER_EXP_OFF )		// ��Сָ��
	{
		if( x.m_data.m_nFraction == Zero )
		{
			// 0
			return x.m_data.m_nSign == 0 ? CLASS_PZ : CLASS_NZ;
		}
		else
		{
			// D
			return x.m_data.m_nSign == 0 ? CLASS_PD : CLASS_ND;
		}
	}
	else if( exp == int( SUPER_EXP_OFF + 1 ) )	// ���ָ��
	{
		if( x.m_data.m_nFraction != Zero )
		{
			// NaN
			HighBit = HighBit & x.m_data.m_nFraction;
			if( HighBit != Zero )
				return CLASS_QNAN;
			else
				return CLASS_SNAN;
		}
		else
		{
			// infinite
			return x.m_data.m_nSign == 0 ? CLASS_PINF : CLASS_NINF;
		}
	}
	else
	{
		// finite
		return x.m_data.m_nSign == 0 ? CLASS_PN : CLASS_NN;
	}
}

#endif