CXX = g++
CXXFLAGS = -Wall -std=c++17 -O2
LDFLAGS = -lssl -lcrypto
TARGET = programa_BRBEPO
SRCS = main.cpp encriptador.cpp descriptador.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)