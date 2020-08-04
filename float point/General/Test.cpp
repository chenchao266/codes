// 本程序是仿真库测试代码
// 酒泉卫星发射中心技术部  刘纯根  2007-7

#include<stdio.h>
#include"FloatPoint.h"
#include"SuperFloat.h"
#include<math.h> 

// 几个测试用例

// [1]病态函数测试,本书1.2节和7.7节的代码
/*
int main()
{
	CSuperFloat< unsigned short, 20, unsigned int> x, y, x0, x1, dx, PRECISION;

	PRECISION = "1.0E-10";
	Precision( PRECISION );

	x0 = "-9.0E-4";
	x1 = " 9.0E-4";
	dx = " 1.0E-4";

	for( x=x0; x<=x1; x=x+dx )
	{
		y = ( Tan( Sin( x ) ) - Sin( Tan( x ) ) )/Xn( x, 7 );
		printf( "%s    ", DataToStr( x, 10 ) );
		printf( "\t%s\n", DataToStr( y, 10 ) );
	}

	return 0;
}
*/


// [2]计算PI,本书4.5节和6.5节的代码
/*
int main(int argc, char* argv[])
{
//  CFloatPoint<unsigned __int64, 11> C05(1,2), C6(6), One(1), Two(2);
//	CFloatPoint<unsigned __int64, 11> x2, ck, x, y, PI_PRECISION, n(1);

	CSuperFloat<unsigned short, 100, unsigned int> C05(1,2), C6(6), One(1), Two(2);
	CSuperFloat<unsigned short, 100, unsigned int> x2, ck, x, y, PI_PRECISION, n(1);

    PI_PRECISION = "1.0E-400";          // 设置函数库的运算精度
	Precision( PI_PRECISION );

    // 计算π的循环
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
    y = y*C6;

    printf( "%s\n", DataToStr( y,400 ) );  // 输出400位,含一个整数位
    return 0;
}
*/
/*
输出结果:
+3.14159265358979323846264338327950288419716939937510582097494459230781640628620
89986280348253421170679821480865132823066470938446095505822317253594081284811174
50284102701938521105559644622948954930381964428810975665933446128475648233786783
16527120190914564856692346034861045432664821339360726024914127372458700660631558
81748815209209628292540917153643678925903600113305305488204665213841469519415116
09E+0
*/


// [3]RSA计算,本书3.7节的代码
/*
#include<stdio.h>
#include"CGUINT.h"
int main(int argc, char* argv[])
{
    CGUINT< unsigned short, 33 > p, q, rsa; 

	rsa = "109417386415705274218097073220403576120037329454492059909138421314763499842889"
		  "34784717997257891267332497625752899781833797076537244027146743531593354333897";
    p   = "102639592829741105772054196573991675900716567808038066803341933521790711307779";
//	q   = "106603488380168454820927220360012878679207958575989291522270608237193062808643";

    q = rsa/p;
    printf( "%s\n", IntToStr( rsa ) );
    return 0;
}
*/

// [4]输出扩展双精度格式极值,参阅5.2节
#pragma pack( push, 1 )
typedef struct _FP_DOUBLE_EXT
{
    unsigned __int64 nFraction;
    unsigned __int16 nExponent  : 15;
    unsigned __int16 nSign      :  1;
} FP_DOUBLE_EXT;
#pragma pack( pop )

#pragma warning( disable: 4101 )
/*
int main()
{
	CSuperFloat< unsigned short, 4, unsigned short> *p;
	CSuperFloat< unsigned short, 6, unsigned short> *p6;
	int temp1[2];
	FP_DOUBLE_EXT data;
	int temp2[2];
	char* pt;
	int i;

	// 最小值
//	data.nSign		= 0;
//	data.nFraction	= 0x8000000000000000;
//	data.nExponent	= 1;

	// 最大值
//	data.nSign		= 0;
//	data.nFraction	= 0xFFFFFFFFFFFFFFFF;
//	data.nExponent	= 0xFFFE;

	// 最小值弱规范数的2倍
//	data.nSign		= 0;
//	data.nFraction	= 0x000000000000002;
//	data.nExponent	= 0;
//	p = (CSuperFloat< unsigned short, 4, unsigned short>*)&data;
//	printf( "%s\n", DataToStr( *p, 30 ) );

	// 用更高精度格式输出最小弱规范数,temp1和temp2用于提供空间
	pt = (char*)&data;
	for( i=0; i<14; i++ )
		pt[i] = 0;
	data.nFraction	= 0x100000000;
	p6 = (CSuperFloat< unsigned short, 6, unsigned short>*)&data;
	printf( "%s\n", DataToStr( *p6, 30 ) );

	return 0;
}
*/
/*
最小弱规范数输出结果与解释:
+5.00000000000000000000000000000E-4951	扩展双精度格式输出的2倍最小弱规范数
+1.82260118918498000411597329735E-4951	更高格式输出的最小弱规范数
由于截断舍入原因,CSuperFloat要损失1位,因此在CSuperFloat< unsigned short,4,unsigned short>中
无法输出最小弱规范数,只能输出它的倍数,且由于有小数字不足,输出结果(约2.5)不可靠,实际是1.82左右
*/

// [5]验证弱规范数支持,参阅5.5.2节
/*
#pragma pack( push, 1 )
typedef struct _FP_DOUBLE_EXT
{
    unsigned __int64 nFraction;
    unsigned __int16 nExponent  : 15;
    unsigned __int16 nSign      :  1;
} FP_DOUBLE_EXT;
#pragma pack( pop )

int main()
{
	CSuperFloat< unsigned short, 4, unsigned short> *p, C(2);
	FP_DOUBLE_EXT data;

	// 最小值
	data.nSign		= 0;
	data.nFraction	= 0x8000000000000000;
	data.nExponent	= 1;
	p = (CSuperFloat< unsigned short, 4, unsigned short>*)&data;

	printf( "%s\n", DataToStr( *p, 30 ) );

	(*p) = (*p)/C;
	printf( "%s\n", DataToStr( *p, 30 ) );

	return 0;
}
*/

// [6]验证弱规范数解释,参阅5.5.2节
/*
typedef struct _FP_SIGLE
{
    unsigned __int32 nFraction : 23;	// 加隐含位共24位有效数字
    unsigned __int32 nExponent :  8;
    unsigned __int32 nSign     :  1;
} FP_SINGLE;

int main()
{
	CFloatPoint< unsigned __int32, 8> *pf;
	CFloatPoint< unsigned __int64, 11> *pd;
	FP_SINGLE data;
	double d;
	float  f;
	
	// 最小值
	data.nSign		= 0;
	data.nFraction	= 0x000001;
	data.nExponent	= 0;

	// x87 FPU输出
	f = *(float*)&data;
	d = f;
	printf( "%+.20E\n",  d );

	// 通用库输出
	pd = ( CFloatPoint<unsigned __int64, 11>*)&d;
	printf( "%s\n", DataToStr( *pd, 20 ) );

	pf = ( CFloatPoint<unsigned __int32, 8>*)&f;
	printf( "%s\n", DataToStr( *pf, 20 ) );

	return 0;
}
*/

/*
结果与解释:
+1.40129846432481710000E-045	浮点库输出,VC6浮点库使用double
+1.4012984643248196370E-45		通用库使用double输出，15位有效
+1.4285714626312255859E-45		通用库使用float输出,由于输出过程中有效数字不足,结果有偏差
*/

// [7]验证数据格式是否符合IEEE
int main()
{
	float x = -767834.87345f;
	CFloatPoint< unsigned int, 8> *px;
	px = (CFloatPoint< unsigned int, 8>*)&x;
	printf( "%s\n", DataToStr( *px, 10 ) );

	double y = -767834.87345;
	CFloatPoint< unsigned __int64, 11> *py;
	py = (CFloatPoint< unsigned __int64, 11>*)&y;
	printf( "%s\n", DataToStr( *py, 10 ) );

	return 0;
}