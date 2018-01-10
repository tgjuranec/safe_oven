################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/LCD1602.c \
../src/adc.c \
../src/buzzer.c \
../src/cr_startup_lpc11xx.c \
../src/crp.c \
../src/data_process.c \
../src/io.c \
../src/safe_oven.c \
../src/sch.c \
../src/sysinit.c 

OBJS += \
./src/LCD1602.o \
./src/adc.o \
./src/buzzer.o \
./src/cr_startup_lpc11xx.o \
./src/crp.o \
./src/data_process.o \
./src/io.o \
./src/safe_oven.o \
./src/sch.o \
./src/sysinit.o 

C_DEPS += \
./src/LCD1602.d \
./src/adc.d \
./src/buzzer.d \
./src/cr_startup_lpc11xx.d \
./src/crp.d \
./src/data_process.d \
./src/io.d \
./src/safe_oven.d \
./src/sch.d \
./src/sysinit.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M0 -D__USE_LPCOPEN -DNO_BOARD_LIB -D__USE_CMSIS_DSPLIB=CMSIS_DSPLIB_CM0 -D__LPC11XX__ -D__REDLIB__ -I"/home/tgjuranec/lpc11xx/safe_oven/inc" -I"/home/tgjuranec/lpc11xx/lpc_chip_11cxx_lib/inc" -I"/home/tgjuranec/lpc11xx/CMSIS_DSPLIB_CM0/inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0 -mthumb -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


