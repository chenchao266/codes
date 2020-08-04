#include"VC6Lib.h"

// VC_FP_LIB--Visaul C++ 6.0浮点库

// 浮点运算结果存储于ST(0)中

// x87 FPU初始化时(Pentium4):
//   控制寄存器0x037F(所有异常被屏蔽、最近舍入、64位运算精度)
//   状态寄存器0(没有异常、堆栈指针是0)
//   标志寄存器0xFFFF(全空)
//   数据寄存器内容不变

// Visual C++ 6.0屏蔽了所有异常,完全从操作系统接管了异常处理
// 这么做的原因也许是避免操作系统异常处理的低效率以及修改x87 FPU的特性

// FB:   forehead byte
// TOS:  



//-----------------------------------------------------------------------------
//                           全局数据区
//-----------------------------------------------------------------------------

extern int errno; // C语言库


// 缺省控制寄存器值
unsigned short _DEFAULT_CW_in_mem = _CW_DEFAULT;

// 不确定值,用于特殊运算的处理,是QNaN
FP_DOUBLE_EXT _indefinite  = 
{
	0xC000000000000000,			// 隐含位1,最高尾数位1
	0x7FFF,
	1
};

// 这个常数用于将SNaN转换为QNaN,参见_convertTOStoQNaN()
double const_SNaN_to_QNaN = 1.0L;


// SNaN,0xFFF800000000
double _d_ind;

// 快速结束标志
int _fastflag;


int _adjust_fdiv;


// 函数表
void* _cfltcvt_tab[];



//-----------------------------------------------------------------------------
//                           杂项函数
//-----------------------------------------------------------------------------


// 由于double格式装载一些特殊数时可能触发异常,
// 故对这些数载装载前转换为扩展双精度格式,避免异常
// 数据指针存储于edx中,仅是示意,非真正代码
// 另外,ZF标志指示double的指数是否是0x&7FF
unsigned int _fload_withFB( double x, int* bExponentIs0x7FF )
{
	FP_DOUBLE		*px = (FP_DOUBLE*)&x;
	FP_DOUBLE_EXT	Extended;

	// 正常值/0/弱规范数
	if( px->nExponent != 0x7FF )
	{
		*bExponentIs0x7FF = 0;

		// 一般值,但可能是弱规范值,可能出发#D异常
		__asm fld x

		// 返回FB
		return FP_DOUBLE_HIGH4( x ) & 0x7FF00000;
	}
	else
	{
		// 对QNaN/SNaN/PINF/NINF扩展以后再装入可以避免SNaN触发#I异常

		// 特殊值(NaN和无穷)
		Extended.nSign		= px->nSign;			// 1
		Extended.nExponent	= 0x7FFF;				// 15,最大指数

		// 尾数是左对齐,double有52位(加隐含1就是53),而扩展精度是64位,需要左移11位
		// 注意这些数的隐含位全是1
		Extended.nFraction =   (((unsigned __int64)(px->nFraction))<<11)|0x8000000000000000;

		__asm fld tbyte ptr Extended

		*bExponentIs0x7FF = 1;

		// 返回FB
		return FP_DOUBLE_HIGH4( x );
	}
}

unsigned int _checkTOS_withFB( double x )
{
	FP_DOUBLE *px = (FP_DOUBLE*)&x;

	// 特殊值
	if( px->nExponent != 0x7FF )
		return FP_DOUBLE_HIGH4( x ) & 0x7FF00000;
	else
		return FP_DOUBLE_HIGH4( x );
}


void _cfltcvt_init()
{
	_cfltcvt_tab[0] = _cfltcvt;
	_cfltcvt_tab[1] = _cropzeros;
	_cfltcvt_tab[2] = _fassign;
	_cfltcvt_tab[3] = _forcdecpt;
	_cfltcvt_tab[4] = _positive;
	_cfltcvt_tab[5] = _cfltcvt;
}

void _fpmath()
{
	_cfltcvt_init();
	_adjust_fdiv = _ms_p5_mp_test_fdiv();
	setdefaultprecision();

	__asm fnclex
}


// 处理过的值在ST(0)中
int _convertTOStoQNaN( double x )
{
	if( FP_DOUBLE_HIGH4( x ) & 0x80000000 == 0 )
	{
		// SNaN,做一次加法运算,SNaN=>QNaN
		__asm fadd const_SNaN_to_QNaN
		return 1;
	}
	else
	{
		// QNaN,数据在ST(0)中
		return 7;
	}	
}

//-----------------------------------------------------------------------------
//                           异常处理函数
//-----------------------------------------------------------------------------

#define EXCEPTION_BITS_OVERFLOW				01	// STATUS_FLOAT_OVERFLOW
#define EXCEPTION_BITS_UNDERFLOW			02	// STATUS_FLOAT_UNDERFLOW
#define EXCEPTION_BITS_DIVIDE_BY_ZERO		04	// STATUS_FLOAT_DIVIDE_BY_ZERO
#define EXCEPTION_BITS_INVALID_OPERATION	08	// STATUS_FLOAT_INVALID_OPERATION
#define EXCEPTION_BITS_INEXACT_RESULT		10	// STATUS_FLOAT_INEXACT_RESULT


const double _Min_Denormal	= -4.9406564584124654E-324;
const double _PMax_Double	=  1.7976931348623157E+308;
const double _NMax_Double	= -1.7976931348623157E+308;
const double _Zero			=  0.0;

const unsigned int _PInfinite[] = { 0, 0x7FF00000 };
const unsigned int _NInfinite[] = { 0, 0xFFF00000 };

typedef struct
{
	int		nOperation;
	char*	name;
} FUNCTION_OPRATION_NAME;

#define _COUNT_OF(x)	sizeof(x)/sizeof(x[0])

FUNCTION_OPRATION_NAME FuncOpName[] =
{
        { 0x14, "exp"   },
        { 0x1D, "pow"   },
        { 0x1A, "log"   },
        { 0x1B, "log10" },
        { 0x1F, "sinh"  },
        { 0x13, "cosh"  },
        { 0x21, "tanh"  },
        { 0x0E, "asin"  },
        { 0x0D, "acos"  },
        { 0x0F, "atan"  },
        { 0x10, "atan2" },
        { 0x05, "sqrt"  },
        { 0x1E, "sin"   },
        { 0x12, "cos"   },
        { 0x20, "tan"   },
        { 0x0C, "ceil"  },
        { 0x0B, "floor" },
        { 0x15, "fabs"  },
        { 0x1C, "modf"  },
        { 0x19, "ldexp" },
        { 0x11, "_cabs" },
        { 0x18, "_hypot"},
        { 0x16, "fmod"  },
        { 0x17, "frexp" },
        { 0x22, "_y0"   },
        { 0x23, "_y1"   },
        { 0x24, "_yn"   }
};

double _handle_qnan1( int nOperation, double arg1, unsigned short usControl )
{
	if( _matherr_flag == 0 )
		return _umatherr( _DOMAIN, nOperation, arg1, 0.0, arg1, usControl );
	
	*_errno() = EDOM;
	_ctrlfp( usControl, 0xFFFF );

	return arg1;
}

typedef int (*PFNUSERMATHERR)( struct _exception *except );

void _setusermatherr( PFNUSERMATHERR fn )
{
	_usermatherr_fn = fn;
	_matherr_flag   = 0;
}


// 系统自带的一个_matherr()
int _matherr( struct _exception *except )
{
	if( _usermatherr_fn != 0 )
		return _usermatherr_fn( except );
	else
		return 0;
}


double _handle_qnan2( int nOperation, double arg1, double arg2, unsigned short usControl )
{
	double retval = arg1 + arg2;

	if( _matherr_flag == 0 )
		return _umatherr( _DOMAIN, nOperation, arg1, arg2, retval, usControl );
	
	*_errno() = EDOM;
	_ctrlfp( usControl, 0xFFFF );

	return retval;
}

double _umatherr( int ntype, int nOperation, double arg1, double arg2, 
				  double retval, unsigned short usControl )
{
	struct _exception ex;
	char* name = GetNameFromOperation( nOperation );

	if( name )
	{
		ex.type		= ntype;
		ex.name		= name;
		ex.arg1		= arg1;
		ex.arg2		= arg2;
		ex.retval	= retval;

		_ctrlfp( usControl, 0xFFFF );

		if( !_matherr( &ex ) )
			_set_errno( ntype );

		return ex.retval;
	}

	_ctrlfp( usControl, 0xFFFF );
	_set_errno( ntype );

	return retval;
}

char *GetNameFromOperation( int nOperation )
{
	int i = 0;
	while( FuncOpName[i].nOperation != nOperation && i < _COUNT_OF(FuncOpName) )
		i++;

	if( i < _COUNT_OF(FuncOpName) )
		return FuncOpName[i].name;
	else
		return 0;
}

int ExceptionBitsToType( unsigned int nExceptionBits )
{
	if( nExceptionBits & 0x20 )
		return _TLOSS;

	if( nExceptionBits & EXCEPTION_BITS_INVALID_OPERATION )
		return _DOMAIN;

	if( nExceptionBits & EXCEPTION_BITS_DIVIDE_BY_ZERO )
		return _SING;

	if( nExceptionBits & EXCEPTION_BITS_OVERFLOW )
		return _OVERFLOW;

	if( nExceptionBits & EXCEPTION_BITS_UNDERFLOW )
		return _UNDERFLOW;

	return 0;
}

double _except2( unsigned int nExceptionBits, int nOperation, double arg1, 
				 double arg2, double retval, unsigned short usControl )
{
	_FPIEEE_RECORD ieee;
	int ntype;

	if( !_handle_exc( nExceptionBits, &retval, uscontrol ) )
	{
		// 只需初始化ieee.Operand2,其他由_raise_exc处理
		ieee.Operand2.OperandValid	= 1;
		ieee.Operand2.Format		= Fp64Value;
		ieee.Operand2.Value			= arg2;
		_raise_exc( &ieee, &uscontrol, nExceptionBits, nOperation, &arg1, &retval );
	}

	ntype = ExceptionBitsToType( nExceptionBits );

	if( __matherr_flag == 0 && ntype )
	{
		retval = _umatherr( ntype, nOperation, arg1, arg2, retval, usControl );
	}
	else
	{
		_set_errno( ntype );
		_ctrlfp( uscontrol, 0xFFFF );
	}

	return retval;
}

double _except1( unsigned int nExceptionBits, int nOperation, 
				 double arg1, double retval, unsigned short usControl )
{
	_FPIEEE_RECORD ieee;
	int ntype;

	if( !_handle_exc( nExceptionBits, &retval, uscontrol ) )
	{
		// 只需初始化ieee.Operand2,其他由_raise_exc处理
		ieee.Operand2.OperandValid = 0;
		_raise_exc( &ieee, &uscontrol, nExceptionBits, nOperation, &arg1, &retval );
	}

	ntype = ExceptionBitsToType( nExceptionBits );

	if( _matherr_flag == 0 && ntype )
	{
		retval = _umatherr( ntype, nOperation, arg1, 0.0, retval, usControl );
	}
	else
	{
		_set_errno( ntype );
		_ctrlfp( uscontrol, 0xFFFF );
	}

	return retval;
}


// 处理被屏蔽的异常,返1表示结束处理
int _handle_exc( unsigned int nExceptionBits, double* pretval, unsigned short usControl )
{
	unsigned int nExceptBits;
	int	intepr, bUnderflowCheck, intexp, bNegative;

	nExceptBits = nExceptionBits & (EM_MASK&~EM_PRECISION);

	if( nExceptionBits & EXCEPTION_BITS_INVALID_OPERATION && usControl & EM_INVALID_OPERATION )
	{
		SetException( EM_INVALID_OPERATION );
		nExceptBits &= ~EXCEPTION_BITS_INVALID_OPERATION;
	}
	else if( nExceptionBits & EXCEPTION_BITS_DIVIDE_BY_ZERO && usControl & EM_ZERO_DIVIDE )
	{
		SetException( EM_ZERO_DIVIDE );
		nExceptBits &= ~EXCEPTION_BITS_DIVIDE_BY_ZERO;
	}
	else if( nExceptionBits & EXCEPTION_BITS_OVERFLOW && usControl & EM_OVERFLOW )
	{
		SetException( EM_OVERFLOW );

		switch( usControl & RC_MASK )
		{
		case RC_NEAREST:		// nearest
			{
				if( *pretval >= 0 )
					*pretval = *(double*)&_PInfinite;
				else
					*pretval = *(double*)&_NInfinite;
			}
			break;

		case RC_DOWN:	// round down
			{
				if( *pretval >= 0 )
					*pretval = *(double*)&_PMax_Double;
				else
					*pretval = *(double*)&_NInfinite;
			}
			break;

		case RC_UP:	// round up
			{
				if( *pretval >= 0 )
					*pretval = *(double*)&_PInfinite;
				else
					*pretval = *(double*)&_NMax_Double;
			}
			break;

		case RC_TRUNCATE:	// truncate
			{
				if( *pretval >= 0 )
					*pretval = *(double*)&_PMax_Double;
				else
					*pretval = *(double*)&_NMax_Double;
			}
			break;

		default:
			break;

		}
		nExceptBits &= ~EXCEPTION_BITS_OVERFLOW;
	}
	else if( nExceptionBits & EXCEPTION_BITS_UNDERFLOW && usControl & EM_UNDERFLOW )
	{
		if( nExceptionBits & EXCEPTION_BITS_INEXACT_RESULT )
			bUnderflowCheck = 1;
		else
			bUnderflowCheck = 0;

		if( *pretval != 0.0 )
		{
			*pretval = _decomp( *pretval, &intepr );
			
			// 下溢时如果约定返回值不是0.0则指数被平移了1536,参见ldexp代码
			intexp	 = intepr - 1536;

			// -1074=-1023-51,规范化后所有有效数字全部丢失,0.0
			if( intexp < -1074 )
			{
				*pretval  *= 0.0;
				bUnderflowCheck  = 1;
			}
			else
			{	
				// 分离符号
				if( *pretval < 0.0 )
					bNegative = 1;

				// 设置整数位,得到完整的有效数字
				*(unsigned __int64*)pretval &= 0x000FFFFFFFFFFFFF;
				*(unsigned __int64*)pretval |= 0x0010000000000000;

				// 规范化为弱规范数形式
				while( ++intexp < -1021 )
					*(unsigned __int64*)pretval >>= 1;

				if( bNegative )
					*pretval = -*pretval;
			}
		}
		// 检查下溢异常
		if( bUnderflowCheck )
			SetException( EM_UNDERFLOW );
		nExceptBits &= ~EXCEPTION_BITS_UNDERFLOW;
	}

	if( nExceptionBits & EXCEPTION_BITS_INEXACT_RESULT && usControl & EM_PRECISION )
	{
		SetException( EM_PRECISION );
		nExceptBits &= ~EXCEPTION_BITS_INEXACT_RESULT;
	}

	if( nExceptBits == 0 )
		return 1;
	else
		return 0;
}

void _87except( int nOperation, struct _exception* pex, unsigned short* pusControl )
{
	_FPIEEE_RECORD ieee;
	unsigned int nExceptionBits;
	unsigned short usControl;

	usControl = *pusControl;

	// 将异常类型转换为位
	switch( pex->type )
	{
	case _DOMAIN:		// 1,定义域错误
	case _TLOSS:		// 5,全部丢失精度
		nExceptionBits = EXCEPTION_BITS_INVALID_OPERATION;
		break;

	case _SING:			// 2,奇点
		nExceptionBits = EXCEPTION_BITS_DIVIDE_BY_ZERO;
		break;

	case _OVERFLOW:		// 3,上溢
		nExceptionBits = EXCEPTION_BITS_INEXACT_RESULT|EXCEPTION_BITS_OVERFLOW;
		break;

	case _UNDERFLOW:	// 4,下溢
		nExceptionBits = EXCEPTION_BITS_INEXACT_RESULT|EXCEPTION_BITS_UNDERFLOW;
		break;

	case 7:
		nExceptionBits = EXCEPTION_BITS_OVERFLOW;
		break;

	case _PLOSS:		// 6,部分丢失精度
	default:
		ExceptionBits = 0;
		break;
	}

	// 处理异常,_handle_exc处理被屏蔽的异常,_raise_exc处理没有被屏蔽的异常
	if( !_handle_exc( nExceptionBits, &pex->retval, usControl ) )
	{
		if( nOperation == _FpCodeAtan2 || nOperation == _FpCodeFmod || nOperation == _FpCodePow )
		{
			ieee.Operand2.OperandValid	= 1;
			ieee.Operand2.Format		= Fp64Value;
			ieee.Operand2.Value			= pex->arg2;
		}
		else
		{
			ieee.Operand2.OperandValid	= 0;
		}

		_raise_exc( &ieee, &usControl, nExceptionBits, nOperation,  &pex->arg1, &pex->retval );
	}

	_ctrlfp( usControl, 0xFFFF );
	if( pex->type != 8 && _matherr_flag )
	{
		if( !_matherr( pex ) )
			_set_errno( pex->type );
	}
}


void _raise_exc( _FPIEEE_RECORD* pieee, unsigned short* pusControl,unsigned int nExceptionBits,
				 int nOperation, double *px, double* pretval )
{
	unsigned int uExceptionCode;
	unsigned short usStatus, usRP;

	// 初始化异常结构
	pieee->Cause	= 0;
	pieee->Enable	= 0;
	pieee->Status	= 0;

	// Cause
	if( nExceptionBits & EXCEPTION_BITS_INEXACT_RESULT )
	{
		uExceptionCode	 = STATUS_FLOAT_INEXACT_RESULT;
		pieee->Cause	|= _SW_INEXACT;
	}

	if( nExceptionBits & EXCEPTION_BITS_UNDERFLOW )
	{
		uExceptionCode	 = STATUS_FLOAT_UNDERFLOW;
		pieee->Cause	|= _SW_UNDERFLOW;
	}

	if( nExceptionBits & EXCEPTION_BITS_OVERFLOW )
	{
		uExceptionCode	 = STATUS_FLOAT_OVERFLOW;
		pieee->Cause	|= _SW_OVERFLOW;
	}

	if( nExceptionBits & EXCEPTION_BITS_DIVIDE_BY_ZERO )
	{
		uExceptionCode	 = STATUS_FLOAT_DIVIDE_BY_ZERO;
		pieee->Cause	|= _SW_ZERODIVIDE;
	}

	if( nExceptionBits & EXCEPTION_BITS_INVALID_OPERATION )
	{
		uExceptionCode	 = STATUS_FLOAT_INVALID_OPERATION;
		pieee->Cause	|= _SW_INVALID;
	}

	// Enable
	if( *pusControl & EM_INVALID_OPERATION )	// 屏蔽#I
	{
		pieee->Enable &= ~_EM_INVALID;
		pieee->Enable |=  _EM_INVALID;
	}

	if( *pusControl & EM_ZERO_DIVIDE )	// 屏蔽#Z
	{
		pieee->Enable &= ~_EM_ZERODIVIDE;
		pieee->Enable |=  _EM_ZERODIVIDE;
	}

	if( *pusControl & EM_OVERFLOW )	// 屏蔽#O
	{
		pieee->Enable &= ~_EM_OVERFLOW;
		pieee->Enable |=  _EM_OVERFLOW;
	}

	if( *pusControl & EM_UNDERFLOW )// 屏蔽#U
	{
		pieee->Enable &= ~_EM_UNDERFLOW;
		pieee->Enable |=  _EM_UNDERFLOW;
	}

	if( *pusControl & EM_PRECISION )// 屏蔽#P
	{
		pieee->Enable &= ~_EM_INEXACT;
		pieee->Enable |=  _EM_INEXACT;
	}

	// Status
	usStatus = _statfp();
	if( usStatus & EM_INVALID_OPERATION )
		pieee->Status |= _SW_INVALID;
	if( usStatus & EM_ZERO_DIVIDE )
		pieee->Status |= _SW_ZERODIVIDE;
	if( usStatus & EM_OVERFLOW )
		pieee->Status |= _SW_OVERFLOW;
	if( usStatus & EM_UNDERFLOW )
		pieee->Status |= _SW_UNDERFLOW;
	if( usStatus & EM_PRECISION )
		pieee->Status |= _SW_INEXACT;

	// RC
	usRP = *pusControl & RC_MASK;
	if( usRP == RC_NEAREST )				// nearest
		pieee->RoundingMode = _FpRoundNearest;
	if( usRP == RC_TRUNCATE )		// truncate
		pieee->RoundingMode = _FpRoundChopped;
	if( usRP == RC_UP )		// round up
		pieee->RoundingMode = _FpRoundPlusInfinity;
	if( usRP == RC_DOWN )		// round down
		pieee->RoundingMode = _FpRoundMinusInfinity;


	// PC
	usRP = *pusControl & PC_MASK;
	if( usRP == PC_FLOAT_24 )				// 24
		pieee->Precision = _FpPrecision24;
	if( usRP == PC_DOUBLE_53 )		// 53
		pieee->Precision = _FpPrecision53;
	if( usRP == PC_EX_DOUBLE_64 )		// 64
		pieee->Precision = _FpPrecisionFull;

	// Operation
	pieee->Operation = nOperation;

	// Operand1
	pieee->Operand1.OperandValid	= 1;
	pieee->Operand1.Format			= _FpFormatFp64;
	pieee->Operand1.Value			= *px;

	// Result
	pieee->Result.OperandValid		= 1;
	pieee->Result.Format			= _FpFormatFp64;
	pieee->Result.Value				= *pretval;

	// 清除异常标志
	_clrfp();

	// 激活异常处理
	RaiseException( uExceptionCode, 0, 1, &pieee );

	// pusControl
	if( pieee->Enable.InvalidOperation )
		*pusControl &= 0xFFFE;
	if( pieee->Enable.ZeroDivide )
		*pusControl &= 0xFFFB;
	if( pieee->Enable.Overflow )
		*pusControl &= 0xFFF7;
	if( pieee->Enable.Underflow )
		*pusControl &= 0xFFEF;
	if( pieee->Enable.Inexact )
		*pusControl &= 0xFFDF;

	// RC
	*pusControl &= 0xF3FF;
	switch( pieee->RoundingMode )
	{
	case _FpRoundMinusInfinity:		// round down
		*pusControl |= RC_DOWN;
		break;

	case _FpRoundPlusInfinity:		// round up
		*pusControl |= RC_UP;
		break;

	case _FpRoundChopped:			// truncate
		*pusControl |= RC_TRUNCATE;
		break;

	default:						// nearest,_FpRoundNearest
		break
	}

	// PC
	*pusControl &= 0xF3FF;			// 此处有错,0xFCFF
	switch( pieee->Precision )
	{
	case _FpPrecisionFull:		// 64
		*pusControl |= PC_EX_DOUBLE_64;
		break;

	case _FpPrecision53:		// 53
		*pusControl |= PC_DOUBLE_53;
		break;

	default:					// 24,_FpPrecision24
		break:
	}

	// retval;
	*pretval = pieee->Result.Value;
}

// 一个巨大的扩展双精度数,+2.35627234572673470e+0313
const BYTE	_Huge_TBYTE[]	= { 0, 0, 0, 0, 0, 0, 0, 0x80, 0x10, 0x44 };


// usControl是x87的控制寄存器值
void SetException( unsigned short usControl )
{
	double vt;
	int nt;

	// 屏蔽#I,存储超出int类型范围的数
	if( usControl & EM_INVALID_OPERATION )
	{
		__asm fld tbyte ptr _Huge_TBYTE
		__asm fistp nt
	}

	// 屏蔽#O,_Huge_TBYTE超出double范围
	if( usControl & EM_OVERFLOW )
	{
		__asm fld tbyte ptr _Huge_TBYTE
		__asm fstp vt
	}

	// 屏蔽#U,载入最小弱规范数
	if( usControl & EM_UNDERFLOW )
	{
		__asm fld _Min_Denormal
		__asm fstp vt
	}

	// 屏蔽#Z, 0/1,这段代码似乎错了
	if( usControl & EM_ZERO_DIVIDE )
	{
		__asm
		{
			fldz
			fld1
			fdivp st(1), st
			fstp st(0)
		}
	}

	// 屏蔽#P,载入PI必然舍入
	if( usControl & EM_PRECISION )
	{
		__asm fldpi
		__asm fstp vt
	}
}

int _fpieee_flt( unsigned long exc_code, struct _EXCEPTION_POINTERS *exc_info, int handler(_FPIEEE_RECORD *) )
{
	int handler_ret;

	switch( exc_code )
	{
	case EXCEPTION_FLT_DIVIDE_BY_ZERO:
	case EXCEPTION_FLT_INEXACT_RESULT:
	case EXCEPTION_FLT_INVALID_OPERATION:
	case EXCEPTION_FLT_OVERFLOW:
	case EXCEPTION_FLT_UNDERFLOW:
		{
			// 重设x87 FPU
			__asm finit

			// 调用用户定义的处理函数,注意_FPIEEE_RECORD的来源
			if( exc_info->ExceptionRecord->ExceptionInformation[0] )
				handler_ret = handler( ((_FPIEEE_RECORD*)(exc_info->ExceptionRecord->ExceptionInformation[0]) );
			else
			{
				// 判断是否是浮点指令
				if( *(PDWORD)(exc_info->ContextRecord->FloatSave.ErrorOffset) & 0xFFFFFFF8 != 0xD8 )
					return EXCEPTION_CONTINUE_SEARCH;

				// 是浮点指令
				// 分析浮点指令,太复杂,难以反汇编
			}

			// 清除异常标志
			exc_info->ContextRecord->FloatSave.StatusWord = 0;
		}
		return handler_ret;

	case STATUS_FLOAT_MULTIPLE_FAULTS:
	case STATUS_FLOAT_MULTIPLE_TRAPS:
		return fn_1024DDC0( exc_code, exc_info, handler );

	default:
		break;
	}

	return EXCEPTION_CONTINUE_SEARCH;
}


// 与_fpieee_flt()类似
int fn_1024DDC0( unsigned long exc_code, struct _EXCEPTION_POINTERS *exc_info, int handler(_FPIEEE_RECORD *) )
{
	// 有SEH,删去
	int handler_ret;

	if( exc_info->ExceptionRecord->ExceptionInformation[0] )
		handler_ret = handler( ((_FPIEEE_RECORD*)(exc_info->ExceptionRecord->ExceptionInformation[0]) );

	// 判断是否有扩展寄存器
	if( exc_info->ContextRecord->ContextFlags & CONTEXT_EXTENDED_REGISTERS != CONTEXT_EXTENDED_REGISTERS )
		return EXCEPTION_CONTINUE_SEARCH;

	// 太复杂,无法继续
}



// name在ECX中,nOperation在EDX中,type在EAX中
// x是原始参数,位于堆栈,usControl是保存的控制寄存器值
// 使用了一个32字节的内部结构
void __fastcall _startOneArgErrorHandling( char* name, int nOperation, int type, 
										   unsigned short usControl, double x )
{
	struct _exception ex;

	// 设置异常结构
	ex.type = type;
	ex.name = name;
	ex.arg1 = x;

	// 返回值
	__asm fstp qword ptr ex.retval

	// 调用异常处理
	_87except( nOperation, &ex, &usControl );

	// 恢复返回值
	__asm fld qword ptr ex.retval

	// 恢复控制寄存器
	if( usControl != _DEFAULT_CW_in_mem )
		__asm fldcw usControl
}



// _fast_exit实际上不是函数,而是一段通用代码,通过JXX指令进入,然后直接返回
// 功能就是恢复被库函数改变的控制寄存器
// 当前堆栈[ESP]中是进入数学函数后立即保存的控制寄存器值
void _fast_exit( unsigned short usControl )
{
	if( usControl != _CW_DEFAULT )
		__asm fldcw usControl
}

// _math_exit实际上不是函数,而是一段通用代码,通过JXX指令进入,然后直接返回
// 与_fast_exit类似,不过检查了一下精度异常
// ECX中是函数名,EDX是一个与函数有关的小整数,例如13之类
// 当前堆栈[ESP]中是进入数学函数后立即保存的控制寄存器值
void __fastcall _math_exit(  char* name, int nOperation, unsigned short usControl, double x )
{
	unsigned short usStatus;

	// 当前控制寄存器设置不是缺省值且要求屏蔽精度异常
	if( ( usControl != _CW_DEFAULT ) && ( usControl & 0x20 ) )
	{
		__asm fstsw	usStatus
		if( usStatus & 0x20 )	// 发现精度异常
			_startOneArgErrorHandling( name, nOperation, EXCEPTION_BITS_INVALID_OPERATION, usControl, x );
	}

	__asm fldcw usControl
}


void _set_errno( unsigned int uerr )
{
	switch( uerr )
	{
	case _DOMAIN:
		errno = EDOM;
		break;

	case _SING:
	case _OVERFLOW:
		errno = ERANGE;
		break;

	default:
		break;
	}
}

int _setfflag( int nflag )
{
	int noldflag = _fastflag;
	_fastflag = nflag;
	return noldflag;
}




//-----------------------------------------------------------------------------
//                           IEEE 754/854要求函数
//-----------------------------------------------------------------------------

// 改变符号函数,IEEE 754 Std
// 直接操作数的格式,没有使用FCHS指令
// 这里改变符号的方式也与fabs不同,fabs一部分使用了FCHS指令
double _chgsign( double x )
{
	FP_DOUBLE  *px = (FP_DOUBLE*)&x;
	px->nSign = !(px->nSign);
	return x;
}

// 拷贝符号函数,IEEE 754 Std,注意没有做任何检测
double _copysign( double x, double y )
{
	FP_DOUBLE  *px = (FP_DOUBLE*)&x;
	FP_DOUBLE  *py = (FP_DOUBLE*)&y;

	px->nSign = py->nSign;

	return x;
}


double _scalb( double x, long exp )
{
	ldexp( x, exp );
}

double _hypot( double x, double y )
{
	return _hypot_k( x, y, _FpCodeHypot );
}

double _hypot_k( double x, double y, int nOperation )
{
	unsigned short  usControl;
	FP_DOUBLE		*px = (FP_DOUBLE*)&x;
	FP_DOUBLE		*py = (FP_DOUBLE*)&y;
	double			maxxy, rv;
	int				nexprv, nexpmax;

	// 装载控制寄存器值: 屏蔽所有异常/64位精度/最近舍入
	usControl = _ctrlfp( 0x133F, 0xFFFF );

	if( px->nExponent == 0x7FF || py->nExponent == 0x7FF )
	{
		// x或y是SNaN
		if( ( px->nFraction & 0x8000000000000 == 0 && px->nFraction != 0 ) ||
			( py->nFraction & 0x8000000000000 == 0 && py->nFraction != 0 ) )
		{
			return _except2( EXCEPTION_BITS_INVALID_OPERATION, nOperation, x, y, x+y, usControl );
		}

		// x或y是QNaN
		if( ( px->nFraction & 0x8000000000000 == 0x8000000000000 ) ||
			( py->nFraction & 0x8000000000000 == 0x8000000000000 ) )
		{
			return _handle_qnan2( nOperation, x, y, usControl );
		}

		_ctrlfp( usControl, 0xFFFF );

		return *(double*)&_PInfinite;
	}
	else
	{
		// 处理符号
		if( x < 0.0 )
			x = -x;
		if( y < 0.0 )
			y = -y;

		if( x >= y )
			maxxy = x;
		else
			maxxy = y;

		if( maxxy == 0.0 )
		{
			_ctrlfp( usControl, 0xFFFF );
			return 0.0;
		}
		
		// 规范化x,y再计算
		x = x/maxxy;
		y = y/maxxy;
		rv = sqrt( x*x + y*y );

		// 恢复原值
		rv		= _decomp( rv, &nexprv );
		maxxy	= _decomp( maxxy, &nexpmax );

		rv		= rv*maxxy;
		nexprv	= nexprv + nexpmax + GetDoubleExponent( rv );
		
		if( nexprv > 1024 )
		{
			rv = _set_exp( rv, nexprv - 1536 );
			return _except2( EXCEPTION_BITS_INEXACT_RESULT|EXCEPTION_BITS_OVERFLOW, nOperation, x, y, rv, usControl );
		}

		if( nexprv < -1021 )
		{
			rv = _set_exp( rv, nexprv + 1536 );
			return _except2( EXCEPTION_BITS_INEXACT_RESULT|EXCEPTION_BITS_UNDERFLOW, nOperation, x, y, rv, usControl );
		}

		rv = _set_exp( rv, nexprv );

		// 没有屏蔽精度异常
		if( usControl & 0x20 == 0 )
		{
			return _except2( EXCEPTION_BITS_INEXACT_RESULT, nOperation, x, y, rv, usControl );
		}
	}

	return rv;
}

double _cabs( struct _complex z )
{
	return _hypot_k( z.x, z.y, _FpCodeCabs );
}


// 这个函数不能理解为log2(x),因为它不检查符号,它等效于log2(fabs(x))
// 就是返回x的指数,即frexp(x,&exp)中exp-1
double _logb( double x )
{
	unsigned short  usControl;
	FP_DOUBLE		*px = (FP_DOUBLE*)&x;
	int				nexp;

	// 装载控制寄存器值: 屏蔽所有异常/64位精度/负无穷舍入
	usControl = _ctrlfp( 0x173F, 0xFFFF );

	if( px->nExponent == 0x7FF )
	{
		// 无穷和NaN
		switch( _sptype( x ) )
		{
		// PINF,NINF
		case 1:
		case 2:
			_ctrlfp( usControl, 0xFFFF );
			return x;

		// QNaN
		case 3:
			return _handle_qnan1( _FpCodeLogb, x, usControl );

			// SNaN
		default:
			return _except1( EXCEPTION_BITS_INVALID_OPERATION, _FpCodeLogb, x, x+1.0, usControl );
		}
	}
	else
	{
		if( x == 0.0 )
		{
			rv = *(double*)&_NInfinite;
			return _except1( EXCEPTION_BITS_DIVIDE_BY_ZERO, _FpCodeLogb, x, rv, usControl );
		}
		
		_decomp( x, &nexp );
		_ctrlfp( usControl, 0xFFFF );
	}

	return nexp - 1;
}

int GetDoubleExponent( double x )
{
	FP_DOUBLE		*px = (FP_DOUBLE*)&x;
	return px->nExponent;
}

double _nextafter( double x, double y )
{
	unsigned short  usControl;
	FP_DOUBLE		*px = (FP_DOUBLE*)&x;
	FP_DOUBLE		*py = (FP_DOUBLE*)&y;

	// 装载控制寄存器值: 屏蔽所有异常/64位精度/负无穷舍入
	usControl = _ctrlfp( 0x173F, 0xFFFF );
	
	// x或y是SNaN
	if( ( ( px->nExponent == 0x7FF )&&( px->nFraction != 0 )&&( px->nFraction & 0x8000000000000 == 0 ) ) ||
		( ( py->nExponent == 0x7FF )&&( py->nFraction != 0 )&&( py->nFraction & 0x8000000000000 == 0 ) ) )
	{
		return _except2( EXCEPTION_BITS_INVALID_OPERATION, _FpCodeNextafter, x, y, x+y, usControl ); 
	}

	// x或y是QNaN
	if( ( ( px->nExponent == 0x7FF )&&( px->nFraction != 0 ) ) ||
		( ( py->nExponent == 0x7FF )&&( py->nFraction != 0 ) ) )
	{
		return _handle_qnan2( _FpCodeNextafter, x, y, usControl ); 
	}

	// IEEE约定
	if( x == y )
	{
		_ctrlfp( usControl, 0xFFFF );
		return x;
	}

	//??

}



//-----------------------------------------------------------------------------
//                           x87 FPU界面函数
//-----------------------------------------------------------------------------

unsigned int _control87( unsigned int newset, unsigned int mask )
{
    unsigned short  usControl;
    unsigned int	uMask;

    // 读取FPU控制字
    __asm    fstcw   usControl

    uMask = ConvertControlToMask( usControl );
    uMask = ( newset & mask ) | ( ~mask & uMask );
    usControl = ConvertMaskToControl( uMask );

    // 设置FPU控制字
    __asm    fldcw    usControl
    return uMask;
}

// 除了_EM_DENORMAL被清除之外，与_control87一样
unsigned int _controlfp( unsigned int newset, unsigned int mask )
{
    return _control87( newset, mask & (~_EM_DENORMAL) );
}

// x87 FPU的控制寄存器与Visual C++ 6.0的标识位定义不同,需要两个函数进行转换
// 将控制寄存器标志转换为库标志,参见ConvertMaskToControl
unsigned int ConvertControlToMask( unsigned short usControl )
{
    unsigned int	uMask = 0;
    unsigned short  usFp;

	// 5个异常标志位

     // 屏蔽非法操作异常,bit0
    if( usControl & EM_INVALID_OPERATION )
        uMask |= _EM_INVALID;

     // 屏蔽被0除异常,bit2
    if( usControl & EM_ZERO_DIVIDE )
        uMask |= _EM_ZERODIVIDE;

     // 屏蔽上溢异常,bit3
    if( usControl & EM_OVERFLOW )
        uMask |= _EM_OVERFLOW;

     // 屏蔽下溢异常,bit4
    if( usControl & EM_UNDERFLOW )
        uMask |= _EM_UNDERFLOW;

     // 屏蔽精度异常
    if( usControl & EM_PRECISION )
        uMask |= _EM_INEXACT;

	// 注意次序,可能出于优化考虑,将不常用的异常位放在后面处理

    // 屏蔽弱规范数异常,bit1
    if( usControl & 0x0002 )
        uMask |= _EM_DENORMAL;


    // 舍入控制,bit10-11
    usFp = usControl & 0x0C00;

    // 向正向无穷大舍入,10
    if( usFp == 0x0800 )
        uMask |= _RC_UP;

    // 向最近舍入,00
    if( usFp == 0x0000 )
        uMask |= _RC_NEAR;

    // 向负向无穷大舍入,01
    if( usFp == 0x0400 )
        uMask |= _RC_DOWN;

    // 向0舍入,截断,11
    if( usFp == 0x0C00 )
        uMask |= _RC_CHOP;


    // 精度控制,bit8-9
    usFp = usControl & 0x0300;

    // 单精度,00
    if( usFp == 0x0000 )
        uMask |= _PC_24;

    // 双精度,10
    if( usFp == 0x0200 )
        uMask |= _PC_53;

    // 扩展精度,11
    if( usFp == 0x0300 )
        uMask |= _PC_64;

	
    // 无穷大控制,bit12
	// 这个标志仅用于兼容287,后来的芯片无用
    if( usControl & 0x1000 )
        uMask |= _IC_AFFINE;

    return uMask;
}

// x87 FPU的控制寄存器与Visual C++ 6.0的标识位定义不同,需要两个函数进行转换
// 将库标志转换为控制寄存器标志,参见ConvertControlToMask
unsigned short ConvertMaskToControl( unsigned int uMask )
{
    unsigned short  usControl = 0;
    unsigned int	uFp;

	// 5个异常位

    // 屏蔽非法操作异常,bit0
    if( uMask & _EM_INVALID )
        usControl |= EM_INVALID_OPERATION;

    // 屏蔽被0除异常,bit2
    if( uMask & _EM_ZERODIVIDE )
        usControl |= EM_ZERO_DIVIDE;

    // 屏蔽上溢异常,bit3
    if( uMask & _EM_OVERFLOW )
        usControl |= EM_OVERFLOW;

    // 屏蔽下溢异常,bit4
    if( uMask & _EM_UNDERFLOW )
        usControl |= EM_UNDERFLOW;

    // 屏蔽精度异常,bit5
    if( uMask & _EM_INEXACT )
        usControl |= EM_PRECISION;

    // 屏蔽格式规范异常,bit1
    if( uMask & _EM_DENORMAL )
        usControl |= EM_DENORMAL_OPERAND;


    // 舍入控制,bit10-11
    uFp = uMask & _MCW_RC;

    // 向正向无穷大舍入,10
    if( uFp == _RC_UP )
		usControl |= RC_UP;

    // 最近舍入,00
    if( uFp == _RC_NEAR )
        usControl |= RC_NEAREST;

    // 向负向无穷大舍入,01
    if( uFp == _RC_DOWN )
        usControl |= RC_DOWN;

    // 向0舍入，截断,11
    if( uFp == _RC_CHOP )
        usControl |= RC_TRUNCATE;


    // 精度控制,bit8-9
    uFp = uMask & _MCW_PC;

    // 扩展精度,11
    if( uFp == _PC_64 )
        usControl |= PC_EX_DOUBLE_64;

    // 双精度,10
    if( uFp == _PC_53 )
        usControl |= PC_DOUBLE_53;

    // 单精度,00
    if( uFp == _PC_24 )
        usControl |= PC_FLOAT_24;

    // 无穷大控制,兼容287
    if( uMask & _IC_AFFINE )
        usControl |= 0x1000;

    return usControl;
}

// #define _clear87 _clearfp
unsigned int _clearfp()
{
    unsigned short usStatus;

    __asm fnstsw usStatus
    __asm fnclex

    return ConvertStatusToMask( usStatus );
}

// #define _status87 _statusfp
unsigned int _statusfp()
{
    unsigned short usStatus;

    __asm wait
    __asm fnstsw usStatus

    return ConvertStatusToMask( usStatus );
}

unsigned int _clrfp()
{
    unsigned short usStatus;

    __asm fnstsw usStatus
    __asm fnclex

    return (unsigned int)usStatus;
}

unsigned int _statfp()
{
	unsigned short usStatus;

    __asm wait
    __asm fnstsw    usStatus

	return (unsigned int)usSatus;
}

// setdefaultprecision有两个版本,一个设置_PC_53,一个设置_PC_64
unsigned int setdefaultprecision()
{
	return _controlfp( _MCW_PC, _PC_53 );
}

unsigned int ConvertStatusToMask( unsigned short usStatus )
{
    unsigned int uMask = 0;

    // 非法操作异常
    if( usStatus & 0x0001 )
        uMask |= _SW_INVALID;

    // 被0除异常
    if( usStatus & 0x0004 )
        uMask |= _SW_ZERODIVIDE;

    // 上溢异常
    if( usStatus & 0x0008 )
        uMask |= _SW_OVERFLOW;

    // 下溢异常
    if( usStatus & 0x0010 )
        uMask |= _SW_UNDERFLOW;

    // 精度异常
    if( usStatus & 0x0020 )
        uMask |= _SW_INEXACT;

    // 格式不规范异常
    if( usStatus & 0x0002 )
        uMask |= _SW_DENORMAL;

    return uMask;
}

void _fpreset( void )
{
	// EXCEPTION_POINTERS中是CONTEXT86结构
	PEXCEPTION_POINTERS	pException = (PEXCEPTION_POINTERS)_pxcptinfoptrs;
	FLOATING_SAVE_AREA	*pFpuSave;

	__asm finit
	setdefaultprecision();

	if( pException != NULL )
	{
		// 如果保存了FPU状态
		if( pException->ContextRecord->ContextFlags & CONTEXT_FLOATING_POINT )
		{
			pFpuSave = &(pException->ContextRecord->FloatSave);
			pFpuSave->StatusWord= 0;
			pFpuSave->TagWord	= 0xFFFF;
		}
	}
}


// 与_contorl87一样,但没有转换发生,直接使用原始定义
unsigned short _ctrlfp( unsigned short usnew, unsigned short usmask )
{
	unsigned short usControlOld;
	unsigned short usControl;

	__asm wait
	__asm fnstcw usControlOld

	usControl = ( usnew & usmask ) | ( ~usmask & usControlOld );

	__asm fldcw usControl

	return usControlOld;
}

// 这个函数的处理说明,Visual C++ 6.0需要屏蔽所有异常,且一般不处理精读异常
void _load_CW( unsigned usControl )
{
	// 清除精度控制位
	usControl = usControl & 0x300;
	// 屏蔽所有异常
	usControl = usControl | 0x7F;

	__asm fldcw usControl
}

//-----------------------------------------------------------------------------
//                           分类函数
//-----------------------------------------------------------------------------

// 在x87 FPU中有FXAM指令,但VC_FP_LIB没有使用这个指令,而是直接解释double格式
// 除了与0比较时使用了FLD/FCOM/FNSTSW指令之外,没有使用任何其他浮点指令

int _fpclass( double x )
{
	FP_DOUBLE *px = (FP_DOUBLE*)&x;

	// 指数全1
	if( px->nExponent == 0x7FF )		
	{
		// 正负无穷,SNaN,QNaN

		switch( _sptype(x) )
		{
		case 1:
			return _FPCLASS_PINF;
		case 2:
			return _FPCLASS_NINF;
		case 3:
			return _FPCLASS_QNAN;
		default: // 4
			return _FPCLASS_SNAN;
		}
	}
	else
	{
		// 正数/负数,+0/-0,正弱规范数/负弱规范数

		// 正0/负0,正弱规范数/负弱规范数
		if( px->nExponent == 0 )
		{
			// 正弱规范数/负弱规范数
			if( px->nFraction != 0 )
			{
				if( px->nSign == 0 )
					return _FPCLASS_PD;
				else
					return _FPCLASS_ND;
			}
		}
	}

	// 正数/负数/+0/-0,
	if( x > 0 )
		return _FPCLASS_PN;
	else if( x < 0 )
		return _FPCLASS_NN;
	else
	{
		// +0/-0
		if( px->nSign == 0 )
			return _FPCLASS_PZ;
		else
			return _FPCLASS_NZ;
	}
}

// 所谓有限数就是指数不是全1(最大值)的数,包括常见数/0和弱规范数
int _finite( double x )
{
	FP_DOUBLE *px = (FP_DOUBLE*)&x;

	if( px->nExponent == 0x7FF )
		return 0;
	else
		return 1;
}

// NaN:指数全1,尾数非0,参见_sptype
int _isnan( double x )
{
	FP_DOUBLE *px = (FP_DOUBLE*)&x;

	// 尾数高位0是SNaN
	if( ( FP_DOUBLE_HIGH4(x)&0x7FF80000==0x7FF00000) && (px->nFraction!=0) )
		return 1;

	// 尾数高位1是QNaN
	if( ( FP_DOUBLE_HIGH4(x) & 0x7FF80000 == 0x7FF80000 ) )
		return 1;

//	以上代码将NaN分为QNaN和SNaN处理,如果一起处理更有效:
//	if( ( FP_DOUBLE_HIGH4(x)&0x7FF0000==0x7FF00000) && (px->nFraction!=0) )
//		return 1;

	return 0;
}

// 指数全1的数的类型判断:QNaN/SNaN/PINF/NINF
int _sptype( double x )
{
	FP_DOUBLE *px = (FP_DOUBLE*)&x;

	// INF:尾数0

	// PINF:符号位0
	if( (FP_DOUBLE_HIGH4(x)==0x7FF00000) && (px->nFraction==0) ) 
		return 1;

	// NINF:符号位1
	if( (FP_DOUBLE_HIGH4(x)==0xFFF00000) && (px->nFraction==0) ) 
		return 2;

	// NaN:尾数非0

	// QNaN:尾数最高位1
	if( FP_DOUBLE_HIGH4(x) & 0x7FF80000 == 0x7FF80000 ) 
		return 3;

	// SNaN:尾数最高位0
	if( (FP_DOUBLE_HIGH4(x)&0x7FF80000==0x7FF00000) && (px->nFraction!=0) )
		return 4;

	// 似乎不可能
	return 0;
}

//-----------------------------------------------------------------------------
//                           浮点库函数
//-----------------------------------------------------------------------------

// x87 FPU有一条指令FABS干这事,但fabs没有使用这条指令
double fabs( double x )
{
	unsigned short  usControl;
	FP_DOUBLE		*px = (FP_DOUBLE*)&x;

	// 装载缺省控制寄存器值: 屏蔽所有异常/64位精度/最近舍入
	usControl = _ctrlfp( 0x133F, 0xFFFF ); 

	// 有限值,设置+符号
	if( px->nExponent != 0x7FF )
	{
		// 设置+符号
		px->nSign = 0;
		// 恢复控制寄存器值
		_ctrlfp( usControl, 0xFFFF ); 
		return x;
	}

	// 指数0x7FF
	switch( _sptype( x ) )
	{
		case 1:		// _FPCLASS_PINF
			// 已经是正值,恢复控制寄存器值
			_ctrlfp( usControl, 0xFFFF ); 
			break;

		case 2:		// _FPCLASS_NINF
			// 设置+符号,恢复控制寄存器值,改变符号
			_ctrlfp( usControl, 0xFFFF ); 
			__asm fld x
			__asm fchs
			__asm ret
			break;

		case 3:		// _FPCLASS_QNAN,正常
			x = _handle_qnan1( _FpCodeFabs, x, usControl );
			break;

		default:	// _FPCLASS_SNAN,触发Invalid exception
			x = _except1( EXCEPTION_BITS_INVALID_OPERATION, _FpCodeFabs, x, x+1.0L, usControl );
			break;
	}

	return x;
}


double sin( double x )
{
	// 当前控制寄存器值
	unsigned short usControl;
	FP_DOUBLE	  *px = (FP_DOUBLE*)&x;

	int bExponentIs0x7FF;

	_fload_withFB( x, &bExponentIs0x7FF );

	__asm fstcw usControl

	if( !bExponentIs0x7FF )
	{
		// 使用缺省控制寄存器值,0x27F,屏蔽所有异常/53位精度/最近舍入
		if( usControl != _DEFAULT_CW_in_mem )
			__asm fldcw _DEFAULT_CW_in_mem

		__asm
		{
			fsin
			fstsw ax
			sahf
			jnp complete
			
			// C2=1,超出范围
			fld tbyte ptr [_pi_by_2_to_61]
			fxch st(1)

		again:
			fprem1
			fstsw
			sahf
			jpe again

			fstp st(1)
			fsin
	complete:
		}

		// sin定义域是整个实数,故有限值不可能出现异常
		if( _fastflag )
			_fast_exit( usControl );
		else
			_math_exit( "sin", _FpCodeSin, usControl, x );
	}
	else
	{
		//  这部分的处理与FSIN指令类似,加入这些代码似乎唯一的用处就是在NaN参与运算时调用_matherr()
		// 特殊值: NaN=>NaN,无穷=>*

		if( px->nFraction == 0 )
		{
			//  无穷=>*(实际是QNaN)

			// 清空数据寄存器
			__asm fstp st(0)

			// 设置返回值
			__asm fld _indefinite
		}
		else
		{
			// NaN=>NaN,数据在ST(0)中
			_convertTOStoQNaN( x )

			if( _fastflag )
				_fast_exit( usControl );
			else
				_startOneArgErrorHandling( "sin", _FpCodeSin, _DOMAIN, usControl, x );
		}
	}
}

double cos( double x )
{
	// 当前控制寄存器值
	unsigned short usControl;
	FP_DOUBLE	  *px = (FP_DOUBLE*)&x;

	int bExponentIs0x7FF;

	_fload_withFB( x, &bExponentIs0x7FF );

	__asm fstcw usControl

	if( !bExponentIs0x7FF )
	{
		// 使用缺省控制寄存器值,0x27F,屏蔽所有异常/53位精度/最近舍入
		if( usControl != _DEFAULT_CW_in_mem )
			__asm fldcw _DEFAULT_CW_in_mem

		__asm
		{
			fcos
			fstsw
			sahf
			jnp complete
			
			// C2=1,超出范围
			fld tbyte ptr [_pi_by_2_to_61]
			fxch st(1)

		again:
			fprem1
			fstsw
			sahf
			jpe again

			fstp st(1)
			fcos

	complete:
		}

		if( _fastflag )
			_fast_exit( usControl );
		else
			_math_exit( "cos", usControl, x );
	}
	else
	{
		// 特殊值: NaN=>NaN,无穷=>*
		if( px->nFraction == 0 )
		{
			//  无穷=>*(实际是QNaN)

			// 清空数据寄存器
			__asm fstp st(0)

			// 设置返回值
			__asm fld _indefinite
		}
		else
		{
			// NaN=>NaN,数据在ST(0)中
			_convertTOStoQNaN( x )

			if( _fastflag )
				_fast_exit( usControl );
			else
				_startOneArgErrorHandling( "cos", _FpCodeCos, _DOMAIN, usControl, x );
		}
	}
}


double tan( double x )
{
	// 当前控制寄存器值
	unsigned short usControl;
	FP_DOUBLE	  *px = (FP_DOUBLE*)&x;

	int bExponentIs0x7FF;

	_fload_withFB( x, &bExponentIs0x7FF );

	__asm fstcw usControl

	if( !bExponentIs0x7FF )
	{
		// 使用缺省控制寄存器值,0x27F,屏蔽所有异常/53位精度/最近舍入
		if( usControl != _DEFAULT_CW_in_mem )
			__asm fldcw _DEFAULT_CW_in_mem

		__asm
		{
			fptan
			fstsw
			sahf
			jnp complete
			
			// C2=1,超出范围
			fld tbyte ptr [_pi_by_2_to_61]
			fxch st(1)

		again:
			fprem1
			fstsw
			sahf
			jpe again

			fstp st(1)
			fptan

	complete:
		}

		if( _fastflag )
			_fast_exit( usControl );
		else
			_math_exit( "tan", _FpCodeTan, usControl, x );
	}
	else
	{
		// 特殊值: NaN=>NaN,无穷=>*
		if( px->nFraction == 0 )
		{
			//  无穷=>*(实际是QNaN)

			// 清空数据寄存器
			__asm fstp st(0)

			// 设置返回值
			__asm fld _indefinite
		}
		else
		{
			// NaN=>NaN,数据在ST(0)中
			_convertTOStoQNaN( x )

			if( _fastflag )
				_fast_exit( usControl );
			else
				_startOneArgErrorHandling( "tan", _FpCodeTan, _DOMAIN, usControl, x );
		}
	}
}


double atan( double x )
{
	// 当前控制寄存器值
	unsigned short usControl;
	FP_DOUBLE	  *px = (FP_DOUBLE*)&x;

	int bExponentIs0x7FF;

	_fload_withFB( x, &bExponentIs0x7FF );

	__asm fstcw usControl

	if( !bExponentIs0x7FF )
	{
		// 使用缺省控制寄存器值,0x27F,屏蔽所有异常/53位精度/最近舍入
		if( usControl != _DEFAULT_CW_in_mem )
			__asm fldcw _DEFAULT_CW_in_mem

		// 注意fpatan指令有些特别,它完成ST(1)=atan[ST(0)/ST(1)],然后弹出ST(0)
		__asm
		{
			fld1
			fpatan
		}

		if( _fastflag )
			_fast_exit( usControl );
		else
			_math_exit( "atan", _FpCodeAtan, usControl, x );
	}
	else
	{
		// +-无穷
		if( px->nFraction == 0 )
		{
			// 清空数据寄存器
			__asm fstp st(0)

			// 设置返回值,PI/2
			__asm fld _piby2
			if( px->nSign )
				__asm fchs

		}
		else
		{
			// NaN=>NaN,数据在ST(0)中
			_convertTOStoQNaN( x )

			if( _fastflag )
				_fast_exit( usControl );
			else
				_startOneArgErrorHandling( "atan", _FpCodeAtan, _DOMAIN, usControl, x );
		}
	}
}

double asin( double x )
{
	// 当前控制寄存器值
	unsigned short usControl;
	FP_DOUBLE	  *px = (FP_DOUBLE*)&x;

	int bExponentIs0x7FF;
	_fload_withFB( x, &bExponentIs0x7FF );

	__asm fstcw usControl

	if( !bExponentIs0x7FF )
	{
		// 使用缺省控制寄存器值,0x27F,屏蔽所有异常/53位精度/最近舍入
		if( usControl != _DEFAULT_CW_in_mem )
			__asm fldcw _DEFAULT_CW_in_mem

		if( x < 1.0 && x > -1.0 )
		{
			// atan( x/sqrt(1-x*x) )
			__asm
			{
				// 计算(1.0+x)(1.0-x),即cos**2
				fld1
				fadd st, st(1)

				fld1
				fsub st, st(2)

				fmulp st(1), st

				// 计算cos
				fsqrt

				// 得到最终值
				fpatan
			}
		}

		if( x == 1.0 || x == -1.0 )
		{
			__asm fld _piby2
			if( px->nSign )
				__asm fchs
		}

		if( x > 1.0 || x < -1.0 )
		{
			__asm fld _indefinite

			if( _fastflag )
				_fast_exit( usControl );
			else
				_startOneArgErrorHandling( "asin", _FpCodeAsin, _DOMAIN, usControl, x );
		}

		if( _fastflag )
			_fast_exit( usControl );
		else
			_math_exit( "asin", _FpCodeAsin, usControl, x );
	}
	else
	{
		// 特殊值: NaN=>NaN,无穷=>*
		_convertTOStoQNaN( x )

		if( _fastflag )
			_fast_exit( usControl );
		else
			_startOneArgErrorHandling( "asin", _FpCodeAsin, _DOMAIN, usControl, x );
	}
}


double acos( double x )
{
	// 当前控制寄存器值
	unsigned short usControl;
	FP_DOUBLE	  *px = (FP_DOUBLE*)&x;

	int bExponentIs0x7FF;

	_fload_withFB( x, &bExponentIs0x7FF );

	__asm fstcw usControl

	if( !bExponentIs0x7FF )
	{
		// 使用缺省控制寄存器值,0x27F,屏蔽所有异常/53位精度/最近舍入
		if( usControl != _DEFAULT_CW_in_mem )
			__asm fldcw _DEFAULT_CW_in_mem

		if( x < 1.0 && x > -1.0 )
		{
			// atan( x/sqrt(1-x*x) )
			__asm
			{
				// 计算(1.0+x)(1.0-x),即sin**2
				fld1
				fadd st, st(1)

				fld1
				fsub st, st(2)

				fmulp st(1), st

				// 计算sin
				fsqrt

				// 得到最终值
				fxch
				fpatan
			}
		}

		if( x == 1.0 || x == -1.0 )
		{
			if( x > 0.0 )
				__asm fldz
			else
				__asm fldpi
		}

		if( x > 1.0 || x < -1.0 )
		{
			__asm fld _indefinite

			if( _fastflag )
				_fast_exit( usControl );
			else
				_startOneArgErrorHandling( "acos", _FpCodeAcos, _DOMAIN, usControl, x );
		}

		if( _fastflag )
			_fast_exit( usControl );
		else
			_math_exit( "acos", _FpCodeAcos, usControl, x );
	}
	else
	{
		// 特殊值: NaN=>NaN,无穷=>*
		_convertTOStoQNaN( x )

		if( _fastflag )
			_fast_exit( usControl );
		else
			_startOneArgErrorHandling( "acos", _FpCodeAcos, _DOMAIN, usControl, x );
	}
}


// x 已经在ST0中
// _ftol用于int k = 4.5等场合
int _ftol( double x )
{
	int irv;
	unsigned short usControl, usControlOld;

	// 保存控制寄存器
	__asm fstcw usControlOld

	// 重设控制寄存器, RC=>11, 向0(截断)舍入
	usControl = usControlOld | 0x0C00;
	__asm fldcw usControl

	// 转换数据
	__asm fistp irv

	// 恢复控制寄存器
	__asm fldcw usControlOld

	return irv;
}


// 极其啰嗦,取整函数,如何取整由RC模式决定
double _frnd( double x )
{
	double rv;

	__asm
	{
		fld x
		frndint
		fstp rv
	}
	return rv;
}

/*

	__frnd:
		push    ebp
		mov     ebp,esp
		
		sub     esp,8

		push    ebx
		push    esi
		push    edi
		
		fld     qword ptr [ebp+8]
		frndint

		fstp    qword ptr [ebp-8]
		fld     qword ptr [ebp-8]

		pop     edi
		pop     esi
		pop     ebx
		
		mov     esp,ebp
		pop     ebp
		ret
只需三条指令
__declspecl(naked) double _frnd( double x )
{
	__asm
	{
		fld qword ptr [esp+4]
		frndint
		ret
	}
}
*/


double floor( double x )
{
	unsigned short  usControl;
	FP_DOUBLE		*px = (FP_DOUBLE*)&x;
	double			rv;

	// 装载控制寄存器值: 屏蔽所有异常/64位精度/负无穷舍入
	usControl = _ctrlfp( 0x173F, 0xFFFF );

	if( px->nExponent == 0x7FF )
	{
		// 无穷和NaN
		switch( _sptype( x ) )
		{
		// PINF,NINF
		case 1:
		case 2:
			_ctrlfp( usControl, 0xFFFF );
			return x;

		// QNaN
		case 3:
			return _handle_qnan1( _FpCodeFloor, x, usControl );

			// SNaN
		default:
			return _except1( EXCEPTION_BITS_INVALID_OPERATION, _FpCodeFloor, x, x+1.0, usControl );
		}
	}
	else
	{
		// 关键操作,向负无穷舍入
		rv = _frnd( x );

		if( x == rv )
			_ctrlfp( usControl, 0xFFFF );
		else
		{
			// 有舍入操作
			if( usControl & 0x20 )
				// 屏蔽了精度异常
				_ctrlfp( usControl, 0xFFFF );
			else
				// 没有屏蔽精度异常,需要异常处理
				return _except1( EXCEPTION_BITS_INVALID_OPERATION, _FpCodeFloor, x, x+1.0, usControl );
		}
	}

	return rv;
}

double ceil( double x )
{
	unsigned short  usControl;
	FP_DOUBLE		*px = (FP_DOUBLE*)&x;
	double			rv;

	// 装载控制寄存器值: 屏蔽所有异常/64位精度/正无穷舍入
	usControl = _ctrlfp( 0x1B3F, 0xFFFF );

	if( px->nExponent == 0x7FF )
	{
		// 无穷和NaN
		switch( _sptype( x ) )
		{
		// PINF,NINF
		case 1:
		case 2:
			_ctrlfp( usControl, 0xFFFF );
			return x;

		// QNaN
		case 3:
			return _handle_qnan1( _FpCodeCeil, x, usControl );

			// SNaN
		default:
			return _except1( EXCEPTION_BITS_INVALID_OPERATION, _FpCodeCeil, x, x+1.0, usControl );
		}
	}
	else
	{
		// 关键操作,向正无穷舍入
		rv = _frnd( x );

		if( x == rv )
			_ctrlfp( usControl, 0xFFFF );
		else
		{
			// 有舍入操作
			if( usControl & 0x20 )
				// 屏蔽了精度异常
				_ctrlfp( usControl, 0xFFFF );
			else
				// 没有屏蔽精度异常,需要异常处理
				return _except1( EXCEPTION_BITS_INVALID_OPERATION, _FpCodeCeil, x, x+1.0, uscontrol );
		}
	}

	return rv;
}

// 将指数设置为n+1023-1,没有考虑数的类型
double _set_exp( double x, int n )
{
	FP_DOUBLE *px = (FP_DOUBLE*)&x;
	px->nExponent = n + 1023 - 1;
	return x;
}


// 将x分解为y*2**n,y位于[0.5,1.0)
double frexp( double x, int *expptr )
{
	unsigned short  usControl;
	FP_DOUBLE		*px = (FP_DOUBLE*)&x;
	double			rv;

	// 装载控制寄存器值: 屏蔽所有异常/64位精度/最近舍入
	usControl = _ctrlfp( 0x133F, 0xFFFF );

	if( px->nExponent == 0x7FF )
	{
		*expptr = -1;

		// 无穷和NaN
		switch( _sptype( x ) )
		{
		// PINF,NINF
		case 1:
		case 2:
			return _except1( EXCEPTION_BITS_INVALID_OPERATION, _FpCodeFrexp, x, _d_ind, usControl );

		// QNaN
		case 3:
			return _handle_qnan1( _FpCodeFrexp, x, usControl );

			// SNaN
		default:
			return _except1( EXCEPTION_BITS_INVALID_OPERATION, _FpCodeFrexp, x, x+1.0, uscontrol );
		}
	}
	else
	{
		rv = _decomp( x, expptr );
		_ctrlfp( usControl, 0xFFFF );
	}

	return rv;
}

// 将浮点数分解为无偏指数和尾数,尾数区间是[0,1]
// 注意_decomp没有考虑INF和NaN
double _decomp( double x, int *expptr )
{
	FP_DOUBLE  *px = (FP_DOUBLE*)&x;
	double		rv;
	int			sign;

	// 0
	if( x == 0.0 )
	{
		*expptr = 0;
		return 0.0;
	}

	// Denormals,通过减小指数移动尾数直至可以返回为止
	if( px->nExponent == 0 && px->nFraction != 0 )
	{
		// 正常指数是隐含位的基
		*expptr = -1021;

		// 保留符号
		sign = px->nSign;

		// 将Denormals转化为正常数

		// 移位直至找到一个1设置在隐含位上
		while( px->nExponent & 1 == 0 )
		{
			*(unsigned __int64*)&x = *(unsigned __int64*)&x << 1;
			*expptr --;
		}

		px->nSign = sign;
		rv = _set_exp( x, 0 );

		return rv;
	}
	else
	{
		// 正常数
		rv = _set_exp( x, 0 );
		*expptr = px->nExponent - 1023 + 1;
	}

	return rv;
}


// 计算x*2**exp
double ldexp( double x, int exp )
{
	unsigned short  usControl;
	FP_DOUBLE		*px = (FP_DOUBLE*)&x;
	double			rv;
	int				xe, re;
	unsigned int	te;

	// 装载控制寄存器值: 屏蔽所有异常/64位精度/最近舍入
	usControl = _ctrlfp( 0x133F, 0xFFFF );

	if( px->nExponent == 0x7FF )
	{
		// 无穷和NaN
		switch( _sptype( x ) )
		{
		// PINF,NINF,INF*2**ANY => INF
		case 1:
		case 2:
			_ctrlfp( usControl, 0xFFFF );
			return x;

		// QNaN
		case 3:
			return _handle_qnan2( _FpCodeLdexp, x, exp, usControl );

			// SNaN
		default:
			return _except2( EXCEPTION_BITS_INVALID_OPERATION, _FpCodeLdexp, x, exp, x+1.0, uscontrol );
		}
	}

	// 0.0*ANY => 0.0
	if( x == 0.0 )
	{
		_ctrlfp( usControl, 0xFFFF );
		return x;
	}

	// 分解x.本函数未作运算,只是将exp附加到x的指数上即可
	rv = _decomp( x, &xe );

	// 判断exp的范围
	if( exp >= 0 )
		te = exp;
	else
		te = -exp;

	if( te <= INT_MAX )
		re = xe + exp;
	else
		re = exp;	// exp超出范围,不能计算,触发异常处理

	// (2560,+INF),指数太大,认为是无穷,无穷的符号由x决定,2560=1024+1536
	if( re > 2560 )
	{
		// PINF
		rv = _copysign( _d_inf, rv );
		return _except2( 0x11, _FpCodeLdexp, x, exp, rv, usControl );
	}

	// (1024,2560],指数平移1536,7FF-3FF=400(1024)
	if( re > 1024 )
	{
		rv = _set_exp( rv, re - 1536 );
		return _except2( EXCEPTION_BITS_INEXACT_RESULT|EXCEPTION_BITS_OVERFLOW, _FpCodeLdexp, x, exp, rv, usControl );
	}

	// (-INF,2557),太小,认为是0,2557=1536+1021
	if( re < -2557 )
	{
		return _except2( EXCEPTION_BITS_INEXACT_RESULT|EXCEPTION_BITS_UNDERFLOW, _FpCodeLdexp, x, exp, rv*0.0, usControl );
	}

	// [2557,1021),指数平移1536,过小
	if( re < -1021 )
	{
		rv = _set_exp( rv, re + 1536 );
		return _except2( EXCEPTION_BITS_INEXACT_RESULT|EXCEPTION_BITS_UNDERFLOW, _FpCodeLdexp, x, exp, rv, usControl );
	}

	// 一般数
	rv = _set_exp( rv, re );
	_ctrlfp( usControl, 0xFFFF );

	return rv;
}


double modf( double x, double *intptr )
{
	unsigned short  usControl;
	FP_DOUBLE		*px = (FP_DOUBLE*)&x;
	double			rv;

	// 保存原先的,再设置新的.没有必要两次调用,因为_ctrlfp返回原先的,不同于_control87
	usControl = _ctrlfp( 0, 0 );
	// 屏蔽所有异常/截断舍入/64位精度
	_ctrlfp( 0x1F3F, 0xFFFF );

	if( px->nExponent == 0x7FF )
	{
		// 无穷和NaN
		switch( _sptype( x ) )
		{
		// PINF,NINF,INF*2**ANY => INF
		case 1:
		case 2:
			{
				*intptr = x;
				rv = _copysign( 0.0, x );
				_ctrlfp( usControl, 0xFFFF );
			}
			return rv;

		// QNaN
		case 3:
			*intptr = x;
			return _handle_qnan1( _FpCodeModf, x, usControl );

			// SNaN
		default:
			return _except1( EXCEPTION_BITS_INVALID_OPERATION, _FpCodeModf, x, x+1.0, usControl );
		}
	}

	if( x == 0.0 )
	{
		*intptr = x;
		return x;
	}

	*intptr = _frnd( x );
	rv		= x - *intptr;

	return rv;
}


void _fload( double x )
{
	FP_DOUBLE		*px = (FP_DOUBLE*)&x;
	FP_DOUBLE_EXT	Extended;

	// 正常值/0/弱规范数
	if( px->nExponent != 0x7FF )
	{
		// 一般值,但可能是弱规范值,可能出发#D异常
		__asm fld x
	}
	else
	{
		// 对QNaN/SNaN/PINF/NINF扩展以后再装入可以避免SNaN触发#I异常

		// 特殊值(NaN和无穷)
		Extended.nSign		= px->nSign;			// 1
		Extended.nExponent	= 0x7FFF;				// 15,最大指数

		// 尾数是左对齐,double有52位(加隐含1就是53),而扩展精度是64位,需要左移11位
		// 注意这些数的隐含位全是1
		Extended.nFraction =   (((unsigned __int64)(px->nFraction))<<11)|0x8000000000000000;

		__asm fld tbyte ptr Extended
	}
}

double log( double x )
{
	// 当前控制寄存器值
	unsigned short usControl;
	FP_DOUBLE	  *px = (FP_DOUBLE*)&x;

	int bExponentIs0x7FF;

	_fload_withFB( x, &bExponentIs0x7FF );

	__asm fstcw usControl

	if( !bExponentIs0x7FF )
	{
		// 使用缺省控制寄存器值,0x27F,屏蔽所有异常/53位精度/最近舍入
		if( usControl != _DEFAULT_CW_in_mem )
			__asm fldcw _DEFAULT_CW_in_mem

		// 0
		if( px->nExponent == 0 && px->nFraction == 0 )
		{
			// 清空ST(0)
			__asm fstp st(0)

			// 设置返回值,-0,但这个值后来被改写为NINF
			__asm fld _minfinity

			if( _fastflag )
				_fast_exit( usControl );
			else
				_startOneArgErrorHandling( "log", _FpCodeLog, _SING, usControl, x );
		}

		// -
		if( px->nSign )
		{
			__asm fstp st(0)
			__asm fld _indefinite

			if( _fastflag )
				_fast_exit( usControl );
			else
				_startOneArgErrorHandling( "log", _FpCodeLog, _DOMAIN, usControl, x );
		}

		// +F,+D, lnx=log2(x)/log2(e)=log2(x)*ln(2)
		__asm
		{
			fldln2
			fxch st(1)
			fyl2x
		}

	
		if( _fastflag )
			_fast_exit( usControl );
		else
			_math_exit( "log", _FpCodeLog, usControl, x );
	}
	else
	{
		// PINF=>PINF,NINF=>*,NaN=>NaN,数据在ST(0)中
		if( px->nFraction == 0 )
		{
			// NINF
			if( px->nSign )
				__asm fld _indefinite
			else
			{
				if( _fastflag )
					_fast_exit( usControl );
				else
					_math_exit( "log", _FpCodeLog, usControl, x );
			}
		}
		else
			_convertTOStoQNaN( x )

		if( _fastflag )
			_fast_exit( usControl );
		else
			_startOneArgErrorHandling( "log", _FpCodeLog, _DOMAIN, usControl, 1 );
	}
}

double log10( double x )
{
	// 当前控制寄存器值
	unsigned short usControl;
	FP_DOUBLE	  *px = (FP_DOUBLE*)&x;

	int bExponentIs0x7FF;

	_fload_withFB( x, &bExponentIs0x7FF );

	__asm fstcw usControl

	if( !bExponentIs0x7FF )
	{
		// 使用缺省控制寄存器值,0x27F,屏蔽所有异常/53位精度/最近舍入
		if( usControl != _DEFAULT_CW_in_mem )
			__asm fldcw _DEFAULT_CW_in_mem


		// 0
		if( px->nExponent == 0 && px->nFraction == 0 )
		{
			// 清空ST(0)
			__asm fstp st(0)

			// 设置返回值,-0,但这个值后来被改写为NINF
			__asm fld _minfinity

			if( _fastflag )
				_fast_exit( usControl );
			else
				_startOneArgErrorHandling( "log10", _FpCodeLog10, _SING, usControl, x );
		}

		// -
		if( px->nSign )
		{
			__asm fstp st(0)
			__asm fld _indefinite

			if( _fastflag )
				_fast_exit( usControl );
			else
				_startOneArgErrorHandling( "log10", _FpCodeLog10, _DOMAIN, usControl, x );
		}

		// +F,+D, log10x=log2(x)/log2(10)=log2(x)*log10(2)
		__asm
		{
			fldlg2
			fxch st(1)
			fyl2x
		}

		if( _fastflag )
			_fast_exit( usControl );
		else
			_math_exit( "log10", _FpCodeLog10, usControl, x );
	}
	else
	{
		// PINF=>PINF,NINF=>*,NaN=>NaN,数据在ST(0)中
		if( px->nFraction == 0 )
		{
			// NINF
			if( px->nSign )
				__asm fld _indefinite
			else
			{
				if( _fastflag )
					_fast_exit( usControl );
				else
					_math_exit( "log10", _FpCodeLog10, usControl, x );
			}
		}
		else
			_convertTOStoQNaN( x )

		if( _fastflag )
			_fast_exit( usControl );
		else
			_startOneArgErrorHandling( "log10", _FpCodeLog10, _DOMAIN, usControl, x );
	}
}

double sqrt( double x )
{
	unsigned short usControl;
	FP_DOUBLE	  *px = (FP_DOUBLE*)&x;

	int bExponentIs0x7FF;
	_fload_withFB( x, &bExponentIs0x7FF );

	__asm FSTCW usControl

	if( !bExponentIs0x7FF )
	{
		if( usControl != _DEFAULT_CW_in_mem )
			__asm fldcw _DEFAULT_CW_in_mem

		if( px->nSign == 0 )
			__asm fsqrt
		
		if( ( px->nSign == 1 )&& ( px->nFraction != 0 || px->nExponent != 0 ) ) // !-0.0
			break;

		if( _fastflag )
			_fast_exit( usControl );
		else
			_math_exit( "sqrt", _FpCodeSquareRoot, usControl, x );
	}
	else
	{
		// PINF=>PINF,NINF=>*,NaN=>NaN,数据在ST(0)中
		if( px->nFraction == 0 )
		{
			// NINF
			if( px->nSign )
				__asm fld _indefinite
			else
			{
				if( _fastflag )
					_fast_exit( usControl );
				else
					_math_exit( "log10", _FpCodeLog10, usControl, x );
			}
		}
		else
			_convertTOStoQNaN( x )

		if( _fastflag )
			_fast_exit( usControl );
		else
			_startOneArgErrorHandling( "sqrt", _FpCodeSquareRoot, _DOMAIN, usControl, x );
	}
}


// 参数与返回值均在ST(0)中
__declspec(naked) double _adj_fpatan()
{
	__asm fpatan
	__asm ret
}

// 参数与返回值均在ST(0)中
__declspec(naked) double _adj_fptan()
{
	__asm fptan
	__asm ret
}

// 参数与返回值均在ST(0)中
__declspec(naked) double _safe_fprem1()
{
	_adj_fprem1();
	__asm ret
}

// 参数与返回值均在ST(0)中
__declspec(naked) double _safe_fprem()
{
	_adj_fprem();
	__asm ret
}

