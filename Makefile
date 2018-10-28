CXX		= g++
CXXFLAGS	+= -std=c++11

OBJS = $(patsubst %.cpp,%.o,$(shell ls *.cpp))
TARGET = HO_Proj_CETS_ACO
INCLUDE := ./
all:$(TARGET)
$(TARGET):$(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) ./objs/*.o
$(OBJS):%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c $< $(INCLUDE) -o ./objs/$@
clean:
	rm -f hello ./objs/*.o
