CXX := c++
SRC := ./src/
INCLUDE := ./include/
LIBS := -lboost_serialization -lboost_filesystem -lboost_system -lz `root-config --libs`
CFLAGS := -I $(INCLUDE) `root-config --cflags`

BIN := gts

OBJ := $(wildcard $(SRC)*.cxx)
OBJ := $(OBJ:.cxx=.o)

all: $(OBJ)
	@echo ----------------------------------
	@echo Linking ....
	@$(CXX) $(OBJ) $(LIBS) -o $(BIN)
	@echo Binary file \"$(BIN)\" has been created.

$(SRC)%.o: $(SRC)%.cxx $(INCLUDE)%.h
	@echo Compiling $< ....
	@$(CXX) -c $< $(CFLAGS) -o $@

$(SRC)%.o: $(SRC)%.cxx
	@echo Compiling $< ....
	@$(CXX) -c $< $(CFLAGS) -o $@

clean:
	@rm -f $(SRC)*.o $(BIN)

.PHONY: all clean

