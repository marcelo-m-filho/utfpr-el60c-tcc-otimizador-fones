################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/STM32F769I-Discovery/Src/stm32f769i_discovery.c 

OBJS += \
./Drivers/BSP/STM32F769I-Discovery/Src/stm32f769i_discovery.o 

C_DEPS += \
./Drivers/BSP/STM32F769I-Discovery/Src/stm32f769i_discovery.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/STM32F769I-Discovery/Src/%.o: ../Drivers/BSP/STM32F769I-Discovery/Src/%.c Drivers/BSP/STM32F769I-Discovery/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F769xx -DUSE_STM32F769I_DISCO -DUSE_IOEXPANDER -DUSE_USB_FS -DUSE_USB_FS_INTO_HS -DUSE_USB_AUDIO_PLAYBACK=1 -c -I"C:/Users/Marcelo/Repositories/Cube/projects/horoscope/Middlewares/STM32_USBD_Library/Core/Inc" -I"C:/Users/Marcelo/Repositories/Cube/projects/horoscope/Drivers/BSP/STM32F769I-Discovery/Inc" -I"C:/Users/Marcelo/Repositories/Cube/projects/horoscope/Application/Common/Middlewares/ST/STM32_USB_Device_Library/Class/AUDIO_10/Inc" -I"C:/Users/Marcelo/Repositories/Cube/projects/horoscope/Application/Common/Streaming/Inc" -I"C:/Users/Marcelo/Repositories/Cube/projects/horoscope/Application/Extension/Drivers/BSP/Components/wm8994/Inc" -I"C:/Users/Marcelo/Repositories/Cube/projects/horoscope/Application/Extension/Drivers/BSP/STM32F769I-DISCOVERY/Inc" -I"C:/Users/Marcelo/Repositories/Cube/projects/horoscope/Application/User/Inc" -I"C:/Users/Marcelo/Repositories/Cube/projects/horoscope/Drivers/STM32F7xx_HAL_Driver/Inc" -I"C:/Users/Marcelo/Repositories/Cube/projects/horoscope/Drivers/CMSIS/Device/ST/STM32F7xx/Include" -I"C:/Users/Marcelo/Repositories/Cube/projects/horoscope/Drivers/CMSIS/Include" -I"C:/Users/Marcelo/Repositories/Cube/projects/horoscope/Application/Extension/Drivers/STM32F7xx_HAL_Driver" -I"C:/Users/Marcelo/Repositories/Cube/projects/horoscope/Application/Extension/Drivers/BSP/Components/common" -Os -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

