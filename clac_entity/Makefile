#
# Makefile for the clac entity library.
#

CXX=clang++
CXXFLAGS=-c -O -emit-llvm
LLVMLINK=llvm-link
LLVMLINKFLAGS=
SOURCES=BinaryEntity.cpp    \
	ComplexEntity.cpp   \
	convert.cpp         \
	DirectoryEntity.cpp \
	DisplayState.cpp    \
	Entity.cpp          \
	FloatEntity.cpp     \
	IntegerEntity.cpp   \
	LabeledEntity.cpp   \
	ListEntity.cpp      \
	MatrixEntity.cpp    \
	ProgramEntity.cpp   \
	RationalEntity.cpp  \
	StringEntity.cpp    \
	support.cpp         \
	VectorEntity.cpp    \
	VeryLong.cpp
OBJECTS=$(SOURCES:.cpp=.bc)
LIBRARY=libClacEntity.bc

%.bc:	%.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

$(LIBRARY):	$(OBJECTS)
	$(LLVMLINK) $(LLVMLINKFLAGS) -o $@ $(OBJECTS)

# File Dependencies
###################

# Module dependencies -- Produced with 'depend' on Sat Jul  6 09:02:57 2013


BinaryEntity.o:	BinaryEntity.cpp BinaryEntity.hpp Entity.hpp DisplayState.hpp FloatEntity.hpp support.hpp \
	

ComplexEntity.o:	ComplexEntity.cpp ComplexEntity.hpp Entity.hpp DisplayState.hpp FloatEntity.hpp support.hpp \
	

convert.o:	convert.cpp convert.hpp Entity.hpp BinaryEntity.hpp ComplexEntity.hpp DirectoryEntity.hpp \
	FloatEntity.hpp IntegerEntity.hpp VeryLong.hpp LabeledEntity.hpp ListEntity.hpp MatrixEntity.hpp \
	ProgramEntity.hpp RationalEntity.hpp Rational.hpp StringEntity.hpp VectorEntity.hpp 

DirectoryEntity.o:	DirectoryEntity.cpp DirectoryEntity.hpp Entity.hpp 

DisplayState.o:	DisplayState.cpp DisplayState.hpp 

Entity.o:	Entity.cpp Entity.hpp 

FloatEntity.o:	FloatEntity.cpp ComplexEntity.hpp Entity.hpp DisplayState.hpp FloatEntity.hpp IntegerEntity.hpp \
	VeryLong.hpp support.hpp 

IntegerEntity.o:	IntegerEntity.cpp IntegerEntity.hpp Entity.hpp VeryLong.hpp 

LabeledEntity.o:	LabeledEntity.cpp LabeledEntity.hpp Entity.hpp 

ListEntity.o:	ListEntity.cpp ListEntity.hpp Entity.hpp 

MatrixEntity.o:	MatrixEntity.cpp convert.hpp Entity.hpp IntegerEntity.hpp VeryLong.hpp MatrixEntity.hpp \
	

ProgramEntity.o:	ProgramEntity.cpp ProgramEntity.hpp Entity.hpp 

RationalEntity.o:	RationalEntity.cpp FloatEntity.hpp Entity.hpp IntegerEntity.hpp VeryLong.hpp RationalEntity.hpp \
	Rational.hpp 

StringEntity.o:	StringEntity.cpp StringEntity.hpp Entity.hpp 

support.o:	support.cpp DisplayState.hpp support.hpp Entity.hpp 

VectorEntity.o:	VectorEntity.cpp VectorEntity.hpp Entity.hpp 

VeryLong.o:	VeryLong.cpp VeryLong.hpp 

# Additional Rules
##################
clean:
	rm -f *.bc *.o $(LIBRARY) *.s *.ll *~
