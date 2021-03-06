#######################################
# setting build environment and rules #
#######################################
NASM32 = nasm
GCC32 = x86_64-pc-linux-gcc -c -m32 -ffreestanding
LD32 = x86_64-pc-linux-ld -melf_i386 -T ../elf_i386.x -nostdlib -e Main -Ttext 0x10200
OBJCOPY32 = x86_64-pc-linux-objcopy -j .text -j .data -j .rodata -j .bss -S -O binary

OBJECTDIRECTORY = Temp
SOURCEDIRECTORY = Source

########################################
# setting build item and build method. #
# ######################################

all: prepare Kernel32.bin

prepare:
	mkdir -p $(OBJECTDIRECTORY)

$(OBJECTDIRECTORY)/EntryPoint.bin: $(SOURCEDIRECTORY)/EntryPoint.s
	$(NASM32) -o $@ $<

dep:
	@echo === Make Dependency file ===
	make -C $(OBJECTDIRECTORY) -f ../makefile InternelDependency
	@echo === Dependency Search Compelete ===

ExecuteInternelBuild: dep
	make -C $(OBJECTDIRECTORY) -f ../makefile Kernel32.elf

$(OBJECTDIRECTORY)/Kernel32.elf.bin: ExecuteInternelBuild
	$(OBJCOPY32) $(OBJECTDIRECTORY)/Kernel32.elf $@

Kernel32.bin: $(OBJECTDIRECTORY)/EntryPoint.bin $(OBJECTDIRECTORY)/Kernel32.elf.bin
	cat $^ > $@

clean:
	rm -f *.bin
	rm -f $(OBJECTDIRECTORY)/*.*

##########################################################
# The part called again by Make, executed based on Temp. #
# ########################################################
CENTRYPOINTOBJECTFILE = Main.o
CSOURCEFILES = $(wildcard ../$(SOURCEDIRECTORY)/*.c)
ASSEMBLYSOURCEFILES = $(wildcard ../$(SOURCEDIRECTORY)/*.asm)
COBJECTFILES = $(subst Main.o, , $(notdir $(patsubst %.c,%.o,$(CSOURCEFILES))))
ASSEMBLYOBJECTFILES = $(notdir $(patsubst %.asm,%.o,$(ASSEMBLYSOURCEFILES)))

%.o: ../$(SOURCEDIRECTORY)/%.c
	$(GCC32) -c $<

%.o: ../$(SOURCEDIRECTORY)/%.asm
	$(NASM32) -f elf32 -o $@ $<

InternelDependency:
	$(GCC32) -MM $(CSOURCEFILES) > Dependency.dep

Kernel32.elf: $(CENTRYPOINTOBJECTFILE) $(COBJECTFILES) $(ASSEMBLYOBJECTFILES)
	$(LD32) -o $@ $^

ifeq (Dependency.dep, $(wildcard Dependency.dep))
include Dependency.dep
endif
