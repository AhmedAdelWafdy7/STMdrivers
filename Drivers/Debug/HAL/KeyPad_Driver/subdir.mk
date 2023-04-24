################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HAL/KeyPad_Driver/keypad.c 

OBJS += \
./HAL/KeyPad_Driver/keypad.o 

C_DEPS += \
./HAL/KeyPad_Driver/keypad.d 


# Each subdirectory must supply rules for building sources it contributes
HAL/KeyPad_Driver/%.o HAL/KeyPad_Driver/%.su HAL/KeyPad_Driver/%.cyclo: ../HAL/KeyPad_Driver/%.c HAL/KeyPad_Driver/subdir.mk
	arm-none-eabi-gcc -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -c -I../Inc -I"D:/embeded Systems/STMdrivers/Drivers/HAL/inc" -I"D:/embeded Systems/STMdrivers/Drivers/Stm32_F103C6_Drivers/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-HAL-2f-KeyPad_Driver

clean-HAL-2f-KeyPad_Driver:
	-$(RM) ./HAL/KeyPad_Driver/keypad.cyclo ./HAL/KeyPad_Driver/keypad.d ./HAL/KeyPad_Driver/keypad.o ./HAL/KeyPad_Driver/keypad.su

.PHONY: clean-HAL-2f-KeyPad_Driver

