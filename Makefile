DEBUG_BUILD=1
BIN_DIR = ./bin
BIN_EXE = demo.exe
SRC_DIR = .
BUILD_DIR = ./build

CXX = i686-w64-mingw32-g++
WINDRES = i686-w64-mingw32-windres
STRIP = i686-w64-mingw32-strip
CFLAGS = --std=c++17 -Wall -Wextra -Werror -Wno-unused-parameter \
	-D___cplusplus
LINKFLAGS = -static-libgcc -static-libstdc++ -static
INCLUDES = -I. 
LIBS= -lgdi32 -lwinmm -lopengl32 -lole32

ifeq ($(DEBUG_BUILD), 1)
	BUILD_NAME = debug
	REV +=-DEBUG
	CFLAGS += -g -D_DEBUG
else
	BUILD_NAME = release
	CFLAGS += -nostdlib -O6
	LIBS += -lkernel32 -luser32 -lmsvcrt
endif
CFLAGS += $(INCLUDES)

SOURCES = $(shell find $(SRC_DIR) -name '*.cpp')
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
DEPS = $(OBJECTS:.o=.d)

print-%: ; @echo $*=$($*)

export V := 0
export CMD_PREFIX := @
ifeq ($(V),1)
	CMD_PREFIX :=
endif

TIME_FILE = $(dir $@).$(notdir $@)_time
START_TIME = date '+%s' > $(TIME_FILE)
END_TIME = read st < $(TIME_FILE) ; \
	$(RM) $(TIME_FILE) ; \
	st=$$((`date '+%s'` - $$st - 86400)) ; \
	echo `date -u -d @$$st '+%H:%M:%S'`

.PHONY: all
all:
	@echo "Beginning $(BUILD_NAME) build"
	@$(START_TIME)
	@$(MAKE) $(BUILD_NAME) --no-print-directory
	@echo -n "Total build time: "
	@$(END_TIME)

dirs:
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(BUILD_DIR)

ModeSwitchDemo.rc.o: dirs
	$(CMD_PREFIX)$(WINDRES) -J rc ModeSwitchDemo.rc $(BUILD_DIR)/ModeSwitchDemo.rc.o

debug: dirs $(BIN_DIR)/$(BIN_EXE)

release: dirs $(BIN_DIR)/$(BIN_EXE)
	$(CMD_PREFIX)$(STRIP) -s $(BIN_DIR)/$(BIN_EXE)

.PHONY: clean
clean:
	- rm -rf $(BIN_DIR) $(BUILD_DIR)

# compile
-include $(DEPS)

.SECONDARY: $(OBJECTS)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compiling: $< -> $@"
	$(CMD_PREFIX)$(CXX) $(CFLAGS) -MP -MMD -c $< -o $@

# link
$(BIN_DIR)/$(BIN_EXE): ModeSwitchDemo.rc.o $(OBJECTS)
	@echo "Linking: $@"
	$(CMD_PREFIX)$(CXX) $(OBJECTS) $(BUILD_DIR)/ModeSwitchDemo.rc.o $(LINKFLAGS) $(LIBS) -o $@
