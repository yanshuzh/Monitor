INC=-I/usr/include/SDL -I/usr/local/ffmpeg/include -I/usr/local/2.95.3/jrtplib/include/jrtplib3 -I/usr/local/2.95.3/jthread/include/jthread -I/usr/include/mysql
LIB=-L/usr/local/2.95.3/jthread/lib -ljthread -L/usr/local/2.95.3/jrtplib/lib -ljrtp -L/usr/local/ffmpeg/lib -lavcodec -lavformat -lavfilter -lswresample -lswscale -lavdevice -lavutil  -lSDL -lmysqlclient -lpthread 


CXX=g++
CC_FLAG=-Wall -std=c++11 

TARGET=server
OBJ= mysdl.o mydecoder.o myrtpreceiver.o myrtpsender.o threadpool.o MySQLManager.o server.o

$(TARGET):$(OBJ)
	$(CXX) -o $@ $(OBJ) $(INC) $(LIB)
.SUFFIXES: .c .o .cpp
.cpp.o:
	$(CXX) -c $*.cpp -o $*.o $(CC_FLAG) $(INC)

.PRONY:clean
clean:
	@echo "Removing linked and compiled files...."
	rm -f $(OBJ) $(TARGET)
