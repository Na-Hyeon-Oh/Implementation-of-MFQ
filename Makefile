#Makefile
TARGET=MFQ
CXX=gcc
CXXFLAGS=-Wall
OBJECTS=MFQ.o
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)
MFQ.o: MFQ.c
	$(CXX) $(CXXFLAGS) -c MFQ.c
clean:
	rm $(OBJECTS) $(TARGET)
