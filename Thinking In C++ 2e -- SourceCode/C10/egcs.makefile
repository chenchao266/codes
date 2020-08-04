# From Thinking in C++, 2nd Edition
# At http://www.BruceEckel.com
# (c) Bruce Eckel 1999
# Copyright notice in Copyright.txt
# Automatically-generated MAKEFILE 
# For examples in directory C10
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
	Statfun \
	Funobj \
	StaticDestructors \
	Statinit \
	StaticArray \
	Local \
	StaticMemberFunctions \
	Selfmem \
	Depend2 

test: all 
	Statfun  
	Funobj  
	StaticDestructors  
	Statinit  
	StaticArray  
	Local  
	StaticMemberFunctions  
	Selfmem  
	Depend2  

bugs: 
	@echo No compiler bugs in this directory!

Statfun: Statfun.o 
	$(CPP) $(OFLAG)Statfun Statfun.o 

Funobj: Funobj.o 
	$(CPP) $(OFLAG)Funobj Funobj.o 

StaticDestructors: StaticDestructors.o 
	$(CPP) $(OFLAG)StaticDestructors StaticDestructors.o 

Statinit: Statinit.o 
	$(CPP) $(OFLAG)Statinit Statinit.o 

StaticArray: StaticArray.o 
	$(CPP) $(OFLAG)StaticArray StaticArray.o 

Local: Local.o 
	$(CPP) $(OFLAG)Local Local.o 

StaticMemberFunctions: StaticMemberFunctions.o 
	$(CPP) $(OFLAG)StaticMemberFunctions StaticMemberFunctions.o 

Selfmem: Selfmem.o 
	$(CPP) $(OFLAG)Selfmem Selfmem.o 

Depend2: Depend2.o Depdefs.o Depend.o 
	$(CPP) $(OFLAG)Depend2 Depend2.o Depdefs.o Depend.o 


Statfun.o: Statfun.cpp ../require.h 
Funobj.o: Funobj.cpp 
StaticDestructors.o: StaticDestructors.cpp 
Statinit.o: Statinit.cpp 
StaticArray.o: StaticArray.cpp 
Local.o: Local.cpp 
StaticMemberFunctions.o: StaticMemberFunctions.cpp 
Selfmem.o: Selfmem.cpp 
Depdefs.o: Depdefs.cpp Depend.h 
Depend.o: Depend.cpp Depend.h 
Depend2.o: Depend2.cpp Depend.h 

