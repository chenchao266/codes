# From Thinking in C++, 2nd Edition
# At http://www.BruceEckel.com
# (c) Bruce Eckel 1999
# Copyright notice in Copyright.txt
# Automatically-generated MAKEFILE 
# For examples in directory C14
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
	Compose \
	Compose2 \
	Inherit \
	Combined \
	Order \
	Hide \
	Ninherit \
	Car \
	FName1 \
	FName2 \
	InheritStack \
	Privinh \
	Protect \
	Wind \
	Ccright \
	InheritStack2 

test: all 
	Compose  
	Compose2  
	Inherit  
	Combined  
	Order  
	Hide  
	Ninherit  
	Car  
	FName1  
	FName2  
	InheritStack  
	Privinh  
	Protect  
	Wind  
	Ccright  
	InheritStack2  

bugs: 
	@echo No compiler bugs in this directory!

Compose: Compose.o 
	$(CPP) $(OFLAG)Compose Compose.o 

Compose2: Compose2.o 
	$(CPP) $(OFLAG)Compose2 Compose2.o 

Inherit: Inherit.o 
	$(CPP) $(OFLAG)Inherit Inherit.o 

Combined: Combined.o 
	$(CPP) $(OFLAG)Combined Combined.o 

Order: Order.o 
	$(CPP) $(OFLAG)Order Order.o 

Hide: Hide.o 
	$(CPP) $(OFLAG)Hide Hide.o 

Ninherit: Ninherit.o 
	$(CPP) $(OFLAG)Ninherit Ninherit.o 

Car: Car.o 
	$(CPP) $(OFLAG)Car Car.o 

FName1: FName1.o 
	$(CPP) $(OFLAG)FName1 FName1.o 

FName2: FName2.o 
	$(CPP) $(OFLAG)FName2 FName2.o 

InheritStack: InheritStack.o ../C13/Stack4.o 
	$(CPP) $(OFLAG)InheritStack InheritStack.o ../C13/Stack4.o 

Privinh: Privinh.o 
	$(CPP) $(OFLAG)Privinh Privinh.o 

Protect: Protect.o 
	$(CPP) $(OFLAG)Protect Protect.o 

Wind: Wind.o 
	$(CPP) $(OFLAG)Wind Wind.o 

Ccright: Ccright.o 
	$(CPP) $(OFLAG)Ccright Ccright.o 

InheritStack2: InheritStack2.o ../C13/Stack4.o 
	$(CPP) $(OFLAG)InheritStack2 InheritStack2.o ../C13/Stack4.o 


Compose.o: Compose.cpp Useful.h 
Compose2.o: Compose2.cpp Useful.h 
Inherit.o: Inherit.cpp Useful.h 
Combined.o: Combined.cpp 
Order.o: Order.cpp 
Hide.o: Hide.cpp 
Ninherit.o: Ninherit.cpp 
Car.o: Car.cpp 
FName1.o: FName1.cpp ../require.h 
FName2.o: FName2.cpp ../require.h 
InheritStack.o: InheritStack.cpp ../C13/Stack4.h ../require.h 
Privinh.o: Privinh.cpp 
Protect.o: Protect.cpp 
Wind.o: Wind.cpp 
Ccright.o: Ccright.cpp 
InheritStack2.o: InheritStack2.cpp ../C13/Stack4.h ../require.h 

