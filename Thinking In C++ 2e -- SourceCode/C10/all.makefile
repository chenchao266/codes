# From Thinking in C++, 2nd Edition
# At http://www.BruceEckel.com
# (c) Bruce Eckel 1999
# Copyright notice in Copyright.txt
# Automatically-generated MAKEFILE 
# For examples in directory C10
# using the all compiler
# Note: does not make files that will 
# not compile with this compiler
# Invoke with: make -f all.makefile

.SUFFIXES : .obj .cpp .c
.cpp.obj :
	$(CPP) $(CPPFLAGS) -c $<
.c.obj :
	$(CPP) $(CPPFLAGS) -c $<

all: \
	Statfun.exe \
	Funobj.exe \
	StaticDestructors.exe \
	Statinit.exe \
	StaticArray.exe \
	Local.exe \
	StaticMemberFunctions.exe \
	Selfmem.exe \
	Depend2.exe 

test: all 
	Statfun.exe  
	Funobj.exe  
	StaticDestructors.exe  
	Statinit.exe  
	StaticArray.exe  
	Local.exe  
	StaticMemberFunctions.exe  
	Selfmem.exe  
	Depend2.exe  

bugs: 
	@echo No compiler bugs in this directory!

Statfun.exe: Statfun.obj 
	$(CPP) $(OFLAG)Statfun.exe Statfun.obj 

Funobj.exe: Funobj.obj 
	$(CPP) $(OFLAG)Funobj.exe Funobj.obj 

StaticDestructors.exe: StaticDestructors.obj 
	$(CPP) $(OFLAG)StaticDestructors.exe StaticDestructors.obj 

Statinit.exe: Statinit.obj 
	$(CPP) $(OFLAG)Statinit.exe Statinit.obj 

StaticArray.exe: StaticArray.obj 
	$(CPP) $(OFLAG)StaticArray.exe StaticArray.obj 

Local.exe: Local.obj 
	$(CPP) $(OFLAG)Local.exe Local.obj 

StaticMemberFunctions.exe: StaticMemberFunctions.obj 
	$(CPP) $(OFLAG)StaticMemberFunctions.exe StaticMemberFunctions.obj 

Selfmem.exe: Selfmem.obj 
	$(CPP) $(OFLAG)Selfmem.exe Selfmem.obj 

Depend2.exe: Depend2.obj Depdefs.obj Depend.obj 
	$(CPP) $(OFLAG)Depend2.exe Depend2.obj Depdefs.obj Depend.obj 


Statfun.obj: Statfun.cpp ..\require.h 
Funobj.obj: Funobj.cpp 
StaticDestructors.obj: StaticDestructors.cpp 
Statinit.obj: Statinit.cpp 
StaticArray.obj: StaticArray.cpp 
Local.obj: Local.cpp 
StaticMemberFunctions.obj: StaticMemberFunctions.cpp 
Selfmem.obj: Selfmem.cpp 
Depdefs.obj: Depdefs.cpp Depend.h 
Depend.obj: Depend.cpp Depend.h 
Depend2.obj: Depend2.cpp Depend.h 

