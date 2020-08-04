# From Thinking in C++, 2nd Edition
# At http://www.BruceEckel.com
# (c) Bruce Eckel 1999
# Copyright notice in Copyright.txt
# Automatically-generated MAKEFILE 
# For examples in directory C15
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
	Wind2.exe \
	Wind3.exe \
	Wind4.exe \
	Sizes.exe \
	Early.exe \
	Wind5.exe \
	Pvdef.exe \
	Addv.exe \
	Slice.exe \
	Pvdest.exe 

test: all 
	Wind2.exe  
	Wind3.exe  
	Wind4.exe  
	Sizes.exe  
	Early.exe  
	Wind5.exe  
	Pvdef.exe  
	Addv.exe  
	Slice.exe  
	Pvdest.exe  

bugs: 
	@echo No compiler bugs in this directory!

Wind2.exe: Wind2.obj 
	$(CPP) $(OFLAG)Wind2.exe Wind2.obj 

Wind3.exe: Wind3.obj 
	$(CPP) $(OFLAG)Wind3.exe Wind3.obj 

Wind4.exe: Wind4.obj 
	$(CPP) $(OFLAG)Wind4.exe Wind4.obj 

Sizes.exe: Sizes.obj 
	$(CPP) $(OFLAG)Sizes.exe Sizes.obj 

Early.exe: Early.obj 
	$(CPP) $(OFLAG)Early.exe Early.obj 

Wind5.exe: Wind5.obj 
	$(CPP) $(OFLAG)Wind5.exe Wind5.obj 

Pvdef.exe: Pvdef.obj 
	$(CPP) $(OFLAG)Pvdef.exe Pvdef.obj 

Addv.exe: Addv.obj 
	$(CPP) $(OFLAG)Addv.exe Addv.obj 

Slice.exe: Slice.obj 
	$(CPP) $(OFLAG)Slice.exe Slice.obj 

Pvdest.exe: Pvdest.obj 
	$(CPP) $(OFLAG)Pvdest.exe Pvdest.obj 


Wind2.obj: Wind2.cpp 
Wind3.obj: Wind3.cpp 
Wind4.obj: Wind4.cpp 
Sizes.obj: Sizes.cpp 
Early.obj: Early.cpp 
Wind5.obj: Wind5.cpp 
Pvdef.obj: Pvdef.cpp 
Addv.obj: Addv.cpp 
Slice.obj: Slice.cpp 
Pvdest.obj: Pvdest.cpp 

