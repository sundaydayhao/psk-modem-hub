##
## Auto Generated makefile by CDK
## Do not modify this file, and any manual changes will be erased!!!   
##
## BuildSet
ProjectName            :=AiM5101HXX_FlashAlgo
ConfigurationName      :=BuildSet
WorkspacePath          :=../../../Users/zhang/Documents/aim5101/
ProjectPath            :=./
IntermediateDirectory  :=Obj
OutDir                 :=$(IntermediateDirectory)
User                   :=zhang
Date                   :=25/07/2026
CDKPath                :=../../../C-Sky/CDK
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
ObjectsFileList        :=$(IntermediateDirectory)/AiM5101HXX_FlashAlgo.txt
MakeDirCommand         :=mkdir
LinkOptions            := -mcpu=e906  -nostartfiles -Wl,--gc-sections -T"$(ProjectPath)/ckcpu.ld" -pipe 
LinkOtherFlagsOption   :=
IncludePackagePath     :=
IncludeCPath           := $(IncludeSwitch). $(IncludeSwitch)../../../C-Sky/CDK/CSKY/Flash  
IncludeAPath           := $(IncludeSwitch).  
Libs                   :=
ArLibs                 := 
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
CXXFLAGS :=-mcpu=e906 -Og  -g  -Wall -fno-isolate-erroneous-paths-dereference -pipe 
CFLAGS   :=-mcpu=e906 -Og  -g  -Wall -fno-isolate-erroneous-paths-dereference -pipe 
ASFLAGS  :=-mcpu=e906  -Wa,-gdwarf-2 -pipe 
PreprocessFlags  :=-mcpu=e906 -Og  -g  -Wall -fno-isolate-erroneous-paths-dereference -pipe 


Objects0=$(IntermediateDirectory)/crt0$(ObjectSuffix) $(IntermediateDirectory)/chip_name$(ObjectSuffix) $(IntermediateDirectory)/main$(ObjectSuffix) $(IntermediateDirectory)/driver$(ObjectSuffix) $(IntermediateDirectory)/FlashDev$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all
all: $(IntermediateDirectory)/$(OutputFile)

$(IntermediateDirectory)/$(OutputFile):  $(Objects) Always_Link 
	$(LinkerName) $(OutputSwitch) $(IntermediateDirectory)/$(OutputFile)$(ExeSuffix) $(LinkerNameoption) @$(ObjectsFileList)  $(LinkOptions) $(LibPath) $(Libs) $(LinkOtherFlagsOption)
	$(OBJDUMP) $(ObjdumpSwitch) $(ProjectPath)/$(IntermediateDirectory)/$(OutputFile)$(ExeSuffix)  > $(ProjectPath)/Lst/$(OutputFile)$(DisassemSuffix) 
	@echo size of target:
	@$(SIZE) $(ProjectPath)$(IntermediateDirectory)/$(OutputFile)$(ExeSuffix) 
	@echo -n checksum value of target:  
	@$(CHECKSUM) $(ProjectPath)/$(IntermediateDirectory)/$(OutputFile)$(ExeSuffix) 
	@AiM5101HXX_FlashAlgo.modify.bat $(IntermediateDirectory) $(OutputFile)$(ExeSuffix) 

Always_Link:


##
## Objects
##
$(IntermediateDirectory)/crt0$(ObjectSuffix): crt0.S  
	$(AS) $(SourceSwitch) crt0.S $(ASFLAGS) -MMD -MP -MT$(IntermediateDirectory)/crt0$(ObjectSuffix) -MF$(IntermediateDirectory)/crt0$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/crt0$(ObjectSuffix) $(IncludeAPath) $(IncludePackagePath)
Lst/crt0$(PreprocessSuffix): crt0.S
	$(CC) $(CFLAGS)$(IncludeAPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/crt0$(PreprocessSuffix) crt0.S

$(IntermediateDirectory)/chip_name$(ObjectSuffix): chip_name.c  
	$(CC) $(SourceSwitch) chip_name.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/chip_name$(ObjectSuffix) -MF$(IntermediateDirectory)/chip_name$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/chip_name$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/chip_name$(PreprocessSuffix): chip_name.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/chip_name$(PreprocessSuffix) chip_name.c

$(IntermediateDirectory)/main$(ObjectSuffix): main.c  
	$(CC) $(SourceSwitch) main.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/main$(ObjectSuffix) -MF$(IntermediateDirectory)/main$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/main$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/main$(PreprocessSuffix): main.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/main$(PreprocessSuffix) main.c

$(IntermediateDirectory)/driver$(ObjectSuffix): driver.c  
	$(CC) $(SourceSwitch) driver.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/driver$(ObjectSuffix) -MF$(IntermediateDirectory)/driver$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/driver$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/driver$(PreprocessSuffix): driver.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/driver$(PreprocessSuffix) driver.c

$(IntermediateDirectory)/FlashDev$(ObjectSuffix): FlashDev.c  
	$(CC) $(SourceSwitch) FlashDev.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/FlashDev$(ObjectSuffix) -MF$(IntermediateDirectory)/FlashDev$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/FlashDev$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/FlashDev$(PreprocessSuffix): FlashDev.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/FlashDev$(PreprocessSuffix) FlashDev.c


-include $(IntermediateDirectory)/*$(DependSuffix)
