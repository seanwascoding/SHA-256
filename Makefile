CXX=g++
CXXFLAGS=-std=c++11
OBJ=main.o SHA-256.o
SRC=main.cpp SHA-256.cpp sha-256.h
EXE=test
all: $(EXE)
$(EXE): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^
$(OBJ): $(SRC)
	$(CXX) $(CXXFLAGS) -c $(SRC)
clean:
	rm -f $(EXE)
	rm -f $(OBJ)
