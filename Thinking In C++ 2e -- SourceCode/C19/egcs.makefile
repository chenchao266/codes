# From Thinking in C++, 2nd Edition
# At http://www.BruceEckel.com
# (c) Bruce Eckel 1999
# Copyright notice in Copyright.txt
# Automatically-generated MAKEFILE 
# For examples in directory C19
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
	TypenamedID \
	UsingTypename \
	Getmem \
	ArraySize \
	TemplateFunctionAddress \
	applyGromit \
	TemplateTemplate \
	DelayedInstantiation 

test: all 
	TypenamedID  
	UsingTypename  
	Getmem  
	ArraySize  
	TemplateFunctionAddress  
	applyGromit  
	TemplateTemplate  
	DelayedInstantiation  

bugs: \
	stringConvTest \
	applyGromit2 \
	Sorted \
	ExplicitInstantiation 

TypenamedID: TypenamedID.o 
	$(CPP) $(OFLAG)TypenamedID TypenamedID.o 

UsingTypename: UsingTypename.o 
	$(CPP) $(OFLAG)UsingTypename UsingTypename.o 

stringConvTest: stringConvTest.o 
	$(CPP) $(OFLAG)stringConvTest stringConvTest.o 

Getmem: Getmem.o 
	$(CPP) $(OFLAG)Getmem Getmem.o 

ArraySize: ArraySize.o 
	$(CPP) $(OFLAG)ArraySize ArraySize.o 

TemplateFunctionAddress: TemplateFunctionAddress.o 
	$(CPP) $(OFLAG)TemplateFunctionAddress TemplateFunctionAddress.o 

applyGromit: applyGromit.o 
	$(CPP) $(OFLAG)applyGromit applyGromit.o 

TemplateTemplate: TemplateTemplate.o 
	$(CPP) $(OFLAG)TemplateTemplate TemplateTemplate.o 

applyGromit2: applyGromit2.o 
	$(CPP) $(OFLAG)applyGromit2 applyGromit2.o 

Sorted: Sorted.o 
	$(CPP) $(OFLAG)Sorted Sorted.o 

ExplicitInstantiation: ExplicitInstantiation.o 
	$(CPP) $(OFLAG)ExplicitInstantiation ExplicitInstantiation.o 

DelayedInstantiation: DelayedInstantiation.o 
	$(CPP) $(OFLAG)DelayedInstantiation DelayedInstantiation.o 


TypenamedID.o: TypenamedID.cpp 
UsingTypename.o: UsingTypename.cpp 
stringConvTest.o: stringConvTest.cpp stringConv.h 
Getmem.o: Getmem.cpp Getmem.h 
ArraySize.o: ArraySize.cpp ../arraySize.h 
TemplateFunctionAddress.o: TemplateFunctionAddress.cpp 
applyGromit.o: applyGromit.cpp Gromit.h applySequence.h 
TemplateTemplate.o: TemplateTemplate.cpp 
applyGromit2.o: applyGromit2.cpp Gromit.h applyMember.h 
Sorted.o: Sorted.cpp Sorted.h Urand.h ../arraySize.h 
ExplicitInstantiation.o: ExplicitInstantiation.cpp Urand.h Sorted.h 
DelayedInstantiation.o: DelayedInstantiation.cpp 

