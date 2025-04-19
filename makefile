BIN := main
SRC_PATH := src
OBJ_PATH := build

CXX := g++
DEP_FLAGS := -MP -MD
LD_FLAGS :=  -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
CXX_FLAGS := -g -Wall -std=c++17 -fpermissive $(DEP_FLAGS) $(LD_FLAGS)
CPP_FILES := $(wildcard $(SRC_PATH)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_PATH)/%.cpp,$(OBJ_PATH)/%.o,$(CPP_FILES))
DEP_FILES := $(patsubst $(SRC_PATH)/%.cpp,$(OBJ_PATH)/%.d,$(CPP_FILES))

ifeq ($(OS),Windows_NT)
	RM := rmdir /s /q
	MKDIR := if not exist "$(OBJ_PATH)" mkdir "$(OBJ_PATH)"
	RUN := $(OBJ_PATH)\$(BIN).exe
else
	RM := rm -rf
	MKDIR := mkdir -p $(OBJ_PATH)
	RUN := ./$(OBJ_PATH)/$(BIN)
endif

all: $(OBJ_PATH)/$(BIN)

$(OBJ_PATH)/$(BIN): $(OBJ_FILES)
	$(CXX) -o $@ $^ $(LD_FLAGS)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp
	$(MKDIR)
	$(CXX) $(CXX_FLAGS) -c -o $@ $<

run: all
	$(RUN)

clean:
	$(RM) $(OBJ_PATH)

-include $(DEP_FILES)

.PHONY: all run clean
