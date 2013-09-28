#
# Makefile for the clac project.
#

CXX=clang++
CXXFLAGS=-std=c++11 -c -O -emit-llvm -I../Spica/Cpp -I../Scr -Iclac_entity -Iclac_engine
LLVMLINK=llvm-link
LLVMLINKFLAGS=
# It shouldn't be necessary to use Boost here since my application does not require it.
LINKFLAGS=-L/usr/lib/i386-linux-gnu -L$(BOOST_HOME)/stage/lib -lboost_regex -lncurses -lpthread
SOURCES=clac.cpp              \
	ClacCommandWindow.cpp \
	DirectoryWindow.cpp   \
	record_f.cpp          \
	StackWindow.cpp
OBJECTS=$(SOURCES:.cpp=.bc)
EXECUTABLE=clac
LIBSPICA=../Spica/Cpp/libSpicaCpp.bc
LIBSCR=../Scr/libScr.bc
LIBENTITY=clac_entity/libClacEntity.bc
LIBENGINE=clac_engine/libClacEngine.bc

%.bc:	%.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

$(EXECUTABLE):	clac.o
	$(CXX) $< $(LINKFLAGS) -o $@

clac.o:		clac.bc2
	llc -filetype=obj clac.bc2 -o clac.o

clac.bc2:	clac.bc1
	opt -o clac.bc2 -globaldce clac.bc1

clac.bc1:	$(OBJECTS) $(LIBSPICA) $(LIBSCR) $(LIBENTITY) $(LIBENGINE)
	$(LLVMLINK) $(LLVMLINKFLAGS) -o $@ $(OBJECTS) $(LIBSPICA) $(LIBSCR) $(LIBENTITY) $(LIBENGINE)

# File Dependencies
###################

# Module dependencies -- Produced with 'depend' on Fri Sep 27 19:58:58 2013


clac.o:	clac.cpp clac_entity/BinaryEntity.hpp clac_entity/Entity.hpp clac_entity/FloatEntity.hpp \
	clac_entity/IntegerEntity.hpp clac_entity/VeryLong.hpp clac_entity/StringEntity.hpp clac_entity/convert.hpp \
	clac_entity/support.hpp clac_engine/actions.hpp clac_engine/cstack.hpp clac_engine/EditBuffer.hpp \
	clac_engine/get.hpp clac_engine/WordStream.hpp clac_engine/global.hpp clac_engine/words.hpp \
	../Scr/debug.hpp ../Scr/scr.hpp ../Scr/TextWindow.hpp ../Scr/Window.hpp ../Scr/ImageBuffer.hpp \
	ClacCommandWindow.hpp ../Scr/CommandWindow.hpp ../Scr/Manager.hpp DirectoryWindow.hpp clac_entity/DirectoryEntity.hpp \
	StackWindow.hpp 

ClacCommandWindow.o:	ClacCommandWindow.cpp ClacCommandWindow.hpp ../Scr/CommandWindow.hpp ../Scr/ImageBuffer.hpp \
	../Scr/scr.hpp ../Scr/Manager.hpp ../Scr/Window.hpp clac_engine/global.hpp clac_entity/VeryLong.hpp \
	clac_engine/cstack.hpp clac_entity/Entity.hpp clac_engine/WordStream.hpp 

DirectoryWindow.o:	DirectoryWindow.cpp clac_entity/LabeledEntity.hpp clac_entity/Entity.hpp DirectoryWindow.hpp \
	../Scr/ImageBuffer.hpp ../Scr/scr.hpp ../Scr/Manager.hpp ../Scr/Window.hpp clac_entity/DirectoryEntity.hpp \
	

record_f.o:	record_f.cpp record_f.h 

StackWindow.o:	StackWindow.cpp ../Scr/scr.hpp StackWindow.hpp ../Scr/ImageBuffer.hpp ../Scr/Manager.hpp \
	../Scr/Window.hpp clac_engine/cstack.hpp clac_entity/Entity.hpp clac_entity/VeryLong.hpp \
	


# Additional Rules
##################
clean:
	rm -f *.bc *.bc1 *.bc2 *.o $(EXECUTABLE) *.s *.ll *~
