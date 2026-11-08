################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/Src/joystick.c \
../App/Src/snake.c 

OBJS += \
./App/Src/joystick.o \
./App/Src/snake.o 

C_DEPS += \
./App/Src/joystick.d \
./App/Src/snake.d 


# Each subdirectory must supply rules for building sources it contributes
App/Src/%.o App/Src/%.su App/Src/%.cyclo: ../App/Src/%.c App/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"/home/mats/Projects/STM32Cube/snakeGame/snakeGame/Lib/stm32-ssd1306-master/ssd1306" -I"/home/mats/Projects/STM32Cube/snakeGame/snakeGame/App/Inc" -I"/home/mats/Projects/STM32Cube/snakeGame/snakeGame/App/Src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-App-2f-Src

clean-App-2f-Src:
	-$(RM) ./App/Src/joystick.cyclo ./App/Src/joystick.d ./App/Src/joystick.o ./App/Src/joystick.su ./App/Src/snake.cyclo ./App/Src/snake.d ./App/Src/snake.o ./App/Src/snake.su

.PHONY: clean-App-2f-Src

