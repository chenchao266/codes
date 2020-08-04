# From Thinking in C++, 2nd Edition
# At http://www.BruceEckel.com
# (c) Bruce Eckel 1999
# Copyright notice in Copyright.txt
# Automatically-generated MAKEFILE 
# For examples in directory C02
# using the Microsoft compiler
# Note: does not make files that will 
# not compile with this compiler
# Invoke with: make -f Microsoft.makefile

# Note: this requires the service Pack 3 from
# www.Microsoft.com for successful compilation!
CPP = cl
CPPFLAGS = -GX -GR
OFLAG = -o
.SUFFIXES : .obj .cpp .c
.cpp.obj :
	$(CPP) $(CPPFLAGS) -c $<
.c.obj :
	$(CPP) $(CPPFLAGS) -c $<

all: \
	Hello.exe \
	Stream2.exe \
	Concat.exe \
	Numconv.exe \
	Incr.exe \
	HelloStrings.exe \
	Scopy.exe \
	FillString.exe \
	Fillvector.exe \
	GetWords.exe 

test: all 
	Hello.exe  
	Stream2.exe  
	Concat.exe  
	Numconv.exe  
	Incr.exe  
	HelloStrings.exe  
	Scopy.exe  
	FillString.exe  
	Fillvector.exe  
	GetWords.exe  

bugs: \
	Incident.exe \
	Intvector.exe 

Hello.exe: Hello.obj 
	$(CPP) $(OFLAG)Hello.exe Hello.obj 

Stream2.exe: Stream2.obj 
	$(CPP) $(OFLAG)Stream2.exe Stream2.obj 

Concat.exe: Concat.obj 
	$(CPP) $(OFLAG)Concat.exe Concat.obj 

Numconv.exe: Numconv.obj 
	$(CPP) $(OFLAG)Numconv.exe Numconv.obj 

Incr.exe: Incr.obj 
	$(CPP) $(OFLAG)Incr.exe Incr.obj 

Incident.exe: Incident.obj 
	$(CPP) $(OFLAG)Incident.exe Incident.obj 

HelloStrings.exe: HelloStrings.obj 
	$(CPP) $(OFLAG)HelloStrings.exe HelloStrings.obj 

Scopy.exe: Scopy.obj 
	$(CPP) $(OFLAG)Scopy.exe Scopy.obj 

FillString.exe: FillString.obj 
	$(CPP) $(OFLAG)FillString.exe FillString.obj 

Fillvector.exe: Fillvector.obj 
	$(CPP) $(OFLAG)Fillvector.exe Fillvector.obj 

GetWords.exe: GetWords.obj 
	$(CPP) $(OFLAG)GetWords.exe GetWords.obj 

Intvector.exe: Intvector.obj 
	$(CPP) $(OFLAG)Intvector.exe Intvector.obj 


Hello.obj: Hello.cpp 
Stream2.obj: Stream2.cpp 
Concat.obj: Concat.cpp 
Numconv.obj: Numconv.cpp 
Incr.obj: Incr.cpp 
Incident.obj: Incident.cpp 
HelloStrings.obj: HelloStrings.cpp 
Scopy.obj: Scopy.cpp 
FillString.obj: FillString.cpp 
Fillvector.obj: Fillvector.cpp 
GetWords.obj: GetWords.cpp 
Intvector.obj: Intvector.cpp 

