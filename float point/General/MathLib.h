// ��ģ���ǵ�7��<����������>��ʵ��,֧��ͨ�ö���͸�������
// ��Ȫ���Ƿ������ļ�����  ������  2007-7

#ifndef __LCG_MATH_LIB_2005_11_15__
#define __LCG_MATH_LIB_2005_11_15__

// Ϊ�����׼CRT����,����ʹ�����ر������,���Ҫ֧��double֮��,����:
// [1]ͨ���꽫����������,���������ع���
//    #define ArcSin  asin
//    ����ArcSin(1.0)/ArcSin(CSuperFloat<>)/sin(1.0)/sin(CSuperFlaot<>)����ʹ��,
// [2]�����ػ���Щ����,����
//    double ArcSin( double x ){ return sin(x); }

// _TYPE��Ҫ�ṩ:
//
//	  ���캯��
//		_TYPE()							���캯��,��ʼ��0.0
//		_TYPE( int n )					���캯��,��int����ʵ��
//		_TYPE( int n, int m )			���캯��,�ӷ���n/m����ʵ��
//		_TYPE( const char *str )		���캯��,���ַ�������ʵ��
//
//	  ������
//		+��-��*��/						��������
//		-								ȡ������
//		>,>=,<,<=,!=,==					�߼��Ƚ�
//      int								ת��
//
//	  ���ͺ���
//      int   Class  ( _TYPE x )		���ͺ���
//		_TYPE Zero( int sign )			����+0��-0
//		_TYPE QNaN()					����QNaN
//		_TYPE Infinite( int sign )		����+INF��-INF
//
//	  ����
//		_TYPE Integer( _TYPE x )		ȡ��������,ע�ⷵ��������_TYPE
//      _TYPE Abs( _TYPE x )
//		_TYPE Scale( _TYPE x, int exp ) ����x*2**n
//		_TYPE FractionExp( _TYPE x, int& exp )
//										��x�ֽ�Ϊβ����2��ָ������,y*2**exp

// ȱʡ����,���ʹ��Precision�����Լ��ľ���Ҫ��
#define _STR_DEFAULT_PRECISION	"0.00001"

#define _L2X_TABLE_COUNT		128			// Xn

// �쳣
#define EXCEPTION_INVALID		0x00000001
#define EXCEPTION_DENORMAL		0x00000002
#define EXCEPTION_DIVIDEDBYZERO	0x00000004
#define EXCEPTION_OVERFLOW		0x00000008
#define EXCEPTION_UNDERFLOW		0x00000010
#define EXCEPTION_PRECISION		0x00000020

#define ERROR_ADD_CARRAY		0x00010000
#define ERROR_SUB_BORROW		0x00100000
#define ERROR_MUL_OVERFLOW		0x01000000	// �˷��߶���չ���ֱ�����
#define ERROR_MUL_PRECISION		0x02000000	// ����ʵ���Ͷ�С��������
#define ERROR_DIV_BYZERO		0x10000000
#define ERROR_DIV_PRECISION		0x20000000	// ������

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
//                           ���Ǻ���
//-----------------------------------------------------------------------------

// ȫ������sin����
template< class _TYPE >
_TYPE Sin( _TYPE x )
{
	_TYPE Zero, Two(2), Four(4), k, _PI=Math_PI(Zero), _PI2;
	int sign;

	switch( Class( x ) )
	{
	// QNaN => QNaN,����;+-0����
	case CLASS_QNAN:
	case CLASS_NZ:
	case CLASS_PZ:
		return x;

	// +-���� => QNaN,SNaN => QNaN
	case CLASS_SNAN:
	case CLASS_PINF:
	case CLASS_NINF:
		Exception( EXCEPTION_INVALID, 0 );
		return _TYPE::QNaN();

	// ����[�������������������淶��]��������
	default:
		break;
	}

	// �淶��[-Pi,PI]
	_PI2 = _PI*Two;
	while( Abs( x ) > _PI )
	{
		k = Nearest( x/_PI2 );
		x = x - k*_PI2;
	}


	// �淶��[0,PI]
	if( x < Zero )
	{
		x 	 = -x;
		sign = 1;
	}
	else
		sign = 0;

	// �ֳ�4������
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

	// ��������,��PI�������ܳ���4,sin(PI)=0.0
	default:
		return _TYPE::Zero( 0 );
	}

	if( sign )
		x = -x;

	return x;
}

// ȫ������cos����
template< class _TYPE >
_TYPE Cos( _TYPE x )
{
	_TYPE Zero, One(1), Two(2), Four(4), k, _PI=Math_PI(Zero), _PI2;

	switch( Class( x ) )
	{
	// QNaN => QNaN,����
	case CLASS_QNAN:
		return x;

	case CLASS_NZ:
	case CLASS_PZ:
		return One;

	// +-����QNaN => QNaN
	case CLASS_SNAN:
	case CLASS_PINF:
	case CLASS_NINF:
		Exception( EXCEPTION_INVALID, 0 );
		return _TYPE::QNaN();

	// ����[�������������������淶��]��������
	default:
		break;
	}

	// �淶��[-Pi,PI]
	_PI2 = _PI*Two;
	while( Abs( x ) > _PI )
	{
		k = Nearest( x/_PI2 );
		x = x - k*_PI2;
	}

	// �淶��[0,2PI]
	if( x < Zero )
		x = -x;

	// �ֳ�4������
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

	// ��������,��PI�����п��ܳ���4,cos(PI)=-1.0
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
	// QNaN/+-0����
	case CLASS_QNAN:
	case CLASS_NZ:
	case CLASS_PZ:
		return x;

	// �Ƿ�����
	case CLASS_SNAN:
	case CLASS_PINF:
	case CLASS_NINF:
		Exception( EXCEPTION_INVALID, 0 );
		return _TYPE::QNaN();

	// ����[�������������������淶��]��������
	default:
		break;
	}

	// �淶��[-PI/2,PI/2]
	while( Abs( x ) > _PI/Two )
	{
		k = Nearest( x/_PI );
		x = x - k*_PI;
	}

	x = Sin( x );
	x = x/Sqrt( One - x*x );

	return x;
}

// ����sin��0.0��չ����̩�ռ�������sin��[0,PI/4]��ֵ
template< class _TYPE >
_TYPE Sin_k( _TYPE x )
{
	_TYPE Zero, PRECISION=Precision(Zero);
	_TYPE n(1), n1(1), xn, x2, y, ck, One(1), Two(2);

	// n��2����,����2
	// n1��1/(2n+1)!
	// xn��x**(2n+1)

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

		// ��һ��
		xn= xn*x2;
		n = n + Two;
		n1= n1/n/( n - One );

	// ����xλ��[0,PI/4]֮��,���ck>=0 
	}while( ck >= PRECISION );

	return y;
}

// ����cos��0.0��չ����̩�ռ�������cos��[0,PI/4]��ֵ
template< class _TYPE >
_TYPE Cos_k( _TYPE x )
{
	_TYPE Zero, PRECISION=Precision(Zero);
	_TYPE n(0), n1(1), xn(1), x2, y, ck, One(1), Two(2);

	// n��1����,����2
	// n1��2n!
	// xn��x**2n

	// 1-x**2/2!+x**4/4!-...+(-1)**n*x**2n/2n!+...

	x2 = x*x;

	do{
		ck = xn*n1;
		
		n  = n + Two;
		n1 = n1/n/( n - One );
		xn = xn*x2;

		ck = ck - xn*n1;
		y  = y + ck; 

		// ��һ��
		xn= xn*x2;
		n = n + Two;
		n1 = n1/n/( n - One );

	// ����xλ��[0,PI/4]֮��,���ck>=0 
	}while( ck >= PRECISION );

	return y;
}

//-----------------------------------------------------------------------------
//                           �����Ǻ���
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

	// ����xλ��[0,1]֮��,���ck>=0 
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

	// ע����������ķ���
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

	// ע����������ķ���
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
	// �Ƿ�����
	case CLASS_SNAN:
		return _TYPE::QNaN();

	// ֱ�ӷ���
	case CLASS_QNAN:
	case CLASS_NZ:
	case CLASS_PZ:
		return x;

	case CLASS_PINF:
		return _PI/Two;

	case CLASS_NINF:
		return -_PI/Two;

	// ���������������淶����������
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

			// ��һ��
			n = n + Two;
			xn= xn* x2;

		// ��Ϊxλ��[0,1]����,����ck>=0
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

			// ��һ��
			n = n + Two;
			xn= xn* x2;

		// ��Ϊxλ��[1,PINF]����,����ck>=0
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
// ����ţ�ٵ�������ArcTan������[-PI/2,PI/2]��ֵ
template< class _TYPE >
_TYPE ArcTan( _TYPE x )
{
	_TYPE Zero, PRECISION=Precision(Zero), _PI=Math_PI(Zero);
	_TYPE yn, yn1, sn, cn, k, Two(2), Four(4);

	switch( Class( x ) )
	{
	// �Ƿ�����
	case CLASS_SNAN:
		return _TYPE::QNaN();

	// ֱ�ӷ���
	case CLASS_QNAN:
	case CLASS_NZ:
	case CLASS_PZ:
		return x;

	case CLASS_PINF:
		return _PI/Two;

	case CLASS_NINF:
		return -_PI/Two;

	// ���������������淶����������
	default:
		break;
	}

	// ��ֵ��,x>0.0ȡPI/4,x<0.0ȡ-PI/4
	yn1 = _PI/Four;
	if( x < Zero )
		yn1 = -_PI/Four;

	do{	
		yn = yn1;

		sn = Sin_k( yn );
		cn = Cos_k( yn );
		yn1 = yn - ( sn - x*cn  )/( cn + x*sn );

	}while( Abs( yn1 - yn ) >= PRECISION );

	// ����ֵ����������ķ�֧,�淶��[-PI/2,PI/2]
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
//                           ��������
//-----------------------------------------------------------------------------

// ln
template< class _TYPE >
_TYPE Log( _TYPE x )
{
	_TYPE Zero, One(1), y, C05(1,2);
	int exp;

	switch( Class( x ) )
	{
	// �Ƿ�����
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

	// ������������
	default:
		break;
	}

	if( x == One )
		return Zero;

	// ��x�ֽ�Ϊβ����ָ�����ֱַ���
	y = FractionExp( x, exp );

	// β������,y����[1,2]����,����1/y����[0.5,1]����
	y = -Log_k( One/y );

	// ����ָ������
	x = exp;
	x = y - x*Log_k( C05 );

	return x;
}

// ֻ����[0,1]�����ֵ,��1.0��չ��
template< class _TYPE >
_TYPE Log_k( _TYPE x )
{
	_TYPE Zero, PRECISION=-Precision(Zero);
	_TYPE ck, xn, y, n(1), One(1), Two(2);
	
	// xλ��[0,1]����
	x  = x - One;
	xn = x;

	// ��ʱx����[-1,0]����

	do{
		ck = xn/n;
		
		n  =  n + One;
		xn = xn*x;

		ck = ck - xn/n;
		y = y + ck;

		n  = n + One;
		xn = xn*x;

	// ����xλ��[-1,0]����,����ck<=0
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
//                           ָ������
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

	// ������������
	default:
		break;
	}
	
	// ��ָ���ֽ�Ϊ�������ֺ�С������
	// Exp_kֻ�ܼ���[-1,1]����
	n = Integer( x );

	if( n != Zero )
		x = Exp_k( x - n ) * Xn( _E, n );
	else
		x = Exp_k( x );

	return x;
}

// ������0.0����̩�ռ�������ָ��������[-1,1]��ֵ
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

	// ��0.0��չ��
	ck = x;

	do{
		y = y + ck;
		n = n + One;
		ck= ck*x/n;

	// ���ڴ�ʱxλ��[0,1]����,����ck>=0
	}while( ck >= PRECISION );

	if( sign )
		y = One/y;

	return y;
}

//-----------------------------------------------------------------------------
//                           �ݺ���
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

	// x<0,y��������,�Ƿ�
	if( x < Zero )
	{
		Exception( EXCEPTION_INVALID, 0 );
		return _TYPE::QNaN();
	}

	return Exp( y * Log( x ) );
}

// Xn�ĸ�������,�ж��Ƿ��������
template< class _TYPE >
int XnCheck( _TYPE x, int xe, int nodd, _TYPE& y )
{
	_TYPE Zero;

	// ��ʼ������ֵ
	y = Zero;

	// �Ѿ���0, ���ټ���
	if( x == Zero )
		return 0;

	// �ѵ�����,���ټ���
	if( !IsFinite( x ) )
	{
		// �����������η�
		if( xe && nodd )
			y = _TYPE::Infinite( 1 );
		else
			y = _TYPE::Infinite( 0 );

		return 0;
	}

	// �������μ���
	return 1;
}

// x**n,����n������
template< class _TYPE >
_TYPE Xn( _TYPE x, _TYPE n )
{
	// �������̫��,�Լ���ʼ��̫��,ʹ�þ�̬����
	static _TYPE _LX2[_L2X_TABLE_COUNT];
	_TYPE Zero, One(1), e(1), Two(2), em, dt;
	int i, m, odd, xe;

	// ȡ��һ��
	n = Integer( n );

	// ָ�������Ǹ�ֵ
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

	// x�ķ���,���ھ���������������
	if( x < Zero )
		xe = 1;
	else
		xe = 0;

	// n��ż��
	dt = Integer( n/Two );
	dt = dt*Two;
	if( dt == n )
		odd = 0;
	else
		odd = 1;

	// ����Ԫ�ر�,ÿ���Ӧ��ָ��1,2,4,8,16,32,64,128...ֱ������ָ��ֵ
	for( i=0; i<_L2X_TABLE_COUNT; i++ )
	{
		// �ж��Ƿ����
		if( !XnCheck( x, xe, odd, dt ) )
			return dt;

		_LX2[i] = x;

		// ��һ��ָ���Ƿ��ѳ���?
		x = x * x;
		e = e + e;
		if( e > n )
			break;
	}

	// �����źͶ�Ӧָ��
	m  = i;
	em = e/Two;

	// ����Ԫ�ر�,�ȴ���һ������,�����Ԫ����Ϊ��λ�ۻ�
	x = One;
	if( m >= _L2X_TABLE_COUNT )
	{
		while( n >= em )
		{
			x = x * _LX2[m];
			n  = n - em;

			// �ж��Ƿ����
			if( !XnCheck( x, xe, odd, dt ) )
				return dt;
		}
	}

	// Ѱ��ƥ��Ԫ��,Ȼ���ۻ�
	do{
		// Ѱ�����Ԫ��
		e = em;
		for( i=m; i>=0; i-- )
		{
			if( n >= e )
			{
				// �´δӴ˿�ʼ
				em = e;
				m  = i;
				break;
			}

			e = e/Two;
		}

		if( i >= 0 )
		{
			x = x * _LX2[i];

			// �ж��Ƿ����
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

	// ��x�ֽ�Ϊβ����ָ��
	y = FractionExp( x, exp );

	// β������,y����[1,2]����
	y = Sqrt_k( y );

	// ָ�����ּ���
	x = Scale( One, exp >> 1 );
	if( exp & 1 == 1 )
		x = x * Sqrt_k( Two );

	return x*y;	
}

// ��������[1,2]�����ֵ
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

	// ����y����[1,2]����,�������[1,1.414]����
	}while( Abs( yn1 - yn ) >= PRECISION );

	return yn1;
}


//-----------------------------------------------------------------------------
//                           ��������
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

	// ��������
	y = Integer( x );
	
	// С������
	x = x - y;

	if( x > C05  )
		y = y + One;

	if( x < -C05 )
		y =  y - One;

	return y;
}

//-----------------------------------------------------------------------------
//                           �쳣����;��ȿ���
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
//                           �����������
//-----------------------------------------------------------------------------

template<class _TYPE >
int DataFromStr( _TYPE& x, const char* str )
{
	int sign, exp, es, i;
	_TYPE CBase(10), CBase01(1,10), off;

	// �������������
	static _TYPE digits[10];
	static int bInited = 0;

	if( bInited == 0 )
	{
		for( i=0; i<10; i++ )
			digits[i] = i;
		bInited = 1;
	}

	// [1]��ʼ������
	x = 0;

	if( str == 0 )
		return 0;

	i = 0;

	// �����Ƿ�����
	while( str[i] != '-' && str[i] != '+' && str[i] != '.' &&
		   ( str[i] > '9' || str[i] < '0' ) && str[i] != '\0' )
	{
		i++;
	}

	// [2]�������
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

	// [3]����ֵ

	// ������������
	x = 0;
	while( str[i] >= '0' && str[i] <= '9' )
	{
		x = x * CBase + digits[ str[i] - '0' ];
		i++;
	}

	// ����С������
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

	// ����ָ������
	if( str[i] == 'E' || str[i] == 'e' )
	{
		i++;

		// ָ������
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

	// ���ش�����ַ�����
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

	// [1]��������ֵ
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

	// ����,С����
	if( digits != 0 )
		digits += 2;

	i = 0;

	// [2]�������
	if( y < Zero )
	{
		str[i] = '-';
		y = -y;
	}
	else
		str[i] = '+';
	i++;

	// [3]��ȡָ��
	if( xclass != CLASS_NZ && xclass != CLASS_PZ )
	{
		dt = Log10( y );

		// ע��,����y->0���ܵ���Log10()����NINF
		if( Class( dt ) != CLASS_NINF )
		{
			dt  = Integer( dt );
			exp = dt;

			// �������ܲ����ļ������,����0.09999999999999֮��
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
			// ָ��-INF,��0����
			y = Zero;
		}
	}

	// [4]������������
	k = y;
	str[i++] = k + '0';

	//  ��������0�����Ȼ��0
	if( k == 0 )
		exp = 0;

	// [5]С������
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

	// [6]��䳤��
	while( i < digits )
		str[i++] = '0';

	// [7]���ָ������
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

	// [8]�����������
	if( pCnt != 0 )
		*pCnt = i;

	return str;
}

//-----------------------------------------------------------------------------
//                           ��Ȼ����e��pi�ļ��㺯��
//-----------------------------------------------------------------------------

template< class _TYPE >
_TYPE Math_E( _TYPE PRECISION )
{
	static _TYPE Zero, MATH_E, E_PRECISION=Precision(Zero);
	_TYPE ck(1), n(1), One(1);

	// ʹ��ȱʡ����,��Ϊ�����õľ��ȿ��ܱȵ�ǰ��
	if( PRECISION <= Zero )
		PRECISION = Precision( Zero );

	// Ҫ�󲻸�,ֱ�ӷ���E
	if( ( MATH_E != Zero ) && ( PRECISION >= E_PRECISION ) )
		return MATH_E;

	// ��Ҫ��,��Ҫ����,���þ���Ҫ��
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

// ����arcsin(PI/6)����
template< class _TYPE >
_TYPE Math_PI( _TYPE PRECISION )
{
	static _TYPE Zero, MATH_PI, PI_PRECISION = Precision(Zero);
	_TYPE C05(1,2), C6(6), x2, ck, n(1), x, y, One(1), Two(2);

	// ʹ��ȱʡ����
	if( PRECISION <= Zero )
		PRECISION = Precision( Zero );

	// Ҫ�󲻸�,ֱ�ӷ���E
	if( ( MATH_PI != Zero ) && ( PRECISION >= PI_PRECISION ) )
		return MATH_PI;

	// ��Ҫ��,��Ҫ����,���þ���Ҫ��
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