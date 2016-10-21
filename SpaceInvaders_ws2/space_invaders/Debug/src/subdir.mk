################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/control.c \
../src/globals.c \
../src/platform.c \
../src/render.c \
../src/sound_control.c \
../src/sountTest.c \
../src/tankFireSound.c \
../src/vdmaTest.c \
../src/xac97_l.c 

LD_SRCS += \
../src/lscript.ld 

OBJS += \
./src/control.o \
./src/globals.o \
./src/platform.o \
./src/render.o \
./src/sound_control.o \
./src/sountTest.o \
./src/tankFireSound.o \
./src/vdmaTest.o \
./src/xac97_l.o 

C_DEPS += \
./src/control.d \
./src/globals.d \
./src/platform.d \
./src/render.d \
./src/sound_control.d \
./src/sountTest.d \
./src/tankFireSound.d \
./src/vdmaTest.d \
./src/xac97_l.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo Building file: $<
	@echo Invoking: MicroBlaze gcc compiler
	mb-gcc -Wall -O0 -g3 -c -fmessage-length=0 -I../../space_invaders_bsp/microblaze_0/include -mlittle-endian -mxl-barrel-shift -mxl-pattern-compare -mcpu=v8.20.b -mno-xl-soft-mul -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo Finished building: $<
	@echo ' '


