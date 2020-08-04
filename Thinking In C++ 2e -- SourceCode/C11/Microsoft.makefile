# From Thinking in C++, 2nd Edition
# At http://www.BruceEckel.com
# (c) Bruce Eckel 1999
# Copyright notice in Copyright.txt
# Automatically-generated MAKEFILE 
# For examples in directory C11
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
	Reference.exe \
	Pasconst.exe \
	Refptr.exe \
	PassStruct.exe \
	HowMany.exe \
	Linenum.exe \
	Stopcc.exe \
	Pmem.exe 

test: all 
	Reference.exe  
	Pasconst.exe  
	Refptr.exe  
	PassStruct.exe  
	HowMany.exe  
	Linenum.exe  
	Stopcc.exe  
	Pmem.exe  

bugs: \
	HowMany2.exe \
	Autocc.exe \
	Pmem2.exe 

Reference.exe: Reference.obj 
	$(CPP) $(OFLAG)Reference.exe Reference.obj 

Pasconst.exe: Pasconst.obj 
	$(CPP) $(OFLAG)Pasconst.exe Pasconst.obj 

Refptr.exe: Refptr.obj 
	$(CPP) $(OFLAG)Refptr.exe Refptr.obj 

PassStruct.exe: PassStruct.obj 
	$(CPP) $(OFLAG)PassStruct.exe PassStruct.obj 

HowMany.exe: HowMany.obj 
	$(CPP) $(OFLAG)HowMany.exe HowMany.obj 

HowMany2.exe: HowMany2.obj 
	$(CPP) $(OFLAG)HowMany2.exe HowMany2.obj 

Linenum.exe: Linenum.obj 
	$(CPP) $(OFLAG)Linenum.exe Linenum.obj 

Autocc.exe: Autocc.obj 
	$(CPP) $(OFLAG)Autocc.exe Autocc.obj 

Stopcc.exe: Stopcc.obj 
	$(CPP) $(OFLAG)Stopcc.exe Stopcc.obj 

Pmem.exe: Pmem.obj 
	$(CPP) $(OFLAG)Pmem.exe Pmem.obj 

Pmem2.exe: Pmem2.obj 
	$(CPP) $(OFLAG)Pmem2.exe Pmem2.obj 


Reference.obj: Reference.cpp 
Pasconst.obj: Pasconst.cpp 
Refptr.obj: Refptr.cpp 
PassStruct.obj: PassStruct.cpp 
HowMany.obj: HowMany.cpp 
HowMany2.obj: HowMany2.cpp 
Linenum.obj: Linenum.cpp ..\require.h 
Autocc.obj: Autocc.cpp 
Stopcc.obj: Stopcc.cpp 
Pmem.obj: Pmem.cpp 
Pmem2.obj: Pmem2.cpp 

