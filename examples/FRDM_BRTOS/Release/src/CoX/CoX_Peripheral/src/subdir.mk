################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/CoX/CoX_Peripheral/src/xadc.c \
../src/CoX/CoX_Peripheral/src/xcore.c \
../src/CoX/CoX_Peripheral/src/xdebug.c \
../src/CoX/CoX_Peripheral/src/xgpio.c \
../src/CoX/CoX_Peripheral/src/xsysctl.c \
../src/CoX/CoX_Peripheral/src/xuart.c \
../src/CoX/CoX_Peripheral/src/xwdt.c 

OBJS += \
./src/CoX/CoX_Peripheral/src/xadc.o \
./src/CoX/CoX_Peripheral/src/xcore.o \
./src/CoX/CoX_Peripheral/src/xdebug.o \
./src/CoX/CoX_Peripheral/src/xgpio.o \
./src/CoX/CoX_Peripheral/src/xsysctl.o \
./src/CoX/CoX_Peripheral/src/xuart.o \
./src/CoX/CoX_Peripheral/src/xwdt.o 

C_DEPS += \
./src/CoX/CoX_Peripheral/src/xadc.d \
./src/CoX/CoX_Peripheral/src/xcore.d \
./src/CoX/CoX_Peripheral/src/xdebug.d \
./src/CoX/CoX_Peripheral/src/xgpio.d \
./src/CoX/CoX_Peripheral/src/xsysctl.d \
./src/CoX/CoX_Peripheral/src/xuart.d \
./src/CoX/CoX_Peripheral/src/xwdt.d 


# Each subdirectory must supply rules for building sources it contributes
src/CoX/CoX_Peripheral/src/%.o: ../src/CoX/CoX_Peripheral/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -O1 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall -Wextra  -g -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DMKL25Z4 -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/kl25-sc" -I"C:\Users\gusta\ecp_workspace\FRDM_BRTOS\src\CONFIG" -I"C:\Users\gusta\ecp_workspace\FRDM_BRTOS\src\brtos\includes" -I"C:\Users\gusta\ecp_workspace\FRDM_BRTOS\src\hal" -I"C:\Users\gusta\ecp_workspace\FRDM_BRTOS\src\Drivers" -I"C:\Users\gusta\ecp_workspace\FRDM_BRTOS\src\CoX\CoX_Peripheral\inc" -I"C:\Users\gusta\ecp_workspace\FRDM_BRTOS\src\Drivers\CPU" -I"C:\Users\gusta\ecp_workspace\FRDM_BRTOS\src\Drivers\MMA8451" -I"C:\Users\gusta\ecp_workspace\FRDM_BRTOS\src\Drivers\SPI" -I"C:\Users\gusta\ecp_workspace\FRDM_BRTOS\src\Drivers\DAC" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


