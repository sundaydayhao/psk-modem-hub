##
## Auto Generated makefile by CDK
## Do not modify this file, and any manual changes will be erased!!!   
##
## BuildSet
ProjectName            :=aim5101-gpio
ConfigurationName      :=BuildSet
WorkspacePath          :=../../../../../../
ProjectPath            :=./
IntermediateDirectory  :=Obj
OutDir                 :=$(IntermediateDirectory)
User                   :=zhang
Date                   :=25/07/2026
CDKPath                :=../../../../../../../../../C-Sky/CDK
ToolchainPath          :=C:/C-Sky/CDKRepo/Toolchain/XTGccElfNewlib/V2.10.0/R/
LinkerName             :=riscv64-unknown-elf-gcc
LinkerNameoption       :=
SIZE                   :=riscv64-unknown-elf-size
READELF                :=riscv64-unknown-elf-readelf
CHECKSUM               :=crc32
SharedObjectLinkerName :=
ObjectSuffix           :=.o
DependSuffix           :=.d
PreprocessSuffix       :=.i
DisassemSuffix         :=.asm
IHexSuffix             :=.ihex
BinSuffix              :=.bin
ExeSuffix              :=.elf
LibSuffix              :=.a
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
ElfInfoSwitch          :=-hlS
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
UnPreprocessorSwitch   :=-U
SourceSwitch           :=-c 
ObjdumpSwitch          :=-S
ObjcopySwitch          :=-O ihex
ObjcopyBinSwitch       :=-O binary
OutputFile             :=$(ProjectName)
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
PreprocessOnlyDisableLineSwitch   :=-P
ObjectsFileList        :=$(IntermediateDirectory)/aim5101-gpio.txt
MakeDirCommand         :=mkdir
LinkOptions            := -mcpu=e906  -nostartfiles -Wl,--gc-sections -T"$(ProjectPath)/../../../../../board/aim5101_board/gcc_csky.ld" -pipe 
LinkOtherFlagsOption   :=-Wl,-zmax-page-size=1024
IncludePackagePath     :=
IncludeCPath           := $(IncludeSwitch). $(IncludeSwitch)../../../../../csi_core/include $(IncludeSwitch)../../../../../csi_driver/include $(IncludeSwitch)../../../../../libs/include $(IncludeSwitch)../../../../../csi_driver/aim5101_rv32/include $(IncludeSwitch)../../../../../csi_kernel/include $(IncludeSwitch)../../../../../board/aim5101_board/include $(IncludeSwitch)../configs  
IncludeAPath           := $(IncludeSwitch). $(IncludeSwitch)../configs  
Libs                   := -Wl,--start-group  -Wl,--end-group $(LibrarySwitch)m  
ArLibs                 := "m" 
PackagesLibPath        :=
LibPath                := $(PackagesLibPath) 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       :=riscv64-unknown-elf-ar rcu
CXX      :=riscv64-unknown-elf-g++
CC       :=riscv64-unknown-elf-gcc
AS       :=riscv64-unknown-elf-gcc
OBJDUMP  :=riscv64-unknown-elf-objdump
OBJCOPY  :=riscv64-unknown-elf-objcopy
CXXFLAGS :=-mcpu=e906 -Os  -g3  -Wall -ffunction-sections -fdata-sections -pipe 
CFLAGS   :=-mcpu=e906 -Os  -g3  -Wall -ffunction-sections -fdata-sections -pipe 
ASFLAGS  :=-mcpu=e906  -Wa,-gdwarf-2 -pipe 
PreprocessFlags  :=-mcpu=e906 -Os    -Wall -ffunction-sections -fdata-sections -pipe 


Objects0=$(IntermediateDirectory)/aim5101_board_board_init$(ObjectSuffix) $(IntermediateDirectory)/aim5101_rv32_ck_irq$(ObjectSuffix) $(IntermediateDirectory)/aim5101_rv32_ck_usart$(ObjectSuffix) $(IntermediateDirectory)/aim5101_rv32_devices$(ObjectSuffix) $(IntermediateDirectory)/aim5101_rv32_dw_gpio$(ObjectSuffix) $(IntermediateDirectory)/aim5101_rv32_dw_timer$(ObjectSuffix) $(IntermediateDirectory)/aim5101_rv32_isr$(ObjectSuffix) $(IntermediateDirectory)/aim5101_rv32_lib$(ObjectSuffix) $(IntermediateDirectory)/aim5101_rv32_novic_irq_tbl$(ObjectSuffix) $(IntermediateDirectory)/aim5101_rv32_pinmux$(ObjectSuffix) \
	$(IntermediateDirectory)/aim5101_rv32_startup$(ObjectSuffix) $(IntermediateDirectory)/aim5101_rv32_sys_freq$(ObjectSuffix) $(IntermediateDirectory)/aim5101_rv32_system$(ObjectSuffix) $(IntermediateDirectory)/aim5101_rv32_trap_c$(ObjectSuffix) $(IntermediateDirectory)/aim5101_rv32_vectors$(ObjectSuffix) $(IntermediateDirectory)/libc_clock_gettime$(ObjectSuffix) $(IntermediateDirectory)/libc__init$(ObjectSuffix) $(IntermediateDirectory)/libc_malloc$(ObjectSuffix) $(IntermediateDirectory)/libc_minilibc_port$(ObjectSuffix) $(IntermediateDirectory)/libc_printf$(ObjectSuffix) \
	$(IntermediateDirectory)/mm_dq_addlast$(ObjectSuffix) $(IntermediateDirectory)/mm_dq_rem$(ObjectSuffix) $(IntermediateDirectory)/mm_lib_mallinfo$(ObjectSuffix) $(IntermediateDirectory)/mm_mm_addfreechunk$(ObjectSuffix) $(IntermediateDirectory)/mm_mm_free$(ObjectSuffix) $(IntermediateDirectory)/mm_mm_initialize$(ObjectSuffix) $(IntermediateDirectory)/mm_mm_leak$(ObjectSuffix) $(IntermediateDirectory)/mm_mm_mallinfo$(ObjectSuffix) $(IntermediateDirectory)/mm_mm_malloc$(ObjectSuffix) $(IntermediateDirectory)/mm_mm_size2ndx$(ObjectSuffix) \
	$(IntermediateDirectory)/ringbuffer_ringbuffer$(ObjectSuffix) $(IntermediateDirectory)/syslog_syslog$(ObjectSuffix) $(IntermediateDirectory)/gpio_example_gpio$(ObjectSuffix) $(IntermediateDirectory)/gpio_aim5101_codec_gpio$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all
all: $(IntermediateDirectory)/$(OutputFile)

$(IntermediateDirectory)/$(OutputFile):  $(Objects) Always_Link 
	$(LinkerName) $(OutputSwitch) $(IntermediateDirectory)/$(OutputFile)$(ExeSuffix) $(LinkerNameoption) $(Objects)  $(LinkOptions) $(LibPath) $(Libs) $(LinkOtherFlagsOption)
	$(OBJDUMP) $(ObjdumpSwitch) $(ProjectPath)/$(IntermediateDirectory)/$(OutputFile)$(ExeSuffix)  > $(ProjectPath)/Lst/$(OutputFile)$(DisassemSuffix) 
	@echo size of target:
	@$(SIZE) $(ProjectPath)$(IntermediateDirectory)/$(OutputFile)$(ExeSuffix) 
	@echo -n checksum value of target:  
	@$(CHECKSUM) $(ProjectPath)/$(IntermediateDirectory)/$(OutputFile)$(ExeSuffix) 
	@aim5101-gpio.modify.bat $(IntermediateDirectory) $(OutputFile)$(ExeSuffix) 

Always_Link:


##
## Objects
##
$(IntermediateDirectory)/aim5101_board_board_init$(ObjectSuffix): ../../../../../board/aim5101_board/board_init.c  
	$(CC) $(SourceSwitch) ../../../../../board/aim5101_board/board_init.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/aim5101_board_board_init$(ObjectSuffix) -MF$(IntermediateDirectory)/aim5101_board_board_init$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/aim5101_board_board_init$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/aim5101_board_board_init$(PreprocessSuffix): ../../../../../board/aim5101_board/board_init.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/aim5101_board_board_init$(PreprocessSuffix) ../../../../../board/aim5101_board/board_init.c

$(IntermediateDirectory)/aim5101_rv32_ck_irq$(ObjectSuffix): ../../../../../csi_driver/aim5101_rv32/ck_irq.c  
	$(CC) $(SourceSwitch) ../../../../../csi_driver/aim5101_rv32/ck_irq.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/aim5101_rv32_ck_irq$(ObjectSuffix) -MF$(IntermediateDirectory)/aim5101_rv32_ck_irq$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/aim5101_rv32_ck_irq$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/aim5101_rv32_ck_irq$(PreprocessSuffix): ../../../../../csi_driver/aim5101_rv32/ck_irq.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/aim5101_rv32_ck_irq$(PreprocessSuffix) ../../../../../csi_driver/aim5101_rv32/ck_irq.c

$(IntermediateDirectory)/aim5101_rv32_ck_usart$(ObjectSuffix): ../../../../../csi_driver/aim5101_rv32/ck_usart.c  
	$(CC) $(SourceSwitch) ../../../../../csi_driver/aim5101_rv32/ck_usart.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/aim5101_rv32_ck_usart$(ObjectSuffix) -MF$(IntermediateDirectory)/aim5101_rv32_ck_usart$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/aim5101_rv32_ck_usart$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/aim5101_rv32_ck_usart$(PreprocessSuffix): ../../../../../csi_driver/aim5101_rv32/ck_usart.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/aim5101_rv32_ck_usart$(PreprocessSuffix) ../../../../../csi_driver/aim5101_rv32/ck_usart.c

$(IntermediateDirectory)/aim5101_rv32_devices$(ObjectSuffix): ../../../../../csi_driver/aim5101_rv32/devices.c  
	$(CC) $(SourceSwitch) ../../../../../csi_driver/aim5101_rv32/devices.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/aim5101_rv32_devices$(ObjectSuffix) -MF$(IntermediateDirectory)/aim5101_rv32_devices$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/aim5101_rv32_devices$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/aim5101_rv32_devices$(PreprocessSuffix): ../../../../../csi_driver/aim5101_rv32/devices.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/aim5101_rv32_devices$(PreprocessSuffix) ../../../../../csi_driver/aim5101_rv32/devices.c

$(IntermediateDirectory)/aim5101_rv32_dw_gpio$(ObjectSuffix): ../../../../../csi_driver/aim5101_rv32/dw_gpio.c  
	$(CC) $(SourceSwitch) ../../../../../csi_driver/aim5101_rv32/dw_gpio.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/aim5101_rv32_dw_gpio$(ObjectSuffix) -MF$(IntermediateDirectory)/aim5101_rv32_dw_gpio$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/aim5101_rv32_dw_gpio$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/aim5101_rv32_dw_gpio$(PreprocessSuffix): ../../../../../csi_driver/aim5101_rv32/dw_gpio.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/aim5101_rv32_dw_gpio$(PreprocessSuffix) ../../../../../csi_driver/aim5101_rv32/dw_gpio.c

$(IntermediateDirectory)/aim5101_rv32_dw_timer$(ObjectSuffix): ../../../../../csi_driver/aim5101_rv32/dw_timer.c  
	$(CC) $(SourceSwitch) ../../../../../csi_driver/aim5101_rv32/dw_timer.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/aim5101_rv32_dw_timer$(ObjectSuffix) -MF$(IntermediateDirectory)/aim5101_rv32_dw_timer$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/aim5101_rv32_dw_timer$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/aim5101_rv32_dw_timer$(PreprocessSuffix): ../../../../../csi_driver/aim5101_rv32/dw_timer.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/aim5101_rv32_dw_timer$(PreprocessSuffix) ../../../../../csi_driver/aim5101_rv32/dw_timer.c

$(IntermediateDirectory)/aim5101_rv32_isr$(ObjectSuffix): ../../../../../csi_driver/aim5101_rv32/isr.c  
	$(CC) $(SourceSwitch) ../../../../../csi_driver/aim5101_rv32/isr.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/aim5101_rv32_isr$(ObjectSuffix) -MF$(IntermediateDirectory)/aim5101_rv32_isr$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/aim5101_rv32_isr$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/aim5101_rv32_isr$(PreprocessSuffix): ../../../../../csi_driver/aim5101_rv32/isr.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/aim5101_rv32_isr$(PreprocessSuffix) ../../../../../csi_driver/aim5101_rv32/isr.c

$(IntermediateDirectory)/aim5101_rv32_lib$(ObjectSuffix): ../../../../../csi_driver/aim5101_rv32/lib.c  
	$(CC) $(SourceSwitch) ../../../../../csi_driver/aim5101_rv32/lib.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/aim5101_rv32_lib$(ObjectSuffix) -MF$(IntermediateDirectory)/aim5101_rv32_lib$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/aim5101_rv32_lib$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/aim5101_rv32_lib$(PreprocessSuffix): ../../../../../csi_driver/aim5101_rv32/lib.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/aim5101_rv32_lib$(PreprocessSuffix) ../../../../../csi_driver/aim5101_rv32/lib.c

$(IntermediateDirectory)/aim5101_rv32_novic_irq_tbl$(ObjectSuffix): ../../../../../csi_driver/aim5101_rv32/novic_irq_tbl.c  
	$(CC) $(SourceSwitch) ../../../../../csi_driver/aim5101_rv32/novic_irq_tbl.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/aim5101_rv32_novic_irq_tbl$(ObjectSuffix) -MF$(IntermediateDirectory)/aim5101_rv32_novic_irq_tbl$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/aim5101_rv32_novic_irq_tbl$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/aim5101_rv32_novic_irq_tbl$(PreprocessSuffix): ../../../../../csi_driver/aim5101_rv32/novic_irq_tbl.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/aim5101_rv32_novic_irq_tbl$(PreprocessSuffix) ../../../../../csi_driver/aim5101_rv32/novic_irq_tbl.c

$(IntermediateDirectory)/aim5101_rv32_pinmux$(ObjectSuffix): ../../../../../csi_driver/aim5101_rv32/pinmux.c  
	$(CC) $(SourceSwitch) ../../../../../csi_driver/aim5101_rv32/pinmux.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/aim5101_rv32_pinmux$(ObjectSuffix) -MF$(IntermediateDirectory)/aim5101_rv32_pinmux$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/aim5101_rv32_pinmux$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/aim5101_rv32_pinmux$(PreprocessSuffix): ../../../../../csi_driver/aim5101_rv32/pinmux.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/aim5101_rv32_pinmux$(PreprocessSuffix) ../../../../../csi_driver/aim5101_rv32/pinmux.c

$(IntermediateDirectory)/aim5101_rv32_startup$(ObjectSuffix): ../../../../../csi_driver/aim5101_rv32/startup.S  
	$(AS) $(SourceSwitch) ../../../../../csi_driver/aim5101_rv32/startup.S $(ASFLAGS) -MMD -MP -MT$(IntermediateDirectory)/aim5101_rv32_startup$(ObjectSuffix) -MF$(IntermediateDirectory)/aim5101_rv32_startup$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/aim5101_rv32_startup$(ObjectSuffix) $(IncludeAPath) $(IncludePackagePath)
Lst/aim5101_rv32_startup$(PreprocessSuffix): ../../../../../csi_driver/aim5101_rv32/startup.S
	$(CC) $(CFLAGS)$(IncludeAPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/aim5101_rv32_startup$(PreprocessSuffix) ../../../../../csi_driver/aim5101_rv32/startup.S

$(IntermediateDirectory)/aim5101_rv32_sys_freq$(ObjectSuffix): ../../../../../csi_driver/aim5101_rv32/sys_freq.c  
	$(CC) $(SourceSwitch) ../../../../../csi_driver/aim5101_rv32/sys_freq.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/aim5101_rv32_sys_freq$(ObjectSuffix) -MF$(IntermediateDirectory)/aim5101_rv32_sys_freq$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/aim5101_rv32_sys_freq$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/aim5101_rv32_sys_freq$(PreprocessSuffix): ../../../../../csi_driver/aim5101_rv32/sys_freq.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/aim5101_rv32_sys_freq$(PreprocessSuffix) ../../../../../csi_driver/aim5101_rv32/sys_freq.c

$(IntermediateDirectory)/aim5101_rv32_system$(ObjectSuffix): ../../../../../csi_driver/aim5101_rv32/system.c  
	$(CC) $(SourceSwitch) ../../../../../csi_driver/aim5101_rv32/system.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/aim5101_rv32_system$(ObjectSuffix) -MF$(IntermediateDirectory)/aim5101_rv32_system$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/aim5101_rv32_system$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/aim5101_rv32_system$(PreprocessSuffix): ../../../../../csi_driver/aim5101_rv32/system.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/aim5101_rv32_system$(PreprocessSuffix) ../../../../../csi_driver/aim5101_rv32/system.c

$(IntermediateDirectory)/aim5101_rv32_trap_c$(ObjectSuffix): ../../../../../csi_driver/aim5101_rv32/trap_c.c  
	$(CC) $(SourceSwitch) ../../../../../csi_driver/aim5101_rv32/trap_c.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/aim5101_rv32_trap_c$(ObjectSuffix) -MF$(IntermediateDirectory)/aim5101_rv32_trap_c$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/aim5101_rv32_trap_c$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/aim5101_rv32_trap_c$(PreprocessSuffix): ../../../../../csi_driver/aim5101_rv32/trap_c.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/aim5101_rv32_trap_c$(PreprocessSuffix) ../../../../../csi_driver/aim5101_rv32/trap_c.c

$(IntermediateDirectory)/aim5101_rv32_vectors$(ObjectSuffix): ../../../../../csi_driver/aim5101_rv32/vectors.S  
	$(AS) $(SourceSwitch) ../../../../../csi_driver/aim5101_rv32/vectors.S $(ASFLAGS) -MMD -MP -MT$(IntermediateDirectory)/aim5101_rv32_vectors$(ObjectSuffix) -MF$(IntermediateDirectory)/aim5101_rv32_vectors$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/aim5101_rv32_vectors$(ObjectSuffix) $(IncludeAPath) $(IncludePackagePath)
Lst/aim5101_rv32_vectors$(PreprocessSuffix): ../../../../../csi_driver/aim5101_rv32/vectors.S
	$(CC) $(CFLAGS)$(IncludeAPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/aim5101_rv32_vectors$(PreprocessSuffix) ../../../../../csi_driver/aim5101_rv32/vectors.S

$(IntermediateDirectory)/libc_clock_gettime$(ObjectSuffix): ../../../../../libs/libc/clock_gettime.c  
	$(CC) $(SourceSwitch) ../../../../../libs/libc/clock_gettime.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/libc_clock_gettime$(ObjectSuffix) -MF$(IntermediateDirectory)/libc_clock_gettime$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/libc_clock_gettime$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/libc_clock_gettime$(PreprocessSuffix): ../../../../../libs/libc/clock_gettime.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/libc_clock_gettime$(PreprocessSuffix) ../../../../../libs/libc/clock_gettime.c

$(IntermediateDirectory)/libc__init$(ObjectSuffix): ../../../../../libs/libc/_init.c  
	$(CC) $(SourceSwitch) ../../../../../libs/libc/_init.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/libc__init$(ObjectSuffix) -MF$(IntermediateDirectory)/libc__init$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/libc__init$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/libc__init$(PreprocessSuffix): ../../../../../libs/libc/_init.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/libc__init$(PreprocessSuffix) ../../../../../libs/libc/_init.c

$(IntermediateDirectory)/libc_malloc$(ObjectSuffix): ../../../../../libs/libc/malloc.c  
	$(CC) $(SourceSwitch) ../../../../../libs/libc/malloc.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/libc_malloc$(ObjectSuffix) -MF$(IntermediateDirectory)/libc_malloc$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/libc_malloc$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/libc_malloc$(PreprocessSuffix): ../../../../../libs/libc/malloc.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/libc_malloc$(PreprocessSuffix) ../../../../../libs/libc/malloc.c

$(IntermediateDirectory)/libc_minilibc_port$(ObjectSuffix): ../../../../../libs/libc/minilibc_port.c  
	$(CC) $(SourceSwitch) ../../../../../libs/libc/minilibc_port.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/libc_minilibc_port$(ObjectSuffix) -MF$(IntermediateDirectory)/libc_minilibc_port$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/libc_minilibc_port$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/libc_minilibc_port$(PreprocessSuffix): ../../../../../libs/libc/minilibc_port.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/libc_minilibc_port$(PreprocessSuffix) ../../../../../libs/libc/minilibc_port.c

$(IntermediateDirectory)/libc_printf$(ObjectSuffix): ../../../../../libs/libc/printf.c  
	$(CC) $(SourceSwitch) ../../../../../libs/libc/printf.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/libc_printf$(ObjectSuffix) -MF$(IntermediateDirectory)/libc_printf$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/libc_printf$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/libc_printf$(PreprocessSuffix): ../../../../../libs/libc/printf.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/libc_printf$(PreprocessSuffix) ../../../../../libs/libc/printf.c

$(IntermediateDirectory)/mm_dq_addlast$(ObjectSuffix): ../../../../../libs/mm/dq_addlast.c  
	$(CC) $(SourceSwitch) ../../../../../libs/mm/dq_addlast.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/mm_dq_addlast$(ObjectSuffix) -MF$(IntermediateDirectory)/mm_dq_addlast$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/mm_dq_addlast$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/mm_dq_addlast$(PreprocessSuffix): ../../../../../libs/mm/dq_addlast.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/mm_dq_addlast$(PreprocessSuffix) ../../../../../libs/mm/dq_addlast.c

$(IntermediateDirectory)/mm_dq_rem$(ObjectSuffix): ../../../../../libs/mm/dq_rem.c  
	$(CC) $(SourceSwitch) ../../../../../libs/mm/dq_rem.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/mm_dq_rem$(ObjectSuffix) -MF$(IntermediateDirectory)/mm_dq_rem$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/mm_dq_rem$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/mm_dq_rem$(PreprocessSuffix): ../../../../../libs/mm/dq_rem.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/mm_dq_rem$(PreprocessSuffix) ../../../../../libs/mm/dq_rem.c

$(IntermediateDirectory)/mm_lib_mallinfo$(ObjectSuffix): ../../../../../libs/mm/lib_mallinfo.c  
	$(CC) $(SourceSwitch) ../../../../../libs/mm/lib_mallinfo.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/mm_lib_mallinfo$(ObjectSuffix) -MF$(IntermediateDirectory)/mm_lib_mallinfo$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/mm_lib_mallinfo$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/mm_lib_mallinfo$(PreprocessSuffix): ../../../../../libs/mm/lib_mallinfo.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/mm_lib_mallinfo$(PreprocessSuffix) ../../../../../libs/mm/lib_mallinfo.c

$(IntermediateDirectory)/mm_mm_addfreechunk$(ObjectSuffix): ../../../../../libs/mm/mm_addfreechunk.c  
	$(CC) $(SourceSwitch) ../../../../../libs/mm/mm_addfreechunk.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/mm_mm_addfreechunk$(ObjectSuffix) -MF$(IntermediateDirectory)/mm_mm_addfreechunk$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/mm_mm_addfreechunk$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/mm_mm_addfreechunk$(PreprocessSuffix): ../../../../../libs/mm/mm_addfreechunk.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/mm_mm_addfreechunk$(PreprocessSuffix) ../../../../../libs/mm/mm_addfreechunk.c

$(IntermediateDirectory)/mm_mm_free$(ObjectSuffix): ../../../../../libs/mm/mm_free.c  
	$(CC) $(SourceSwitch) ../../../../../libs/mm/mm_free.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/mm_mm_free$(ObjectSuffix) -MF$(IntermediateDirectory)/mm_mm_free$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/mm_mm_free$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/mm_mm_free$(PreprocessSuffix): ../../../../../libs/mm/mm_free.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/mm_mm_free$(PreprocessSuffix) ../../../../../libs/mm/mm_free.c

$(IntermediateDirectory)/mm_mm_initialize$(ObjectSuffix): ../../../../../libs/mm/mm_initialize.c  
	$(CC) $(SourceSwitch) ../../../../../libs/mm/mm_initialize.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/mm_mm_initialize$(ObjectSuffix) -MF$(IntermediateDirectory)/mm_mm_initialize$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/mm_mm_initialize$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/mm_mm_initialize$(PreprocessSuffix): ../../../../../libs/mm/mm_initialize.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/mm_mm_initialize$(PreprocessSuffix) ../../../../../libs/mm/mm_initialize.c

$(IntermediateDirectory)/mm_mm_leak$(ObjectSuffix): ../../../../../libs/mm/mm_leak.c  
	$(CC) $(SourceSwitch) ../../../../../libs/mm/mm_leak.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/mm_mm_leak$(ObjectSuffix) -MF$(IntermediateDirectory)/mm_mm_leak$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/mm_mm_leak$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/mm_mm_leak$(PreprocessSuffix): ../../../../../libs/mm/mm_leak.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/mm_mm_leak$(PreprocessSuffix) ../../../../../libs/mm/mm_leak.c

$(IntermediateDirectory)/mm_mm_mallinfo$(ObjectSuffix): ../../../../../libs/mm/mm_mallinfo.c  
	$(CC) $(SourceSwitch) ../../../../../libs/mm/mm_mallinfo.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/mm_mm_mallinfo$(ObjectSuffix) -MF$(IntermediateDirectory)/mm_mm_mallinfo$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/mm_mm_mallinfo$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/mm_mm_mallinfo$(PreprocessSuffix): ../../../../../libs/mm/mm_mallinfo.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/mm_mm_mallinfo$(PreprocessSuffix) ../../../../../libs/mm/mm_mallinfo.c

$(IntermediateDirectory)/mm_mm_malloc$(ObjectSuffix): ../../../../../libs/mm/mm_malloc.c  
	$(CC) $(SourceSwitch) ../../../../../libs/mm/mm_malloc.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/mm_mm_malloc$(ObjectSuffix) -MF$(IntermediateDirectory)/mm_mm_malloc$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/mm_mm_malloc$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/mm_mm_malloc$(PreprocessSuffix): ../../../../../libs/mm/mm_malloc.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/mm_mm_malloc$(PreprocessSuffix) ../../../../../libs/mm/mm_malloc.c

$(IntermediateDirectory)/mm_mm_size2ndx$(ObjectSuffix): ../../../../../libs/mm/mm_size2ndx.c  
	$(CC) $(SourceSwitch) ../../../../../libs/mm/mm_size2ndx.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/mm_mm_size2ndx$(ObjectSuffix) -MF$(IntermediateDirectory)/mm_mm_size2ndx$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/mm_mm_size2ndx$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/mm_mm_size2ndx$(PreprocessSuffix): ../../../../../libs/mm/mm_size2ndx.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/mm_mm_size2ndx$(PreprocessSuffix) ../../../../../libs/mm/mm_size2ndx.c

$(IntermediateDirectory)/ringbuffer_ringbuffer$(ObjectSuffix): ../../../../../libs/ringbuffer/ringbuffer.c  
	$(CC) $(SourceSwitch) ../../../../../libs/ringbuffer/ringbuffer.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/ringbuffer_ringbuffer$(ObjectSuffix) -MF$(IntermediateDirectory)/ringbuffer_ringbuffer$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/ringbuffer_ringbuffer$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/ringbuffer_ringbuffer$(PreprocessSuffix): ../../../../../libs/ringbuffer/ringbuffer.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/ringbuffer_ringbuffer$(PreprocessSuffix) ../../../../../libs/ringbuffer/ringbuffer.c

$(IntermediateDirectory)/syslog_syslog$(ObjectSuffix): ../../../../../libs/syslog/syslog.c  
	$(CC) $(SourceSwitch) ../../../../../libs/syslog/syslog.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/syslog_syslog$(ObjectSuffix) -MF$(IntermediateDirectory)/syslog_syslog$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/syslog_syslog$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/syslog_syslog$(PreprocessSuffix): ../../../../../libs/syslog/syslog.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/syslog_syslog$(PreprocessSuffix) ../../../../../libs/syslog/syslog.c

$(IntermediateDirectory)/gpio_example_gpio$(ObjectSuffix): ../example_gpio.c  
	$(CC) $(SourceSwitch) ../example_gpio.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/gpio_example_gpio$(ObjectSuffix) -MF$(IntermediateDirectory)/gpio_example_gpio$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/gpio_example_gpio$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/gpio_example_gpio$(PreprocessSuffix): ../example_gpio.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/gpio_example_gpio$(PreprocessSuffix) ../example_gpio.c

$(IntermediateDirectory)/gpio_aim5101_codec_gpio$(ObjectSuffix): ../aim5101_codec_gpio.c  
	$(CC) $(SourceSwitch) ../aim5101_codec_gpio.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/gpio_aim5101_codec_gpio$(ObjectSuffix) -MF$(IntermediateDirectory)/gpio_aim5101_codec_gpio$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/gpio_aim5101_codec_gpio$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/gpio_aim5101_codec_gpio$(PreprocessSuffix): ../aim5101_codec_gpio.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/gpio_aim5101_codec_gpio$(PreprocessSuffix) ../aim5101_codec_gpio.c


-include $(IntermediateDirectory)/*$(DependSuffix)
