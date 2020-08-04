# From Thinking in C++, 2nd Edition
# At http://www.BruceEckel.com
# (c) Bruce Eckel 1999
# Copyright notice in Copyright.txt
# Automatically-generated MAKEFILE 
# For examples in directory C11
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
	Reference \
	Pasconst \
	Refptr \
	PassStruct \
	HowMany \
	HowMany2 \
	Linenum \
	Autocc \
	Stopcc \
	Pmem \
	Pmem2 

test: all 
	Reference  
	Pasconst  
	Refptr  
	PassStruct  
	HowMany  
	HowMany2  
	Linenum  
	Autocc  
	Stopcc  
	Pmem  
	Pmem2  

bugs: 
	@echo No compiler bugs in this directory!

Reference: Reference.o 
	$(CPP) $(OFLAG)Reference Reference.o 

Pasconst: Pasconst.o 
	$(CPP) $(OFLAG)Pasconst Pasconst.o 

Refptr: Refptr.o 
	$(CPP) $(OFLAG)Refptr Refptr.o 

PassStruct: PassStruct.o 
	$(CPP) $(OFLAG)PassStruct PassStruct.o 

HowMany: HowMany.o 
	$(CPP) $(OFLAG)HowMany HowMany.o 

HowMany2: HowMany2.o 
	$(CPP) $(OFLAG)HowMany2 HowMany2.o 

Linenum: Linenum.o 
	$(CPP) $(OFLAG)Linenum Linenum.o 

Autocc: Autocc.o 
	$(CPP) $(OFLAG)Autocc Autocc.o 

Stopcc: Stopcc.o 
	$(CPP) $(OFLAG)Stopcc Stopcc.o 

Pmem: Pmem.o 
	$(CPP) $(OFLAG)Pmem Pmem.o 

Pmem2: Pmem2.o 
	$(CPP) $(OFLAG)Pmem2 Pmem2.o 


Reference.o: Reference.cpp 
Pasconst.o: Pasconst.cpp 
Refptr.o: Refptr.cpp 
PassStruct.o: PassStruct.cpp 
HowMany.o: HowMany.cpp 
HowMany2.o: HowMany2.cpp 
Linenum.o: Linenum.cpp ../require.h 
Autocc.o: Autocc.cpp 
Stopcc.o: Stopcc.cpp 
Pmem.o: Pmem.cpp 
Pmem2.o: Pmem2.cpp 

