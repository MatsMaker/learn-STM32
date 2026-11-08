################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/SSD1306/Src/ssd1306.c \
../Drivers/SSD1306/Src/ssd1306_fonts.c 

OBJS += \
./Drivers/SSD1306/Src/ssd1306.o \
./Drivers/SSD1306/Src/ssd1306_fonts.o 

C_DEPS += \
./Drivers/SSD1306/Src/ssd1306.d \
./Drivers/SSD1306/Src/ssd1306_fonts.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/SSD1306/Src/%.o Drivers/SSD1306/Src/%.su Drivers/SSD1306/Src/%.cyclo: ../Drivers/SSD1306/Src/%.c Drivers/SSD1306/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"/home/mats/Projects/STM32Cube/flightStick_bluetooth/flightStick_bluetooth/Drivers/SSD1306/Inc" -I"/home/mats/Projects/STM32Cube/flightStick_bluetooth/flightStick_bluetooth/Drivers/SSD1306/Src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-SSD1306-2f-Src

clean-Drivers-2f-SSD1306-2f-Src:
	-$(RM) ./Drivers/SSD1306/Src/ssd1306.cyclo ./Drivers/SSD1306/Src/ssd1306.d ./Drivers/SSD1306/Src/ssd1306.o ./Drivers/SSD1306/Src/ssd1306.su ./Drivers/SSD1306/Src/ssd1306_fonts.cyclo ./Drivers/SSD1306/Src/ssd1306_fonts.d ./Drivers/SSD1306/Src/ssd1306_fonts.o ./Drivers/SSD1306/Src/ssd1306_fonts.su

.PHONY: clean-Drivers-2f-SSD1306-2f-Src

