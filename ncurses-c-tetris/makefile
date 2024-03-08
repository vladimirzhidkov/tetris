PROJECT = tetris.out

SRC_DIR = src
BUILD_DIR = bin
INCLUDE_DIR = $(SRC_DIR)/include
CFILES = $(patsubst %,$(SRC_DIR)/%, main.c tetromino.c board.c view.c)
AFILES = $(patsubst %,$(SRC_DIR)/%,)
LIBS = -lncurses


VERBOSE = 0
CC = gcc
AS = $(CC) 
LD = $(CC) 
CPPFLAGS = -MD -I$(INCLUDE_DIR)
CFLAGS = -g -O0 -c -std=c99 -Wall
ASFLAGS = -g -arch arm64
LDFLAGS = $(LIBS)
#LDFLAGS = -arch arm64 -e _start -lSystem -syslibroot `xcrun -sdk macosx --show-sdk-path` 

OBJS = $(CFILES:%.c=$(BUILD_DIR)/%.o)
OBJS += $(AFILES:%.s=$(BUILD_DIR)/%.o)


ifeq ($(VERBOSE), 0)
	Q = @
endif

.PHONY: all clean

all: $(PROJECT)
run:
	$(Q)./$(PROJECT)
clean:
	rm -rf $(BUILD_DIR) $(PROJECT)
$(PROJECT): $(OBJS)
	@printf "LD\t$@\n"
	$(Q)$(LD) $(LDFLAGS) -o $@ $^
#	@ls -lah $@
$(BUILD_DIR)/%.o: %.c
	@printf "CC\t$@\n"
	@mkdir -p $(dir $@)
	$(Q)$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $< 
$(BUILD_DIR)/%.o: %.s
	@printf "AS\t$@\n"
	@mkdir -p $(dir $@)
	$(Q)$(AS) $(ASFLAGS) -o $@ $< 

-include $(OBJS:%.o=%.d)
