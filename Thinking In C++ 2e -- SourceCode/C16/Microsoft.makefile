# From Thinking in C++, 2nd Edition
# At http://www.BruceEckel.com
# (c) Bruce Eckel 1999
# Copyright notice in Copyright.txt
# Automatically-generated MAKEFILE 
# For examples in directory C16
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
	Mblock.exe \
	TStashTest.exe \
	TStackTest.exe \
	Recycle.exe 

test: all 
	Mblock.exe  
	TStashTest.exe  
	TStackTest.exe  
	Recycle.exe  

bugs: \
	IStack.exe \
	Stemp.exe \
	Stemp2.exe \
	Stackt.exe 

IStack.exe: IStack.obj 
	$(CPP) $(OFLAG)IStack.exe IStack.obj 

Stemp.exe: Stemp.obj 
	$(CPP) $(OFLAG)Stemp.exe Stemp.obj 

Stemp2.exe: Stemp2.obj 
	$(CPP) $(OFLAG)Stemp2.exe Stemp2.obj 

Stackt.exe: Stackt.obj 
	$(CPP) $(OFLAG)Stackt.exe Stackt.obj 

Mblock.exe: Mblock.obj 
	$(CPP) $(OFLAG)Mblock.exe Mblock.obj 

TStashTest.exe: TStashTest.obj 
	$(CPP) $(OFLAG)TStashTest.exe TStashTest.obj 

TStackTest.exe: TStackTest.obj 
	$(CPP) $(OFLAG)TStackTest.exe TStackTest.obj 

Recycle.exe: Recycle.obj 
	$(CPP) $(OFLAG)Recycle.exe Recycle.obj 


IStack.obj: IStack.cpp ..\require.h 
Stemp.obj: Stemp.cpp ..\require.h 
Stemp2.obj: Stemp2.cpp ..\require.h 
Stackt.obj: Stackt.cpp Stackt.h ..\require.h 
Mblock.obj: Mblock.cpp ..\require.h 
TStashTest.obj: TStashTest.cpp TStash.h ..\require.h 
TStackTest.obj: TStackTest.cpp TStack.h ..\require.h 
Recycle.obj: Recycle.cpp TStack.h 

