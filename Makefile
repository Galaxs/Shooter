# Compiler settings
CXX := g++
CXX_FLAGS := -Wall -Wextra -std=c++17

# Directories
BIN := bin
SRC := src
INCLUDE := include
LIB := lib

# SFML libraries
SFML_LIBRARIES := -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

# Source files
SRCS := $(wildcard $(SRC)/*.cpp)

# Object files
OBJS := $(patsubst $(SRC)/%.cpp,$(BIN)/%.o,$(SRCS))

# Executable name
EXECUTABLE := Shooter.exe

all: compile link run

run:
	cmd /c "$(BIN)\$(EXECUTABLE)"

compile:
	@if not exist $(BIN) mkdir $(BIN)
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -c $(SRC)/Main.cpp -o $(BIN)/Main.o
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -c $(SRC)/Player.cpp -o $(BIN)/Player.o
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -c $(SRC)/Arena.cpp -o $(BIN)/Arena.o
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -c $(SRC)/Zombie.cpp -o $(BIN)/Zombie.o
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -c $(SRC)/TextureHolder.cpp -o $(BIN)/TextureHolder.o
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -c $(SRC)/CreateHorde.cpp -o $(BIN)/CreateHorde.o
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -c $(SRC)/Bullet.cpp -o $(BIN)/Bullet.o




	


link: $(OBJS)
	$(CXX) $(CXX_FLAGS) -L$(LIB) $^ -o $(BIN)/$(EXECUTABLE) $(SFML_LIBRARIES)

clean:
	del /Q $(BIN)\*.*
