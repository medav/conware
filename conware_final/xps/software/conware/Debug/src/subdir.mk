################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/backup.c \
../src/dma_util.c \
../src/helloworld.c \
../src/platform.c 

LD_SRCS += \
../src/lscript.ld 

OBJS += \
./src/backup.o \
./src/dma_util.o \
./src/helloworld.o \
./src/platform.o 

C_DEPS += \
./src/backup.d \
./src/dma_util.d \
./src/helloworld.d \
./src/platform.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM gcc compiler'
	arm-xilinx-eabi-gcc -Wall -O0 -g3 -c -fmessage-length=0 -I../../video_bsp/ps7_cortexa9_0/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


