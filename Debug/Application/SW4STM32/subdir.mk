################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Application/SW4STM32/startup_stm32f769xx.s 

OBJS += \
./Application/SW4STM32/startup_stm32f769xx.o 

S_DEPS += \
./Application/SW4STM32/startup_stm32f769xx.d 


# Each subdirectory must supply rules for building sources it contributes
Application/SW4STM32/%.o: ../Application/SW4STM32/%.s Application/SW4STM32/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m7 -g3 -c -I"C:/Users/Marcelo/Repositories/Cube/projects/horoscope/Middlewares/STM32_USBD_Library/Core/Inc" -I"C:/Users/Marcelo/Repositories/Cube/projects/horoscope/Drivers/BSP/STM32F769I-Discovery/Inc" -I"C:/Users/Marcelo/Repositories/Cube/projects/horoscope/Application/Common/Middlewares/ST/STM32_USB_Device_Library/Class/AUDIO_10/Inc" -I"C:/Users/Marcelo/Repositories/Cube/projects/horoscope/Application/Common/Streaming/Inc" -I"C:/Users/Marcelo/Repositories/Cube/projects/horoscope/Application/Extension/Drivers/BSP/Components/wm8994/Inc" -I"C:/Users/Marcelo/Repositories/Cube/projects/horoscope/Application/Extension/Drivers/BSP/STM32F769I-DISCOVERY/Inc" -I"C:/Users/Marcelo/Repositories/Cube/projects/horoscope/Application/User/Inc" -I"C:/Users/Marcelo/Repositories/Cube/projects/horoscope/Drivers/STM32F7xx_HAL_Driver/Inc" -I"C:/Users/Marcelo/Repositories/Cube/projects/horoscope/Drivers/CMSIS/Device/ST/STM32F7xx/Include" -I"C:/Users/Marcelo/Repositories/Cube/projects/horoscope/Drivers/CMSIS/Include" -I"C:/Users/Marcelo/Repositories/Cube/projects/horoscope/Application/Extension/Drivers/STM32F7xx_HAL_Driver" -I"C:/Users/Marcelo/Repositories/Cube/projects/horoscope/Application/Extension/Drivers/BSP/Components/common" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

