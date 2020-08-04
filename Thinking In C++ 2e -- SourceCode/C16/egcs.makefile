# From Thinking in C++, 2nd Edition
# At http://www.BruceEckel.com
# (c) Bruce Eckel 1999
# Copyright notice in Copyright.txt
# Automatically-generated MAKEFILE 
# For examples in directory C16
# using the egcs compiler
# Note: does not make files that will 
# not compile with this compiler
# Invoke with: make -f egcs.makefile

CPP = g++
OFLAG = -o
.SUFFIXES : .o .cpp .c
.cpp.o :
	$(CPP) $(CPPFLAGS) -c $<
.c.o :
	$(CPP) $(CPPFLAGS) -c $<

all: \
	IStack \
	Stemp \
	Stemp2 \
	Stackt \
	Mblock \
	TStashTest \
	TStackTest \
	Recycle 

test: all 
	IStack  
	Stemp  
	Stemp2  
	Stackt  
	Mblock  
	TStashTest  
	TStackTest  
	Recycle  

bugs: 
	@echo No compiler bugs in this directory!

IStack: IStack.o 
	$(CPP) $(OFLAG)IStack IStack.o 

Stemp: Stemp.o 
	$(CPP) $(OFLAG)Stemp Stemp.o 

Stemp2: Stemp2.o 
	$(CPP) $(OFLAG)Stemp2 Stemp2.o 

Stackt: Stackt.o 
	$(CPP) $(OFLAG)Stackt Stackt.o 

Mblock: Mblock.o 
	$(CPP) $(OFLAG)Mblock Mblock.o 

TStashTest: TStashTest.o 
	$(CPP) $(OFLAG)TStashTest TStashTest.o 

TStackTest: TStackTest.o 
	$(CPP) $(OFLAG)TStackTest TStackTest.o 

Recycle: Recycle.o 
	$(CPP) $(OFLAG)Recycle Recycle.o 


IStack.o: IStack.cpp ../require.h 
Stemp.o: Stemp.cpp ../require.h 
Stemp2.o: Stemp2.cpp ../require.h 
Stackt.o: Stackt.cpp Stackt.h ../require.h 
Mblock.o: Mblock.cpp ../require.h 
TStashTest.o: TStashTest.cpp TStash.h ../require.h 
TStackTest.o: TStackTest.cpp TStack.h ../require.h 
Recycle.o: Recycle.cpp TStack.h 

