INC=-I/home/zhuang/JRTP_arm/jthread/include/jthread -I/home/zhuang/JRTP_arm/jrtplib/include/jrtplib3 -I/usr/local/x264-arm-share/include
LIB=-L/home/zhuang/JRTP_arm/jthread/lib -ljthread  -L/home/zhuang/JRTP_arm/jrtplib/lib/ -ljrtp -L/usr/local/x264-arm-share/lib -lx264 -lpthread

CXX=arm-linux-g++
CC_FLAG=-Wall

TARGET=rtptest
OBJ=mycoding.o videocapture.o myrtpsender.o main.o

$(TARGET):$(OBJ)
	$(CXX) $(INC) $(LIB) -o $@  $(OBJ)
	
.SUFFIXES: .c .o .cpp
.cpp.o:
	$(CXX) $(CC_FLAG) $(INC) -c $*.cpp -o $*.o

.PRONY:clean
clean:
	@echo "Removing linked and compiled files......"
	rm -f $(OBJ) $(TARGET)
