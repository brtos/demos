################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Drivers/MMA8451/accel.c 

OBJS += \
./src/Drivers/MMA8451/accel.o 

C_DEPS += \
./src/Drivers/MMA8451/accel.d 


# Each subdirectory must supply rules for building sources it contributes
src/Drivers/MMA8451/%.o: ../src/Drivers/MMA8451/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DMKL25Z4 -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/kl25-sc" -I"C:\Users\gusta\Documents\Git\demos\examples\FRDM_BRTOS\src\CONFIG" -I"C:\Users\gusta\Documents\Git\demos\examples\FRDM_BRTOS\src\brtos\includes" -I"C:\Users\gusta\Documents\Git\demos\examples\FRDM_BRTOS\src\hal" -I"C:\Users\gusta\Documents\Git\demos\examples\FRDM_BRTOS\src\Drivers" -I"C:\Users\gusta\Documents\Git\demos\examples\FRDM_BRTOS\src\CoX\CoX_Peripheral\inc" -I"C:\Users\gusta\Documents\Git\demos\examples\FRDM_BRTOS\src\Drivers\CPU" -I"C:\Users\gusta\Documents\Git\demos\examples\FRDM_BRTOS\src\Drivers\MMA8451" -I"C:\Users\gusta\Documents\Git\demos\examples\FRDM_BRTOS\src\Drivers\SPI" -I"C:\Users\gusta\Documents\Git\demos\examples\FRDM_BRTOS\src\Drivers\DAC" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


