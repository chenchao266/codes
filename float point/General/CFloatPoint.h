#ifndef __LCG_FLOAT_POINT_2005_11_11__
#define __LCG_FLOAT_POINT_2005_11_11__

#include"CGUINT.h"
#include"MathLib.h"

// 小数部分最高位,用于区分QNaN和SNaN
#define FRACTION_HIGH_BIT		(((_TYPE)1)<<(sizeof(_TYPE)*8-_EXP_BITS-2))
// 用于舍入检测
#define ROUND_LIMIT				(((_TYPE)1)<<(_EXP_BITS-2))
// 有效数字位
#define DIGITS_BITS				(sizeof(_TYPE)*8-_EXP_BITS)
// 指数偏移
#define FLOAT_EXP_OFF			( (((int)1)<<(_EXP_BITS-1)) - 1 )

template<class _TYPE,int _EXP_BITS>
class CFloatPoint
{
	public:
		// IEEE浮点格式
		typedef struct
		{
			_TYPE m_nFraction : sizeof(_TYPE)*8-_EXP_BITS-1;
			_TYPE m_nExp	  : _EXP_BITS;
			_TYPE m_nSign     : 1;
		} _DATA;
	
		// IEEE浮点格式展开
		typedef struct
		{
			_TYPE m_nRoundings: _EXP_BITS-1;
			_TYPE m_nFraction : sizeof(_TYPE)*8-_EXP_BITS-1;
			_TYPE m_nInt	  : 2;
		} _BUF;

		// 乘法扩展
		typedef struct
		{
			_TYPE m_nDiscard;
			_TYPE m_nRoundings: _EXP_BITS-3;
			_TYPE m_nFraction : sizeof(_TYPE)*8-_EXP_BITS-1;
			_TYPE m_nInt	  : 4;
		} _MUL_BUF;

	public:	// 构造函数
		CFloatPoint();
		CFloatPoint( int n );
		CFloatPoint( int n, int m );
		CFloatPoint( const char *str );

	public:	// 特殊函数

		CFloatPoint operator=( int n );
		operator int();

		// IEEE格式展开与恢复
		friend int BufExpand( const CFloatPoint& x, _BUF& buf, int& exp, int& sign );
		friend CFloatPoint BufRestore( _BUF& buf, int exp, int sign );
		friend CFloatPoint MulBufRestore( _MUL_BUF& buf, int exp, int sign );

		friend CFloatPoint Integer( const CFloatPoint& x );
		friend CFloatPoint Abs( const CFloatPoint& x );
		friend CFloatPoint Scale( const CFloatPoint& x, int n );
		friend CFloatPoint FractionExp( const CFloatPoint& x, int& exp );

		// 特殊值
		static CFloatPoint Zero( int sign );		// +0.0,-0.0
		static CFloatPoint QNaN();					// QNaN
		static CFloatPoint Infinite( int sign );	// +INF,-INF

	public:	// IEEE建议函数

		friend int Class( const CFloatPoint& x );
	
	public:	// 友函数,重载操作符+,-,*,/
		friend CFloatPoint operator+( const CFloatPoint& x, const CFloatPoint& y );
		friend CFloatPoint Add( const CFloatPoint& x, const CFloatPoint& y );

		friend CFloatPoint operator-( const CFloatPoint& x, const CFloatPoint& y );
		friend CFloatPoint Sub( const CFloatPoint& x, const CFloatPoint& y );

		friend CFloatPoint operator*( const CFloatPoint& x, const CFloatPoint& y );
		friend CFloatPoint operator/( const CFloatPoint& x, const CFloatPoint& y );

		CFloatPoint  operator-();

		// 逻辑操作
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
//                           构造函数与一些特殊函数
//-----------------------------------------------------------------------------

// 缺省构造函数,0.0
template<class _TYPE,int _EXP_BITS>
CFloatPoint<_TYPE,_EXP_BITS>::CFloatPoint()
{
	m_data.m_nSign	   = 0;
	m_data.m_nExp	   = 0;
	m_data.m_nFraction = 0;
}

// 从int构造CFloatPoint
// 这个算法利用了浮点格式的含义
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

	// 右移sizeof(_TYPE)*8 - 2
	*(_TYPE*)&buf = n;
	exp = sizeof(_TYPE)*8 - 2;

	if( n != 0 )
		*this = BufRestore( buf, exp, sign );
	else
		*this = Zero( 0 );
}

// 由分数构造CFloatPoint
template<class _TYPE,int _EXP_BITS>
CFloatPoint<_TYPE,_EXP_BITS>::CFloatPoint( int n, int m )
{
	CFloatPoint<_TYPE,_EXP_BITS> cn( n ), cm( m );
	*this = cn/cm;
}

// 由int得到CFloatPoint
template<class _TYPE,int _EXP_BITS>
CFloatPoint<_TYPE,_EXP_BITS> CFloatPoint<_TYPE,_EXP_BITS>::operator=( int n )
{
	CFloatPoint<_TYPE,_EXP_BITS> x( n );
	*this = x;
	return *this;
}

// 由字符串构造浮点数
template<class _TYPE,int _EXP_BITS>
CFloatPoint<_TYPE,_EXP_BITS>::CFloatPoint( const char *str )
{
	DataFromStr( *this, str );
}

// 将浮点数转换为整型,截断取整
/*
template<class _TYPE,int _EXP_BITS>
CFloatPoint<_TYPE,_EXP_BITS>::operator int()
{
	int sign, k, n, exp, ns;
	CFloatPoint<_TYPE,_EXP_BITS> x, Two( 2 ), Zero;
	CFloatPoint<_TYPE,_EXP_BITS>::_BUF buf;

	x = *this;

	// 处理符号
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

		// 符号位改变,出现溢出
		if( n < 0 )
			return 0;

		// 清除对应位
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

	// 处理符号
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

// 将浮点数转换为整数,截断取整
template<class _TYPE,int _EXP_BITS> 
CFloatPoint<_TYPE,_EXP_BITS> Integer( const CFloatPoint<_TYPE,_EXP_BITS>& x )
{
	int k;
	CFloatPoint<_TYPE,_EXP_BITS> xx, Zero;

	xx = x;

	k = int( xx.m_data.m_nExp - FLOAT_EXP_OFF );
	if( k >= 0 )
	{
		// 有整数部分

		// 是个整数
		if( k >= sizeof(_TYPE)*8-_EXP_BITS-1 )
			return xx;
		else
		{
			// k-sizeof(_TYPE)*8-_EXP_BITS-1位是整数,删除不是整数的部分
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

	// 同号相加
	if( xx.m_data.m_nSign == yy.m_data.m_nSign )
		return Add( xx, yy );

	// 异号相加转换正数相减

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

	// 异号相减转换为同号相加
	if( xx.m_data.m_nSign != yy.m_data.m_nSign )
	{
		// x-(-y)=x+y, -x-y=-x+(-y)
		if( xx.m_data.m_nSign == 0 )
			yy.m_data.m_nSign = 0;
		else
			yy.m_data.m_nSign = 1;
		
		return Add( xx, yy );
	}

	// 同号相减

	// 负号相减,-x-(-y)=y-x 
	if( xx.m_data.m_nSign == 1 )
	{
		yy.m_data.m_nSign = 0;
		xx.m_data.m_nSign = 0;
		return Sub( yy, xx );
	}

	// 正号相减,x-y
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

// 加法函数内核,x,y的符号必须相同
template<class _TYPE,int _EXP_BITS>
CFloatPoint<_TYPE,_EXP_BITS> Add( const CFloatPoint<_TYPE,_EXP_BITS>& x, 
								  const CFloatPoint<_TYPE,_EXP_BITS>& y )
{
	CFloatPoint<_TYPE,_EXP_BITS>::_BUF a_buf, b_buf, temp;
	int xe, ye, re, xclass, yclass, offbits, sign;

	// [1]特殊值处理
	xclass = Class( x );
	yclass = Class( y );

	// NaN,#I
	if( xclass == CLASS_QNAN || xclass == CLASS_SNAN ||
		yclass == CLASS_QNAN || yclass == CLASS_SNAN )
	{
		Exception( EXCEPTION_INVALID, 0 );
		return CFloatPoint<_TYPE,_EXP_BITS>::QNaN();
	}

	// Infinite+Infinte,x,y符号相同
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

	// [2]设置操作缓冲区,准备计算.只有有限数和弱规范数参与运算
	BufExpand( x, a_buf, xe, sign );
	BufExpand( y, b_buf, ye, sign );
	
	re		= xe;
	offbits = xe - ye;

	// 排序
	if( xe < ye )
	{
		temp	= a_buf;
		a_buf	= b_buf;
		b_buf	= temp;

		re		= ye;
		offbits	= ye - xe;
	}

	// [3]计算

	// 移动b_buf,有效位数共DIGITS_BITS位,移动超过DIGITS_BITS位则全部被舍掉
	if( offbits <= DIGITS_BITS )
	{
		// m_nRoundings只_EXP_BITS-1有位,超越这个值意味着截断,#P
		if( offbits > _EXP_BITS-1 )
			Exception( EXCEPTION_PRECISION, 0 );

		// 移动小指数的数,注意此处进行了截断,没有舍入处理
		*((_TYPE*)&b_buf) = *((_TYPE*)&b_buf) >> offbits;

		// 加法运算
		*((_TYPE*)&a_buf) = *((_TYPE*)&a_buf) + *((_TYPE*)&b_buf);
	}

	// [4]转化为标准格式
	return BufRestore( a_buf, re, x.m_data.m_nSign );
}

// 减法函数内核,x,y的符号必须为正数
template<class _TYPE,int _EXP_BITS>
CFloatPoint<_TYPE,_EXP_BITS> Sub( const CFloatPoint<_TYPE,_EXP_BITS>& x, 
								  const CFloatPoint<_TYPE,_EXP_BITS>& y )
{
	// 操作缓冲区,与每个参与运算的结构对应
	CFloatPoint<_TYPE,_EXP_BITS> xx, yy;

	CFloatPoint<_TYPE,_EXP_BITS>::_BUF a_buf, b_buf, temp;
	int xe, ye, re, xclass, yclass, offbits, sign;

	xx = x, yy = y;

	// [1]特殊值处理
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

	// [2]设置操作缓冲区,准备计算.只有有限数和弱规范数参与运算
	BufExpand( x, a_buf, xe, sign );
	BufExpand( y, b_buf, ye, sign );

	re		= xe;
	offbits = xe - ye;
	sign	= 0;

	// 排序,注意x,y是正数
	if( *(_TYPE*)&xx < *(_TYPE*)&yy )
	{
		temp	= a_buf;
		a_buf	= b_buf;
		b_buf	= temp;

		re		= ye;
		offbits	= ye - xe;
		sign	= 1;
	}

	// [3]计算,参与运算的只有有限数和弱规范数
	if( offbits <= DIGITS_BITS )
	{
		// m_nRoundings只有_EXP_BITS-1位,超越这个值意味着截断,触发精度异常
		if( offbits > _EXP_BITS-1 )
			Exception( EXCEPTION_PRECISION, 0 );

		// 移动小指数的数,注意此处进行了截断,没有舍入处理
		*((_TYPE*)&b_buf) = *((_TYPE*)&b_buf) >> offbits;

		// 减法运算
		*((_TYPE*)&a_buf) =	*((_TYPE*)&a_buf) - *((_TYPE*)&b_buf);
	}

	// [4]转化为标准格式
	return BufRestore( a_buf, re, sign );
}

// 乘法函数
template<class _TYPE,int _EXP_BITS>
CFloatPoint<_TYPE,_EXP_BITS> operator*( const CFloatPoint<_TYPE,_EXP_BITS>& x, 
										const CFloatPoint<_TYPE,_EXP_BITS>& y )
{
	// 操作缓冲区,与每个参与运算的结构对应
	CFloatPoint<_TYPE,_EXP_BITS>::_MUL_BUF c_buf;
	CGUINT<unsigned short, sizeof(_TYPE)> a_buf, b_buf;
	
	int xclass, yclass, sign, xe, ye;

	sign = (int)(x.m_data.m_nSign^y.m_data.m_nSign);

	// [1]特殊值处理
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

	// 设置操作缓冲区,准备计算
	BufExpand( x, *(CFloatPoint<_TYPE,_EXP_BITS>::_BUF*)&a_buf, xe, sign );
	BufExpand( y, *(CFloatPoint<_TYPE,_EXP_BITS>::_BUF*)&b_buf, ye, sign );

	sign = (int)( x.m_data.m_nSign^y.m_data.m_nSign );

	// 乘法运算,注意类型!
	*(CGUINT<unsigned short, sizeof(_TYPE)>*)&c_buf = a_buf * b_buf;

	// [4]规范化
	return MulBufRestore( c_buf, xe+ye, sign );
}


// 乘法函数
template<class _TYPE,int _EXP_BITS>
CFloatPoint<_TYPE,_EXP_BITS> operator/( const CFloatPoint<_TYPE,_EXP_BITS>& x, 
										const CFloatPoint<_TYPE,_EXP_BITS>& y )
{
	// 操作缓冲区,与每个参与运算的结构对应
	CGUINT<unsigned char,sizeof(_TYPE)> *pxy, One( 1 ), temp;

	CFloatPoint<_TYPE,_EXP_BITS>::_BUF a_buf, b_buf, c_buf;
	CGUINT<unsigned short,sizeof(_TYPE)> xx, yy, Zero;
	_TYPE convert;
	
	// 当前指数
	int xe, ye, xclass, yclass, sign;

	sign = int( x.m_data.m_nSign^y.m_data.m_nSign );
	
	// [1]特殊值处理
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

	// [2]设置操作缓冲区,准备计算
	BufExpand( x, a_buf, xe, sign );
	BufExpand( y, b_buf, ye, sign );

	// [3]除法运算
	*(CFloatPoint<_TYPE,_EXP_BITS>::_BUF*)&xx = a_buf;
	*(CFloatPoint<_TYPE,_EXP_BITS>::_BUF*)&yy = b_buf;
	xx = xx << int( sizeof(_TYPE)*8 );

	xx = xx/yy;

	pxy = (CGUINT<unsigned char,sizeof(_TYPE)>*)&xx;

	// 规范化
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

	// 注意类型转换
	c_buf.m_nInt	  = *(_TYPE*)&pxy[1];

	temp	= pxy[0] >> ( _EXP_BITS + 1 );
	convert = *(_TYPE*)&temp;
	c_buf.m_nFraction = convert;

	temp	= ( pxy[0] << int( sizeof(_TYPE)*8-_EXP_BITS-1 ) ) >> int( sizeof(_TYPE)-_EXP_BITS+1 );
	convert = *(_TYPE*)&temp;
	c_buf.m_nRoundings = convert; 

	sign = int( x.m_data.m_nSign^y.m_data.m_nSign );
		
	// [4]规范化
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

	// 差是负数,0,NaN,#I
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

	// 正数,NaN,#I
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

	// 差非0,NaN
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

	// 差正数,NaN
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

	// NaN返回1
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

	// 差正数,NaN,#I
	default:
		return 0;
	}
}

//-----------------------------------------------------------------------------
//                           辅助函数
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

	// 计算指数
	exp	 = x.m_data.m_nExp;
	exp -= (int)FLOAT_EXP_OFF;

	buf.m_nFraction = x.m_data.m_nFraction;
	buf.m_nRoundings = 0;
	buf.m_nInt		 = 0;

	// Denormal的integer需要寻找
	if( xclass == CLASS_PD || xclass == CLASS_ND )
	{
		// 有Denormal参与运算,#D
		Exception( EXCEPTION_DENORMAL, 0 );

		// 将弱规范数的整数位置位
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

	// 注意exp的值可能位于[-2EXP_OFF+1,2EXP_OFF],已接近极限
	// 每一次exp++或exp--操作都必须检测溢出

	// 设置返回值
	rv.m_data.m_nSign	  = sign;
	rv.m_data.m_nExp      = 0;
	rv.m_data.m_nFraction = 0;

	// +-0.0
	if( *(_TYPE*)&buf == 0 )
		return rv;

	// 寻找整数位1
	while( buf.m_nInt < 1 && *(_TYPE*)&buf != 0 )
	{
		// 检查下溢
		if( -exp >= (int)FLOAT_EXP_OFF )
			break;

		*(_TYPE*)&buf = *(_TYPE*)&buf << 1;
		exp--;
	}
	// 注意,整数位1可能找到,也可能因下溢没有找到

	// 向下溢出,转换为Denormals
	if( ( -exp >= (int)FLOAT_EXP_OFF ) && ( *(_TYPE*)&buf != 0 ) )
	{
		Exception( EXCEPTION_UNDERFLOW, 0 );

		*(_TYPE*)&buf = *(_TYPE*)&buf >> 1;

		while( ( -exp > (int)FLOAT_EXP_OFF ) && ( *(_TYPE*)&buf != 0 ) )
		{
			*(_TYPE*)&buf = *(_TYPE*)&buf >> 1;
			exp++;
		}

		// 可能小到连Denormal也无法表示,转换为0
		exp = FLOAT_EXP_OFF;
		exp = -exp;
	}

	// 整数位过大
	while( buf.m_nInt > 1 )
	{
		// 检查上溢
		if( exp >= int( FLOAT_EXP_OFF + 1 ) )
			break;

		*(_TYPE*)&buf = *(_TYPE*)&buf >> 1;
		exp++;
	}

	// 向上溢出
	if( exp >= int( FLOAT_EXP_OFF + 1 ) )
	{
		Exception( EXCEPTION_OVERFLOW, 0 );
		return CFloatPoint<_TYPE,_EXP_BITS>::Infinite( sign );
	}
	
	// 舍入处理,注意Denormal的表现
	if( ( buf.m_nRoundings > ROUND_LIMIT ) ||
		( ( buf.m_nRoundings == ROUND_LIMIT ) && ( buf.m_nFraction & 1 ) ) )
	{
		// 舍入,#P
		Exception( EXCEPTION_PRECISION, 0 );

		// Denormals
		if( ( -exp == (int)FLOAT_EXP_OFF ) && ( *(_TYPE*)&buf != 0 ) )
		{
			*((_TYPE*)&buf) += ( ROUND_LIMIT << 1 );

			// 舍入进位导致Denormal成为Finite
			if( buf.m_nInt )
				exp++;
		}
		else
		{
			// 有限数据进位修正
			*((_TYPE*)&buf) += ( ROUND_LIMIT << 1 );
			
			// 舍入导致整数位发生变化,需要修正
			if( buf.m_nInt > 1 )
			{
				*(_TYPE*)&buf = *(_TYPE*)&buf >> 1;
				exp++;
			}

			// 上溢
			if( exp >= int( FLOAT_EXP_OFF + 1 ) )
			{
				Exception( EXCEPTION_OVERFLOW, 0 );
				return CFloatPoint<_TYPE,_EXP_BITS>::Infinite( sign );
			}
		}
	}

	// 设置返回值
	rv.m_data.m_nExp      = exp + (int)FLOAT_EXP_OFF;
	rv.m_data.m_nFraction = buf.m_nFraction;

	return rv;
}

template<class _TYPE,int _EXP_BITS>
CFloatPoint<_TYPE,_EXP_BITS> 
MulBufRestore( CFloatPoint<_TYPE,_EXP_BITS>::_MUL_BUF& buf, int exp, int sign )
{
	CFloatPoint<_TYPE,_EXP_BITS>::_BUF s_buf;

	// 规范化
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

	s_buf.m_nInt	  = (_TYPE)buf.m_nInt;		// 此处抛弃了高2位
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
//                           IEEE建议函数
//-----------------------------------------------------------------------------

// class
template<class _TYPE,int _EXP_BITS>
int Class( const CFloatPoint<_TYPE,_EXP_BITS>& x )
{
	int exp;
	
	// 计算指数
	exp  = int( x.m_data.m_nExp );
	exp -= (int)FLOAT_EXP_OFF;

	if( -exp == (int)FLOAT_EXP_OFF )		// 最小指数
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
	else if( exp == int( FLOAT_EXP_OFF + 1 ) )	// 最大指数
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