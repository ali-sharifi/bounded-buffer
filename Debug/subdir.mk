################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../bbuff.c \
../candykids.c \
../stats.c 

O_SRCS += \
../bbuff.o \
../candykids.o \
../stats.o 

OBJS += \
./bbuff.o \
./candykids.o \
./stats.o 

C_DEPS += \
./bbuff.d \
./candykids.d \
./stats.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


