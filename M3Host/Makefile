CXXFLAGS =	-O2 -g3 -Wall -fmessage-length=0

CFLAGS = 	-c -Wall $(INCLUDES) 	

INCLUDES = 	-I /home/akos/Programs/OpenCV/opencv-2.4.9/include \
			-I ../Framework/libLogConfigTime/include/ \
			-I ../Framework/libPlatformSpecifics/include/ \
			-I ../Framework/libCommunication/include/

LIBS =		-L/home/akos/Programs/OpenCV/opencv-2.4.9/build/lib \
			-lopencv_core \
			-lopencv_highgui \
			-lopencv_imgproc \
			-lopencv_features2d \
			../Framework/libCommunication/libCommunication.a \
			../Framework/libLogConfigTime/libLogConfigTime.a \
			../Framework/libPlatformSpecifics/libPlatformSpecifics.a
			
OBJS = 		src/main.o

TARGET = 	m3host

$(TARGET): framework $(OBJS)
	$(CXX) $(INCLUDES) -o $(TARGET) $(OBJS) $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

framework:
	make -C ../Framework/
	
all: $(TARGET)
	
clean:
	rm -rf $(OBJS) $(TARGET)
	
cleanall: clean
	make -C ../Framework/ clean


.PHONY: framework clean
