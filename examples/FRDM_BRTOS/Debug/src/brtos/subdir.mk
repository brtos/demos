################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/brtos/BRTOS.c \
../src/brtos/OSInfo.c \
../src/brtos/OSTime.c \
../src/brtos/device.c \
../src/brtos/mbox.c \
../src/brtos/mutex.c \
../src/brtos/queue.c \
../src/brtos/semaphore.c \
../src/brtos/stimer.c 

OBJS += \
./src/brtos/BRTOS.o \
./src/brtos/OSInfo.o \
./src/brtos/OSTime.o \
./src/brtos/device.o \
./src/brtos/mbox.o \
./src/brtos/mutex.o \
./src/brtos/queue.o \
./src/brtos/semaphore.o \
./src/brtos/stimer.o 

C_DEPS += \
./src/brtos/BRTOS.d \
./src/brtos/OSInfo.d \
./src/brtos/OSTime.d \
./src/brtos/device.d \
./src/brtos/mbox.d \
./src/brtos/mutex.d \
./src/brtos/queue.d \
./src/brtos/semaphore.d \
./src/brtos/stimer.d 


# Each subdirectory must supply rules for building sources it contributes
src/brtos/%.o: ../src/brtos/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DMKL25Z4 -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/kl25-sc" -I"C:\Users\gusta\Documents\Git\demos\examples\FRDM_BRTOS\src\CONFIG" -I"C:\Users\gusta\Documents\Git\demos\examples\FRDM_BRTOS\src\brtos\includes" -I"C:\Users\gusta\Documents\Git\demos\examples\FRDM_BRTOS\src\hal" -I"C:\Users\gusta\Documents\Git\demos\examples\FRDM_BRTOS\src\Drivers" -I"C:\Users\gusta\Documents\Git\demos\examples\FRDM_BRTOS\src\CoX\CoX_Peripheral\inc" -I"C:\Users\gusta\Documents\Git\demos\examples\FRDM_BRTOS\src\Drivers\CPU" -I"C:\Users\gusta\Documents\Git\demos\examples\FRDM_BRTOS\src\Drivers\MMA8451" -I"C:\Users\gusta\Documents\Git\demos\examples\FRDM_BRTOS\src\Drivers\SPI" -I"C:\Users\gusta\Documents\Git\demos\examples\FRDM_BRTOS\src\Drivers\DAC" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


