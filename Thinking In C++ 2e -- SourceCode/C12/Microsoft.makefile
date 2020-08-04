# From Thinking in C++, 2nd Edition
# At http://www.BruceEckel.com
# (c) Bruce Eckel 1999
# Copyright notice in Copyright.txt
# Automatically-generated MAKEFILE 
# For examples in directory C12
# using the Microsoft compiler
# Note: does not make files that will 
# not compile with this compiler
# Invoke with: make -f Microsoft.makefile

# Note: this requires the service Pack 3 from
# www.Microsoft.com for successful compilation!
CPP = cl
CPPFLAGS = -GX -GR
OFLAG = -o
.SUFFIXES : .obj .cpp .c
.cpp.obj :
	$(CPP) $(CPPFLAGS) -c $<
.c.obj :
	$(CPP) $(CPPFLAGS) -c $<

all: \
	Opover.exe \
	Unary.exe \
	Binary.exe \
	FeeFi.exe \
	Simpcopy.exe \
	Autoeq.exe \
	Autocnst.exe \
	Opconv.exe \
	Reflex.exe \
	Strings1.exe \
	Strings2.exe \
	Ambig.exe \
	Fanout.exe \
	FeeFi2.exe 

test: all 
	Opover.exe  
	Unary.exe  
	Binary.exe  
	FeeFi.exe  
	Simpcopy.exe  
	Autoeq.exe  
	Autocnst.exe  
	Opconv.exe  
	Reflex.exe  
	Strings1.exe  
	Strings2.exe  
	Ambig.exe  
	Fanout.exe  
	FeeFi2.exe  

bugs: \
	Comma.exe \
	Smartp.exe \
	Iosop.exe \
	Copymem.exe \
	Refcount.exe \
	RefcountTrace.exe 

Opover.exe: Opover.obj 
	$(CPP) $(OFLAG)Opover.exe Opover.obj 

Unary.exe: Unary.obj 
	$(CPP) $(OFLAG)Unary.exe Unary.obj 

Binary.exe: Binary.obj 
	$(CPP) $(OFLAG)Binary.exe Binary.obj 

Comma.exe: Comma.obj 
	$(CPP) $(OFLAG)Comma.exe Comma.obj 

Smartp.exe: Smartp.obj 
	$(CPP) $(OFLAG)Smartp.exe Smartp.obj 

Iosop.exe: Iosop.obj 
	$(CPP) $(OFLAG)Iosop.exe Iosop.obj 

FeeFi.exe: FeeFi.obj 
	$(CPP) $(OFLAG)FeeFi.exe FeeFi.obj 

Simpcopy.exe: Simpcopy.obj 
	$(CPP) $(OFLAG)Simpcopy.exe Simpcopy.obj 

Copymem.exe: Copymem.obj 
	$(CPP) $(OFLAG)Copymem.exe Copymem.obj 

Refcount.exe: Refcount.obj 
	$(CPP) $(OFLAG)Refcount.exe Refcount.obj 

RefcountTrace.exe: RefcountTrace.obj 
	$(CPP) $(OFLAG)RefcountTrace.exe RefcountTrace.obj 

Autoeq.exe: Autoeq.obj 
	$(CPP) $(OFLAG)Autoeq.exe Autoeq.obj 

Autocnst.exe: Autocnst.obj 
	$(CPP) $(OFLAG)Autocnst.exe Autocnst.obj 

Opconv.exe: Opconv.obj 
	$(CPP) $(OFLAG)Opconv.exe Opconv.obj 

Reflex.exe: Reflex.obj 
	$(CPP) $(OFLAG)Reflex.exe Reflex.obj 

Strings1.exe: Strings1.obj 
	$(CPP) $(OFLAG)Strings1.exe Strings1.obj 

Strings2.exe: Strings2.obj 
	$(CPP) $(OFLAG)Strings2.exe Strings2.obj 

Ambig.exe: Ambig.obj 
	$(CPP) $(OFLAG)Ambig.exe Ambig.obj 

Fanout.exe: Fanout.obj 
	$(CPP) $(OFLAG)Fanout.exe Fanout.obj 

FeeFi2.exe: FeeFi2.obj 
	$(CPP) $(OFLAG)FeeFi2.exe FeeFi2.obj 


Opover.obj: Opover.cpp 
Unary.obj: Unary.cpp 
Binary.obj: Binary.cpp ..\require.h 
Comma.obj: Comma.cpp 
Smartp.obj: Smartp.cpp 
Iosop.obj: Iosop.cpp ..\require.h 
FeeFi.obj: FeeFi.cpp 
Simpcopy.obj: Simpcopy.cpp 
Copymem.obj: Copymem.cpp ..\require.h 
Refcount.obj: Refcount.cpp ..\require.h 
RefcountTrace.obj: RefcountTrace.cpp ..\require.h 
Autoeq.obj: Autoeq.cpp 
Autocnst.obj: Autocnst.cpp 
Opconv.obj: Opconv.cpp 
Reflex.obj: Reflex.cpp 
Strings1.obj: Strings1.cpp ..\require.h 
Strings2.obj: Strings2.cpp ..\require.h 
Ambig.obj: Ambig.cpp 
Fanout.obj: Fanout.cpp 
FeeFi2.obj: FeeFi2.cpp 

