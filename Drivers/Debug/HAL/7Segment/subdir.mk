################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HAL/7Segment/se7en_segment.c 

OBJS += \
./HAL/7Segment/se7en_segment.o 

C_DEPS += \
./HAL/7Segment/se7en_segment.d 


# Each subdirectory must supply rules for building sources it contributes
HAL/7Segment/%.o HAL/7Segment/%.su HAL/7Segment/%.cyclo: ../HAL/7Segment/%.c HAL/7Segment/subdir.mk
	arm-none-eabi-gcc -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -c -I../Inc -I"D:/embeded Systems/STMdrivers/Drivers/HAL/inc" -I"D:/embeded Systems/STMdrivers/Drivers/Stm32_F103C6_Drivers/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-HAL-2f-7Segment

clean-HAL-2f-7Segment:
	-$(RM) ./HAL/7Segment/se7en_segment.cyclo ./HAL/7Segment/se7en_segment.d ./HAL/7Segment/se7en_segment.o ./HAL/7Segment/se7en_segment.su

.PHONY: clean-HAL-2f-7Segment

