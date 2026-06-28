CPP			= g++
CPPFLAGS	= -I./include -Wextra -Wall -Wno-unused-parameter

# folder to store .o files
OUT			= build
SRC			= src

CPP_FILES := $(subst src/,$(empty),$(wildcard $(SRC)/*.cpp))
CPPOBJS := \
	$(foreach file,$(CPP_FILES),$(OUT)/$(file:.cpp=.o))

all: CastEngine

$(OUT)/%.o: $(SRC)/%.cpp
	$(CPP) -Wall -g $(CPPFLAGS) -c $< -o $@

CastEngine: $(CPPOBJS)
	ar rcs libCastEngine.a $(CPPOBJS)

$(OUT):
	mkdir -p $(OUT)

$(CPPOBJS): | $(OUT)

clean:
	rm -f $(OUT)/*.o libCastEngine.a
