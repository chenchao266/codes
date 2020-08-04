# From Thinking in C++, 2nd Edition
# At http://www.BruceEckel.com
# (c) Bruce Eckel 1999
# Copyright notice in Copyright.txt
# Automatically-generated MAKEFILE 
# For examples in directory C13
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
	MallocClass \
	Newdel \
	PStashTest \
	Stack4Test \
	Newhandl \
	GlobalNew \
	Framis \
	ArrayNew \
	NoMemory \
	PlacementNew 

test: all 
	MallocClass  
	Newdel  
	PStashTest  
	Stack4Test  
	Newhandl  
	GlobalNew  
	Framis  
	ArrayNew  
	NoMemory  
	PlacementNew  

bugs: 
	@echo No compiler bugs in this directory!

MallocClass: MallocClass.o 
	$(CPP) $(OFLAG)MallocClass MallocClass.o 

Newdel: Newdel.o 
	$(CPP) $(OFLAG)Newdel Newdel.o 

PStashTest: PStashTest.o PStash.o 
	$(CPP) $(OFLAG)PStashTest PStashTest.o PStash.o 

Stack4Test: Stack4Test.o Stack4.o 
	$(CPP) $(OFLAG)Stack4Test Stack4Test.o Stack4.o 

Newhandl: Newhandl.o 
	$(CPP) $(OFLAG)Newhandl Newhandl.o 

GlobalNew: GlobalNew.o 
	$(CPP) $(OFLAG)GlobalNew GlobalNew.o 

Framis: Framis.o 
	$(CPP) $(OFLAG)Framis Framis.o 

ArrayNew: ArrayNew.o 
	$(CPP) $(OFLAG)ArrayNew ArrayNew.o 

NoMemory: NoMemory.o 
	$(CPP) $(OFLAG)NoMemory NoMemory.o 

PlacementNew: PlacementNew.o 
	$(CPP) $(OFLAG)PlacementNew PlacementNew.o 


MallocClass.o: MallocClass.cpp ../require.h 
Newdel.o: Newdel.cpp 
PStash.o: PStash.cpp PStash.h 
PStashTest.o: PStashTest.cpp PStash.h ../require.h 
Stack4.o: Stack4.cpp Stack4.h 
Stack4Test.o: Stack4Test.cpp Stack4.h ../require.h 
Newhandl.o: Newhandl.cpp 
GlobalNew.o: GlobalNew.cpp 
Framis.o: Framis.cpp 
ArrayNew.o: ArrayNew.cpp 
NoMemory.o: NoMemory.cpp 
PlacementNew.o: PlacementNew.cpp 

