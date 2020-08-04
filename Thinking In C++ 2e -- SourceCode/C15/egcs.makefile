# From Thinking in C++, 2nd Edition
# At http://www.BruceEckel.com
# (c) Bruce Eckel 1999
# Copyright notice in Copyright.txt
# Automatically-generated MAKEFILE 
# For examples in directory C15
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
	Wind2 \
	Wind3 \
	Wind4 \
	Sizes \
	Early \
	Wind5 \
	Pvdef \
	Addv \
	Slice \
	Pvdest 

test: all 
	Wind2  
	Wind3  
	Wind4  
	Sizes  
	Early  
	Wind5  
	Pvdef  
	Addv  
	Slice  
	Pvdest  

bugs: 
	@echo No compiler bugs in this directory!

Wind2: Wind2.o 
	$(CPP) $(OFLAG)Wind2 Wind2.o 

Wind3: Wind3.o 
	$(CPP) $(OFLAG)Wind3 Wind3.o 

Wind4: Wind4.o 
	$(CPP) $(OFLAG)Wind4 Wind4.o 

Sizes: Sizes.o 
	$(CPP) $(OFLAG)Sizes Sizes.o 

Early: Early.o 
	$(CPP) $(OFLAG)Early Early.o 

Wind5: Wind5.o 
	$(CPP) $(OFLAG)Wind5 Wind5.o 

Pvdef: Pvdef.o 
	$(CPP) $(OFLAG)Pvdef Pvdef.o 

Addv: Addv.o 
	$(CPP) $(OFLAG)Addv Addv.o 

Slice: Slice.o 
	$(CPP) $(OFLAG)Slice Slice.o 

Pvdest: Pvdest.o 
	$(CPP) $(OFLAG)Pvdest Pvdest.o 


Wind2.o: Wind2.cpp 
Wind3.o: Wind3.cpp 
Wind4.o: Wind4.cpp 
Sizes.o: Sizes.cpp 
Early.o: Early.cpp 
Wind5.o: Wind5.cpp 
Pvdef.o: Pvdef.cpp 
Addv.o: Addv.cpp 
Slice.o: Slice.cpp 
Pvdest.o: Pvdest.cpp 

