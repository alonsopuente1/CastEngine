CPP			= g++
CPPFLAGS	= -I./include -Wextra -Wall -Wno-unused-parameter

# folder to store .o files
OUT			= build
SRC			= src

# all the .cpp files in the src folder
CPP_FILES := $(subst src/,$(empty),$(wildcard $(SRC)/*.cpp))

# replaces .cpp extension with .o and adds the build folder path
CPPOBJS := \
	$(foreach file,$(CPP_FILES),$(OUT)/$(file:.cpp=.o))

# default target
all: CastEngine

$(OUT)/%.o: $(SRC)/%.cpp
	$(CPP) -Wall -g $(CPPFLAGS) -c $< -o $@

CastEngine: $(CPPOBJS)
	ar rcs libCastEngine.a $(CPPOBJS)

$(OUT):
	mkdir -p $(OUT)

clean:
	rm -f $(OUT)/*.o libCastEngine.a
