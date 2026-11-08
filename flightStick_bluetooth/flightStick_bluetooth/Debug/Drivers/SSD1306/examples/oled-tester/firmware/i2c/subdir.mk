################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Drivers/SSD1306/examples/oled-tester/firmware/i2c/startup_stm32f411xe.s 

OBJS += \
./Drivers/SSD1306/examples/oled-tester/firmware/i2c/startup_stm32f411xe.o 

S_DEPS += \
./Drivers/SSD1306/examples/oled-tester/firmware/i2c/startup_stm32f411xe.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/SSD1306/examples/oled-tester/firmware/i2c/%.o: ../Drivers/SSD1306/examples/oled-tester/firmware/i2c/%.s Drivers/SSD1306/examples/oled-tester/firmware/i2c/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m3 -g3 -DDEBUG -c -I"/home/mats/Projects/STM32Cube/flightStick_bluetooth/flightStick_bluetooth/Drivers/SSD1306/ssd1306" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@" "$<"

clean: clean-Drivers-2f-SSD1306-2f-examples-2f-oled-2d-tester-2f-firmware-2f-i2c

clean-Drivers-2f-SSD1306-2f-examples-2f-oled-2d-tester-2f-firmware-2f-i2c:
	-$(RM) ./Drivers/SSD1306/examples/oled-tester/firmware/i2c/startup_stm32f411xe.d ./Drivers/SSD1306/examples/oled-tester/firmware/i2c/startup_stm32f411xe.o

.PHONY: clean-Drivers-2f-SSD1306-2f-examples-2f-oled-2d-tester-2f-firmware-2f-i2c

