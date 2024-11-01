SRC_DIR = src
BUILD_DIR = bin
PROJECT = $(BUILD_DIR)/tetris.out
VERBOSE = 0

CC = gcc
CPP = $(CC)
AS = $(CC)
LD = $(CC)

CPPFLAGS = -MD -I$(SRC_DIR)/include
CFLAGS = -save-temps=obj -g -O0 -c -std=c99 -Wall
ASFLAGS = -g -arch arm64
LDFLAGS = -v
#LDFLAGS = -arch arm64 -e _start -lSystem -syslibroot `xcrun -sdk macosx --show-sdk-path` 

CFILES = main.c tetromino.c board.c view.c terminal.c
SFILES =
IFILES = $(CFILES:%.c=$(BUILD_DIR)/%.i)
OFILES = $(CFILES:%.c=$(BUILD_DIR)/%.o)
OFILES += $(SFILES:%.s=$(BUILD_DIR)/%.o)

ifeq ($(VERBOSE), 0)
	Q = @
endif

.PHONY: all clean
all: $(PROJECT)
run:
	./$(PROJECT)
clean:
	rm -rf $(BUILD_DIR)

$(PROJECT): $(OFILES)
	@printf "LD\t$@\n"
	$(Q)$(LD) $(LDFLAGS) -o$@ -- $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@printf "CC\t$@\n"
	@mkdir -p $(dir $@)
	$(Q)$(CC) $(CPPFLAGS) $(CFLAGS) -o$@ -- $< 

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.s
	@printf "AS\t$@\n"
	@mkdir -p $(dir $@)
	$(Q)$(AS) $(ASFLAGS) -o$@ -- $< 

-include $(OFILES:%.o=%.d)
