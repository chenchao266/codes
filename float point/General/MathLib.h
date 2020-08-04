// 本模板是第7章<基本函数库>的实现,支持通用定点和浮点运算
// 酒泉卫星发射中心技术部  刘纯根  2007-7

#ifndef __LCG_MATH_LIB_2005_11_15__
#define __LCG_MATH_LIB_2005_11_15__

// 为了与标准CRT区别,本库使用了特别的名字,如果要支持double之类,可以:
// [1]通过宏将名字重命名,利用了重载功能
//    #define ArcSin  asin
//    这样ArcSin(1.0)/ArcSin(CSuperFloat<>)/sin(1.0)/sin(CSuperFlaot<>)均可使用,
// [2]可以特化这些函数,例如
//    double ArcSin( double x ){ return sin(x); }

// _TYPE需要提供:
//
//	  构造函数
//		_TYPE()							构造函数,初始化0.0
//		_TYPE( int n )					构造函数,从int构造实数
//		_TYPE( int n, int m )			构造函数,从分数n/m构造实数
//		_TYPE( const char *str )		构造函数,从字符串构造实数
//
//	  操作符
//		+，-，*，/						四则运算
//		-								取反对数
//		>,>=,<,<=,!=,==					逻辑比较
//      int								转换
//
//	  类型函数
//      int   Class  ( _TYPE x )		类型函数
//		_TYPE Zero( int sign )			返回+0或-0
//		_TYPE QNaN()					返回QNaN
//		_TYPE Infinite( int sign )		返回+INF和-INF
//
//	  其他
//		_TYPE Integer( _TYPE x )		取整数部分,注意返回类型是_TYPE
//      _TYPE Abs( _TYPE x )
//		_TYPE Scale( _TYPE x, int exp ) 计算x*2**n
//		_TYPE FractionExp( _TYPE x, int& exp )
//										将x分解为尾数和2的指数部分,y*2**exp

// 缺省精度,最好使用Precision设置自己的精度要求
#define _STR_DEFAULT_PRECISION	"0.00001"

#define _L2X_TABLE_COUNT		128			// Xn

// 异常
#define EXCEPTION_INVALID		0x00000001
#define EXCEPTION_DENORMAL		0x00000002
#define EXCEPTION_DIVIDEDBYZERO	0x00000004
#define EXCEPTION_OVERFLOW		0x00000008
#define EXCEPTION_UNDERFLOW		0x00000010
#define EXCEPTION_PRECISION		0x00000020

#define ERROR_ADD_CARRAY		0x00010000
#define ERROR_SUB_BORROW		0x00100000
#define ERROR_MUL_OVERFLOW		0x01000000	// 乘法高端扩展部分被丢弃
#define ERROR_MUL_PRECISION		0x02000000	// 定点实数低端小数被舍弃
#define ERROR_DIV_BYZERO		0x10000000
#define ERROR_DIV_PRECISION		0x20000000	// 有余数

#define ERROR_ALL				0xFFFFFFFF
	
#define CLASS_SNAN				0x0001  /* signaling NaN */
#define CLASS_QNAN				0x0002  /* quiet NaN */
#define CLASS_NINF				0x0004  /* negative infinity */
#define CLASS_NN				0x0008  /* negative normal */
#define CLASS_ND				0x0010  /* negative denormal */
#define CLASS_NZ				0x0020  /* -0 */
#define CLASS_PZ				0x0040  /* +0 */
#define CLASS_PD				0x0080  /* positive denormal */
#define CLASS_PN				0x0100  /* positive normal */
#define CLASS_PINF				0x0200  /* positive infinity */

//-----------------------------------------------------------------------------
//                           三角函数
//-----------------------------------------------------------------------------

// 全定义域sin函数
template< class _TYPE >
_TYPE Sin( _TYPE x )
{
	_TYPE Zero, Two(2), Four(4), k, _PI=Math_PI(Zero), _PI2;
	int sign;

	switch( Class( x ) )
	{
	// QNaN => QNaN,不变;+-0不变
	case CLASS_QNAN:
	case CLASS_NZ:
	case CLASS_PZ:
		return x;

	// +-无穷 => QNaN,SNaN => QNaN
	case CLASS_SNAN:
	case CLASS_PINF:
	case CLASS_NINF:
		Exception( EXCEPTION_INVALID, 0 );
		return _TYPE::QNaN();

	// 其他[正负有限数和正负弱规范数]参与运算
	default:
		break;
	}

	// 规范到[-Pi,PI]
	_PI2 = _PI*Two;
	while( Abs( x ) > _PI )
	{
		k = Nearest( x/_PI2 );
		x = x - k*_PI2;
	}


	// 规范到[0,PI]
	if( x < Zero )
	{
		x 	 = -x;
		sign = 1;
	}
	else
		sign = 0;

	// 分成4个区间
	k = x*Four/_PI;

	switch( (int)k )
	{
	case 0:		// [0,PI/4)
		x = Sin_k( x );
		break;

	case 1:		// [PI/4,PI/2)
		x = Cos_k( _PI/Two - x );
		break;

	case 2:		// [PI/2,3PI/4)
		x = Cos_k( x - _PI/Two );
		break;

	case 3:		// [3PI/4,PI)
		x = Sin_k( _PI - x );
		break;

	// 由于舍入,在PI附近可能出现4,sin(PI)=0.0
	default:
		return _TYPE::Zero( 0 );
	}

	if( sign )
		x = -x;

	return x;
}

// 全定义域cos函数
template< class _TYPE >
_TYPE Cos( _TYPE x )
{
	_TYPE Zero, One(1), Two(2), Four(4), k, _PI=Math_PI(Zero), _PI2;

	switch( Class( x ) )
	{
	// QNaN => QNaN,不变
	case CLASS_QNAN:
		return x;

	case CLASS_NZ:
	case CLASS_PZ:
		return One;

	// +-无穷QNaN => QNaN
	case CLASS_SNAN:
	case CLASS_PINF:
	case CLASS_NINF:
		Exception( EXCEPTION_INVALID, 0 );
		return _TYPE::QNaN();

	// 其他[正负有限数和正负弱规范数]参与运算
	default:
		break;
	}

	// 规范到[-Pi,PI]
	_PI2 = _PI*Two;
	while( Abs( x ) > _PI )
	{
		k = Nearest( x/_PI2 );
		x = x - k*_PI2;
	}

	// 规范到[0,2PI]
	if( x < Zero )
		x = -x;

	// 分成4个区间
	k = x*Four/_PI;
	switch( (int)k )
	{
	case 0:
		x = Cos_k( x );
		break;

	case 1:
		x = Sin_k( _PI/Two - x );
		break;

	case 2:
		x = -Sin_k( x - _PI/Two );
		break;

	case 3:
		x = -Cos_k( _PI - x );
		break;

	// 由于舍入,在PI附近有可能出现4,cos(PI)=-1.0
	default:
		return -One;
	}

	return x;
}

template< class _TYPE >
_TYPE Tan( _TYPE x )
{
	_TYPE Zero, One(1), Two(2), k, _PI=Math_PI(Zero);
	
	switch( Class( x ) )
	{
	// QNaN/+-0不变
	case CLASS_QNAN:
	case CLASS_NZ:
	case CLASS_PZ:
		return x;

	// 非法运算
	case CLASS_SNAN:
	case CLASS_PINF:
	case CLASS_NINF:
		Exception( EXCEPTION_INVALID, 0 );
		return _TYPE::QNaN();

	// 其他[正负有限数和正负弱规范数]参与运算
	default:
		break;
	}

	// 规范到[-PI/2,PI/2]
	while( Abs( x ) > _PI/Two )
	{
		k = Nearest( x/_PI );
		x = x - k*_PI;
	}

	x = Sin( x );
	x = x/Sqrt( One - x*x );

	return x;
}

// 利用sin在0.0处展开的泰勒级数计算sin在[0,PI/4]的值
template< class _TYPE >
_TYPE Sin_k( _TYPE x )
{
	_TYPE Zero, PRECISION=Precision(Zero);
	_TYPE n(1), n1(1), xn, x2, y, ck, One(1), Two(2);

	// n自2递增,步长2
	// n1是1/(2n+1)!
	// xn是x**(2n+1)

	// x-x**3/3!+x**5/5!-...+(-1)**n*x**(2n+1)/(2n+1)!+...

	xn = x;
	x2 = x*x;

	do{
		ck = xn*n1;

		n  = n + Two;
		n1 = n1/n/( n - One );
		xn = xn*x2;

		ck = ck - xn*n1;
		y  = y + ck; 

		// 下一次
		xn= xn*x2;
		n = n + Two;
		n1= n1/n/( n - One );

	// 由于x位于[0,PI/4]之间,因此ck>=0 
	}while( ck >= PRECISION );

	return y;
}

// 利用cos在0.0处展开的泰勒级数计算cos在[0,PI/4]的值
template< class _TYPE >
_TYPE Cos_k( _TYPE x )
{
	_TYPE Zero, PRECISION=Precision(Zero);
	_TYPE n(0), n1(1), xn(1), x2, y, ck, One(1), Two(2);

	// n自1递增,步长2
	// n1是2n!
	// xn是x**2n

	// 1-x**2/2!+x**4/4!-...+(-1)**n*x**2n/2n!+...

	x2 = x*x;

	do{
		ck = xn*n1;
		
		n  = n + Two;
		n1 = n1/n/( n - One );
		xn = xn*x2;

		ck = ck - xn*n1;
		y  = y + ck; 

		// 下一次
		xn= xn*x2;
		n = n + Two;
		n1 = n1/n/( n - One );

	// 由于x位于[0,PI/4]之间,因此ck>=0 
	}while( ck >= PRECISION );

	return y;
}

//-----------------------------------------------------------------------------
//                           反三角函数
//-----------------------------------------------------------------------------

template< class _TYPE >
_TYPE ArcSin( _TYPE x )
{
	_TYPE Zero, PRECISION=Precision(Zero),_PI=Math_PI(Zero);
	_TYPE x2, ck, y, n(1), One(1), Two(2);
	int sign;

	if( x > One || x < -One )
	{
		Exception( EXCEPTION_INVALID, 0 );
		return _TYPE::QNaN();
	}

	if( x == One )
		return _PI/Two;

	if( x == -One )
		return -_PI/Two;

	if( x < Zero )
	{
		x = -x;
		sign = 1;
	}
	else
		sign = 0;

	// x+x**3/(2*3)+x**5*(1*3)/(2*4*5)+x**7*(1*3*5)/(2*4*6*7)

	x2 = x*x;
	y  = x;
	ck = x;

	do{
		ck = ck*n/( n + One );
		ck = ck*n/( n + Two );
		ck = ck*x2;

		y  = y + ck;
		n  = n + Two;

	// 由于x位于[0,1]之间,因此ck>=0 
	}while( ck >= PRECISION );

	if( sign )
		y = -y;

	return y;
}

/*
template< class _TYPE >
_TYPE ArcSin( _TYPE x )
{
	_TYPE One(1);

	if( x > One || x < -One )
	{
		Exception( EXCEPTION_INVALID, 0 );
		return _TYPE::QNaN();
	}

	// 注意正负无穷的符号
	return ArcTan( x/Sqrt( One - x*x ) );
}
*/

template< class _TYPE >
_TYPE ArcCos( _TYPE x )
{
	_TYPE Zero, One(1), Two(2), _PI=Math_PI(Zero);

	if( x > One || x < -One )
	{
		Exception( EXCEPTION_INVALID, 0 );
		return _TYPE::QNaN();
	}

	return _PI/Two - ArcSin( x );
}

/*
template< class _TYPE >
_TYPE ArcCos( _TYPE x )
{
	_TYPE Zero, One(1), _PI=Math_PI(Zero);

	if( x > One || x < -One )
	{
		Exception( EXCEPTION_INVALID, 0 );
		return _TYPE::QNaN();
	}

	x = ArcTan( Sqrt( One - x*x )/x );
	if( x < Zero )
		x = x + _PI;

	// 注意正负无穷的符号
	return x;
}
*/

template< class _TYPE >
_TYPE ArcTan( _TYPE x )
{
	_TYPE Zero, _PI=Math_PI(Zero), PRECISION=Precision(Zero);
	_TYPE One(1), Two(2), Four(4), n(1), xn, y, x2, ck;
	int sign;

	switch( Class( x ) )
	{
	// 非法运算
	case CLASS_SNAN:
		return _TYPE::QNaN();

	// 直接返回
	case CLASS_QNAN:
	case CLASS_NZ:
	case CLASS_PZ:
		return x;

	case CLASS_PINF:
		return _PI/Two;

	case CLASS_NINF:
		return -_PI/Two;

	// 正负有限数和弱规范数参与运算
	default:
		break;
	}

	if( x == Zero )
		return Zero;

	if( x == One )
		return _PI/Four;

	if( x == -One )
		return -_PI/Four;

	if( x < Zero )
	{
		x = -x;
		sign = 1;
	}

	if( x < One )
	{
		// x-x**3/3+x**5/5-x**7/7+...+(-1)**n*x**(2n+1)/(2n+1)+...
		xn = x;
		x2 = x*x;

		do{
			ck = xn/n;

			n = n + Two;
			xn= xn* x2;

			ck = ck - xn/n;
			y  = y + ck;

			// 下一次
			n = n + Two;
			xn= xn* x2;

		// 因为x位于[0,1]区间,所以ck>=0
		}while( ck >= PRECISION );
	}
	else
	{
		// ?PI/2-x+1/(3x**3)-1/(5x**5)+1/7x**7+...+(-1)**(n+1)/(2n+1)x**(2n+1)+...
		xn = x;
		x2 = x*x;

		do{
			ck = One/n/xn;

			n = n + Two;
			xn= xn* x2;

			ck = ck - One/n/xn;
			y  = y + ck;

			// 下一次
			n = n + Two;
			xn= xn* x2;

		// 因为x位于[1,PINF]区间,所以ck>=0
		}while( ck >= PRECISION );

		y = _PI/Two - y;
		if( x < Zero )
			y = y - _PI;
	}

	if( sign )
		y = -y;

	return y;
}
/*
// 利用牛顿迭代计算ArcTan函数在[-PI/2,PI/2]的值
template< class _TYPE >
_TYPE ArcTan( _TYPE x )
{
	_TYPE Zero, PRECISION=Precision(Zero), _PI=Math_PI(Zero);
	_TYPE yn, yn1, sn, cn, k, Two(2), Four(4);

	switch( Class( x ) )
	{
	// 非法运算
	case CLASS_SNAN:
		return _TYPE::QNaN();

	// 直接返回
	case CLASS_QNAN:
	case CLASS_NZ:
	case CLASS_PZ:
		return x;

	case CLASS_PINF:
		return _PI/Two;

	case CLASS_NINF:
		return -_PI/Two;

	// 正负有限数和弱规范数参与运算
	default:
		break;
	}

	// 初值点,x>0.0取PI/4,x<0.0取-PI/4
	yn1 = _PI/Four;
	if( x < Zero )
		yn1 = -_PI/Four;

	do{	
		yn = yn1;

		sn = Sin_k( yn );
		cn = Cos_k( yn );
		yn1 = yn - ( sn - x*cn  )/( cn + x*sn );

	}while( Abs( yn1 - yn ) >= PRECISION );

	// 收敛值可能在另外的分支,规范到[-PI/2,PI/2]
	k = Integer( yn1/_PI );
	yn1 = yn1 - k*_PI;

	if( yn1 > _PI/Two )
		yn1 = yn1 - _PI;

	if( yn1 < -_PI/Two )
		yn1 = yn1 + _PI;
	
	return yn1;
}
*/

//-----------------------------------------------------------------------------
//                           对数函数
//-----------------------------------------------------------------------------

// ln
template< class _TYPE >
_TYPE Log( _TYPE x )
{
	_TYPE Zero, One(1), y, C05(1,2);
	int exp;

	switch( Class( x ) )
	{
	// 非法运算
	case CLASS_PINF:
	case CLASS_NINF:
	case CLASS_SNAN:
	case CLASS_QNAN:
	case CLASS_NN:
	case CLASS_ND:
		Exception( EXCEPTION_INVALID, 0 );
		return _TYPE::QNaN();

	case CLASS_NZ:
	case CLASS_PZ:
		return _TYPE::Infinite( 1 );;

	// 其他参与运算
	default:
		break;
	}

	if( x == One )
		return Zero;

	// 将x分解为尾数和指数部分分别处理
	y = FractionExp( x, exp );

	// 尾数部分,y属于[1,2]区间,所以1/y属于[0.5,1]区间
	y = -Log_k( One/y );

	// 附加指数部分
	x = exp;
	x = y - x*Log_k( C05 );

	return x;
}

// 只计算[0,1]区间的值,在1.0处展开
template< class _TYPE >
_TYPE Log_k( _TYPE x )
{
	_TYPE Zero, PRECISION=-Precision(Zero);
	_TYPE ck, xn, y, n(1), One(1), Two(2);
	
	// x位于[0,1]区间
	x  = x - One;
	xn = x;

	// 此时x属于[-1,0]区间

	do{
		ck = xn/n;
		
		n  =  n + One;
		xn = xn*x;

		ck = ck - xn/n;
		y = y + ck;

		n  = n + One;
		xn = xn*x;

	// 由于x位于[-1,0]区间,所以ck<=0
	// -ck >= PRECISION => ck <= -PRECISION
	}while( ck <= PRECISION );

	return y;
}

template< class _TYPE >
_TYPE Log10( _TYPE x )
{
	_TYPE C10(10);
	return Log( x )/Log( C10 );
}


//-----------------------------------------------------------------------------
//                           指数函数
//-----------------------------------------------------------------------------


template< class _TYPE >
_TYPE Exp( _TYPE x )
{
	_TYPE Zero, One(1), n, y, _E=Math_E(Zero);

	switch( Class( x ) )
	{
	case CLASS_QNAN:
	case CLASS_PINF:
		return x;

	case CLASS_SNAN:
		Exception( EXCEPTION_INVALID, 0 );
		return _TYPE::QNaN();

	case CLASS_NINF:
		return Zero;

	case CLASS_NZ:
	case CLASS_PZ:
		return One;

	// 其他参与运算
	default:
		break;
	}
	
	// 将指数分解为整数部分和小数部分
	// Exp_k只能计算[-1,1]区间
	n = Integer( x );

	if( n != Zero )
		x = Exp_k( x - n ) * Xn( _E, n );
	else
		x = Exp_k( x );

	return x;
}

// 利用在0.0处的泰勒级数计算指数函数在[-1,1]的值
template< class _TYPE >
_TYPE Exp_k( _TYPE x )
{
	_TYPE Zero, PRECISION=Precision(Zero);
	_TYPE y(1), ck, n(1), One(1);
	int sign;

	// 1+x/1!+x**2/2!+...+x**n/n!+...

	if( x < Zero )
	{
		x = -x;
		sign = 1;
	}

	// 在0.0处展开
	ck = x;

	do{
		y = y + ck;
		n = n + One;
		ck= ck*x/n;

	// 由于此时x位于[0,1]区间,所以ck>=0
	}while( ck >= PRECISION );

	if( sign )
		y = One/y;

	return y;
}

//-----------------------------------------------------------------------------
//                           幂函数
//-----------------------------------------------------------------------------

// x**n
template< class _TYPE >
_TYPE Power( _TYPE x, int n )
{
	_TYPE y( n );
	return Power( x, y );
}

template< class _TYPE >
_TYPE Power( _TYPE x, _TYPE y )
{
	_TYPE Zero, n, One(1);

	if( y == Zero )
		return One;

	if( x == Zero )
	{
		Exception( EXCEPTION_INVALID, 0 );
		return _TYPE::QNaN();
	}

	n = Integer( y );
	if( n == y )
		return Xn( x, n );

	// x<0,y不是整数,非法
	if( x < Zero )
	{
		Exception( EXCEPTION_INVALID, 0 );
		return _TYPE::QNaN();
	}

	return Exp( y * Log( x ) );
}

// Xn的辅助函数,判断是否继续计算
template< class _TYPE >
int XnCheck( _TYPE x, int xe, int nodd, _TYPE& y )
{
	_TYPE Zero;

	// 初始化返回值
	y = Zero;

	// 已经到0, 不再继续
	if( x == Zero )
		return 0;

	// 已到无穷,不再继续
	if( !IsFinite( x ) )
	{
		// 负数的奇数次方
		if( xe && nodd )
			y = _TYPE::Infinite( 1 );
		else
			y = _TYPE::Infinite( 0 );

		return 0;
	}

	// 其他情形继续
	return 1;
}

// x**n,其中n是整数
template< class _TYPE >
_TYPE Xn( _TYPE x, _TYPE n )
{
	// 可能体积太大,以及初始化太慢,使用静态变量
	static _TYPE _LX2[_L2X_TABLE_COUNT];
	_TYPE Zero, One(1), e(1), Two(2), em, dt;
	int i, m, odd, xe;

	// 取整一下
	n = Integer( n );

	// 指数可能是负值
	if( n < Zero )
	{
		n = -n;
		x = One/x;
	}

	if( n == Zero )
		return One;

	if( x == Zero )
		return Zero;

	if( x == One )
		return One;

	// x的符号,用于决定返回正负无穷
	if( x < Zero )
		xe = 1;
	else
		xe = 0;

	// n奇偶性
	dt = Integer( n/Two );
	dt = dt*Two;
	if( dt == n )
		odd = 0;
	else
		odd = 1;

	// 生成元素表,每项对应于指数1,2,4,8,16,32,64,128...直至超过指数值
	for( i=0; i<_L2X_TABLE_COUNT; i++ )
	{
		// 判断是否继续
		if( !XnCheck( x, xe, odd, dt ) )
			return dt;

		_LX2[i] = x;

		// 下一个指数是否已超过?
		x = x * x;
		e = e + e;
		if( e > n )
			break;
	}

	// 最大项号和对应指数
	m  = i;
	em = e/Two;

	// 超过元素表,先处理一下输入,以最大元素项为单位累积
	x = One;
	if( m >= _L2X_TABLE_COUNT )
	{
		while( n >= em )
		{
			x = x * _LX2[m];
			n  = n - em;

			// 判断是否继续
			if( !XnCheck( x, xe, odd, dt ) )
				return dt;
		}
	}

	// 寻找匹配元素,然后累积
	do{
		// 寻找最大元素
		e = em;
		for( i=m; i>=0; i-- )
		{
			if( n >= e )
			{
				// 下次从此开始
				em = e;
				m  = i;
				break;
			}

			e = e/Two;
		}

		if( i >= 0 )
		{
			x = x * _LX2[i];

			// 判断是否继续
			if( !XnCheck( x, xe, odd, dt ) )
				return dt;

			n  = n - e;	
		}
	}while( n > Zero );

	return x;
}

template< class _TYPE >
_TYPE Xn( _TYPE x, int n )
{
	_TYPE y( n ), One, Two;

	if( x == Two )
		return Scale( One, n );
	else
		return Xn( x, y );
}

template< class _TYPE >
_TYPE Sqrt( _TYPE x )
{
	_TYPE Zero, y, Two(2), One(1);
	int exp;

	if( x < Zero )
	{
		Exception( EXCEPTION_INVALID, 0 );
		return _TYPE::QNaN();
	}

	if( x == Zero )
		return Zero;

	// 将x分解为尾数和指数
	y = FractionExp( x, exp );

	// 尾数部分,y属于[1,2]区间
	y = Sqrt_k( y );

	// 指数部分减半
	x = Scale( One, exp >> 1 );
	if( exp & 1 == 1 )
		x = x * Sqrt_k( Two );

	return x*y;	
}

// 迭代计算[1,2]区间的值
template< class _TYPE >
_TYPE Sqrt_k( _TYPE x )
{
	_TYPE Zero, PRECISION=Precision(Zero);
	_TYPE yn, Two(2), yn1;

	yn1 = x;

	do{
		yn  = yn1;
		yn1 = yn + x/yn;
		yn1 = yn1/Two;

	// 由于y属于[1,2]区间,结果属于[1,1.414]区间
	}while( Abs( yn1 - yn ) >= PRECISION );

	return yn1;
}


//-----------------------------------------------------------------------------
//                           辅助函数
//-----------------------------------------------------------------------------

template< class _TYPE >
int IsNaN( _TYPE x )
{
	switch( Class( x ) )
	{
	case CLASS_SNAN:
	case CLASS_QNAN:
		return 1;

	default:
		break;
	}
	return 0;
}

template< class _TYPE >
int IsInfinite( _TYPE x )
{
	switch( Class( x ) )
	{
	case CLASS_PINF:
	case CLASS_NINF:
		return 1;

	default:
		break;
	}
	return 0;
}


template< class _TYPE >
int IsFinite( _TYPE x )
{
	switch( Class( x ) )
	{
	case CLASS_PN:
	case CLASS_NN:
	case CLASS_PZ:
	case CLASS_NZ:
	case CLASS_PD:
	case CLASS_ND:
		return 1;

	// INF,NaN
	default:
		break;
	}
	return 0;
}

template< class _TYPE >
_TYPE Nearest( _TYPE x )
{
	_TYPE C05(1,2), One(1), y;		// 0.5

	// 整数部分
	y = Integer( x );
	
	// 小数部分
	x = x - y;

	if( x > C05  )
		y = y + One;

	if( x < -C05 )
		y =  y - One;

	return y;
}

//-----------------------------------------------------------------------------
//                           异常处理和精度控制
//-----------------------------------------------------------------------------

unsigned int Exception( unsigned int nExceptSet, unsigned int nExceptClr )
{
	static unsigned int EXCEPTIONS=0;
	unsigned int nExcepts=EXCEPTIONS;

	EXCEPTIONS &= ~nExceptClr;
	EXCEPTIONS |=  nExceptSet;

	return nExcepts;
}

template<class _TYPE >
_TYPE Precision( const _TYPE& Precision )
{
	static _TYPE Zero, PRECISION=_STR_DEFAULT_PRECISION;

	if( Precision != Zero )
		PRECISION = Precision;

	return PRECISION;
}

//-----------------------------------------------------------------------------
//                           输入输出函数
//-----------------------------------------------------------------------------

template<class _TYPE >
int DataFromStr( _TYPE& x, const char* str )
{
	int sign, exp, es, i;
	_TYPE CBase(10), CBase01(1,10), off;

	// 用于输入的数字
	static _TYPE digits[10];
	static int bInited = 0;

	if( bInited == 0 )
	{
		for( i=0; i<10; i++ )
			digits[i] = i;
		bInited = 1;
	}

	// [1]初始化与检测
	x = 0;

	if( str == 0 )
		return 0;

	i = 0;

	// 跳过非法符号
	while( str[i] != '-' && str[i] != '+' && str[i] != '.' &&
		   ( str[i] > '9' || str[i] < '0' ) && str[i] != '\0' )
	{
		i++;
	}

	// [2]处理符号
	if( str[i] == '-' )
	{
		sign = 1;
		i++;
	}
	else if( str[i] == '+' )
	{
		sign = 0;
		i++;
	}
	else
		sign = 0;

	// [3]处理值

	// 处理整数部分
	x = 0;
	while( str[i] >= '0' && str[i] <= '9' )
	{
		x = x * CBase + digits[ str[i] - '0' ];
		i++;
	}

	// 处理小数部分
	off = CBase01;
	if( str[i] == '.' )
	{
		i++;
		while( str[i] >= '0' && str[i] <= '9' )
		{
			x = x + digits[ str[i] - '0' ]*off;
			i++;
			off = off * CBase01;
		}
	}

	// 处理指数部分
	if( str[i] == 'E' || str[i] == 'e' )
	{
		i++;

		// 指数符号
		if( str[i] == '-' )
		{
			es = 1;
			i++;
		}
		else if( str[i] == '+' )
		{
			es = 0;
			i++;
		}
		else
			es = 0;

		exp = 0;
		while( str[i] >= '0' && str[i] <= '9' )
		{
			exp = exp * 10 + str[i] - '0';
			i++;
		}

		if( es )
			exp = -exp;

		if( exp != 0 )
			x = x * Xn( CBase, exp );
	}

	if( sign )
		x = -x;

	// 返回处理的字符长度
	return i;
}

template<class _TYPE >
const char* DataToStr( const _TYPE& x, int digits = 0, int* pCnt = 0 )
{
	static char str[sizeof(_TYPE)*3+32];
	char temp[sizeof(_TYPE)*3+32];

	_TYPE Zero, C10( 10 ),One( 1 ), y, dt, C5( 5 );
	int i, exp, k, xclass;

	y = x;

	// [1]处理特殊值
	if( pCnt != 0 )
		*pCnt = 4;

	xclass = Class( y );
	switch( xclass )
	{
	case CLASS_PINF:
		return "+INF";

	case CLASS_NINF:
		return "-INF";

	case CLASS_QNAN:
		return "QNAN";

	case CLASS_SNAN:
		return "SNAN";

	default:
		break;
	}

	// 符号,小数点
	if( digits != 0 )
		digits += 2;

	i = 0;

	// [2]处理符号
	if( y < Zero )
	{
		str[i] = '-';
		y = -y;
	}
	else
		str[i] = '+';
	i++;

	// [3]获取指数
	if( xclass != CLASS_NZ && xclass != CLASS_PZ )
	{
		dt = Log10( y );

		// 注意,由于y->0可能导致Log10()返回NINF
		if( Class( dt ) != CLASS_NINF )
		{
			dt  = Integer( dt );
			exp = dt;

			// 修正可能产生的计算误差,例如0.09999999999999之类
			y   = y / Xn( C10, dt );

			while( y >= C10 )
			{
				exp++;
				y = y/C10;
			}

			while( y < One && y != Zero )
			{
				exp--;
				y = y*C10;
			}
		}
		else
		{
			// 指数-INF,按0处理
			y = Zero;
		}
	}

	// [4]处理整数部分
	k = y;
	str[i++] = k + '0';

	//  整数部分0结果必然是0
	if( k == 0 )
		exp = 0;

	// [5]小数部分
	dt = k;
	y  = y - dt;

	str[i++] = '.';

	while( y != Zero )
	{
		y = y * C10;
		k = y;
		str[i++] = k + '0';
		dt = k;
		y = y - dt; 

		if( i >= sizeof(str) - 1 )
			break;

		if( digits && i >= digits )
			break;
	}

	// [6]填充长度
	while( i < digits )
		str[i++] = '0';

	// [7]输出指数部分
	str[i++] = 'E';
	if( exp < 0 )
	{
		str[i++] = '-';
		exp = -exp;
	}
	else
		str[i++] = '+';

	k = 0;
	do{
		temp[k++] = '0' + exp%10;
		exp = exp/10;
	}while( exp != 0 );

	k--;
	while( k >= 0 )
		str[i++] = temp[k--];

	str[i] = '\0';

	// [8]返回输出长度
	if( pCnt != 0 )
		*pCnt = i;

	return str;
}

//-----------------------------------------------------------------------------
//                           自然常数e和pi的计算函数
//-----------------------------------------------------------------------------

template< class _TYPE >
_TYPE Math_E( _TYPE PRECISION )
{
	static _TYPE Zero, MATH_E, E_PRECISION=Precision(Zero);
	_TYPE ck(1), n(1), One(1);

	// 使用缺省精度,因为新设置的精度可能比当前高
	if( PRECISION <= Zero )
		PRECISION = Precision( Zero );

	// 要求不高,直接返回E
	if( ( MATH_E != Zero ) && ( PRECISION >= E_PRECISION ) )
		return MATH_E;

	// 高要求,需要计算,设置精度要求
	if ( PRECISION < E_PRECISION )
		E_PRECISION = PRECISION;

	MATH_E = One;

	do{
		MATH_E = MATH_E + ck;
		n = n + One;
		ck= ck*One/n;
	}while( ck >= E_PRECISION );

	return MATH_E;
}

// 利用arcsin(PI/6)计算
template< class _TYPE >
_TYPE Math_PI( _TYPE PRECISION )
{
	static _TYPE Zero, MATH_PI, PI_PRECISION = Precision(Zero);
	_TYPE C05(1,2), C6(6), x2, ck, n(1), x, y, One(1), Two(2);

	// 使用缺省精度
	if( PRECISION <= Zero )
		PRECISION = Precision( Zero );

	// 要求不高,直接返回E
	if( ( MATH_PI != Zero ) && ( PRECISION >= PI_PRECISION ) )
		return MATH_PI;

	// 高要求,需要计算,设置精度要求
	if ( PRECISION < PI_PRECISION )
		PI_PRECISION = PRECISION;

	x  = C05;
	x2 = x*x;
	y  = x;
	ck = x;

	do{
		ck = ck*n/( n + One );
		ck = ck*n/( n + Two );
		ck = ck*x2;

		y  = y + ck;
		n  = n + Two;

	}while( ck >= PI_PRECISION );

	MATH_PI = y*C6;

	return MATH_PI;
}

#endif