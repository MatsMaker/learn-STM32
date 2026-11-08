################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Lib/stm32-ssd1306-master/ssd1306/ssd1306.c \
../Lib/stm32-ssd1306-master/ssd1306/ssd1306_fonts.c \
../Lib/stm32-ssd1306-master/ssd1306/ssd1306_tests.c 

OBJS += \
./Lib/stm32-ssd1306-master/ssd1306/ssd1306.o \
./Lib/stm32-ssd1306-master/ssd1306/ssd1306_fonts.o \
./Lib/stm32-ssd1306-master/ssd1306/ssd1306_tests.o 

C_DEPS += \
./Lib/stm32-ssd1306-master/ssd1306/ssd1306.d \
./Lib/stm32-ssd1306-master/ssd1306/ssd1306_fonts.d \
./Lib/stm32-ssd1306-master/ssd1306/ssd1306_tests.d 


# Each subdirectory must supply rules for building sources it contributes
Lib/stm32-ssd1306-master/ssd1306/%.o Lib/stm32-ssd1306-master/ssd1306/%.su Lib/stm32-ssd1306-master/ssd1306/%.cyclo: ../Lib/stm32-ssd1306-master/ssd1306/%.c Lib/stm32-ssd1306-master/ssd1306/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"/home/mats/Projects/STM32Cube/snakeGame/snakeGame/Lib/stm32-ssd1306-master/ssd1306" -I"/home/mats/Projects/STM32Cube/snakeGame/snakeGame/App/Inc" -I"/home/mats/Projects/STM32Cube/snakeGame/snakeGame/App/Src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Lib-2f-stm32-2d-ssd1306-2d-master-2f-ssd1306

clean-Lib-2f-stm32-2d-ssd1306-2d-master-2f-ssd1306:
	-$(RM) ./Lib/stm32-ssd1306-master/ssd1306/ssd1306.cyclo ./Lib/stm32-ssd1306-master/ssd1306/ssd1306.d ./Lib/stm32-ssd1306-master/ssd1306/ssd1306.o ./Lib/stm32-ssd1306-master/ssd1306/ssd1306.su ./Lib/stm32-ssd1306-master/ssd1306/ssd1306_fonts.cyclo ./Lib/stm32-ssd1306-master/ssd1306/ssd1306_fonts.d ./Lib/stm32-ssd1306-master/ssd1306/ssd1306_fonts.o ./Lib/stm32-ssd1306-master/ssd1306/ssd1306_fonts.su ./Lib/stm32-ssd1306-master/ssd1306/ssd1306_tests.cyclo ./Lib/stm32-ssd1306-master/ssd1306/ssd1306_tests.d ./Lib/stm32-ssd1306-master/ssd1306/ssd1306_tests.o ./Lib/stm32-ssd1306-master/ssd1306/ssd1306_tests.su

.PHONY: clean-Lib-2f-stm32-2d-ssd1306-2d-master-2f-ssd1306

