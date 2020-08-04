# From Thinking in C++, 2nd Edition
# At http://www.BruceEckel.com
# (c) Bruce Eckel 1999
# Copyright notice in Copyright.txt
# Automatically-generated MAKEFILE 
# For examples in directory C13
# using the Borland compiler
# Note: does not make files that will 
# not compile with this compiler
# Invoke with: make -f Borland.makefile

# Note: this requires the upgrade from
# www.Borland.com for successful compilation!
CPP = Bcc32
CPPFLAGS = -w-inl -w-csu -wnak
OFLAG = -e
.SUFFIXES : .obj .cpp .c
.cpp.obj :
	$(CPP) $(CPPFLAGS) -c $<
.c.obj :
	$(CPP) $(CPPFLAGS) -c $<

all: \
	MallocClass.exe \
	Newdel.exe \
	PStashTest.exe \
	Stack4Test.exe \
	Newhandl.exe \
	GlobalNew.exe \
	Framis.exe \
	ArrayNew.exe \
	NoMemory.exe \
	PlacementNew.exe 

test: all 
	MallocClass.exe  
	Newdel.exe  
	PStashTest.exe  
	Stack4Test.exe  
	Newhandl.exe  
	GlobalNew.exe  
	Framis.exe  
	ArrayNew.exe  
	NoMemory.exe  
	PlacementNew.exe  

bugs: 
	@echo No compiler bugs in this directory!

MallocClass.exe: MallocClass.obj 
	$(CPP) $(OFLAG)MallocClass.exe MallocClass.obj 

Newdel.exe: Newdel.obj 
	$(CPP) $(OFLAG)Newdel.exe Newdel.obj 

PStashTest.exe: PStashTest.obj PStash.obj 
	$(CPP) $(OFLAG)PStashTest.exe PStashTest.obj PStash.obj 

Stack4Test.exe: Stack4Test.obj Stack4.obj 
	$(CPP) $(OFLAG)Stack4Test.exe Stack4Test.obj Stack4.obj 

Newhandl.exe: Newhandl.obj 
	$(CPP) $(OFLAG)Newhandl.exe Newhandl.obj 

GlobalNew.exe: GlobalNew.obj 
	$(CPP) $(OFLAG)GlobalNew.exe GlobalNew.obj 

Framis.exe: Framis.obj 
	$(CPP) $(OFLAG)Framis.exe Framis.obj 

ArrayNew.exe: ArrayNew.obj 
	$(CPP) $(OFLAG)ArrayNew.exe ArrayNew.obj 

NoMemory.exe: NoMemory.obj 
	$(CPP) $(OFLAG)NoMemory.exe NoMemory.obj 

PlacementNew.exe: PlacementNew.obj 
	$(CPP) $(OFLAG)PlacementNew.exe PlacementNew.obj 


MallocClass.obj: MallocClass.cpp ..\require.h 
Newdel.obj: Newdel.cpp 
PStash.obj: PStash.cpp PStash.h 
PStashTest.obj: PStashTest.cpp PStash.h ..\require.h 
Stack4.obj: Stack4.cpp Stack4.h 
Stack4Test.obj: Stack4Test.cpp Stack4.h ..\require.h 
Newhandl.obj: Newhandl.cpp 
GlobalNew.obj: GlobalNew.cpp 
Framis.obj: Framis.cpp 
ArrayNew.obj: ArrayNew.cpp 
NoMemory.obj: NoMemory.cpp 
PlacementNew.obj: PlacementNew.cpp 

