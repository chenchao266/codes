# From Thinking in C++, 2nd Edition
# At http://www.BruceEckel.com
# (c) Bruce Eckel 1999
# Copyright notice in Copyright.txt
# Automatically-generated MAKEFILE 
# For examples in directory C09
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
	Macro.exe \
	Inline.exe \
	Access.exe \
	Rectangle.exe \
	Rectangle2.exe \
	Cpptime.exe \
	Evorder.exe \
	Hidden.exe \
	Noinsitu.exe \
	ErrTest.exe 

test: all 
	Macro.exe  
	Inline.exe  
	Access.exe  
	Rectangle.exe  
	Rectangle2.exe  
	Cpptime.exe  
	Evorder.exe  
	Hidden.exe  
	Noinsitu.exe  
	ErrTest.exe  

bugs: 
	@echo No compiler bugs in this directory!

Macro.exe: Macro.obj 
	$(CPP) $(OFLAG)Macro.exe Macro.obj 

Inline.exe: Inline.obj 
	$(CPP) $(OFLAG)Inline.exe Inline.obj 

Access.exe: Access.obj 
	$(CPP) $(OFLAG)Access.exe Access.obj 

Rectangle.exe: Rectangle.obj 
	$(CPP) $(OFLAG)Rectangle.exe Rectangle.obj 

Rectangle2.exe: Rectangle2.obj 
	$(CPP) $(OFLAG)Rectangle2.exe Rectangle2.obj 

Cpptime.exe: Cpptime.obj 
	$(CPP) $(OFLAG)Cpptime.exe Cpptime.obj 

Evorder.exe: Evorder.obj 
	$(CPP) $(OFLAG)Evorder.exe Evorder.obj 

Hidden.exe: Hidden.obj 
	$(CPP) $(OFLAG)Hidden.exe Hidden.obj 

Noinsitu.exe: Noinsitu.obj 
	$(CPP) $(OFLAG)Noinsitu.exe Noinsitu.obj 

ErrTest.exe: ErrTest.obj 
	$(CPP) $(OFLAG)ErrTest.exe ErrTest.obj 


Macro.obj: Macro.cpp ..\require.h 
Inline.obj: Inline.cpp 
Access.obj: Access.cpp 
Rectangle.obj: Rectangle.cpp 
Rectangle2.obj: Rectangle2.cpp 
Cpptime.obj: Cpptime.cpp Cpptime.h 
Evorder.obj: Evorder.cpp 
Hidden.obj: Hidden.cpp 
Noinsitu.obj: Noinsitu.cpp 
ErrTest.obj: ErrTest.cpp ..\require.h 

