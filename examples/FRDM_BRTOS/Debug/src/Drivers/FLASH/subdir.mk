################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Drivers/FLASH/FLASH.c \
../src/Drivers/FLASH/FLASH1.c 

OBJS += \
./src/Drivers/FLASH/FLASH.o \
./src/Drivers/FLASH/FLASH1.o 

C_DEPS += \
./src/Drivers/FLASH/FLASH.d \
./src/Drivers/FLASH/FLASH1.d 


# Each subdirectory must supply rules for building sources it contributes
src/Drivers/FLASH/%.o: ../src/Drivers/FLASH/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DMKL25Z4 -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/kl25-sc" -I"C:\Users\gusta\ecp_workspace\FRDM_BRTOS\src\CONFIG" -I"C:\Users\gusta\ecp_workspace\FRDM_BRTOS\src\brtos\includes" -I"C:\Users\gusta\ecp_workspace\FRDM_BRTOS\src\hal" -I"C:\Users\gusta\ecp_workspace\FRDM_BRTOS\src\Drivers" -I"C:\Users\gusta\ecp_workspace\FRDM_BRTOS\src\CoX\CoX_Peripheral\inc" -I"C:\Users\gusta\ecp_workspace\FRDM_BRTOS\src\Drivers\CPU" -I"C:\Users\gusta\ecp_workspace\FRDM_BRTOS\src\Drivers\MMA8451" -I"C:\Users\gusta\ecp_workspace\FRDM_BRTOS\src\Drivers\SPI" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


