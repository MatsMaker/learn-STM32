################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/SSD1306/examples/oled-tester/firmware/i2c/Src/main.c \
../Drivers/SSD1306/examples/oled-tester/firmware/i2c/Src/stm32f4xx_hal_msp.c \
../Drivers/SSD1306/examples/oled-tester/firmware/i2c/Src/stm32f4xx_it.c \
../Drivers/SSD1306/examples/oled-tester/firmware/i2c/Src/system_stm32f4xx.c 

OBJS += \
./Drivers/SSD1306/examples/oled-tester/firmware/i2c/Src/main.o \
./Drivers/SSD1306/examples/oled-tester/firmware/i2c/Src/stm32f4xx_hal_msp.o \
./Drivers/SSD1306/examples/oled-tester/firmware/i2c/Src/stm32f4xx_it.o \
./Drivers/SSD1306/examples/oled-tester/firmware/i2c/Src/system_stm32f4xx.o 

C_DEPS += \
./Drivers/SSD1306/examples/oled-tester/firmware/i2c/Src/main.d \
./Drivers/SSD1306/examples/oled-tester/firmware/i2c/Src/stm32f4xx_hal_msp.d \
./Drivers/SSD1306/examples/oled-tester/firmware/i2c/Src/stm32f4xx_it.d \
./Drivers/SSD1306/examples/oled-tester/firmware/i2c/Src/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/SSD1306/examples/oled-tester/firmware/i2c/Src/%.o Drivers/SSD1306/examples/oled-tester/firmware/i2c/Src/%.su Drivers/SSD1306/examples/oled-tester/firmware/i2c/Src/%.cyclo: ../Drivers/SSD1306/examples/oled-tester/firmware/i2c/Src/%.c Drivers/SSD1306/examples/oled-tester/firmware/i2c/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"/home/mats/Projects/STM32Cube/flightStick_bluetooth/flightStick_bluetooth/Drivers/SSD1306/ssd1306" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-SSD1306-2f-examples-2f-oled-2d-tester-2f-firmware-2f-i2c-2f-Src

clean-Drivers-2f-SSD1306-2f-examples-2f-oled-2d-tester-2f-firmware-2f-i2c-2f-Src:
	-$(RM) ./Drivers/SSD1306/examples/oled-tester/firmware/i2c/Src/main.cyclo ./Drivers/SSD1306/examples/oled-tester/firmware/i2c/Src/main.d ./Drivers/SSD1306/examples/oled-tester/firmware/i2c/Src/main.o ./Drivers/SSD1306/examples/oled-tester/firmware/i2c/Src/main.su ./Drivers/SSD1306/examples/oled-tester/firmware/i2c/Src/stm32f4xx_hal_msp.cyclo ./Drivers/SSD1306/examples/oled-tester/firmware/i2c/Src/stm32f4xx_hal_msp.d ./Drivers/SSD1306/examples/oled-tester/firmware/i2c/Src/stm32f4xx_hal_msp.o ./Drivers/SSD1306/examples/oled-tester/firmware/i2c/Src/stm32f4xx_hal_msp.su ./Drivers/SSD1306/examples/oled-tester/firmware/i2c/Src/stm32f4xx_it.cyclo ./Drivers/SSD1306/examples/oled-tester/firmware/i2c/Src/stm32f4xx_it.d ./Drivers/SSD1306/examples/oled-tester/firmware/i2c/Src/stm32f4xx_it.o ./Drivers/SSD1306/examples/oled-tester/firmware/i2c/Src/stm32f4xx_it.su ./Drivers/SSD1306/examples/oled-tester/firmware/i2c/Src/system_stm32f4xx.cyclo ./Drivers/SSD1306/examples/oled-tester/firmware/i2c/Src/system_stm32f4xx.d ./Drivers/SSD1306/examples/oled-tester/firmware/i2c/Src/system_stm32f4xx.o ./Drivers/SSD1306/examples/oled-tester/firmware/i2c/Src/system_stm32f4xx.su

.PHONY: clean-Drivers-2f-SSD1306-2f-examples-2f-oled-2d-tester-2f-firmware-2f-i2c-2f-Src

