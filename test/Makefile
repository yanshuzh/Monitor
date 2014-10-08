INC=-I/usr/local/2.95.3/jthread/include/jthread -I/usr/local/2.95.3/jrtplib/include/jrtplib3 
LIB=-L/usr/local/2.95.3/jthread/lib -ljthread -L/usr/local/2.95.3/jrtplib/lib -ljrtp -lpthread

CXX=g++
CC_FLAG=-Wall

TARGET=example4
OBJ=example4.o
	
$(TARGET):$(OBJ)
	$(CXX)  -o $@ $(OBJ) $(INC) $(LIB)

.SUFFIXES: .c .o .cpp
.cpp.o:
	$(CXX) $(CC_FLAG) -c $*.cpp -o $*.o $(INC) $(LIB)

.PRONY:clean
clean:
	@echo "Removing linked and compiled files...."
	rm -f $(OBJ) $(TARGET)
