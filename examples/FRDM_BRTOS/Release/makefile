################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include system/src/newlib/subdir.mk
-include system/src/kl25-sc/subdir.mk
-include system/src/diag/subdir.mk
-include system/src/cortexm/subdir.mk
-include system/src/cmsis/subdir.mk
-include src/hal/subdir.mk
-include src/brtos/subdir.mk
-include src/Drivers/SPI/subdir.mk
-include src/Drivers/MMA8451/subdir.mk
-include src/Drivers/LPO/subdir.mk
-include src/Drivers/DAC/subdir.mk
-include src/Drivers/CPU/subdir.mk
-include src/Drivers/subdir.mk
-include src/CoX/CoX_Peripheral/src/subdir.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
FRDM_BRTOS.hex \

SECONDARY_SIZE += \
FRDM_BRTOS.siz \


# All Target
all: FRDM_BRTOS.elf secondary-outputs

# Tool invocations
FRDM_BRTOS.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross ARM C++ Linker'
	arm-none-eabi-g++ -mcpu=cortex-m0plus -mthumb -O1 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall -Wextra  -g -T mem.ld -T libs.ld -T sections.ld -nostartfiles -Xlinker --gc-sections -L"../ldscripts" -Wl,-Map,"FRDM_BRTOS.map" --specs=nano.specs -o "FRDM_BRTOS.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

FRDM_BRTOS.hex: FRDM_BRTOS.elf
	@echo 'Invoking: Cross ARM GNU Create Flash Image'
	arm-none-eabi-objcopy -O ihex "FRDM_BRTOS.elf"  "FRDM_BRTOS.hex"
	@echo 'Finished building: $@'
	@echo ' '

FRDM_BRTOS.siz: FRDM_BRTOS.elf
	@echo 'Invoking: Cross ARM GNU Print Size'
	arm-none-eabi-size --format=berkeley "FRDM_BRTOS.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(OBJS)$(C_UPPER_DEPS)$(CXX_DEPS)$(SECONDARY_FLASH)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_UPPER_DEPS)$(C_DEPS)$(CPP_DEPS) FRDM_BRTOS.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
