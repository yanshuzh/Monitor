INC=-I/usr/include/SDL -I/home/zhuang/ffmpeg/include -I/usr/local/2.95.3/jrtplib/include/jrtplib3 -I/usr/local/2.95.3/jthread/include/jthread 
LIB=-L/usr/local/2.95.3/jthread/lib -ljthread -L/usr/local/2.95.3/jrtplib/lib -ljrtp -L/home/zhuang/ffmpeg/lib -lavcodec -lavformat -lavfilter -lswresample -lswscale -lavdevice -lavutil  -lSDL -lpthread


CXX=g++
CC_FLAG=-Wall

TARGET=example4
OBJ= mysdl.o mydecoder.o example4.o

$(TARGET):$(OBJ)
	$(CXX) -o $@ $(OBJ) $(INC) $(LIB)
.SUFFIXES: .c .o .cpp
.cpp.o:
	$(CXX) -c $*.cpp -o $*.o $(CC_FLAG) $(INC)

.PRONY:clean
clean:
	@echo "Removing linked and compiled files...."
	rm -f $(OBJ) $(TARGET)
