################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Code/Joystick.c \
../Code/Pipes.c \
../Code/RS232.c \
../Code/main.c 

OBJS += \
./Code/Joystick.o \
./Code/Pipes.o \
./Code/RS232.o \
./Code/main.o 

C_DEPS += \
./Code/Joystick.d \
./Code/Pipes.d \
./Code/RS232.d \
./Code/main.d 


# Each subdirectory must supply rules for building sources it contributes
Code/%.o: ../Code/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


