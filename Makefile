CPP			= g++
CPPFLAGS	= -I./include -Wextra -Wall -Wno-unused-parameter


LINKFLAGS 	=
ifeq ($(OS), Windows_NT) # windows link flags
	LINKFLAGS += -L./lib/windows -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_mixer -lSDL2_image -lWinmm -lGdi32 -lRpcrt4 -lWs2_32 -lDbghelp
else	# linux link flags
	LINKFLAGS += -L/usr/local/lib/ $(shell pkg-config --libs sdl2 SDL2_ttf SDL2_image SDL2_mixer) -lm
endif

# folder to store .o files
OUT			= build
SRC			= src

CPP_FILES := $(subst src/,$(empty),$(wildcard $(SRC)/*.cpp))
CPPOBJS := \
	$(foreach file,$(CPP_FILES),$(OUT)/$(file:.cpp=.o))

all: RayCaster

$(OUT)/%.o: $(SRC)/%.cpp
	$(CPP) -Wall -g $(CPPFLAGS) -c $< -o $@

RayCaster: $(CPPOBJS)
	$(CPP) -Wall -g $(CPPOBJS) $(LINKFLAGS) -o RayCaster

$(OUT):
	mkdir -p $(OUT)

clean:
	rm -f $(OUT)/*.o RayCaster
