################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Buzzer.c \
../MC2main.c \
../dc_motor.c \
../external_eeprom.c \
../g_interrupt.c \
../gpio.c \
../lcd.c \
../timer0.c \
../timer1.c \
../twi.c \
../uart.c 

OBJS += \
./Buzzer.o \
./MC2main.o \
./dc_motor.o \
./external_eeprom.o \
./g_interrupt.o \
./gpio.o \
./lcd.o \
./timer0.o \
./timer1.o \
./twi.o \
./uart.o 

C_DEPS += \
./Buzzer.d \
./MC2main.d \
./dc_motor.d \
./external_eeprom.d \
./g_interrupt.d \
./gpio.d \
./lcd.d \
./timer0.d \
./timer1.d \
./twi.d \
./uart.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


