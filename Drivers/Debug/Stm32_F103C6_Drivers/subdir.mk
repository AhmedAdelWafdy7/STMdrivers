################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Stm32_F103C6_Drivers/Stm32_F103C6_GPIO.c 

OBJS += \
./Stm32_F103C6_Drivers/Stm32_F103C6_GPIO.o 

C_DEPS += \
./Stm32_F103C6_Drivers/Stm32_F103C6_GPIO.d 


# Each subdirectory must supply rules for building sources it contributes
Stm32_F103C6_Drivers/%.o Stm32_F103C6_Drivers/%.su Stm32_F103C6_Drivers/%.cyclo: ../Stm32_F103C6_Drivers/%.c Stm32_F103C6_Drivers/subdir.mk
	arm-none-eabi-gcc -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -c -I../Inc -I"C:/Users/user/STM32CubeIDE/workspace_1.12.0/Drivers/Stm32_F103C6_Drivers/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Stm32_F103C6_Drivers

clean-Stm32_F103C6_Drivers:
	-$(RM) ./Stm32_F103C6_Drivers/Stm32_F103C6_GPIO.cyclo ./Stm32_F103C6_Drivers/Stm32_F103C6_GPIO.d ./Stm32_F103C6_Drivers/Stm32_F103C6_GPIO.o ./Stm32_F103C6_Drivers/Stm32_F103C6_GPIO.su

.PHONY: clean-Stm32_F103C6_Drivers

