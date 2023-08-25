################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/W9825G6KH-6/w9825g6kh.c 

OBJS += \
./Drivers/W9825G6KH-6/w9825g6kh.o 

C_DEPS += \
./Drivers/W9825G6KH-6/w9825g6kh.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/W9825G6KH-6/%.o Drivers/W9825G6KH-6/%.su Drivers/W9825G6KH-6/%.cyclo: ../Drivers/W9825G6KH-6/%.c Drivers/W9825G6KH-6/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H750xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../Drivers/W25Q64JV -I../Drivers/W9825G6KH-6 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-W9825G6KH-2d-6

clean-Drivers-2f-W9825G6KH-2d-6:
	-$(RM) ./Drivers/W9825G6KH-6/w9825g6kh.cyclo ./Drivers/W9825G6KH-6/w9825g6kh.d ./Drivers/W9825G6KH-6/w9825g6kh.o ./Drivers/W9825G6KH-6/w9825g6kh.su

.PHONY: clean-Drivers-2f-W9825G6KH-2d-6

