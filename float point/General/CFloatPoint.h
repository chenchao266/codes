#ifndef __LCG_FLOAT_POINT_2005_11_11__
#define __LCG_FLOAT_POINT_2005_11_11__

#include"CGUINT.h"
#include"MathLib.h"

// С���������λ,��������QNaN��SNaN
#define FRACTION_HIGH_BIT		(((_TYPE)1)<<(sizeof(_TYPE)*8-_EXP_BITS-2))
// ����������
#define ROUND_LIMIT				(((_TYPE)1)<<(_EXP_BITS-2))
// ��Ч����λ
#define DIGITS_BITS				(sizeof(_TYPE)*8-_EXP_BITS)
// ָ��ƫ��
#define FLOAT_EXP_OFF			( (((int)1)<<(_EXP_BITS-1)) - 1 )

template<class _TYPE,int _EXP_BITS>
class CFloatPoint
{
	public:
		// IEEE�����ʽ
		typedef struct
		{
			_TYPE m_nFraction : sizeof(_TYPE)*8-_EXP_BITS-1;
			_TYPE m_nExp	  : _EXP_BITS;
			_TYPE m_nSign     : 1;
		} _DATA;
	
		// IEEE�����ʽչ��
		typedef struct
		{
			_TYPE m_nRoundings: _EXP_BITS-1;
			_TYPE m_nFraction : sizeof(_TYPE)*8-_EXP_BITS-1;
			_TYPE m_nInt	  : 2;
		} _BUF;

		// �˷���չ
		typedef struct
		{
			_TYPE m_nDiscard;
			_TYPE m_nRoundings: _EXP_BITS-3;
			_TYPE m_nFraction : sizeof(_TYPE)*8-_EXP_BITS-1;
			_TYPE m_nInt	  : 4;
		} _MUL_BUF;

	public:	// ���캯��
		CFloatPoint();
		CFloatPoint( int n );
		CFloatPoint( int n, int m );
		CFloatPoint( const char *str );

	public:	// ���⺯��

		CFloatPoint operator=( int n );
		operator int();

		// IEEE��ʽչ����ָ�
		friend int BufExpand( const CFloatPoint& x, _BUF& buf, int& exp, int& sign );
		friend CFloatPoint BufRestore( _BUF& buf, int exp, int sign );
		friend CFloatPoint MulBufRestore( _MUL_BUF& buf, int exp, int sign );

		friend CFloatPoint Integer( const CFloatPoint& x );
		friend CFloatPoint Abs( const CFloatPoint& x );
		friend CFloatPoint Scale( const CFloatPoint& x, int n );
		friend CFloatPoint FractionExp( const CFloatPoint& x, int& exp );

		// ����ֵ
		static CFloatPoint Zero( int sign );		// +0.0,-0.0
		static CFloatPoint QNaN();					// QNaN
		static CFloatPoint Infinite( int sign );	// +INF,-INF

	public:	// IEEE���麯��

		friend int Class( const CFloatPoint& x );
	
	public:	// �Ѻ���,���ز�����+,-,*,/
		friend CFloatPoint operator+( const CFloatPoint& x, const CFloatPoint& y );
		friend CFloatPoint Add( const CFloatPoint& x, const CFloatPoint& y );

		friend CFloatPoint operator-( const CFloatPoint& x, const CFloatPoint& y );
		friend CFloatPoint Sub( const CFloatPoint& x, const CFloatPoint& y );

		friend CFloatPoint operator*( const CFloatPoint& x, const CFloatPoint& y );
		friend CFloatPoint operator/( const CFloatPoint& x, const CFloatPoint& y );

		CFloatPoint  operator-();

		// �߼�����
		friend int operator> ( const CFloatPoint& x, const CFloatPoint& y );
		friend int operator>=( const CFloatPoint& x, const CFloatPoint& y );
		friend int operator< ( const CFloatPoint& x, const CFloatPoint& y );
		friend int operator<=( const CFloatPoint& x, const CFloatPoint& y );
		friend int operator==( const CFloatPoint& x, const CFloatPoint& y );
		friend int operator!=( const CFloatPoint& x, const CFloatPoint& y );
		friend int operator! ( const CFloatPoint& y );

	private:
		_DATA m_data;
};

//-----------------------------------------------------------------------------
//                           ���캯����һЩ���⺯��
//-----------------------------------------------------------------------------

// ȱʡ���캯��,0.0
template<class _TYPE,int _EXP_BITS>
CFloatPoint<_TYPE,_EXP_BITS>::CFloatPoint()
{
	m_data.m_nSign	   = 0;
	m_data.m_nExp	   = 0;
	m_data.m_nFraction = 0;
}

// ��int����CFloatPoint
// ����㷨�����˸����ʽ�ĺ���
template<class _TYPE,int _EXP_BITS>
CFloatPoint<_TYPE,_EXP_BITS>::CFloatPoint( int n )
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

	// ����sizeof(_TYPE)*8 - 2
	*(_TYPE*)&buf = n;
	exp = sizeof(_TYPE)*8 - 2;

	if( n != 0 )
		*this = BufRestore( buf, exp, sign );
	else
		*this = Zero( 0 );
}

// �ɷ�������CFloatPoint
template<class _TYPE,int _EXP_BITS>
CFloatPoint<_TYPE,_EXP_BITS>::CFloatPoint( int n, int m )
{
	CFloatPoint<_TYPE,_EXP_BITS> cn( n ), cm( m );
	*this = cn/cm;
}

// ��int�õ�CFloatPoint
template<class _TYPE,int _EXP_BITS>
CFloatPoint<_TYPE,_EXP_BITS> CFloatPoint<_TYPE,_EXP_BITS>::operator=( int n )
{
	CFloatPoint<_TYPE,_EXP_BITS> x( n );
	*this = x;
	return *this;
}

// ���ַ������측����
template<class _TYPE,int _EXP_BITS>
CFloatPoint<_TYPE,_EXP_BITS>::CFloatPoint( const char *str )
{
	DataFromStr( *this, str );
}

// ��������ת��Ϊ����,�ض�ȡ��
/*
template<class _TYPE,int _EXP_BITS>
CFloatPoint<_TYPE,_EXP_BITS>::operator int()
{
	int sign, k, n, exp, ns;
	CFloatPoint<_TYPE,_EXP_BITS> x, Two( 2 ), Zero;
	CFloatPoint<_TYPE,_EXP_BITS>::_BUF buf;

	x = *this;

	// �������
	if( x < Zero )
	{
		sign = 1;
		x = -x;
	}
	else
		sign = 0;

	n = 0;
	while( x.m_data.m_nExp >= FLOAT_EXP_OFF )
	{
		k  = int( x.m_data.m_nExp - FLOAT_EXP_OFF );

		n += ( 1 << k );

		// ����λ�ı�,�������
		if( n < 0 )
			return 0;

		// �����Ӧλ
		BufExpand( x, buf, exp, ns );
		buf.m_nInt = 0;
		x = BufRestore( buf, exp, ns );
	}

	if( sign )
		n = -n;
	
	return n;
}
*/

template<class _TYPE,int _EXP_BITS>
CFloatPoint<_TYPE,_EXP_BITS>::operator int()
{
	int sign, k, exp, ns;
	CFloatPoint<_TYPE,_EXP_BITS> x, Zero;
	CFloatPoint<_TYPE,_EXP_BITS>::_BUF buf;
	_TYPE n;

	x = *this;

	// �������
	if( x < Zero )
	{
		sign = 1;
		x = -x;
	}
	else
		sign = 0;

	n = 0;
	if( x.m_data.m_nExp >= FLOAT_EXP_OFF )
	{
		k  = int( x.m_data.m_nExp - FLOAT_EXP_OFF );
		BufExpand( x, buf, exp, ns );
		n = *(_TYPE*)&buf;

		if( k >= sizeof(_TYPE)*8 - 2 )
			n <<= k - sizeof(_TYPE)*8 - 2;
		else
			n >>= sizeof(_TYPE)*8 - 2 - k;
	}

	if( sign )
		n = ~n + 1;
	
	return (int)n;
}

template<class _TYPE,int _EXP_BITS> 
CFloatPoint<_TYPE,_EXP_BITS> Abs( const CFloatPoint<_TYPE,_EXP_BITS>& x )
{
	CFloatPoint<_TYPE,_EXP_BITS> y;

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
template<class _TYPE,int _EXP_BITS> 
CFloatPoint<_TYPE,_EXP_BITS> Integer( const CFloatPoint<_TYPE,_EXP_BITS>& x )
{
	int k;
	CFloatPoint<_TYPE,_EXP_BITS> xx, Zero;

	xx = x;

	k = int( xx.m_data.m_nExp - FLOAT_EXP_OFF );
	if( k >= 0 )
	{
		// ����������

		// �Ǹ�����
		if( k >= sizeof(_TYPE)*8-_EXP_BITS-1 )
			return xx;
		else
		{
			// k-sizeof(_TYPE)*8-_EXP_BITS-1λ������,ɾ�����������Ĳ���
			xx.m_data.m_nFraction >>= int( sizeof(_TYPE)*8-_EXP_BITS-1-k );
			xx.m_data.m_nFraction <<= int( sizeof(_TYPE)*8-_EXP_BITS-1-k );
		}
	}
	else
		return Zero;

/*
	while( xx.m_data.m_nExp >= FLOAT_EXP_OFF )
	{
		k  = int( xx.m_data.m_nExp - FLOAT_EXP_OFF );
		n2 = CFloatPoint<_TYPE,_EXP_BITS>::Scale( One, k );
		xx = xx - n2;
		rv = rv + n2;
	}
*/	
	return xx;
}

//-----------------------------------------------------------------------------
//                           +,-,*,/
//-----------------------------------------------------------------------------

// +
template<class _TYPE,int _EXP_BITS>
CFloatPoint<_TYPE,_EXP_BITS> operator+( const CFloatPoint<_TYPE,_EXP_BITS>& x, 
										const CFloatPoint<_TYPE,_EXP_BITS>& y )
{
	CFloatPoint<_TYPE,_EXP_BITS> xx, yy;

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
template<class _TYPE,int _EXP_BITS>
CFloatPoint<_TYPE,_EXP_BITS> operator-( const CFloatPoint<_TYPE,_EXP_BITS>& x, 
										const CFloatPoint<_TYPE,_EXP_BITS>& y )
{
	CFloatPoint<_TYPE,_EXP_BITS> xx, yy;

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
template<class _TYPE,int _EXP_BITS>
CFloatPoint<_TYPE,_EXP_BITS> CFloatPoint<_TYPE,_EXP_BITS>::operator-()
{
	CFloatPoint<_TYPE,_EXP_BITS> x;
	
	x = *this;
	x.m_data.m_nSign = !x.m_data.m_nSign;

	return x;
}

// �ӷ������ں�,x,y�ķ��ű�����ͬ
template<class _TYPE,int _EXP_BITS>
CFloatPoint<_TYPE,_EXP_BITS> Add( const CFloatPoint<_TYPE,_EXP_BITS>& x, 
								  const CFloatPoint<_TYPE,_EXP_BITS>& y )
{
	CFloatPoint<_TYPE,_EXP_BITS>::_BUF a_buf, b_buf, temp;
	int xe, ye, re, xclass, yclass, offbits, sign;

	// [1]����ֵ����
	xclass = Class( x );
	yclass = Class( y );

	// NaN,#I
	if( xclass == CLASS_QNAN || xclass == CLASS_SNAN ||
		yclass == CLASS_QNAN || yclass == CLASS_SNAN )
	{
		Exception( EXCEPTION_INVALID, 0 );
		return CFloatPoint<_TYPE,_EXP_BITS>::QNaN();
	}

	// Infinite+Infinte,x,y������ͬ
	if( xclass == CLASS_PINF || xclass == CLASS_NINF ||
		yclass == CLASS_PINF || yclass == CLASS_NINF )
	{
		return CFloatPoint<_TYPE,_EXP_BITS>::Infinite( x.m_data.m_nSign );
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

	// �ƶ�b_buf,��Чλ����DIGITS_BITSλ,�ƶ�����DIGITS_BITSλ��ȫ�������
	if( offbits <= DIGITS_BITS )
	{
		// m_nRoundingsֻ_EXP_BITS-1��λ,��Խ���ֵ��ζ�Žض�,#P
		if( offbits > _EXP_BITS-1 )
			Exception( EXCEPTION_PRECISION, 0 );

		// �ƶ�Сָ������,ע��˴������˽ض�,û�����봦��
		*((_TYPE*)&b_buf) = *((_TYPE*)&b_buf) >> offbits;

		// �ӷ�����
		*((_TYPE*)&a_buf) = *((_TYPE*)&a_buf) + *((_TYPE*)&b_buf);
	}

	// [4]ת��Ϊ��׼��ʽ
	return BufRestore( a_buf, re, x.m_data.m_nSign );
}

// ���������ں�,x,y�ķ��ű���Ϊ����
template<class _TYPE,int _EXP_BITS>
CFloatPoint<_TYPE,_EXP_BITS> Sub( const CFloatPoint<_TYPE,_EXP_BITS>& x, 
								  const CFloatPoint<_TYPE,_EXP_BITS>& y )
{
	// ����������,��ÿ����������Ľṹ��Ӧ
	CFloatPoint<_TYPE,_EXP_BITS> xx, yy;

	CFloatPoint<_TYPE,_EXP_BITS>::_BUF a_buf, b_buf, temp;
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
		return CFloatPoint<_TYPE,_EXP_BITS>::QNaN();
	}

	// Infinite-Infinite,#I
	if( xclass == CLASS_PINF || xclass == CLASS_NINF ||
		yclass == CLASS_PINF || yclass == CLASS_NINF )
	{
		Exception( EXCEPTION_INVALID, 0 );
		return CFloatPoint<_TYPE,_EXP_BITS>::QNaN();
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

	// ����,ע��x,y������
	if( *(_TYPE*)&xx < *(_TYPE*)&yy )
	{
		temp	= a_buf;
		a_buf	= b_buf;
		b_buf	= temp;

		re		= ye;
		offbits	= ye - xe;
		sign	= 1;
	}

	// [3]����,���������ֻ�������������淶��
	if( offbits <= DIGITS_BITS )
	{
		// m_nRoundingsֻ��_EXP_BITS-1λ,��Խ���ֵ��ζ�Žض�,���������쳣
		if( offbits > _EXP_BITS-1 )
			Exception( EXCEPTION_PRECISION, 0 );

		// �ƶ�Сָ������,ע��˴������˽ض�,û�����봦��
		*((_TYPE*)&b_buf) = *((_TYPE*)&b_buf) >> offbits;

		// ��������
		*((_TYPE*)&a_buf) =	*((_TYPE*)&a_buf) - *((_TYPE*)&b_buf);
	}

	// [4]ת��Ϊ��׼��ʽ
	return BufRestore( a_buf, re, sign );
}

// �˷�����
template<class _TYPE,int _EXP_BITS>
CFloatPoint<_TYPE,_EXP_BITS> operator*( const CFloatPoint<_TYPE,_EXP_BITS>& x, 
										const CFloatPoint<_TYPE,_EXP_BITS>& y )
{
	// ����������,��ÿ����������Ľṹ��Ӧ
	CFloatPoint<_TYPE,_EXP_BITS>::_MUL_BUF c_buf;
	CGUINT<unsigned short, sizeof(_TYPE)> a_buf, b_buf;
	
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
		return CFloatPoint<_TYPE,_EXP_BITS>::QNaN();
	}

	// Infinite*0,#I
	if( ( xclass == CLASS_PINF || xclass == CLASS_NINF ) &&
		( yclass == CLASS_PZ   || yclass == CLASS_NZ ) )
	{
		Exception( EXCEPTION_INVALID, 0 );
		return CFloatPoint<_TYPE,_EXP_BITS>::QNaN(  );
	}

	// 0*infinite,#I
	if(	( xclass == CLASS_PZ   || xclass == CLASS_NZ   ) &&
		( yclass == CLASS_PINF || yclass == CLASS_NINF ) )
	{
		Exception( EXCEPTION_INVALID, 0 );
		return CFloatPoint<_TYPE,_EXP_BITS>::QNaN(  );
	}

	// INF*?
	if( xclass == CLASS_PINF || xclass == CLASS_NINF ||
		yclass == CLASS_PINF || yclass == CLASS_NINF )
	{
		return CFloatPoint<_TYPE,_EXP_BITS>::Infinite( sign );
	}

	// 0*?
	if( xclass == CLASS_PZ || xclass == CLASS_NZ ||
		yclass == CLASS_PZ || yclass == CLASS_NZ )
	{
		return CFloatPoint<_TYPE,_EXP_BITS>::Zero( sign );
	}

	// ���ò���������,׼������
	BufExpand( x, *(CFloatPoint<_TYPE,_EXP_BITS>::_BUF*)&a_buf, xe, sign );
	BufExpand( y, *(CFloatPoint<_TYPE,_EXP_BITS>::_BUF*)&b_buf, ye, sign );

	sign = (int)( x.m_data.m_nSign^y.m_data.m_nSign );

	// �˷�����,ע������!
	*(CGUINT<unsigned short, sizeof(_TYPE)>*)&c_buf = a_buf * b_buf;

	// [4]�淶��
	return MulBufRestore( c_buf, xe+ye, sign );
}


// �˷�����
template<class _TYPE,int _EXP_BITS>
CFloatPoint<_TYPE,_EXP_BITS> operator/( const CFloatPoint<_TYPE,_EXP_BITS>& x, 
										const CFloatPoint<_TYPE,_EXP_BITS>& y )
{
	// ����������,��ÿ����������Ľṹ��Ӧ
	CGUINT<unsigned char,sizeof(_TYPE)> *pxy, One( 1 ), temp;

	CFloatPoint<_TYPE,_EXP_BITS>::_BUF a_buf, b_buf, c_buf;
	CGUINT<unsigned short,sizeof(_TYPE)> xx, yy, Zero;
	_TYPE convert;
	
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
		return CFloatPoint<_TYPE,_EXP_BITS>::QNaN();
	}

	// Infinite/Infinite,#I
	if( ( xclass == CLASS_PINF || xclass == CLASS_NINF ) &&
		( yclass == CLASS_PINF || yclass == CLASS_NINF ) )
	{
		Exception( EXCEPTION_INVALID, 0 );
		return CFloatPoint<_TYPE,_EXP_BITS>::QNaN();
	}

	// 0/0,#I
	if(	( xclass == CLASS_PZ || xclass == CLASS_NZ   ) &&
		( yclass == CLASS_PZ || yclass == CLASS_NZ ) )
	{
		Exception( EXCEPTION_INVALID, 0 );
		return CFloatPoint<_TYPE,_EXP_BITS>::QNaN();
	}

	// ?/0,#Z
	if(	yclass == CLASS_PZ || yclass == CLASS_NZ )
	{
		Exception( EXCEPTION_DIVIDEDBYZERO, 0 );
		return CFloatPoint<_TYPE,_EXP_BITS>::Infinite( sign );
	}

	// Infinte/?
	if( xclass == CLASS_PINF || xclass == CLASS_NINF )
	{
		return CFloatPoint<_TYPE,_EXP_BITS>::Infinite( sign );
	}

	// ?/Infinite
	if( yclass == CLASS_PINF || yclass == CLASS_NINF )
	{
		return CFloatPoint<_TYPE,_EXP_BITS>::Zero( sign );
	}

	// 0/?
	if( xclass == CLASS_PZ || xclass == CLASS_NZ )
	{
		return CFloatPoint<_TYPE,_EXP_BITS>::Zero( sign );
	}

	// [2]���ò���������,׼������
	BufExpand( x, a_buf, xe, sign );
	BufExpand( y, b_buf, ye, sign );

	// [3]��������
	*(CFloatPoint<_TYPE,_EXP_BITS>::_BUF*)&xx = a_buf;
	*(CFloatPoint<_TYPE,_EXP_BITS>::_BUF*)&yy = b_buf;
	xx = xx << int( sizeof(_TYPE)*8 );

	xx = xx/yy;

	pxy = (CGUINT<unsigned char,sizeof(_TYPE)>*)&xx;

	// �淶��
	while( pxy[1] > One )
	{
		xx = xx >> 1;
		xe ++;
	}
	
	while( pxy[1] < One && xx != Zero )
	{
		xx = xx << 1;
		xe --;
	}

	// ע������ת��
	c_buf.m_nInt	  = *(_TYPE*)&pxy[1];

	temp	= pxy[0] >> ( _EXP_BITS + 1 );
	convert = *(_TYPE*)&temp;
	c_buf.m_nFraction = convert;

	temp	= ( pxy[0] << int( sizeof(_TYPE)*8-_EXP_BITS-1 ) ) >> int( sizeof(_TYPE)-_EXP_BITS+1 );
	convert = *(_TYPE*)&temp;
	c_buf.m_nRoundings = convert; 

	sign = int( x.m_data.m_nSign^y.m_data.m_nSign );
		
	// [4]�淶��
	return BufRestore( c_buf, xe - ye, sign );
}

//-----------------------------------------------------------------------------
//                           >,>=,<,<=,==,!=
//-----------------------------------------------------------------------------

// >
template<class _TYPE,int _EXP_BITS>
int operator>( const CFloatPoint<_TYPE,_EXP_BITS>& x, const CFloatPoint<_TYPE,_EXP_BITS>& y )
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
template<class _TYPE,int _EXP_BITS>
int operator>=( const CFloatPoint<_TYPE,_EXP_BITS>& x, const CFloatPoint<_TYPE,_EXP_BITS>& y )
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
template<class _TYPE,int _EXP_BITS>
int operator==( const CFloatPoint<_TYPE,_EXP_BITS>& x, const CFloatPoint<_TYPE,_EXP_BITS>& y )
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
template<class _TYPE,int _EXP_BITS>
int operator<=( const CFloatPoint<_TYPE,_EXP_BITS>& x, const CFloatPoint<_TYPE,_EXP_BITS>& y )
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
template<class _TYPE,int _EXP_BITS>
int operator!=( const CFloatPoint<_TYPE,_EXP_BITS>& x, const CFloatPoint<_TYPE,_EXP_BITS>& y )
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
template<class _TYPE,int _EXP_BITS>
int operator<( const CFloatPoint<_TYPE,_EXP_BITS>& x, const CFloatPoint<_TYPE,_EXP_BITS>& y )
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
template<class _TYPE,int _EXP_BITS>
CFloatPoint<_TYPE,_EXP_BITS> CFloatPoint<_TYPE,_EXP_BITS>::Infinite( int sign )
{
	CFloatPoint<_TYPE,_EXP_BITS> rv;

	rv.m_data.m_nSign     = sign;
	rv.m_data.m_nExp      = -1;
	rv.m_data.m_nFraction = 0;

	return rv;
}

template<class _TYPE,int _EXP_BITS>
CFloatPoint<_TYPE,_EXP_BITS> CFloatPoint<_TYPE,_EXP_BITS>::Zero( int sign )
{
	CFloatPoint<_TYPE,_EXP_BITS> rv;

	rv.m_data.m_nSign     = sign;
	rv.m_data.m_nExp      = 0;
	rv.m_data.m_nFraction = 0;

	return rv;
}


template<class _TYPE,int _EXP_BITS>
CFloatPoint<_TYPE,_EXP_BITS> CFloatPoint<_TYPE,_EXP_BITS>::QNaN()
{
	CFloatPoint<_TYPE,_EXP_BITS> rv;

	rv.m_data.m_nSign     = 0;
	rv.m_data.m_nExp      = -1;
	rv.m_data.m_nFraction = FRACTION_HIGH_BIT;

	return rv;
}

template<class _TYPE,int _EXP_BITS>
int BufExpand( const CFloatPoint<_TYPE,_EXP_BITS>& x, 
			   CFloatPoint<_TYPE,_EXP_BITS>::_BUF& buf, int& exp, int& sign )
{
	int xclass = Class( x );

	// ����ָ��
	exp	 = x.m_data.m_nExp;
	exp -= (int)FLOAT_EXP_OFF;

	buf.m_nFraction = x.m_data.m_nFraction;
	buf.m_nRoundings = 0;
	buf.m_nInt		 = 0;

	// Denormal��integer��ҪѰ��
	if( xclass == CLASS_PD || xclass == CLASS_ND )
	{
		// ��Denormal��������,#D
		Exception( EXCEPTION_DENORMAL, 0 );

		// �����淶��������λ��λ
		*(_TYPE*)&buf = *(_TYPE*)&buf << 1;

		while( buf.m_nInt == 0 )
		{
			*(_TYPE*)&buf = *(_TYPE*)&buf << 1;
			exp--;
		}
	}

	// finite,Infinite,NaN
	buf.m_nInt = 1;

	// 0
	if( xclass == CLASS_PZ || xclass == CLASS_NZ )
		buf.m_nInt = 0;

	sign = x.m_data.m_nSign;
	
	return xclass;
}

template<class _TYPE,int _EXP_BITS>
CFloatPoint<_TYPE,_EXP_BITS> 
BufRestore( CFloatPoint<_TYPE,_EXP_BITS>::_BUF& buf, int exp, int sign )
{
	CFloatPoint<_TYPE,_EXP_BITS> rv;

	// ע��exp��ֵ����λ��[-2EXP_OFF+1,2EXP_OFF],�ѽӽ�����
	// ÿһ��exp++��exp--���������������

	// ���÷���ֵ
	rv.m_data.m_nSign	  = sign;
	rv.m_data.m_nExp      = 0;
	rv.m_data.m_nFraction = 0;

	// +-0.0
	if( *(_TYPE*)&buf == 0 )
		return rv;

	// Ѱ������λ1
	while( buf.m_nInt < 1 && *(_TYPE*)&buf != 0 )
	{
		// �������
		if( -exp >= (int)FLOAT_EXP_OFF )
			break;

		*(_TYPE*)&buf = *(_TYPE*)&buf << 1;
		exp--;
	}
	// ע��,����λ1�����ҵ�,Ҳ����������û���ҵ�

	// �������,ת��ΪDenormals
	if( ( -exp >= (int)FLOAT_EXP_OFF ) && ( *(_TYPE*)&buf != 0 ) )
	{
		Exception( EXCEPTION_UNDERFLOW, 0 );

		*(_TYPE*)&buf = *(_TYPE*)&buf >> 1;

		while( ( -exp > (int)FLOAT_EXP_OFF ) && ( *(_TYPE*)&buf != 0 ) )
		{
			*(_TYPE*)&buf = *(_TYPE*)&buf >> 1;
			exp++;
		}

		// ����С����DenormalҲ�޷���ʾ,ת��Ϊ0
		exp = FLOAT_EXP_OFF;
		exp = -exp;
	}

	// ����λ����
	while( buf.m_nInt > 1 )
	{
		// �������
		if( exp >= int( FLOAT_EXP_OFF + 1 ) )
			break;

		*(_TYPE*)&buf = *(_TYPE*)&buf >> 1;
		exp++;
	}

	// �������
	if( exp >= int( FLOAT_EXP_OFF + 1 ) )
	{
		Exception( EXCEPTION_OVERFLOW, 0 );
		return CFloatPoint<_TYPE,_EXP_BITS>::Infinite( sign );
	}
	
	// ���봦��,ע��Denormal�ı���
	if( ( buf.m_nRoundings > ROUND_LIMIT ) ||
		( ( buf.m_nRoundings == ROUND_LIMIT ) && ( buf.m_nFraction & 1 ) ) )
	{
		// ����,#P
		Exception( EXCEPTION_PRECISION, 0 );

		// Denormals
		if( ( -exp == (int)FLOAT_EXP_OFF ) && ( *(_TYPE*)&buf != 0 ) )
		{
			*((_TYPE*)&buf) += ( ROUND_LIMIT << 1 );

			// �����λ����Denormal��ΪFinite
			if( buf.m_nInt )
				exp++;
		}
		else
		{
			// �������ݽ�λ����
			*((_TYPE*)&buf) += ( ROUND_LIMIT << 1 );
			
			// ���뵼������λ�����仯,��Ҫ����
			if( buf.m_nInt > 1 )
			{
				*(_TYPE*)&buf = *(_TYPE*)&buf >> 1;
				exp++;
			}

			// ����
			if( exp >= int( FLOAT_EXP_OFF + 1 ) )
			{
				Exception( EXCEPTION_OVERFLOW, 0 );
				return CFloatPoint<_TYPE,_EXP_BITS>::Infinite( sign );
			}
		}
	}

	// ���÷���ֵ
	rv.m_data.m_nExp      = exp + (int)FLOAT_EXP_OFF;
	rv.m_data.m_nFraction = buf.m_nFraction;

	return rv;
}

template<class _TYPE,int _EXP_BITS>
CFloatPoint<_TYPE,_EXP_BITS> 
MulBufRestore( CFloatPoint<_TYPE,_EXP_BITS>::_MUL_BUF& buf, int exp, int sign )
{
	CFloatPoint<_TYPE,_EXP_BITS>::_BUF s_buf;

	// �淶��
	while( buf.m_nInt > 1 )
	{
		if( exp >= int( FLOAT_EXP_OFF + 1 ) )
		{
			Exception( EXCEPTION_OVERFLOW, 0 );
			return CFloatPoint<_TYPE,_EXP_BITS>::Infinite( sign );
		}

		*(CGUINT<unsigned short, sizeof(_TYPE)>*)&buf = *(CGUINT<unsigned short, sizeof(_TYPE)>*)&buf >> 1;
		exp++;
	}

	s_buf.m_nInt	  = (_TYPE)buf.m_nInt;		// �˴������˸�2λ
	s_buf.m_nFraction = (_TYPE)buf.m_nFraction;
	s_buf.m_nRoundings= (_TYPE)buf.m_nRoundings;

	return BufRestore( s_buf, exp, sign );
}

// 2**n
template<class _TYPE,int _EXP_BITS>
CFloatPoint<_TYPE,_EXP_BITS>
Scale( const CFloatPoint<_TYPE,_EXP_BITS>& x, int n )
{
	int exp, sign;
	CFloatPoint<_TYPE,_EXP_BITS>::_BUF buf;

	BufExpand( x, buf, exp, sign );
	return BufRestore( buf, exp+n, sign );
}

template<class _TYPE,int _EXP_BITS>
CFloatPoint<_TYPE,_EXP_BITS>
FractionExp( const CFloatPoint<_TYPE,_EXP_BITS>& x, int& exp )
{
	CFloatPoint<_TYPE,_EXP_BITS>::_BUF buf;
	int sign;

	BufExpand( x, buf, exp, sign );
	return BufRestore( buf, 0, sign );
}

//-----------------------------------------------------------------------------
//                           IEEE���麯��
//-----------------------------------------------------------------------------

// class
template<class _TYPE,int _EXP_BITS>
int Class( const CFloatPoint<_TYPE,_EXP_BITS>& x )
{
	int exp;
	
	// ����ָ��
	exp  = int( x.m_data.m_nExp );
	exp -= (int)FLOAT_EXP_OFF;

	if( -exp == (int)FLOAT_EXP_OFF )		// ��Сָ��
	{
		if( x.m_data.m_nFraction != 0 )
		{
			// D
			return x.m_data.m_nSign == 0 ? CLASS_PD : CLASS_ND;
		}
		else
		{
			// 0
			return x.m_data.m_nSign == 0 ? CLASS_PZ : CLASS_NZ;
		}
	}
	else if( exp == int( FLOAT_EXP_OFF + 1 ) )	// ���ָ��
	{
		if( x.m_data.m_nFraction != 0 )
		{
			// NaN
			if( x.m_data.m_nFraction & FRACTION_HIGH_BIT )
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