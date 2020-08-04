# From Thinking in C++, 2nd Edition
# At http://www.BruceEckel.com
# (c) Bruce Eckel 1999
# Copyright notice in Copyright.txt
# Automatically-generated MAKEFILE 
# For examples in directory C12
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
	Opover \
	Unary \
	Binary \
	Comma \
	Smartp \
	Iosop \
	FeeFi \
	Simpcopy \
	Copymem \
	Refcount \
	RefcountTrace \
	Autoeq \
	Autocnst \
	Opconv \
	Reflex \
	Strings1 \
	Strings2 \
	Ambig \
	Fanout \
	FeeFi2 

test: all 
	Opover  
	Unary  
	Binary  
	Comma  
	Smartp  
	Iosop  
	FeeFi  
	Simpcopy  
	Copymem  
	Refcount  
	RefcountTrace  
	Autoeq  
	Autocnst  
	Opconv  
	Reflex  
	Strings1  
	Strings2  
	Ambig  
	Fanout  
	FeeFi2  

bugs: 
	@echo No compiler bugs in this directory!

Opover: Opover.o 
	$(CPP) $(OFLAG)Opover Opover.o 

Unary: Unary.o 
	$(CPP) $(OFLAG)Unary Unary.o 

Binary: Binary.o 
	$(CPP) $(OFLAG)Binary Binary.o 

Comma: Comma.o 
	$(CPP) $(OFLAG)Comma Comma.o 

Smartp: Smartp.o 
	$(CPP) $(OFLAG)Smartp Smartp.o 

Iosop: Iosop.o 
	$(CPP) $(OFLAG)Iosop Iosop.o 

FeeFi: FeeFi.o 
	$(CPP) $(OFLAG)FeeFi FeeFi.o 

Simpcopy: Simpcopy.o 
	$(CPP) $(OFLAG)Simpcopy Simpcopy.o 

Copymem: Copymem.o 
	$(CPP) $(OFLAG)Copymem Copymem.o 

Refcount: Refcount.o 
	$(CPP) $(OFLAG)Refcount Refcount.o 

RefcountTrace: RefcountTrace.o 
	$(CPP) $(OFLAG)RefcountTrace RefcountTrace.o 

Autoeq: Autoeq.o 
	$(CPP) $(OFLAG)Autoeq Autoeq.o 

Autocnst: Autocnst.o 
	$(CPP) $(OFLAG)Autocnst Autocnst.o 

Opconv: Opconv.o 
	$(CPP) $(OFLAG)Opconv Opconv.o 

Reflex: Reflex.o 
	$(CPP) $(OFLAG)Reflex Reflex.o 

Strings1: Strings1.o 
	$(CPP) $(OFLAG)Strings1 Strings1.o 

Strings2: Strings2.o 
	$(CPP) $(OFLAG)Strings2 Strings2.o 

Ambig: Ambig.o 
	$(CPP) $(OFLAG)Ambig Ambig.o 

Fanout: Fanout.o 
	$(CPP) $(OFLAG)Fanout Fanout.o 

FeeFi2: FeeFi2.o 
	$(CPP) $(OFLAG)FeeFi2 FeeFi2.o 


Opover.o: Opover.cpp 
Unary.o: Unary.cpp 
Binary.o: Binary.cpp ../require.h 
Comma.o: Comma.cpp 
Smartp.o: Smartp.cpp 
Iosop.o: Iosop.cpp ../require.h 
FeeFi.o: FeeFi.cpp 
Simpcopy.o: Simpcopy.cpp 
Copymem.o: Copymem.cpp ../require.h 
Refcount.o: Refcount.cpp ../require.h 
RefcountTrace.o: RefcountTrace.cpp ../require.h 
Autoeq.o: Autoeq.cpp 
Autocnst.o: Autocnst.cpp 
Opconv.o: Opconv.cpp 
Reflex.o: Reflex.cpp 
Strings1.o: Strings1.cpp ../require.h 
Strings2.o: Strings2.cpp ../require.h 
Ambig.o: Ambig.cpp 
Fanout.o: Fanout.cpp 
FeeFi2.o: FeeFi2.cpp 

