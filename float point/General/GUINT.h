// ��ģ���ǵ�3��<ͨ����������>ʵ�ֵ�һ����,֧�����ⳤ�ȵ��޷�������
// ��Ȫ���Ƿ������ļ�����  ������  2007-7

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
	public:	// ���캯��
		CGUINT();
		CGUINT( unsigned int n );
		CGUINT( const char *str );

	public:	// ����int�ĺ���
		CGUINT operator=( unsigned int n );

		// �������̫Σ��,������ɾ��,������ɺ���Ч
		operator unsigned int();

	public:	// �Ѻ���,���ز�����+,-,*,/,%��>>,<<,&,|,^,~
		// ��������
		friend CGUINT operator+( const CGUINT& a, const CGUINT& b );
		friend CGUINT operator-( const CGUINT& a, const CGUINT& b );
		friend CGUINT operator*( const CGUINT& a, const CGUINT& b );
		friend CGUINT operator/( const CGUINT& a, const CGUINT& b );
		friend CGUINT operator%( const CGUINT& a, const CGUINT& b );

		// ��λ����
		friend CGUINT operator>>( const CGUINT& a, int bits );
		friend CGUINT operator<<( const CGUINT& a, int bits );
		
		// λ����
		friend CGUINT operator&( const CGUINT& a, const CGUINT& b );
		friend CGUINT operator|( const CGUINT& a, const CGUINT& b );
		friend CGUINT operator^( const CGUINT& a, const CGUINT& b );
		friend CGUINT operator~( const CGUINT& b );

		// �߼�����
		friend int operator> ( const CGUINT& a, const CGUINT& b );
		friend int operator>=( const CGUINT& a, const CGUINT& b );
		friend int operator< ( const CGUINT& a, const CGUINT& b );
		friend int operator<=( const CGUINT& a, const CGUINT& b );
		friend int operator==( const CGUINT& a, const CGUINT& b );
		friend int operator!=( const CGUINT& a, const CGUINT& b );
		friend int operator! ( const CGUINT& b );

		// ������������
		friend void Shift_Multiple( CGUINT& a, CGUINT& b, int bLeft );
		friend void Shift_Multiple_M( CGUINT& a, CGUINT& b, int bLeft, int bits );
		friend CGUINT DivKernel( const CGUINT& a, const CGUINT& b );
		friend int GetHighestBit( const CGUINT& a );

	public:// ����
		_TYPE m_data[_COUNT];
};

//-----------------------------------------------------------------------------
//                           ���캯����һЩ���⺯��
//-----------------------------------------------------------------------------

// ȱʡ���캯��
template< class _TYPE, int _COUNT >
CGUINT<_TYPE,_COUNT>::CGUINT()
{
	for( int i=0; i<_COUNT; i++ )
		m_data[i] = 0;
}

// ��unsigned int����CGUINT
template< class _TYPE, int _COUNT >
CGUINT<_TYPE,_COUNT>::CGUINT( unsigned int n )
{
	unsigned int i, k;
	_TYPE *src  = (_TYPE*)&n;

	// ��������,CGUINT��unsigned int��С
	k = sizeof(unsigned int) < _SIZE ? sizeof(unsigned int) : _SIZE;

	for( i=0; i<k/sizeof(_TYPE); i++ )
		m_data[i] = src[i];

	// CGUINT��unsigned int��ʱ
	while( i < _COUNT )
		m_data[i++] = 0;
}

// ���ַ�������CGUINT
template< class _TYPE, int _COUNT >
CGUINT<_TYPE,_COUNT>::CGUINT( const char* str )
{
	IntFromStr( *this, str );
}

// ��CGUINTת��Ϊunsigned int
// �����ڽ����߼��жϻ����ʱ,������Ͳ�һ��,VC6�Ὣ�Զ�CGUINTת��Ϊunsigned int
// �������CGUINT�ĸ߶˲���,����һЩ���Է��ִ���,���ֻ�������д�������Ժ�
// ȷʵ��Ҫ�������ʱ��ʹ����Ч
template< class _TYPE, int _COUNT >
CGUINT<_TYPE,_COUNT>::operator unsigned int()
{
	unsigned int n, i, k;
	_TYPE *dest = (_TYPE*)&n;

	// �����ȳ�ʼ��,����CGUINT��unsigned intСʱ���ִ���
	n = 0;

	// ��������,CGUINT��unsigned intС
	k = sizeof(unsigned int) < _SIZE ? sizeof(unsigned int) : _SIZE;

	for( i=0; i<k/sizeof(_TYPE); i++ )
		dest[i] = m_data[i];

	return n;
}

// ��ֵ����,��unsigned int����CGUINT
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
	CGUINT<_TYPE,_COUNT> c;	// ���캯���Ѿ�������ʼ��Ϊ0
	int				i, m, n, k;
	unsigned long	t = 0;

	// ��ȡ��ߵķ�0λ
	m = n = _COUNT - 1;
	while( a.m_data[m] == 0 && m > 0 ) m--; 
	while( b.m_data[n] == 0 && n > 0 ) n--; 
	k = m > n ? m : n;

	for( i=0; i<=k; i++ )
	{
		t			= a.m_data[i] + b.m_data[i] + ( t >> _TYPE_BITS );
		c.m_data[i]	= (_TYPE)( t & _TYPE_BITS_MASK );
	}

	// ���ý�λ
	if( ( t != 0 ) && k < ( _COUNT - 1 ) )
	{
		c.m_data[k+1] = (_TYPE)( t >> _TYPE_BITS );
		t = 0;
	}

	//  ������Χ
	if( t != 0 )
		Exception( ERROR_ADD_CARRAY, 0 );

	return c;
}

// CGUINT - CGUINT
template< class _TYPE, int _COUNT >
CGUINT<_TYPE,_COUNT> operator-( const CGUINT<_TYPE,_COUNT>& a, const CGUINT<_TYPE,_COUNT>& b )
{
	CGUINT<_TYPE,_COUNT> c;		// ���캯���Ѿ�������ʼ��Ϊ0
	int		i, m, n, k;
	long	t, borrow = 0;	// ������Ҫ�б��λ,t������+һ����unsigned����

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

	// ��չ��λ
	while( borrow && k < _COUNT - 1 )
	{
		k++;
		c.m_data[k] = _TYPE_BITS_MASK;
	}

	//  ������Χ
	if( borrow != 0 )
		Exception( ERROR_SUB_BORROW, 0 );

	return c;
}

/*
// ���ò���򻯵ļ���
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

	// ��ʼ������ָ��
	pcd = (_TYPE*)&cd[0];

	// ȷ��a,b����Ч����[0,n]*[0,m]
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

	// ��bits����Ԥ����
	if( bits < 0 )
	{
		bits  = -bits;
		return a << bits;
	}

	// ����CGUINT��С
	if( bits >= _SIZE*8 )
		return Zero;

	//  С��CGUINT��С,������_TYPE��С
	while( bits >= _TYPE_BITS )
	{
		// ��Ҫ�ƶ��ĵ�Ԫ����
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

	// ��������
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

	//  С��CGUINT��С,������_TYPE��С
	while( bits >= _TYPE_BITS )
	{
		// ��Ҫ�ƶ��ĵ�Ԫ����
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

	// ��������
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

// ��������ں˺���,�������Ҫ����߲�Ϊ0�ĵ�Ԫ�Ĵ�С��С��B/2,�μ���������㷨
template< class _TYPE, int _COUNT >
CGUINT<_TYPE,_COUNT> DivKernel( const CGUINT<_TYPE,_COUNT>& a, const CGUINT<_TYPE,_COUNT>& b )
{
	CGUINT<_TYPE,_COUNT> c, q, One( 1 ), p, d;
	int n, k, m;
	unsigned long r;

	// Ѱ�Ҳ�Ϊ0����ߵ�Ԫ
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

	// ��Ҫѭ���Ĵ���
	k = n - m;
	
	while( k >= 0 )
	{
		r = (unsigned long)d.m_data[m];

		// ��߲�Ϊ0�ĵ�Ԫ���ܾ�����ߵ�Ԫ,��ʱû����λ���й�ֵ,����˵��λ��0
		if( m < _COUNT-1 )
			r |= ( ( (unsigned long)d.m_data[m+1] ) << _TYPE_BITS );

		// ��ֵ������
		q = unsigned int( r/(unsigned long)b.m_data[m] );
		while( d < b*q )
			q = q - One;

		// �ϲ�����
		p = p << int( _TYPE_BITS );
		p = p | q;
		d = d - b*q;

		// ��һ�μ���
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

	// [1]������������
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

	// [2]������������λ����

	// Ѱ����߲�Ϊ0�ĵ�Ԫ
	for( k=_COUNT-1; k>=0; k-- )
	{
		if( b.m_data[k] != 0 )
			break;
	}

	// ����Ŵ�����
	n  = 0;
	bn = b.m_data[k];
	while( bn <= ( _TYPE_BITS_MASK >> 1 ) ) // B/2-1
	{
		bn <<= 1;
		n++;
	}

	// �Ŵ󱻳���
	c  = b;
	if( n > 0 )
		c = c << n;

	// ���ô�������ںӺ��������̵ĸ�λ(>n)����
	p1  = DivKernel( a, c );
	if( n > 0 )
		p1  = ( p1 << n );

	// [2]������λ������nλ��û�н��,ʹ�ö����Ƴ����������λ����[0,n-1]
	c  = a - b*p1;
	lc = GetHighestBit( c );
	lb = GetHighestBit( b );

	// ��λ����
	Shift_Multiple_M( d, c, 1, _SIZE*8 + lb - lc );

	// ѭ������,n�Ǹ�λ������������,������ʣ�µ�λ��
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

	return p1 | p2;	// Ҳ��������+,����|Ч�ʸ�Щ
}

// CGUINT / CGUINT
template< class _TYPE, int _COUNT >
CGUINT<_TYPE,_COUNT> operator%( const CGUINT<_TYPE,_COUNT>& a, const CGUINT<_TYPE,_COUNT>& b )
{
	return a - b*( a/b );
}

// ������������,�õ���Ϊ0�����λ,1��ʼ,0��ʾû���ҵ�
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

// �����Ǿ����Ż��Ĵ���,ʵ���ϻ�����ͨ��BTָ���һ���Ż�
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

// ������λ1λ����
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

// ������λ��λ����
template< class _TYPE, int _COUNT >
void Shift_Multiple_M( CGUINT<_TYPE,_COUNT>& a, CGUINT<_TYPE,_COUNT>& b, int bLeft, int bits )
{
	CGUINT<_TYPE,_COUNT> mask, value, One( 1 );

	// Ԥ����bits
	if( bits < 0 )
	{
		bits  = -bits;
		bLeft = !bLeft;
	}

	// ����˫������,��û��!
	if( bits >= ( _SIZE*8 << 2 ) )
	{
		a = b = 0;
		return;
	}

	// �ƶ���������CGUINT
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

// pCnt����������ַ���
// digits��Ҫ���������
template< class _TYPE, int _COUNT >
const char* IntToStr( const CGUINT<_TYPE,_COUNT>& n, int base = 10, char prefix = 0, 
					  int digits = 0, int bCase = 1, int* pCnt = 0 )
{
	// ���ֵÿλһ���ַ�
	static char str[_SIZE*8+32];

	char temp[_SIZE*8+32];
	int i, j, k, q;
	CGUINT<_TYPE,_COUNT> CBase, nt, Zero;
	
	nt	  = n;	
	CBase = base;

	// ת���ַ�
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

	// ��ʽ��
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

// ���ش�������ַ���
template< class _TYPE, int _COUNT >
int IntFromStr( CGUINT<_TYPE,_COUNT>& n, const char* str = 0 )
{
	CGUINT<_TYPE,_COUNT> k, CBase, One(1), Zero;
	int base, i, sign, stop;

	// ��ʼ��
	n = Zero;

	if( str == 0 )
		return 0;

	i = 0;

	// �����Ƿ��ַ�
	while( str[i] != '+' && str[i] != '-' && ( str[i] > '9' || str[i] < '0' ) )
		i++;

	// �������
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

	// �������
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
	
	// ����ֵ
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

	// �����Ѵ�����ַ���
	return i;
}

#endif