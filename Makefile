SOURCE = $(wildcard src/*.c) $(wildcard utils/*.c) $(wildcard src/*.cpp) $(wildcard utils/*.cpp)  
TARGET = main


CC      = g++
CFLAGS  = -g -Wall 
CFLAGS += -Iinc -Iutils 
CFLAGS += -Wno-unused-value  -Wno-unused-variable 

$(TARGET):$(SOURCE)
	$(CC) -o  $@ $^ $(CFLAGS)


clean:
	-rm -f $(TARGET)
	-rm -f src/*.o
	-rm -f utils/*.o
