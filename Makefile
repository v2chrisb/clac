#
# Makefile for the clac project.
#

CXX=clang++
CXXFLAGS=-std=c++11 -c -O -emit-llvm -I../Spica/Cpp -I../Scr -Iclac_entity -Iclac_engine
LLVMLINK=llvm-link
LLVMLINKFLAGS=
# It shouldn't be necessary to use Boost here since my application does not require it.
LINKFLAGS=-L/usr/lib/i386-linux-gnu -L$(BOOST_HOME)/stage/lib -lboost_regex -lncurses -lpthread
SOURCES=clac.cpp            \
	CommandWindow.cpp   \
	DirectoryWindow.cpp \
	record_f.cpp        \
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

# Module dependencies -- Produced with 'depend' on Sat Jul  6 08:57:42 2013


clac.bc:	clac.cpp clac_entity/BinaryEntity.hpp clac_entity/Entity.hpp clac_entity/FloatEntity.hpp \
	clac_entity/IntegerEntity.hpp clac_entity/VeryLong.hpp clac_entity/StringEntity.hpp clac_entity/convert.hpp \
	clac_entity/support.hpp clac_engine/actions.hpp clac_engine/cstack.hpp clac_engine/EditBuffer.hpp \
	clac_engine/get.hpp clac_engine/WordStream.hpp clac_engine/global.hpp clac_engine/words.hpp \
	../Scr/scr.hpp CommandWindow.hpp ../Scr/ImageBuffer.hpp ../Scr/Manager.hpp ../Scr/Window.hpp \
	StackWindow.hpp 

CommandWindow.bc:	CommandWindow.cpp ../Scr/scr.hpp CommandWindow.hpp ../Scr/ImageBuffer.hpp ../Scr/Manager.hpp \
	../Scr/Window.hpp clac_engine/EditBuffer.hpp 

DirectoryWindow.bc:	DirectoryWindow.cpp clac_entity/LabeledEntity.hpp clac_entity/Entity.hpp DirectoryWindow.hpp \
	clac_entity/DirectoryEntity.hpp ../Scr/Manager.hpp ../Scr/Window.hpp ../Scr/ImageBuffer.hpp \
	../Scr/scr.hpp 

record_f.bc:	record_f.cpp record_f.h 

StackWindow.bc:	StackWindow.cpp ../Scr/scr.hpp StackWindow.hpp ../Scr/ImageBuffer.hpp ../Scr/Manager.hpp \
	../Scr/Window.hpp clac_engine/cstack.hpp clac_entity/Entity.hpp clac_entity/VeryLong.hpp \
	


# Additional Rules
##################
clean:
	rm -f *.bc *.bc1 *.bc2 *.o $(EXECUTABLE) *.s *.ll *~
