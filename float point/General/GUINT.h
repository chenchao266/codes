// 本模板是第3章<通用整型运算>实现的一部分,支持任意长度的无符号整型
// 酒泉卫星发射中心技术部  刘纯根  2007-7

#ifndef __LCG_CPP_CGUINT_2005_11_5__
#define __LCG_CPP_CGUINT_2005_11_5__

#include"MathLib.h"

#define _SIZE				(sizeof(_TYPE)*_COUNT)
#define _TYPE_BITS			(sizeof(_TYPE)*8)
#define _TYPE_BITS_MASK		((_TYPE)-1)
#define _TYPE_HIGHT_BIT		(((_TYPE)1)<<(_TYPE_BITS-1))

template< class _TYPE, int _COUNT >
class CGUINT
{
	public:	// 构造函数
		CGUINT();
		CGUINT( unsigned int n );
		CGUINT( const char *str );

	public:	// 处理int的函数
		CGUINT operator=( unsigned int n );

		// 这个函数太危险,必需先删除,测试完成后有效
		operator unsigned int();

	public:	// 友函数,重载操作符+,-,*,/,%，>>,<<,&,|,^,~
		// 基本运算
		friend CGUINT operator+( const CGUINT& a, const CGUINT& b );
		friend CGUINT operator-( const CGUINT& a, const CGUINT& b );
		friend CGUINT operator*( const CGUINT& a, const CGUINT& b );
		friend CGUINT operator/( const CGUINT& a, const CGUINT& b );
		friend CGUINT operator%( const CGUINT& a, const CGUINT& b );

		// 移位操作
		friend CGUINT operator>>( const CGUINT& a, int bits );
		friend CGUINT operator<<( const CGUINT& a, int bits );
		
		// 位操作
		friend CGUINT operator&( const CGUINT& a, const CGUINT& b );
		friend CGUINT operator|( const CGUINT& a, const CGUINT& b );
		friend CGUINT operator^( const CGUINT& a, const CGUINT& b );
		friend CGUINT operator~( const CGUINT& b );

		// 逻辑操作
		friend int operator> ( const CGUINT& a, const CGUINT& b );
		friend int operator>=( const CGUINT& a, const CGUINT& b );
		friend int operator< ( const CGUINT& a, const CGUINT& b );
		friend int operator<=( const CGUINT& a, const CGUINT& b );
		friend int operator==( const CGUINT& a, const CGUINT& b );
		friend int operator!=( const CGUINT& a, const CGUINT& b );
		friend int operator! ( const CGUINT& b );

		// 除法辅助函数
		friend void Shift_Multiple( CGUINT& a, CGUINT& b, int bLeft );
		friend void Shift_Multiple_M( CGUINT& a, CGUINT& b, int bLeft, int bits );
		friend CGUINT DivKernel( const CGUINT& a, const CGUINT& b );
		friend int GetHighestBit( const CGUINT& a );

	public:// 数据
		_TYPE m_data[_COUNT];
};

//-----------------------------------------------------------------------------
//                           构造函数与一些特殊函数
//-----------------------------------------------------------------------------

// 缺省构造函数
template< class _TYPE, int _COUNT >
CGUINT<_TYPE,_COUNT>::CGUINT()
{
	for( int i=0; i<_COUNT; i++ )
		m_data[i] = 0;
}

// 从unsigned int构造CGUINT
template< class _TYPE, int _COUNT >
CGUINT<_TYPE,_COUNT>::CGUINT( unsigned int n )
{
	unsigned int i, k;
	_TYPE *src  = (_TYPE*)&n;

	// 特殊情形,CGUINT比unsigned int还小
	k = sizeof(unsigned int) < _SIZE ? sizeof(unsigned int) : _SIZE;

	for( i=0; i<k/sizeof(_TYPE); i++ )
		m_data[i] = src[i];

	// CGUINT比unsigned int大时
	while( i < _COUNT )
		m_data[i++] = 0;
}

// 从字符串构造CGUINT
template< class _TYPE, int _COUNT >
CGUINT<_TYPE,_COUNT>::CGUINT( const char* str )
{
	IntFromStr( *this, str );
}

// 将CGUINT转换为unsigned int
// 由于在进行逻辑判断或计算时,如果类型不一致,VC6会将自动CGUINT转换为unsigned int
// 这会抛弃CGUINT的高端部分,导致一些难以发现错误,因此只有在所有代码测试以后
// 确实需要这个函数时才使其生效
template< class _TYPE, int _COUNT >
CGUINT<_TYPE,_COUNT>::operator unsigned int()
{
	unsigned int n, i, k;
	_TYPE *dest = (_TYPE*)&n;

	// 必须先初始化,否则当CGUINT比unsigned int小时出现错误
	n = 0;

	// 特殊情形,CGUINT比unsigned int小
	k = sizeof(unsigned int) < _SIZE ? sizeof(unsigned int) : _SIZE;

	for( i=0; i<k/sizeof(_TYPE); i++ )
		dest[i] = m_data[i];

	return n;
}

// 赋值函数,由unsigned int创建CGUINT
template< class _TYPE, int _COUNT >
CGUINT<_TYPE,_COUNT> CGUINT<_TYPE,_COUNT>::operator=( unsigned int n )
{
	*this = CGUINT<_TYPE,_COUNT>( n );
	return *this;
}

//-----------------------------------------------------------------------------
//                           +,-,*
//-----------------------------------------------------------------------------

// CGUINT + CGUINT
template< class _TYPE, int _COUNT >
CGUINT<_TYPE,_COUNT> operator+( const CGUINT<_TYPE,_COUNT>& a, const CGUINT<_TYPE,_COUNT>& b )
{
	CGUINT<_TYPE,_COUNT> c;	// 构造函数已经隐含初始化为0
	int				i, m, n, k;
	unsigned long	t = 0;

	// 获取最高的非0位
	m = n = _COUNT - 1;
	while( a.m_data[m] == 0 && m > 0 ) m--; 
	while( b.m_data[n] == 0 && n > 0 ) n--; 
	k = m > n ? m : n;

	for( i=0; i<=k; i++ )
	{
		t			= a.m_data[i] + b.m_data[i] + ( t >> _TYPE_BITS );
		c.m_data[i]	= (_TYPE)( t & _TYPE_BITS_MASK );
	}

	// 设置进位
	if( ( t != 0 ) && k < ( _COUNT - 1 ) )
	{
		c.m_data[k+1] = (_TYPE)( t >> _TYPE_BITS );
		t = 0;
	}

	//  超出范围
	if( t != 0 )
		Exception( ERROR_ADD_CARRAY, 0 );

	return c;
}

// CGUINT - CGUINT
template< class _TYPE, int _COUNT >
CGUINT<_TYPE,_COUNT> operator-( const CGUINT<_TYPE,_COUNT>& a, const CGUINT<_TYPE,_COUNT>& b )
{
	CGUINT<_TYPE,_COUNT> c;		// 构造函数已经隐含初始化为0
	int		i, m, n, k;
	long	t, borrow = 0;	// 由于需要判别借位,t不能与+一样是unsigned类型

	m = n = _COUNT - 1;
	while( a.m_data[m] == 0 && m > 0 ) m--; 
	while( b.m_data[n] == 0 && n > 0 ) n--; 
	k = m > n ? m : n;

	for( i=0; i<=k; i++ )
	{
		t = (long)a.m_data[i] - (long)b.m_data[i] - borrow;

		if( t < 0 )
		{
//			c.m_data[i] =  _TYPE_BITS_MASK - b.m_data[i];
//			c.m_data[i]+= a.m_data[i] + 1 - (_TYPE)borrow;

			c.m_data[i] = (_TYPE)( t + ( ( (long)1 ) << _TYPE_BITS ) );
			borrow      = 1;
		}
		else
		{
//			c.m_data[i] = a.m_data[i] - b.m_data[i] - (_TYPE)borrow;

			c.m_data[i] = (_TYPE)t;
			borrow      = 0;
		}
	}

	// 扩展借位
	while( borrow && k < _COUNT - 1 )
	{
		k++;
		c.m_data[k] = _TYPE_BITS_MASK;
	}

	//  超出范围
	if( borrow != 0 )
		Exception( ERROR_SUB_BORROW, 0 );

	return c;
}

/*
// 利用补码简化的减法
template< class _TYPE, int _COUNT >
CGUINT<_TYPE,_COUNT> operator-( const CGUINT<_TYPE,_COUNT>& a, const CGUINT<_TYPE,_COUNT>& b )
{
	CGUINT<_TYPE,_COUNT> c, One(1);
	c = ~b + One;
	return a + c;
}
*/


// CGUINT * CGUINT
template< class _TYPE, int _COUNT >
CGUINT<_TYPE,_COUNT> operator*( const CGUINT<_TYPE,_COUNT>& a, const CGUINT<_TYPE,_COUNT>& b )
{
	CGUINT<_TYPE,_COUNT> cd[2], Zero;
	_TYPE *pcd;

	unsigned long t, carry;
	int i, j, m, n;

	// 初始化操作指针
	pcd = (_TYPE*)&cd[0];

	// 确定a,b的有效数据[0,n]*[0,m]
	n = m = _COUNT - 1;
	while( a.m_data[m] == 0 && m > 0 ) m--;
	while( b.m_data[n] == 0 && n > 0 ) n--;

	for( i=0; i<=m; i++ )
	{
		carry = 0;
		for( j=0; j<=n; j++ )
		{
			t	     = pcd[i+j] + (unsigned long)a.m_data[i]*(unsigned long)b.m_data[j] + carry;
			pcd[i+j] = (_TYPE)( t & _TYPE_BITS_MASK );
			carry    = ( t >> _TYPE_BITS );
		}

		pcd[i+j] = (_TYPE)carry;
	}

	if( cd[1] != Zero )
		Exception( ERROR_MUL_OVERFLOW, 0 );
	
	return cd[0];
}

//-----------------------------------------------------------------------------
//                           >>,<<
//-----------------------------------------------------------------------------
template< class _TYPE, int _COUNT >
CGUINT<_TYPE,_COUNT> operator>>( const CGUINT<_TYPE,_COUNT>& a, int bits )
{
	CGUINT<_TYPE,_COUNT> at, Zero;
	_TYPE mask, highbits, lowbits;
	int	i, k;

	at = a;

	// 对bits进行预处理
	if( bits < 0 )
	{
		bits  = -bits;
		return a << bits;
	}

	// 超过CGUINT大小
	if( bits >= _SIZE*8 )
		return Zero;

	//  小于CGUINT大小,但超过_TYPE大小
	while( bits >= _TYPE_BITS )
	{
		// 需要移动的单元个数
		k = bits/_TYPE_BITS;

		for( i=0; i<_COUNT-k; i++ )
			at.m_data[i] = at.m_data[i+k];

		while( i < _COUNT )
			at.m_data[i++] = 0;

		bits  -= _TYPE_BITS*k;
	}

	if( bits == 0 )
		return at;

	highbits = 0;
	lowbits  = 0;

	// 生成掩码
	mask = 1 << bits;
	mask --;

	for( i=_COUNT-1; i>=0; i-- )
	{
		lowbits        = at.m_data[i] & mask;
		at.m_data[i] >>= bits;
		at.m_data[i]  |= highbits;

		highbits	   = lowbits << ( _TYPE_BITS - bits );
	}

	return at;
}

template< class _TYPE, int _COUNT >
CGUINT<_TYPE,_COUNT> operator<<( const CGUINT<_TYPE,_COUNT>& a, int bits )
{
	CGUINT<_TYPE,_COUNT> at, Zero;
	_TYPE mask, highbits, lowbits;
	int i, k;

	at = a;

	if( bits < 0 )
	{
		bits  = -bits;
		return a >> bits;
	}

	if( bits >= _SIZE*8 )
		return Zero;

	//  小于CGUINT大小,但超过_TYPE大小
	while( bits >= _TYPE_BITS )
	{
		// 需要移动的单元个数
		k = bits/_TYPE_BITS;

		for( i=_COUNT-1; i>=k; i-- )
			at.m_data[i] = at.m_data[i-k];

		while( i >= 0 )
			at.m_data[i--] = 0;

		bits  -= _TYPE_BITS*k;
	}

	if( bits == 0 )
		return at;

	highbits = 0;
	lowbits  = 0;

	// 生成掩码
	mask = 1 << (_TYPE_BITS-bits);
	mask --;
	mask = ~mask;

	for( i=0; i<_COUNT; i++ )
	{
		highbits       = at.m_data[i] & mask;
		at.m_data[i] <<= bits;
		at.m_data[i]  |= lowbits;

		lowbits		   = highbits >> (_TYPE_BITS-bits);
	}

	return at;
}

//-----------------------------------------------------------------------------
//                           /,%
//-----------------------------------------------------------------------------

// 带余除法内核函数,这个函数要求最高不为0的单元的大小不小于B/2,参见带余除法算法
template< class _TYPE, int _COUNT >
CGUINT<_TYPE,_COUNT> DivKernel( const CGUINT<_TYPE,_COUNT>& a, const CGUINT<_TYPE,_COUNT>& b )
{
	CGUINT<_TYPE,_COUNT> c, q, One( 1 ), p, d;
	int n, k, m;
	unsigned long r;

	// 寻找不为0的最高单元
	for( n=_COUNT-1; n>=0; n-- )
	{
		if( a.m_data[n] != 0 )
			break;
	}

	for( m=_COUNT-1; m>=0; m-- )
	{
		if( b.m_data[m] != 0 )
			break;
	}

	c = a;
	Shift_Multiple_M( d, c, 1, (m+_COUNT-n)*_TYPE_BITS );

	// 需要循环的次数
	k = n - m;
	
	while( k >= 0 )
	{
		r = (unsigned long)d.m_data[m];

		// 最高不为0的单元可能就是最高单元,此时没有两位进行估值,或者说高位是0
		if( m < _COUNT-1 )
			r |= ( ( (unsigned long)d.m_data[m+1] ) << _TYPE_BITS );

		// 估值与修正
		q = unsigned int( r/(unsigned long)b.m_data[m] );
		while( d < b*q )
			q = q - One;

		// 合并到商
		p = p << int( _TYPE_BITS );
		p = p | q;
		d = d - b*q;

		// 下一次计算
		Shift_Multiple_M( d, c, 1, _TYPE_BITS );
		k--;
	}

	return p;
}

// CGUINT / CGUINT
template< class _TYPE, int _COUNT >
CGUINT<_TYPE,_COUNT> operator/( const CGUINT<_TYPE,_COUNT>& a, const CGUINT<_TYPE,_COUNT>& b )
{
	_TYPE bn;
	CGUINT<_TYPE,_COUNT> One( 1 ), Zero, c, p1, d, p2;
	int k, n, lc, lb;

	// [1]处理特殊情形
	if( b == Zero )
	{
		Exception( ERROR_DIV_BYZERO, 0 );
		return Zero;
	}

	if( a < b )
	{
		if( a != Zero )
			Exception( ERROR_DIV_PRECISION, 0 );
		return Zero;
	}

	if( a == b )
		return One;

	if( b == One )
		return a;

	// [2]带余除法计算高位部分

	// 寻找最高不为0的单元
	for( k=_COUNT-1; k>=0; k-- )
	{
		if( b.m_data[k] != 0 )
			break;
	}

	// 计算放大因子
	n  = 0;
	bn = b.m_data[k];
	while( bn <= ( _TYPE_BITS_MASK >> 1 ) ) // B/2-1
	{
		bn <<= 1;
		n++;
	}

	// 放大被除数
	c  = b;
	if( n > 0 )
		c = c << n;

	// 利用带余除法内河函数计算商的高位(>n)部分
	p1  = DivKernel( a, c );
	if( n > 0 )
		p1  = ( p1 << n );

	// [2]由于移位可能有n位还没有结果,使用二进制长除法计算低位部分[0,n-1]
	c  = a - b*p1;
	lc = GetHighestBit( c );
	lb = GetHighestBit( b );

	// 高位对齐
	Shift_Multiple_M( d, c, 1, _SIZE*8 + lb - lc );

	// 循环次数,n是高位对齐后或运算中,被除数剩下的位数
	n = lc - lb;
	while( n >= 0 )
	{
		p2 = p2 << 1;
		if( d >= b )
		{
			p2 = p2 | One;
			d  = d - b;
		}
		Shift_Multiple( d, c, 1 );	
		n--;
	}

	if( d != Zero )
		Exception( ERROR_DIV_PRECISION, 0 );

	return p1 | p2;	// 也可以是是+,不过|效率高些
}

// CGUINT / CGUINT
template< class _TYPE, int _COUNT >
CGUINT<_TYPE,_COUNT> operator%( const CGUINT<_TYPE,_COUNT>& a, const CGUINT<_TYPE,_COUNT>& b )
{
	return a - b*( a/b );
}

// 除法辅助函数,得到不为0的最高位,1起始,0表示没有找到
/*
template< class _TYPE, int _COUNT >
int GetHighestBit( const CGUINT<_TYPE,_COUNT>& a )
{
	int nCount;
	CGUINT<_TYPE,_COUNT> One( 1 ), Zero, at;

	at  = a;

	nCount = _SIZE*8;
	while( nCount > 0 )
	{
		if( at.m_data[_COUNT-1] & _TYPE_HIGHT_BIT )
			return nCount;

		nCount--;
		
		at = at << 1;
	}

	return nCount;
}
*/

// 下面是经过优化的代码,实际上还可以通过BT指令进一步优化
int ByteOrder[16] = 
{
	0, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4
};

template< class _TYPE, int _COUNT >
int GetHighestBit( const CGUINT<_TYPE,_COUNT>& a )
{
	unsigned char *pd = (unsigned char*)&a;

	int nHighBit = 0;
	for( int i=_SIZE-1; i>=0; i-- )
	{
		nHighBit += ByteOrder[ ( pd[i] & 0xF0 )>>4 ];
		if( nHighBit >= 1 )
		{
			nHighBit += 4;
			break;
		}

		nHighBit += ByteOrder[pd[i] & 0x0F];
		if( nHighBit >= 1 )
			break;
	}

	if( nHighBit == 0 )
		return -1;

	nHighBit += i*8;

	return nHighBit;
}

// 联合移位1位函数
template< class _TYPE, int _COUNT >
void Shift_Multiple( CGUINT<_TYPE,_COUNT>& a, CGUINT<_TYPE,_COUNT>& b, int bLeft )
{
	if( bLeft )
	{
		a = a << 1;
		if( b.m_data[_COUNT-1] & (_TYPE)_TYPE_HIGHT_BIT )
			a.m_data[0] |= 1;
		b = b << 1;
	}
	else
	{
		b = b >> 1;
		if( a.m_data[0] & 1 )
			b.m_data[_COUNT-1] |= (_TYPE)_TYPE_HIGHT_BIT;
		a = a >> 1;
	}
}

// 联合移位多位函数
template< class _TYPE, int _COUNT >
void Shift_Multiple_M( CGUINT<_TYPE,_COUNT>& a, CGUINT<_TYPE,_COUNT>& b, int bLeft, int bits )
{
	CGUINT<_TYPE,_COUNT> mask, value, One( 1 );

	// 预处理bits
	if( bits < 0 )
	{
		bits  = -bits;
		bLeft = !bLeft;
	}

	// 超过双倍长度,移没了!
	if( bits >= ( _SIZE*8 << 2 ) )
	{
		a = b = 0;
		return;
	}

	// 移动超过整个CGUINT
	if( bits >= _SIZE*8 )
	{
		bits -= _SIZE*8;

		if( bLeft )
		{
			a = b;
			b = 0;

			if( bits > 0 )
				a = ( a << bits );
		}
		else
		{
			b = a;
			a = 0;

			if( bits > 0 )
				b = ( b >> bits );
		}
		return;
	}

	if( bits == 0 )
		return;

	mask = One;
	mask = mask << int( _SIZE*8 - bits );
	mask = mask - One;

	if( bLeft )
	{
		mask  = ~mask;
		value = b & mask;
		value = value >> int( _SIZE*8 - bits );

		a = a << bits;
		a = a | value;
		b = b << bits;
	}
	else
	{
		value = a & mask;
		value = value << int( _SIZE*8 - bits );

		b = b >> bits;
		b = b | value;
		a = a >> bits;
	}
}


//-----------------------------------------------------------------------------
//                           &,^,|,~
//-----------------------------------------------------------------------------

template< class _TYPE, int _COUNT >
CGUINT<_TYPE,_COUNT> operator&( const CGUINT<_TYPE,_COUNT>& a, const CGUINT<_TYPE,_COUNT>& b )
{
	CGUINT<_TYPE,_COUNT> c;

	for( unsigned int i=0; i<_COUNT; i++ )
		c.m_data[i] = a.m_data[i] & b.m_data[i];
	return c;
}

template< class _TYPE, int _COUNT >
CGUINT<_TYPE,_COUNT> operator|( const CGUINT<_TYPE,_COUNT>& a, const CGUINT<_TYPE,_COUNT>& b )
{
	CGUINT<_TYPE,_COUNT> c;

	for( int i=0; i<_COUNT; i++ )
		c.m_data[i] = a.m_data[i] | b.m_data[i];
	return c;
}


template< class _TYPE, int _COUNT >
CGUINT<_TYPE,_COUNT> operator^( const CGUINT<_TYPE,_COUNT>& a, const CGUINT<_TYPE,_COUNT>& b )
{
	CGUINT<_TYPE,_COUNT> c;

	for( int i=0; i<_COUNT; i++ )
		c.m_data[i] = a.m_data[i] ^ b.m_data[i];
	return c;
}

template< class _TYPE, int _COUNT >
CGUINT<_TYPE,_COUNT> operator~( const CGUINT<_TYPE,_COUNT>& b )
{
	CGUINT<_TYPE,_COUNT> c;

	for( int i=0; i<_COUNT; i++ )
		c.m_data[i] = ~b.m_data[i];
	return c;
}

//-----------------------------------------------------------------------------
//                           >,>=,<,<=,==,!=,!
//-----------------------------------------------------------------------------
template< class _TYPE, int _COUNT >
int operator>( const CGUINT<_TYPE,_COUNT>& a, const CGUINT<_TYPE,_COUNT>& b )
{
	for( int i=_COUNT-1; i>=0; i-- )
	{
		if( a.m_data[i] > b.m_data[i] )
			return 1;
		else if( a.m_data[i] < b.m_data[i] )
			return 0;
	}
	return 0;
}

template< class _TYPE, int _COUNT >
int operator<( const CGUINT<_TYPE,_COUNT>& a, const CGUINT<_TYPE,_COUNT>& b )
{
	for( int i=_COUNT-1; i>=0; i-- )
	{
		if( a.m_data[i] < b.m_data[i] )
			return 1;
		if( a.m_data[i] > b.m_data[i] )
			return 0;
	}
	return 0;
}

template< class _TYPE, int _COUNT >
int operator==( const CGUINT<_TYPE,_COUNT>& a, const CGUINT<_TYPE,_COUNT>& b )
{
	for( int i=_COUNT-1; i>=0; i-- )
	{
		if( a.m_data[i] != b.m_data[i] )
			return 0;
	}
	return 1;
}

template< class _TYPE, int _COUNT >
int operator>=( const CGUINT<_TYPE,_COUNT>& a, const CGUINT<_TYPE,_COUNT>& b )
{
	return ( a > b ) || ( a == b );
}

template< class _TYPE, int _COUNT >
int operator<=( const CGUINT<_TYPE,_COUNT>& a, const CGUINT<_TYPE,_COUNT>& b )
{
	return ( a < b ) || ( a == b );	
}


template< class _TYPE, int _COUNT >
int operator!=( const CGUINT<_TYPE,_COUNT>& a, const CGUINT<_TYPE,_COUNT>& b )
{
	return !( a == b );
}

template< class _TYPE, int _COUNT >
int operator!( CGUINT<_TYPE,_COUNT> a )
{
	CGUINT<_TYPE,_COUNT> Zero;

	if( a != Zero )
		return 0;
	else
		return 1;
}


//-----------------------------------------------------------------------------
//                           IntToStr,IntFromStr
//-----------------------------------------------------------------------------

// pCnt返回输出的字符数
// digits是要求输出长度
template< class _TYPE, int _COUNT >
const char* IntToStr( const CGUINT<_TYPE,_COUNT>& n, int base = 10, char prefix = 0, 
					  int digits = 0, int bCase = 1, int* pCnt = 0 )
{
	// 最大值每位一个字符
	static char str[_SIZE*8+32];

	char temp[_SIZE*8+32];
	int i, j, k, q;
	CGUINT<_TYPE,_COUNT> CBase, nt, Zero;
	
	nt	  = n;	
	CBase = base;

	// 转换字符
	k = 0;
	while( nt > Zero )
	{
		q  = nt%CBase;
		nt = nt/CBase;

		if( q < 10 )
			temp[k] = q + '0';
		else
		{
			if( bCase )
				temp[k] = q - 10 + 'A';
			else
				temp[k] = q - 10 + 'a';
		}
		k ++;
	}

	// nt == 0
	if( k == 0 )
		temp[k++] = '0';

	temp[k] = '\0';

	// 格式化
	i = 0;
	if( base == 8 )
	{
		str[0] = '0';
		i++;
	}
	else if( base == 16 )
	{
		str[0] = '0';
		str[1] = bCase ? 'X' : 'x';
		i += 2;
	}

	if( digits-k > 0 && prefix != '\0' )
	{
		for( j=0; j<digits-k; j++ )
			str[i++] = prefix;
	}

	do{
		k--;
		str[i] =  temp[k];
		i++;
	}while( k >= 0 );

	str[i] = '\0';

	if( pCnt != 0 )
		*pCnt = i;

	return str;
}

// 返回处理过的字符数
template< class _TYPE, int _COUNT >
int IntFromStr( CGUINT<_TYPE,_COUNT>& n, const char* str = 0 )
{
	CGUINT<_TYPE,_COUNT> k, CBase, One(1), Zero;
	int base, i, sign, stop;

	// 初始化
	n = Zero;

	if( str == 0 )
		return 0;

	i = 0;

	// 跳过非法字符
	while( str[i] != '+' && str[i] != '-' && ( str[i] > '9' || str[i] < '0' ) )
		i++;

	// 处理符号
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

	// 处理进制
	if( str[i] == '0' )
	{
		base = 8;
		i++;

		if( str[i] == 'x' || str[i] == 'X' )
		{
			base = 16;
			i++;
		}
	}
	else
		base = 10;

	CBase = base;
	
	// 处理值
	stop = 0;
	while( str[i] != '\0' )
	{
		switch( base )
		{
		case 10:
			if( str[i] >= '0' && str[i] <= '9' )
				k = str[i] - '0';
			else
				stop = 1;
			break;

		case 16:
			if( str[i] >= '0' && str[i] <= '9' )
				k = str[i] - '0';
			else if( str[i] >= 'A' && str[i] <= 'F' )
				k = str[i] - 'A' + 10;
			else if( str[i] >= 'a' && str[i] <= 'f' )
				k = str[i] - 'a' + 10;
			else
				stop = 1;
			break;

		case 8:
			if( str[i] >= '0' && str[i] <= '7' )
				k = str[i] - '0';
			else
				stop = 1;
			break;

		default:
			stop = 1;
			break;
		}

		if( stop )
			break;

		n = n * CBase + k;
		i++;
	}

	if( sign )
		n = ~n + One;

	// 返回已处理的字符数
	return i;
}

#endif