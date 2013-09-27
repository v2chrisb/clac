#
# Makefile for the clac engine library.
#

CXX=clang++
CXXFLAGS=-std=c++11 -c -O -emit-llvm -I../clac_entity
LLVMLINK=llvm-link
LLVMLINKFLAGS=
SOURCES=actions.cpp    \
	cstack.cpp     \
	EditBuffer.cpp \
	get.cpp        \
	global.cpp     \
	words.cpp      \
	WordStream.cpp
OBJECTS=$(SOURCES:.cpp=.bc)
LIBRARY=libClacEngine.bc

%.bc:	%.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

$(LIBRARY):	$(OBJECTS)
	$(LLVMLINK) $(LLVMLINKFLAGS) -o $@ $(OBJECTS)

# File Dependencies
###################

# Module dependencies -- Produced with 'depend' on Sat Jul  6 08:44:41 2013


actions.bc:	actions.cpp ../clac_entity/BinaryEntity.hpp ../clac_entity/Entity.hpp ../clac_entity/ComplexEntity.hpp \
	../clac_entity/FloatEntity.hpp ../clac_entity/IntegerEntity.hpp ../clac_entity/VeryLong.hpp \
	../clac_entity/ListEntity.hpp ../clac_entity/MatrixEntity.hpp ../clac_entity/RationalEntity.hpp \
	../clac_entity/Rational.hpp ../clac_entity/StringEntity.hpp actions.hpp cstack.hpp global.hpp \
	WordStream.hpp ../clac_entity/support.hpp 

cstack.bc:	cstack.cpp cstack.hpp ../clac_entity/Entity.hpp ../clac_entity/VeryLong.hpp ../clac_entity/support.hpp \
	

EditBuffer.bc:	EditBuffer.cpp EditBuffer.hpp 

get.bc:	get.cpp ../clac_entity/Rational.hpp ../clac_entity/BinaryEntity.hpp ../clac_entity/Entity.hpp \
	../clac_entity/ComplexEntity.hpp ../clac_entity/FloatEntity.hpp ../clac_entity/IntegerEntity.hpp \
	../clac_entity/VeryLong.hpp ../clac_entity/LabeledEntity.hpp ../clac_entity/ListEntity.hpp \
	../clac_entity/MatrixEntity.hpp ../clac_entity/RationalEntity.hpp ../clac_entity/StringEntity.hpp \
	../clac_entity/support.hpp EditBuffer.hpp get.hpp WordStream.hpp global.hpp cstack.hpp words.hpp \
	

global.bc:	global.cpp global.hpp ../clac_entity/VeryLong.hpp cstack.hpp ../clac_entity/Entity.hpp \
	WordStream.hpp 

words.bc:	words.cpp words.hpp 

WordStream.bc:	WordStream.cpp WordStream.hpp 


# Additional Rules
##################
clean:
	rm -f *.bc *.o $(LIBRARY) *.s *.ll *~
