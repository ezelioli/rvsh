CROSS_COMPILE ?= riscv64-unknown-elf-
RISCV_CC       = $(CROSS_COMPILE)gcc
RISCV_LD       = $(CROSS_COMPILE)ld
RISCV_OBJDUMP  = $(CROSS_COMPILE)objdump
RISCV_OBJCOPY  = $(CROSS_COMPILE)objcopy
RISCV_READELF  = $(CROSS_COMPILE)readelf
CFLAGS_ARCH    = -march=rv64imac_zifencei_zicsr -mabi=lp64 -mcmodel=medany
CFLAGS_GEN     = -Wall -Wextra -O1 -g -static-pie -fno-common -ffreestanding 
LDFLAGS        = -nostdlib -static -T ./link/link.ld # -lgcc 
INCDIRS        = ./include ./include/rvlib
SRCDIRS        = ./rvlib ./src
BUILDDIR       = ./build
SRCS_S         = 
SRCS_C         = 
APPNAME        = rvsh

CFLAGS = $(CFLAGS_ARCH) $(CFLAGS_GEN)

.PHONY: all clean
.PRECIOUS: %.elf

-include $(addsuffix /srcs.mk, $(SRCDIRS))

TARGETS_C := $(SRCS_C:.c=.o)
TARGETS_S := $(SRCS_S:.S=.o)

INCLUDES = $(addprefix -I, $(INCDIRS))

all: $(APPNAME).dump $(APPNAME).info

info:
	@echo "INCDIRS   : $(INCDIRS)"
	@echo "SRCDIRS   : $(SRCDIRS)"
	@echo "BUILDDIR  : $(BUILDDIR)"
	@echo "TARGETS_S : $(TARGETS_S)"
	@echo "TARGETS_C : $(TARGETS_C)"
	@echo "VERBOSE   : $(VERBOSE)"

$(BUILDDIR):
	@echo "Creating build directory tree"
	@mkdir -p $(BUILDDIR)
	@mkdir -p $(addprefix $(BUILDDIR)/, $(SRCDIRS))

$(TARGETS_S): $(BUILDDIR)
	@echo "Compiling source $(@:.o=.S)"
	@$(RISCV_CC) $(CFLAGS) -c $(@:.o=.S) -o $(addprefix $(BUILDDIR)/, $@)

$(TARGETS_C): $(BUILDDIR)
	@echo "Compiling source $(@:.o=.S)"
	@$(RISCV_CC) $(CFLAGS) $(INCLUDES) -c $(@:.o=.c) -o $(addprefix $(BUILDDIR)/, $@)

%.elf: $(TARGETS_S) $(TARGETS_C)
	@echo "Linking objects into $(BUILDDIR)/$@"
	@$(RISCV_LD) $(LDFLAGS) -o $(BUILDDIR)/$@ $(addprefix $(BUILDDIR)/, $^)
	
%.bin: %.elf
	@$(RISCV_OBJCOPY) -S -O binary $(BUILDDIR)/$< $(BUILDDIR)/$@

%.dump: %.elf %.bin $(BUILDDIR)
	@echo "Generating dump file $(BUILDDIR)/$@"
	@$(RISCV_OBJDUMP) -d $(BUILDDIR)/$< > $(BUILDDIR)/$@

%.info: %.elf $(BUILDDIR)
	@echo "Generating info file $(BUILDDIR)/$@"
	@$(RISCV_READELF) -a $(BUILDDIR)/$< > $(BUILDDIR)/$@

clean:
	@echo "Cleaning build files"
	@rm -rf $(BUILDDIR)
