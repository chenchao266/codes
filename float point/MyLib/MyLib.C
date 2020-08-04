// 这是在x87 FPU基础上实现的一个小数学函数库
// 酒泉卫星发射中心技术部  刘纯根  2007-7

#include<stdio.h>
#include<math.h>
#include<float.h>

#define RC_MASK0			0xC00
#define RC_NEAREST0			0x000
#define RC_DOWN0			0x400
#define RC_UP0				0x800
#define RC_TRUNCATE0		0xC00

unsigned char _Pi_By_2n[10];

void _InitLib()
{
	int exp = 61;

	// 初始化Pi*2**61
	__asm
	{
		FILD exp
		FLDPI
		FSCALE
		FSTP TBYTE PTR[_Pi_By_2n]
		FSTP ST(0)
	}
}

__declspec(naked) double Sin( double x )
{
	__asm
	{
		FLD QWORD PTR[ESP+4]
		FSIN
		FSTSW AX
		SAHF
		JNP MYEXIT

		// C2=1,超出范围
		FLD TBYTE PTR[_Pi_By_2n]
		FXCH

	MYLOOP:
		FPREM1
		FSTSW AX
		SAHF
		JPE MYLOOP

		FSTP ST(1)
		FSIN
	MYEXIT:
		RET
	}
}


__declspec(naked) double _SinCos( double x, double* pcosx )
{
    __asm
    {
        FLD QWORD PTR[ESP+4]
        FSINCOS

        // 检测输入参数是否超出范围
        FSTSW AX
        SAHF
        JNP MYEXIT

        // C2=1,输入参数超出范围,需要规范化
        FLD TBYTE PTR[_Pi_By_2n]
        FXCH

        // 将输入参数规范至[-260π,260π]
    MYLOOP:
        FPREM1
        FSTSW AX
        SAHF
        JPE MYLOOP

        FSTP ST(1)
        FSINCOS

    MYEXIT:
        MOV EAX, [ESP+12]
        FSTP QWORD PTR[EAX]
        RET
    }
}

__declspec(naked) double Cos( double x )
{
	__asm
	{
		FLD QWORD PTR[ESP+4]
		FCOS
		FSTSW AX
		SAHF
		JNP MYEXIT

		// C2=1,超出范围
		FLD TBYTE PTR[_Pi_By_2n]
		FXCH

	MYLOOP:
		FPREM1
		FSTSW AX
		SAHF
		JPE MYLOOP

		FSTP ST(1)
		FCOS
	MYEXIT:
		RET
	}
}

__declspec(naked) double Tan( double x )
{
	__asm
	{
		FLD QWORD PTR[ESP+4]
		FPTAN
		FSTSW AX
		SAHF
		JNP MYEXIT

		// C2=1,超出范围
		FSTP ST(0)
		FSTP ST(0)
		FLD TBYTE PTR[_Pi_By_2n]
		FLD QWORD PTR[ESP+4]

	MYLOOP:
		FPREM1
		FSTSW AX
		SAHF
		JPE MYLOOP

		FSTP ST(1)
		FPTAN
	MYEXIT:
		FSTP ST(0)
		RET
	}
}

__declspec(naked) double Sqrt( double x )
{
	__asm
	{
		FLD QWORD PTR[ESP+4]
		FSQRT
		RET
	}
}


__declspec(naked) double ArcTan2( double y, double x )
{
	__asm
	{
		FLD QWORD PTR[ESP+4]
		FLD QWORD PTR[ESP+12]
		FPATAN
		RET
	}
}

__declspec(naked) double ArcTan( double x )
{
	__asm
	{
		FLD QWORD PTR[ESP+4]
		FLD1
		FPATAN
		RET
	}
}

__declspec(naked) double ArcSin( double x )
{
	// x/sqrt((1+x)(1-x))
	__asm
	{
		FLD QWORD PTR[ESP+4]

		FLD1
		FLD QWORD PTR[ESP+4]
		FADDP ST(1), ST

		FLD1
		FLD QWORD PTR[ESP+4]
		FSUBP ST(1), ST

		FMULP ST(1), ST

		FSQRT

		// 得到最终值
		FPATAN

		RET
	}
}

__declspec(naked) double ArcCos( double x )
{
	// sqrt((1+x)(1-x))/x
	__asm
	{
		FLD1
		FLD QWORD PTR[ESP+4]
		FADDP ST(1), ST

		FLD1
		FLD QWORD PTR[ESP+4]
		FSUBP ST(1), ST

		FMULP ST(1), ST

		// 计算SIN
		FSQRT

		// 得到最终值
		FLD QWORD PTR[ESP+4]
		FPATAN

		RET
	}
}

__declspec(naked) double Log( double x )
{
	// lnx = log2(x)/log2(e) = 1/log2(e)*log2(x)
	__asm
	{
		FLD1
		FLDL2E
		FDIVP ST(1), ST(0)

		FLD QWORD PTR[ESP+4]
		FYL2X
		RET
	}
}

__declspec(naked) double Log10( double x )
{
	// lnx = log2(x)/log2(10) = 1/log2(10)*log2(x)
	__asm
	{
		FLD1
		FLDL2T
		FDIVP ST(1), ST(0)

		FLD QWORD PTR[ESP+4]
		FYL2X
		RET
	}
}

__declspec(naked) double Exp( double x )
{
	// 2**(xloge)
	__asm
	{
		// y=x*log2(e)
		FLDL2E
		FLD QWORD PTR[ESP+4]
		FMULP ST(1), ST(0)

		// [y]
		FLD ST(0)
		FRNDINT

		// y-[y]
		FSUB ST(1), ST(0)

		// 2**[y]
        FLD1
        FSCALE
		FXCH
		FSTP ST(0)

		// 2**(y-[y])
		FXCH
        F2XM1
        FLD1
        FADDP ST(1), ST(0)

        FMULP ST(1), ST(0)
		RET
	}
}

__declspec(naked) double Pow( double x, double y )
{
	// 2**(y*logx)
	__asm
	{
		// y*logx
		FLD QWORD PTR[ESP+12]
		FLD QWORD PTR[ESP+4]
		FYL2X

		// [y]
		FLD ST(0)
		FRNDINT

		// y-[y]
		FSUB ST(1), ST(0)

		// 2**[y]
        FLD1
        FSCALE
		FXCH
		FSTP ST(0)

		// 2**(y-[y])
		FXCH
        F2XM1
        FLD1
        FADDP ST(1), ST(0)

        FMULP ST(1), ST(0)
		RET
	}
}

__declspec(naked) double Abs( double x )
{
    __asm
	{
		AND DWORD PTR [ESP+8], 0x7FFFFFFF
		FLD QWORD PTR[ESP+4]
		RET
	}
}


#define CLASS_MASK_C0	0x0100
#define CLASS_MASK_C1	0x0200
#define CLASS_MASK_C2	0x0400
#define CLASS_MASK_C3	0x4000

int Class( double x )
{
	unsigned short status;
	int C0, C1, C2, C3;

	__asm
	{
		FLD x
		FXAM
		FSTSW status
		FSTP ST(0)
	}

	C0 = ( status&CLASS_MASK_C0 ) == 0 ? 0 : 1;
	C1 = ( status&CLASS_MASK_C1 ) == 0 ? 0 : 1;
	C2 = ( status&CLASS_MASK_C2 ) == 0 ? 0 : 1;
	C3 = ( status&CLASS_MASK_C3 ) == 0 ? 0 : 1;

	if( C0==1 && C2==0 && C3==0 )
		return _FPCLASS_QNAN;

	if( C0==0 && C2==1 && C3==0 )
	{
		if( C1 == 0 )
			return _FPCLASS_PN;
		else
			return _FPCLASS_NN;
	}

	if( C0==1 && C2==1 && C3==0 )
	{
		if( C1 == 0 )
			return _FPCLASS_PINF;
		else
			return _FPCLASS_NINF;
	}

	if( C0==0 && C2==0 && C3==1 )
	{
		if( C1 == 0 )
			return _FPCLASS_PZ;
		else
			return _FPCLASS_NZ;
	}

	if( C0==0 && C2==1 && C3==1 )
	{
		if( C1 == 0 )
			return _FPCLASS_PD;
		else
			return _FPCLASS_ND;
	}

	return -1;
}


__declspec(naked) double _Epsilonp1n( double x, double n )
{
    __asm
    {
        // 计算y=nlog2(1+x)
        FLD QWORD PTR[ESP+12]
        FLD QWORD PTR[ESP+4]
        FYL2XP1

        // [y]
        FLD ST(0)
        FRNDINT

        // y-[y]
        FSUB ST(1), ST(0)

        // 2**[y]
        FLD1
        FSCALE
        FXCH
        FSTP ST(0)

        // 2**(y-[y])
        FXCH
        F2XM1
        FLD1
        FADDP ST(1), ST(0)

        FMULP ST(1), ST(0)
        RET
    }
}

__declspec(naked) double _Pow2x( double x )
{
    __asm
    {
        FLD QWORD PTR[ESP+4]

        // [x]
        FLD ST(0)
        FRNDINT

        // x-[x]
        FSUB ST(1), ST(0)

        // 2**[y]
        FLD1
        FSCALE
        FXCH
        FSTP ST(0)

        // 2**(y-[y])
        FXCH
        F2XM1
        FLD1
        FADDP ST(1), ST(0)

        FMULP ST(1), ST(0)
        RET
    }
}

double _Log2x( double x )
{
	__asm
	{
		FLD1
		FLD x
		FYL2X
		FSTP x
	}
	return x;
}


double _Prem1( double x, double y )
{
	__asm
	{
		FLD y
		FLD x

	MYLOOP:
		FPREM1
		FSTSW  AX
		SAHF
		JP MYLOOP

		FSTP x
		FSTP ST(0)
	}
	return x;
}

double _Round( double x, unsigned int nRoundMode )
{
    unsigned short usCWOld, usCWNew;
    __asm FSTCW usCWOld                          // 保护控制寄存器

    usCWNew  = usCWOld & (~RC_MASK0);           // 设置截断舍入模式
    usCWNew |= nRoundMode;
    __asm FLDCW usCWNew

    __asm FLD x                                    // 舍入
    __asm FRNDINT
    __asm FSTP x

    __asm FLDCW usCWOld                           // 恢复控制寄存器
    return x;
}

double _Truncate( double x )
{
    return _Round( x, RC_TRUNCATE0 );
}


double _Nearest( double x )
{
    return _Round( x, RC_NEAREST0 );
}

double _Ceil( double x )
{
    return _Round( x, RC_UP0 );
}

double _Floor( double x )
{
    return _Round( x, RC_DOWN0 );
}

double _Extract( double x, double* intptr )
{
    __asm
    {
        FLD x
        FXTRACT
        FXCH
        MOV EAX, intptr
        FSTP QWORD PTR [EAX]
    }
}

int _IntRound( double x, unsigned int nRoundMode )
{
    int n;
    unsigned short usCWOld, usCWNew;
    __asm FSTCW usCWOld

    usCWNew  = usCWOld & (~RC_MASK0);
    usCWNew |= nRoundMode;
    __asm FLDCW usCWNew

    __asm FLD x
    __asm FISTP n                              // 取整

    __asm FLDCW usCWOld
    return n;
}

int _IntTruncate( double x )
{
    return _IntRound( x, RC_TRUNCATE0 );
}

int _IntCeil( double x )
{
    return _IntRound( x, RC_UP0 );
}

int _IntFloor( double x )
{
    return _IntRound( x, RC_DOWN0 );
}

int _IntNearest( double x )
{
    return _IntRound( x, RC_NEAREST0 );
}

double _RadToDeg( double x )
{
    double pi;
    __asm FLDPI
    __asm FSTP pi
    return x*180.0/pi;
}

double _DegToRad( double x )
{
    double pi;
    __asm FLDPI
    __asm FSTP pi
    return x*pi/180.0;
}

int main()
{
	double x = 3.63468347893479348938E20,y = 3.7;
	_InitLib();
	printf( "%.5E, %f,%f\n", x, Tan(x), tan(x) );
	return 0;
}