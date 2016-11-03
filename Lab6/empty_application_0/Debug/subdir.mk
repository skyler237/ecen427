################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../alien1.c \
../alien2.c \
../alien3.c \
../alien4.c \
../alienExplosion.c \
../alien_explosion.c \
../control.c \
../globals.c \
../pit_timer.c \
../platform.c \
../real_time_clock.c \
../render.c \
../saucerExplosion.c \
../saucerFlying.c \
../sound_control.c \
../sountTest.c \
../tankExplosion.c \
../tankFire.c \
../tankFireSound.c \
../vdmaTest.c \
../xac97_l.c 

LD_SRCS += \
../lscript.ld 

OBJS += \
./alien1.o \
./alien2.o \
./alien3.o \
./alien4.o \
./alienExplosion.o \
./alien_explosion.o \
./control.o \
./globals.o \
./pit_timer.o \
./platform.o \
./real_time_clock.o \
./render.o \
./saucerExplosion.o \
./saucerFlying.o \
./sound_control.o \
./sountTest.o \
./tankExplosion.o \
./tankFire.o \
./tankFireSound.o \
./vdmaTest.o \
./xac97_l.o 

C_DEPS += \
./alien1.d \
./alien2.d \
./alien3.d \
./alien4.d \
./alienExplosion.d \
./alien_explosion.d \
./control.d \
./globals.d \
./pit_timer.d \
./platform.d \
./real_time_clock.d \
./render.d \
./saucerExplosion.d \
./saucerFlying.d \
./sound_control.d \
./sountTest.d \
./tankExplosion.d \
./tankFire.d \
./tankFireSound.d \
./vdmaTest.d \
./xac97_l.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo Building file: $<
	@echo Invoking: MicroBlaze gcc compiler
	mb-gcc -Wall -O3 -g3 -c -fmessage-length=0 -I../../lab6_bsp/microblaze_0/include -mlittle-endian -mxl-barrel-shift -mxl-pattern-compare -mcpu=v8.20.b -mno-xl-soft-mul -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo Finished building: $<
	@echo ' '


