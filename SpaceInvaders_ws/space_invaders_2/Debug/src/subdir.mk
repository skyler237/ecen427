################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/globals.c \
../src/platform.c \
../src/render.c \
../src/vdmaTest.c 

LD_SRCS += \
../src/lscript.ld 

OBJS += \
./src/globals.o \
./src/platform.o \
./src/render.o \
./src/vdmaTest.o 

C_DEPS += \
./src/globals.d \
./src/platform.d \
./src/render.d \
./src/vdmaTest.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo Building file: $<
	@echo Invoking: MicroBlaze gcc compiler
	mb-gcc -Wall -O0 -g3 -c -fmessage-length=0 -I../../space_invaders_bsp_2/microblaze_0/include -mlittle-endian -mxl-barrel-shift -mxl-pattern-compare -mcpu=v8.20.b -mno-xl-soft-mul -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo Finished building: $<
	@echo ' '


