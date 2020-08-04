// ����VC6�����ķ�������
// ��Ȫ���Ƿ������ļ�����  ������  2007-7

#ifndef __LCG_FLOAT_2004_12_H__
#define __LCG_FLOAT_2004_12_H__

#include<windows.h>
#include<float.h>
#include<signal.h>
#include<math.h>

// �����ȸ�ʽ
typedef struct _FP_SIGLE
{
    unsigned __int32 nFraction : 23;	// ������λ��24λ��Ч����
    unsigned __int32 nExponent :  8;
    unsigned __int32 nSign     :  1;
} FP_SINGLE;

// ˫���ȸ�ʽ
typedef struct _FP_DOUBLE
{
    unsigned __int64 nFraction  : 52;	// ������λ��53λ��Ч����
    unsigned __int64 nExponent  : 11;
    unsigned __int64 nSign      :  1;
} FP_DOUBLE;

// ��չ˫���ȸ�ʽ
typedef struct _FP_DOUBLE_EXT
{
    unsigned __int64 nFraction;			// û������λ,64λ��Ч����
    unsigned __int16 nExponent  : 15;
    unsigned __int16 nSign      :  1;
} FP_DOUBLE_EXT;

// ȡdouble�ĸ�DWORD����,����ָ���ͷ���λ
#define FP_DOUBLE_HIGH4( data )		( *( ((unsigned __int32*)&data)+1) )
#define FP_DOUBLE_LOW4( data )		( *((unsigned __int32*)&data) )

// FPU״̬�Ĵ��� vs ����
unsigned int ConvertStatusToMask( unsigned short usStatus );

// FPU���ƼĴ��� vs ����
unsigned int ConvertControlToMask( unsigned short usControl );
unsigned short ConvertMaskToControl( unsigned int uMask );

// ȱʡ���ƼĴ���ֵ
#define DEFAULT_CONTROL_WORD	0x27F

#define EM_MASK					0x3F
#define EM_INVALID_OPERATION	0x01
#define EM_DENORMAL_OPERAND		0x02
#define EM_ZERO_DIVIDE			0x04
#define EM_OVERFLOW				0x08
#define EM_UNDERFLOW			0x10
#define EM_PRECISION			0x20

#define PC_MASK					0x300
#define PC_FLOAT_24				0x000
#define PC_RESERVED				0x100
#define PC_DOUBLE_53			0x200
#define PC_EX_DOUBLE_64			0x300

#define RC_MASK					0xC00
#define RC_NEAREST				0x000
#define RC_DOWN					0x400
#define RC_UP					0x800
#define RC_TRUNCATE				0xC00

#endif