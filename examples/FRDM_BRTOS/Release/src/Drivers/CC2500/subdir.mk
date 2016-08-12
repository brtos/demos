################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Drivers/CC2500/CC2500.c 

OBJS += \
./src/Drivers/CC2500/CC2500.o 

C_DEPS += \
./src/Drivers/CC2500/CC2500.d 


# Each subdirectory must supply rules for building sources it contributes
src/Drivers/CC2500/%.o: ../src/Drivers/CC2500/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall -Wextra  -g -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DMKL25Z4 -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/kl25-sc" -I"C:\Users\gusta\ecp_workspace\FRDM_BRTOS\src\CONFIG" -I"C:\Users\gusta\ecp_workspace\FRDM_BRTOS\src\brtos\includes" -I"C:\Users\gusta\ecp_workspace\FRDM_BRTOS\src\hal" -I"C:\Users\gusta\ecp_workspace\FRDM_BRTOS\src\Drivers" -I"C:\Users\gusta\ecp_workspace\FRDM_BRTOS\src\CoX\CoX_Peripheral\inc" -I"C:\Users\gusta\ecp_workspace\FRDM_BRTOS\src\Drivers\CPU" -I"C:\Users\gusta\ecp_workspace\FRDM_BRTOS\src\Drivers\MMA8451" -I"C:\Users\gusta\ecp_workspace\FRDM_BRTOS\src\Drivers\SPI" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


