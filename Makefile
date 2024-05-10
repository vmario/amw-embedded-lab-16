.PHONY: clean build all program erase sizebefore sizeafter

MCU = atmega328p
F_CPU = 16000000

USRBIN ?= C:/msys64/usr/bin/
TOOLCHAIN ?= C:/Bin/avr8-gnu-toolchain-win32_x86_64/bin/avr-
AVRDUDE ?= C:/Bin/avrdude-v7.1-windows-x64/avrdude
PORT ?= /dev/ttyACM0

CC = $(TOOLCHAIN)gcc
CXX = $(TOOLCHAIN)g++
OBJCOPY = $(TOOLCHAIN)objcopy
SIZE = $(TOOLCHAIN)size
OBJDUMP = $(TOOLCHAIN)objdump
NM = $(TOOLCHAIN)nm
RM = $(USRBIN)rm -f
RMDIR = $(USRBIN)rm -rf
MKDIR = $(USRBIN)mkdir

FORMAT = ihex
MSG_SIZE_BEFORE = Size before:
MSG_SIZE_AFTER = Size after:

DEPDIR = dep
OBJDIR = obj
TARGETDIR = bin
LIBDIR = lib

INCLUDES = -I.
LIBS = -lm
OPTIMIZATION = -O2

CFLAGS = -mmcu=$(MCU)
CFLAGS += -std=c99 -Wall -Wundef -Wextra -pedantic -Wstrict-prototypes
CFLAGS += $(OPTIMIZATION) -g0 -flto
CFLAGS += -Wa,-adhlns=$(<:%.c=$(OBJDIR)/%.lst)
CFLAGS += -MD -MP -MF $(DEPDIR)/$(basename $(@F)).d
CXXFLAGS = -mmcu=$(MCU)
CXXFLAGS += -std=c++14 -Wall -Wundef -Wextra -pedantic
CXXFLAGS += $(OPTIMIZATION) -g0 -flto -fno-exceptions
CXXFLAGS += -Wa,-adhlns=$(<:%.cpp=$(OBJDIR)/%.lst)
CXXFLAGS += -MD -MP -MF $(DEPDIR)/$(basename $(@F)).d
LDFLAGS = -mmcu=$(MCU)
LDFLAGS += $(OPTMIZATION) -g0 -flto
LDFLAGS += -Wl,-u,vfprintf -lprintf_flt
LDFLAGS += -Wl,-Map=$(TARGET).map,--cref
DEFS = -DF_CPU=$(F_CPU)ul

SRCS = $(wildcard *.cpp)
OBJS = $(addprefix $(OBJDIR)/,$(SRCS:.cpp=.o))
OBJS_LIB = $(wildcard $(LIBDIR)/*.o)
DEPS = $(addprefix $(DEPDIR)/,$(SRCS:.cpp=.d))
TARGET = $(TARGETDIR)/laboratory

all: sizebefore build sizeafter

build: $(TARGET).hex $(TARGET).lss $(TARGET).sym

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex $< $@

$(TARGET).elf: $(OBJS) $(OBJS_LIB) | $(TARGETDIR)
	$(CXX) $(LDFLAGS) $(LIBS) $(DEFS) -o $@ $(OBJS) $(OBJS_LIB)

$(TARGET).lss: $(TARGET).elf
	$(OBJDUMP) -h -S $< > $@

$(TARGET).sym: $(TARGET).elf
	$(NM) -n $< > $@

$(OBJDIR)/%.o: %.c | $(OBJDIR) $(DEPDIR)
	$(CC) $(CFLAGS) $(INCLUDES) $(DEFS) -o $@ -c $<

$(OBJDIR)/%.o: %.cpp | $(OBJDIR) $(DEPDIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEFS) -o $@ -c $<

$(TARGETDIR):
	$(MKDIR) $(TARGETDIR)

$(OBJDIR):
	$(MKDIR) $(OBJDIR)

$(DEPDIR):
	$(MKDIR) $(DEPDIR)

-include $(DEPS)

clean:
	$(RM) $(DEPS)
	$(RM) $(OBJS)
	$(RMDIR) $(OBJDIR)
	$(RM) $(DEPS)
	$(RMDIR) $(DEPDIR)
	$(RM) $(TARGET).map
	$(RM) $(TARGET).lss
	$(RM) $(TARGET).sym
	$(RM) $(TARGET).elf
	$(RM) $(TARGET).hex
	$(RMDIR) $(TARGETDIR)

program: all
	$(AVRDUDE) \
		-p $(MCU) -c arduino -P $(PORT) -b 115200 \
		-U flash:w:$(TARGET).hex:i

erase:
	$(AVRDUDE) \
		-p $(MCU) -c usbtiny \
		-U lfuse:w:0xff:m -U hfuse:w:0xde:m -U efuse:w:0xfd:m -U lock:w:0xcf:m \
		-U flash:w:bootloader/optiboot_atmega328.hex:i

ELFSIZE = $(SIZE) --format=avr --mcu=$(MCU) $(TARGET).elf

sizebefore:
	@if test -f $(TARGET).elf; then echo; echo $(MSG_SIZE_BEFORE); $(ELFSIZE); fi

sizeafter:
	@if test -f $(TARGET).elf; then echo; echo $(MSG_SIZE_AFTER); $(ELFSIZE); fi
