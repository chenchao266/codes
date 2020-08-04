# From Thinking in C++, 2nd Edition
# At http://www.BruceEckel.com
# (c) Bruce Eckel 1999
# Copyright notice in Copyright.txt
# Automatically-generated MAKEFILE 
# For examples in directory C02
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
	Hello \
	Stream2 \
	Concat \
	Numconv \
	Incr \
	Incident \
	HelloStrings \
	Scopy \
	FillString \
	Fillvector \
	GetWords \
	Intvector 

test: all 
	Hello  
	Stream2  
	Concat  
	Numconv  
	Incr  
	Incident  
	HelloStrings  
	Scopy  
	FillString  
	Fillvector  
	GetWords  
	Intvector  

bugs: 
	@echo No compiler bugs in this directory!

Hello: Hello.o 
	$(CPP) $(OFLAG)Hello Hello.o 

Stream2: Stream2.o 
	$(CPP) $(OFLAG)Stream2 Stream2.o 

Concat: Concat.o 
	$(CPP) $(OFLAG)Concat Concat.o 

Numconv: Numconv.o 
	$(CPP) $(OFLAG)Numconv Numconv.o 

Incr: Incr.o 
	$(CPP) $(OFLAG)Incr Incr.o 

Incident: Incident.o 
	$(CPP) $(OFLAG)Incident Incident.o 

HelloStrings: HelloStrings.o 
	$(CPP) $(OFLAG)HelloStrings HelloStrings.o 

Scopy: Scopy.o 
	$(CPP) $(OFLAG)Scopy Scopy.o 

FillString: FillString.o 
	$(CPP) $(OFLAG)FillString FillString.o 

Fillvector: Fillvector.o 
	$(CPP) $(OFLAG)Fillvector Fillvector.o 

GetWords: GetWords.o 
	$(CPP) $(OFLAG)GetWords GetWords.o 

Intvector: Intvector.o 
	$(CPP) $(OFLAG)Intvector Intvector.o 


Hello.o: Hello.cpp 
Stream2.o: Stream2.cpp 
Concat.o: Concat.cpp 
Numconv.o: Numconv.cpp 
Incr.o: Incr.cpp 
Incident.o: Incident.cpp 
HelloStrings.o: HelloStrings.cpp 
Scopy.o: Scopy.cpp 
FillString.o: FillString.cpp 
Fillvector.o: Fillvector.cpp 
GetWords.o: GetWords.cpp 
Intvector.o: Intvector.cpp 

