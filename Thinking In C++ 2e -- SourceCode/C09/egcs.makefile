# From Thinking in C++, 2nd Edition
# At http://www.BruceEckel.com
# (c) Bruce Eckel 1999
# Copyright notice in Copyright.txt
# Automatically-generated MAKEFILE 
# For examples in directory C09
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
	Macro \
	Inline \
	Access \
	Rectangle \
	Rectangle2 \
	Cpptime \
	Evorder \
	Hidden \
	Noinsitu \
	ErrTest 

test: all 
	Macro  
	Inline  
	Access  
	Rectangle  
	Rectangle2  
	Cpptime  
	Evorder  
	Hidden  
	Noinsitu  
	ErrTest  

bugs: 
	@echo No compiler bugs in this directory!

Macro: Macro.o 
	$(CPP) $(OFLAG)Macro Macro.o 

Inline: Inline.o 
	$(CPP) $(OFLAG)Inline Inline.o 

Access: Access.o 
	$(CPP) $(OFLAG)Access Access.o 

Rectangle: Rectangle.o 
	$(CPP) $(OFLAG)Rectangle Rectangle.o 

Rectangle2: Rectangle2.o 
	$(CPP) $(OFLAG)Rectangle2 Rectangle2.o 

Cpptime: Cpptime.o 
	$(CPP) $(OFLAG)Cpptime Cpptime.o 

Evorder: Evorder.o 
	$(CPP) $(OFLAG)Evorder Evorder.o 

Hidden: Hidden.o 
	$(CPP) $(OFLAG)Hidden Hidden.o 

Noinsitu: Noinsitu.o 
	$(CPP) $(OFLAG)Noinsitu Noinsitu.o 

ErrTest: ErrTest.o 
	$(CPP) $(OFLAG)ErrTest ErrTest.o 


Macro.o: Macro.cpp ../require.h 
Inline.o: Inline.cpp 
Access.o: Access.cpp 
Rectangle.o: Rectangle.cpp 
Rectangle2.o: Rectangle2.cpp 
Cpptime.o: Cpptime.cpp Cpptime.h 
Evorder.o: Evorder.cpp 
Hidden.o: Hidden.cpp 
Noinsitu.o: Noinsitu.cpp 
ErrTest.o: ErrTest.cpp ../require.h 

