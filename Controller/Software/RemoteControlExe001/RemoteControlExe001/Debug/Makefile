################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=
LINKER_SCRIPT_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS :=  \
../inc \
../scr


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../scr/main.cpp \
../scr/stepper.cpp


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
scr/main.o \
scr/stepper.o

OBJS_AS_ARGS +=  \
scr/main.o \
scr/stepper.o

C_DEPS +=  \
scr/main.d \
scr/stepper.d

C_DEPS_AS_ARGS +=  \
scr/main.d \
scr/stepper.d

OUTPUT_FILE_PATH +=RemoteControlExe001.elf

OUTPUT_FILE_PATH_AS_ARGS +=RemoteControlExe001.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

LINKER_SCRIPT_DEP+= 


# AVR32/GNU C Compiler
scr/main.o: ../scr/main.cpp
	@echo Building file: $<
	@echo Invoking: AVR32/GNU C++ Compiler : 4.4.7
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr32\avr32-gnu-toolchain\bin\avr32-g++.exe$(QUOTE) -DDEBUG  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\UC3D_DFP\1.0.54\include\ATUC128D3" -I"../inc"  -O1 -ffunction-sections -masm-addr-pseudos -g3 -Wall -mpart=uc128d3 -c -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

scr/stepper.o: ../scr/stepper.cpp
	@echo Building file: $<
	@echo Invoking: AVR32/GNU C++ Compiler : 4.4.7
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr32\avr32-gnu-toolchain\bin\avr32-g++.exe$(QUOTE) -DDEBUG  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\UC3D_DFP\1.0.54\include\ATUC128D3" -I"../inc"  -O1 -ffunction-sections -masm-addr-pseudos -g3 -Wall -mpart=uc128d3 -c -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	




# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP) $(LINKER_SCRIPT_DEP)
	@echo Building target: $@
	@echo Invoking: AVR32/GNU Linker : 4.4.7
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr32\avr32-gnu-toolchain\bin\avr32-g++.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="RemoteControlExe001.map" -Wl,--start-group -lm  -Wl,--end-group -Wl,--gc-sections --rodata-writable -Wl,--direct-data -mpart=uc128d3  
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr32\avr32-gnu-toolchain\bin\avr32-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature "RemoteControlExe001.elf" "RemoteControlExe001.hex"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr32\avr32-gnu-toolchain\bin\avr32-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "RemoteControlExe001.elf" "RemoteControlExe001.eep" || exit 0
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr32\avr32-gnu-toolchain\bin\avr32-objdump.exe" -h -S "RemoteControlExe001.elf" > "RemoteControlExe001.lss"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr32\avr32-gnu-toolchain\bin\avr32-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures "RemoteControlExe001.elf" "RemoteControlExe001.srec"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr32\avr32-gnu-toolchain\bin\avr32-size.exe" "RemoteControlExe001.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "RemoteControlExe001.elf" "RemoteControlExe001.a" "RemoteControlExe001.hex" "RemoteControlExe001.lss" "RemoteControlExe001.eep" "RemoteControlExe001.map" "RemoteControlExe001.srec" "RemoteControlExe001.usersignatures"
	