
#include<cmath>
#include<cstdlib>
#include<fstream>
#include<ctime>
#include<iomanip>
#include<cctype>
//using namespace std;
#include"HugeNumber.hpp"

void AddPositive(const HugeNumber& lhg, const HugeNumber& rhg, HugeNumber& hg)
{

    hg.m_sign=HugeNumber::PLUS;
    int sft=lhg.m_scale-rhg.m_scale;
    if(sft>0){

        int offset=sft>>BITSHIFT;
        hg.m_scale=rhg.m_scale;
        hg.m_size=((lhg.m_size+offset)>rhg.m_size) ? lhg.m_size+offset: rhg.m_size;
    	hg.m_value=new uint32_t[hg.m_size];

        size_t j=0;
        size_t i=offset;
        int64_t tmp=0;
        tmp=lhg.m_value[i]+rhg.m_value[j];

        hg.m_value[j]=(uint32_t)tmp;

        for( ++i,++j; (i<lhg.m_size)&&(j<rhg.m_size); ++i,++j )
        {

            tmp=lhg.m_value[i]+rhg.m_value[j]+(tmp>>BITLENGTH);
            hg.m_value[j]=(uint32_t)tmp;
        }
        while(i<lhg.m_size){
            hg.m_value[j]=lhg.m_value[i];
            ++i;
        }
        while(j<rhg.m_size){
            hg.m_value[j]=rhg.m_value[j];
            ++j;
        }
        // overflow??
    	//LeftShift(lhg,hg,sft);
    }
    else if(sft<0){

        sft=-sft;

        int offset=sft>>BITSHIFT;
        hg.m_scale=lhg.m_scale;
        hg.m_size=(rhg.m_size+offset>lhg.m_size) ? rhg.m_size+offset: lhg.m_size;
    	hg.m_value=new uint32_t[hg.m_size];

        size_t j=0;
        size_t i=offset;
        int64_t tmp=0;
        tmp=rhg.m_value[i]+lhg.m_value[j];
        hg.m_value[j]=(uint32_t)tmp;

        for( ++i,++j; (i<rhg.m_size)&&(j<lhg.m_size); ++i,++j )
        {

            tmp=rhg.m_value[i]+lhg.m_value[j]+(tmp>>BITLENGTH);
            hg.m_value[j]=(uint32_t)tmp;
        }
        while(i<rhg.m_size){

            hg.m_value[j]=rhg.m_value[i];
            ++i;
        }
        while(j<lhg.m_size){

            hg.m_value[j]=lhg.m_value[j];
            ++j;
        }
        // overflow??
        //LeftShift(rhg,hg,sft);
    }

    hg.GetSize();
}
void SubtractPositive(const HugeNumber& lhg, const HugeNumber& rhg, HugeNumber& hg)
{

    int sft=lhg.m_scale-rhg.m_scale;

        size_t j=0;
        size_t i=0;
        int64_t tmp=0;
        int borrow=0;
        int offset=0;
    if(sft>0){

        offset=sft>>BITSHIFT;
        hg.m_scale=rhg.m_scale;
        hg.m_size=(lhg.m_size+offset>rhg.m_size) ? (lhg.m_size+offset): rhg.m_size;
    	hg.m_value=new uint32_t[hg.m_size];

        j=0;
        i=offset;
        tmp=0;
        borrow=0;
        tmp=lhg.m_value[i]-rhg.m_value[j];

		if( tmp < 0 ){
			hg.m_value[j]= (uint32_t)(tmp+(((int64_t)1)<<BITLENGTH));
			borrow = 1;
		}
		else{
            hg.m_value[j]= (uint32_t)(tmp);
			borrow = 0;
		}


        for( ++i,++j; (i<lhg.m_size)&&(j<rhg.m_size); ++i,++j )
        {

            tmp=lhg.m_value[i]-rhg.m_value[j]+borrow;
            if( tmp < 0 ) {
                hg.m_value[j]= (uint32_t)(tmp+(((int64_t)1)<<BITLENGTH));
                borrow = 1;
            }
            else{
                hg.m_value[j]= (uint32_t)(tmp);
                borrow = 0;
            }
        }
        while(i<lhg.m_size){
            hg.m_value[j]=(uint32_t)(lhg.m_value[i]-borrow);
            borrow = 0;
            ++i;
        }
        while(j<rhg.m_size){
            tmp=-rhg.m_value[j]-borrow;
            hg.m_value[j]= (uint32_t)(tmp+(((int64_t)1)<<BITLENGTH));
            borrow = 1;
            ++j;
        }
        // overflow??
    	//LeftShift(lhg,hg,sft);
    }
    else if(sft<0){

        sft=-sft;

        offset=sft>>BITSHIFT;
        hg.m_scale=lhg.m_scale;
        hg.m_size=(rhg.m_size+offset>lhg.m_size) ? (rhg.m_size+offset): lhg.m_size;
    	hg.m_value=new uint32_t[hg.m_size];

        j=0;
        i=offset;
        tmp=0;
        borrow=0;
        tmp=lhg.m_value[j]-rhg.m_value[i];
		if( tmp < 0 ){
			hg.m_value[j]= (uint32_t)(tmp+(((int64_t)1)<<BITLENGTH));
			borrow = 1;
		}
		else{
            hg.m_value[j]= (uint32_t)(tmp);
			borrow = 0;
		}
        for( ++i,++j; (i<rhg.m_size)&&(j<lhg.m_size); ++i,++j )
        {

            tmp=lhg.m_value[j]-rhg.m_value[i]-borrow;

            if( tmp < 0 ){
                hg.m_value[j]= (uint32_t)(tmp+(((int64_t)1)<<BITLENGTH));
                borrow = 1;
            }
            else{
                hg.m_value[j]= (uint32_t)(tmp);
                borrow = 0;
            }
        }
        while(i<rhg.m_size){

            tmp=-rhg.m_value[i]-borrow;
            hg.m_value[j]= (uint32_t)(tmp+(((int64_t)1)<<BITLENGTH));
            borrow = 1;
            ++i;
        }
        while(j<lhg.m_size){

            hg.m_value[j]= (uint32_t)(lhg.m_value[j]-borrow);
            borrow = 0;
            ++j;
        }
        // overflow??
        //LeftShift(rhg,hg,sft);
    }

    hg.GetSize();
    if(borrow==0)
        hg.m_sign=HugeNumber::PLUS;
    else //borrow==1
        hg.m_sign=HugeNumber::MINUS;
}
const HugeNumber operator+(const HugeNumber& lhg, const HugeNumber& rhg)
{
    HugeNumber hg;
    if(lhg.m_sign!=rhg.m_sign){
        SubtractPositive(lhg, rhg, hg);
        if (lhg.m_sign==HugeNumber::MINUS)
            -hg;
    }else {
        AddPositive(lhg, rhg, hg);
        hg.m_sign=lhg.m_sign;
    }
    return hg;
}

const HugeNumber operator-(const HugeNumber& lhg, const HugeNumber& rhg)
{
    HugeNumber hg;
    if(lhg.m_sign!=rhg.m_sign) {
        AddPositive(lhg, rhg, hg);
    }
    else {
        SubtractPositive(lhg, rhg, hg);
    }
    if (lhg.m_sign==HugeNumber::MINUS)
        -hg;
    return hg;
}

const HugeNumber operator*(const HugeNumber& lhg, const HugeNumber& rhg)
{
    HugeNumber hg;
    if(lhg.m_sign==rhg.m_sign)
        hg.m_sign=HugeNumber::PLUS;
    else
        hg.m_sign=HugeNumber::MINUS;
    hg.m_size=lhg.m_size+rhg.m_size;
    hg.m_scale=lhg.m_scale+rhg.m_scale;
    hg.m_value=new uint32_t[hg.m_size];


    for( size_t i=0; i<lhg.m_size;  ++i )
	{
		uint64_t carry = 0;
		size_t j=0;
		for( ; j<rhg.m_size; ++j )
		{
			uint64_t tmp = hg.m_value[i+j] + (uint64_t)lhg.m_value[i]*(uint64_t)rhg.m_value[j] + carry;
			hg.m_value[i+j] = (uint32_t)( tmp & BIT32MASK );
			carry = tmp >> BITLENGTH ;
		}

        hg.m_value[i+j] = (uint32_t)carry;
	}
	hg.GetSize();
    return hg;
}

const HugeNumber operator/(const HugeNumber& lhg, const HugeNumber& rhg)
{
    HugeNumber hg;
    hg.GetSize();
    return hg;
}

const HugeNumber div(const HugeNumber& lhg, const HugeNumber& rhg)
{
    uint32_t bn;
	HugeNumber One( 1 ), Zero, c, p1, d, p2;
	int k, n, lc, lb;

	// [1]������������
	if( rhg == Zero )
	{
		//Exception( ERROR_DIV_BYZERO, 0 );
		return Zero;
	}

	if( lhg < rhg )
	{
		if( lhg != Zero )
			//Exception( ERROR_DIV_PRECISION, 0 );
		return Zero;
	}

	if( lhg == rhg )
		return One;

	if( rhg == One )
		return lhg;

	// [2]������������λ����

	// Ѱ����߲�Ϊ0�ĵ�Ԫ
	for( k=rhg.m_size-1; k>=0; k-- )
	{
		if( rhg.m_value[k] != 0 )
			break;
	}

	// ����Ŵ�����
	n  = 0;
	bn = rhg.m_value[k];
	while( bn <= ( BIT32MASK >> 1 ) ) // B/2-1
	{
		bn <<= 1;
		n++;
	}

	// �Ŵ󱻳���
	c  = rhg;
	if( n > 0 )
		c = c << n;

	// ���ô�������ںӺ��������̵ĸ�λ(>n)����
	p1  = DivKernel( lhg, c );
	if( n > 0 )
		p1  = ( p1 << n );

	// [2]������λ������nλ��û�н��,ʹ�ö����Ƴ����������λ����[0,n-1]
	c  = lhg - rhg*p1;
	lc = GetHighestBit( c );
	lb = GetHighestBit( rhg );

	// ��λ����
	Shift_Multiple_M( d, c, 1, rhg.m_length*32 + lb - lc );

	// ѭ������,n�Ǹ�λ������������,������ʣ�µ�λ��
	n = lc - lb;
	while( n >= 0 )
	{
		p2 = p2 << 1;
		if( d >= rhg )
		{
			p2 = p2 | One;
			d  = d - rhg;
		}
		Shift_Multiple( d, c, 1 );
		n--;
	}

	if( d != Zero );
		//Exception( ERROR_DIV_PRECISION, 0 );

	return p1 | p2;	// Ҳ��������+,����|Ч�ʸ�Щ
}

// ������������,�õ���Ϊ0�����λ,1��ʼ,0��ʾû���ҵ�
/*

int GetHighestBit( const HugeNumber& lhg )
{
	int nCount;
	HugeNumber One( 1 ), Zero, at;

	at  = lhg;

	nCount = m_length*4;
	while( nCount > 0 )
	{
		if( at.m_value[m_size-1] & BIT32HIGHT )
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


int GetHighestBit( const HugeNumber& lhg )
{
	unsigned char *pd = (unsigned char*)&lhg;

	int nHighBit = 0;
	int i=lhg.m_length*4-1;
	for( ; i>=0; --i )
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

	nHighBit += i*8;//??

	return nHighBit;
}

// ������λ1λ����

void Shift_Multiple( HugeNumber& lhg, HugeNumber& rhg, int bLeft )
{
	if( bLeft )
	{
		lhg = lhg << 1;
		if( rhg.m_value[rhg.m_size-1] & BIT32HIGHT )
			lhg.m_value[0] |= 1;
		rhg = rhg << 1;
	}
	else
	{
		rhg = rhg >> 1;
		if( lhg.m_value[0] & 1 )
			rhg.m_value[rhg.m_size-1] |= BIT32HIGHT;
		lhg = lhg >> 1;
	}
}

// ������λ��λ����

void Shift_Multiple_M( HugeNumber& lhg, HugeNumber& rhg, int bLeft, int bits )
{
	HugeNumber mask, value, One( 1 );

	// Ԥ����bits
	if( bits < 0 )
	{
		bits  = -bits;
		bLeft = !bLeft;
	}

	// ����˫������,��û��!
	if( bits >= ( rhg.m_length*32 << 2 ) )
	{
		lhg = rhg = uint32_t(0);
		return;
	}

	// �ƶ���������CGUINT
	if( bits >= rhg.m_length*32 )
	{
		bits -= rhg.m_length*32;

		if( bLeft )
		{
			lhg = rhg;
			rhg = uint32_t(0);

			if( bits > 0 )
				lhg = ( lhg << bits );
		}
		else
		{
			rhg = lhg;
			lhg = uint32_t(0);

			if( bits > 0 )
				rhg = ( rhg >> bits );
		}
		return;
	}

	if( bits == 0 )
		return;

	mask = One;
	mask = mask << int( rhg.m_length*32 - bits );
	mask = mask - One;

	if( bLeft )
	{
		mask  = ~mask;
		value = rhg & mask;
		value = value >> int( rhg.m_length*32 - bits );

		lhg = lhg << bits;
		lhg = lhg | value;
		rhg = rhg << bits;
	}
	else
	{
		value = lhg & mask;
		value = value << int( rhg.m_length*32 - bits );

		rhg = rhg >> bits;
		rhg = rhg | value;
		lhg = lhg >> bits;
	}
}

HugeNumber DivKernel( const HugeNumber& lhg, const HugeNumber& rhg )
{
	HugeNumber c, q, One( 1 ), p, d;
	int n, k, m;
	uint64_t r;

	// Ѱ�Ҳ�Ϊ0����ߵ�Ԫ
	for( n=lhg.m_size-1; n>=0; n-- )
	{
		if( lhg.m_value[n] != 0 )
			break;
	}

	for( m=rhg.m_size-1; m>=0; m-- )
	{
		if( rhg.m_value[m] != 0 )
			break;
	}

	c = lhg;
	Shift_Multiple_M( d, c, 1, (m+rhg.m_size-n)*BITLENGTH );

	// ��Ҫѭ���Ĵ���
	k = n - m;

	while( k >= 0 )
	{
		r = (uint64_t)d.m_value[m];

		// ��߲�Ϊ0�ĵ�Ԫ���ܾ�����ߵ�Ԫ,��ʱû����λ���й�ֵ,����˵��λ��0
		if( m < rhg.m_size-1 )
			r |= ( ( (uint64_t)d.m_value[m+1] ) << BITLENGTH );

		// ��ֵ������
		q = (unsigned int)( r/(uint64_t)rhg.m_value[m] );
		while( d < rhg*q )
			q = q - One;

		// �ϲ�����
		p = p << int( BITLENGTH );
		p = p | q;
		d = d - rhg*q;

		// ��һ�μ���
		Shift_Multiple_M( d, c, 1, BITLENGTH );
		k--;
	}

	return p;
}
