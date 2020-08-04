// 本程序是第12章<异常处理机制>的错误定位框架的演示代码
// 酒泉卫星发射中心技术部  刘纯根  2007-7

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<float.h>
#include<windows.h>
#include<fpieee.h>

#define asin  myasin

#define ADD_PROC( name )   AddProc( #name, (unsigned int)name )

#pragma pack( push, 1 )
typedef struct
{
	char byCode;
	unsigned int offset;
} JMP_CODE;
#pragma pack( pop )

typedef struct
{
	char name[32];
	unsigned int addr;
} ADDR_NAME;

ADDR_NAME g_addr[10];
int g_addr_count = 0;

void AddProc( char* name, unsigned int addr )
{
	JMP_CODE *ptr = (JMP_CODE*)addr;
	strcpy( g_addr[g_addr_count].name, name );
	g_addr[g_addr_count].addr = addr + ptr->offset + 5;
	g_addr_count++;
}

void InsertHere( char* name )
{
	unsigned int addr;

	__asm
	{
		PUSH EAX
		MOV EAX, [EBP+4]
		MOV addr, EAX
		POP EAX
	}

	strcpy( g_addr[g_addr_count].name, name );
	g_addr[g_addr_count].addr = addr;
	g_addr_count++;
}

void SortAddrName()
{
    int bContinue = 1, i, k;
    ADDR_NAME temp;

    k = g_addr_count;
    while( bContinue )
    {
        k--;
        bContinue = 0;
        for( i=0; i<k; i++ )
        {
            if( g_addr[i].addr > g_addr[i+1].addr )
            {
                temp = g_addr[i];
                g_addr[i] = g_addr[i+1];
                g_addr[i+1] = temp;

				bContinue = 1;
            }
        }
    }
}

int FindAddrName( unsigned int addr )
{
	int i = 0;

	SortAddrName();
	while( ( i <= g_addr_count-1 ) && ( addr > g_addr[i].addr ) ) i++;

	if( addr == g_addr[i].addr )
		return i;
	else
		return i-1;
}

DWORD AddrFlter( LPEXCEPTION_POINTERS ptrs )
{
	int k;
	unsigned char x87Info[108];
	
	k = FindAddrName( (unsigned int )ptrs->ExceptionRecord->ExceptionAddress );
	if( k == -1 )
	{
		__asm FNSAVE x87Info
		k = FindAddrName( *(unsigned int*)(&x87Info[12]) );
	}

	printf( "SEH:\n");
	if( k != -1 )
		printf( "%s\n", g_addr[k].name );
	else
		printf( "not found!\n" );

	return EXCEPTION_EXECUTE_HANDLER;
}

int fpieee_handler( _FPIEEE_RECORD *pieee )
{
	printf( "FPIEEE:\n" );

	switch( pieee->Operation )
	{
	case _FpCodeAsin:
		printf( "asin(%f):\n", pieee->Operand1 );
		break;

	default:
		break;
	}

	return EXCEPTION_EXECUTE_HANDLER;
}

double myasin( double x )
{
	int k;
	unsigned int addr;

	__asm
	{
		PUSH EAX
		MOV EAX, [EBP+4]
		MOV addr, EAX
		POP EAX
	}

	__try
	{
#undef asin
		asin( x );
#define asin  myasin
	}
	__except( EXCEPTION_EXECUTE_HANDLER )
	{
		k = FindAddrName( addr );
		printf( "SEH asin:\n" );
		if( k != -1 )
			printf( "%s\n", g_addr[k].name );
		else
			printf( "not found!\n" );
	}
	return 0;
}

void Test1()
{
};

void Test2()
{
	sin( 3.0 );
	InsertHere( "Test2-1" );	
	asin( 2.0 );
};

void Test3()
{
	double x = 1.0, y = 0.0;
	
//	InsertHere( "Test3-1" );
	x =x/y;
};

void Test4()
{
};

int main()
{
	ADD_PROC( Test1 );
	ADD_PROC( Test2 );
	ADD_PROC( Test3 );
	ADD_PROC( Test4 );

	_controlfp( ~(_EM_ZERODIVIDE|_EM_INVALID),_MCW_EM );

	__try
	{
		Test1();
		Test2();
		Test3();
		Test4();
	}
//	__except ( _fpieee_flt( GetExceptionCode(), GetExceptionInformation(), fpieee_handler ) )
	__except( AddrFlter( GetExceptionInformation() ) )
	{
	}

	return 0;
}

int _matherr( struct _exception *except )
{
	printf( "MATH:\n");
	printf( "%s(%f)\n", except->name, except->arg1 );
	return 0;
}