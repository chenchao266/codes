# From Thinking in C++, 2nd Edition
# At http://www.BruceEckel.com
# (c) Bruce Eckel 1999
# Copyright notice in Copyright.txt
# Automatically-generated MAKEFILE 
# For examples in directory C14
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
	Compose.exe \
	Compose2.exe \
	Inherit.exe \
	Combined.exe \
	Order.exe \
	Hide.exe \
	Ninherit.exe \
	Car.exe \
	FName1.exe \
	FName2.exe \
	InheritStack.exe \
	Privinh.exe \
	Protect.exe \
	Wind.exe \
	Ccright.exe \
	InheritStack2.exe 

test: all 
	Compose.exe  
	Compose2.exe  
	Inherit.exe  
	Combined.exe  
	Order.exe  
	Hide.exe  
	Ninherit.exe  
	Car.exe  
	FName1.exe  
	FName2.exe  
	InheritStack.exe  
	Privinh.exe  
	Protect.exe  
	Wind.exe  
	Ccright.exe  
	InheritStack2.exe  

bugs: 
	@echo No compiler bugs in this directory!

Compose.exe: Compose.obj 
	$(CPP) $(OFLAG)Compose.exe Compose.obj 

Compose2.exe: Compose2.obj 
	$(CPP) $(OFLAG)Compose2.exe Compose2.obj 

Inherit.exe: Inherit.obj 
	$(CPP) $(OFLAG)Inherit.exe Inherit.obj 

Combined.exe: Combined.obj 
	$(CPP) $(OFLAG)Combined.exe Combined.obj 

Order.exe: Order.obj 
	$(CPP) $(OFLAG)Order.exe Order.obj 

Hide.exe: Hide.obj 
	$(CPP) $(OFLAG)Hide.exe Hide.obj 

Ninherit.exe: Ninherit.obj 
	$(CPP) $(OFLAG)Ninherit.exe Ninherit.obj 

Car.exe: Car.obj 
	$(CPP) $(OFLAG)Car.exe Car.obj 

FName1.exe: FName1.obj 
	$(CPP) $(OFLAG)FName1.exe FName1.obj 

FName2.exe: FName2.obj 
	$(CPP) $(OFLAG)FName2.exe FName2.obj 

InheritStack.exe: InheritStack.obj ..\C13\Stack4.obj 
	$(CPP) $(OFLAG)InheritStack.exe InheritStack.obj ..\C13\Stack4.obj 

Privinh.exe: Privinh.obj 
	$(CPP) $(OFLAG)Privinh.exe Privinh.obj 

Protect.exe: Protect.obj 
	$(CPP) $(OFLAG)Protect.exe Protect.obj 

Wind.exe: Wind.obj 
	$(CPP) $(OFLAG)Wind.exe Wind.obj 

Ccright.exe: Ccright.obj 
	$(CPP) $(OFLAG)Ccright.exe Ccright.obj 

InheritStack2.exe: InheritStack2.obj ..\C13\Stack4.obj 
	$(CPP) $(OFLAG)InheritStack2.exe InheritStack2.obj ..\C13\Stack4.obj 


Compose.obj: Compose.cpp Useful.h 
Compose2.obj: Compose2.cpp Useful.h 
Inherit.obj: Inherit.cpp Useful.h 
Combined.obj: Combined.cpp 
Order.obj: Order.cpp 
Hide.obj: Hide.cpp 
Ninherit.obj: Ninherit.cpp 
Car.obj: Car.cpp 
FName1.obj: FName1.cpp ..\require.h 
FName2.obj: FName2.cpp ..\require.h 
InheritStack.obj: InheritStack.cpp ..\C13\Stack4.h ..\require.h 
Privinh.obj: Privinh.cpp 
Protect.obj: Protect.cpp 
Wind.obj: Wind.cpp 
Ccright.obj: Ccright.cpp 
InheritStack2.obj: InheritStack2.cpp ..\C13\Stack4.h ..\require.h 

