################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Application/User/Src/audio_speaker_node.c \
../Application/User/Src/main.c \
../Application/User/Src/stm32f7xx_hal_msp.c \
../Application/User/Src/stm32f7xx_it.c \
../Application/User/Src/system_stm32f7xx.c \
../Application/User/Src/usbd_conf.c \
../Application/User/Src/usbd_desc.c 

OBJS += \
./Application/User/Src/audio_speaker_node.o \
./Application/User/Src/main.o \
./Application/User/Src/stm32f7xx_hal_msp.o \
./Application/User/Src/stm32f7xx_it.o \
./Application/User/Src/system_stm32f7xx.o \
./Application/User/Src/usbd_conf.o \
./Application/User/Src/usbd_desc.o 

C_DEPS += \
./Application/User/Src/audio_speaker_node.d \
./Application/User/Src/main.d \
./Application/User/Src/stm32f7xx_hal_msp.d \
./Application/User/Src/stm32f7xx_it.d \
./Application/User/Src/system_stm32f7xx.d \
./Application/User/Src/usbd_conf.d \
./Application/User/Src/usbd_desc.d 


# Each subdirectory must supply rules for building sources it contributes
Application/User/Src/%.o: ../Application/User/Src/%.c Application/User/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F769xx -DUSE_STM32F769I_DISCO -DUSE_IOEXPANDER -DUSE_USB_FS -DUSE_USB_FS_INTO_HS -DUSE_USB_AUDIO_PLAYBACK=1 -c -I"C:/Users/Marcelo/Repositories/Cube/projects/horoscope/Middlewares/STM32_USBD_Library/Core/Inc" -I"C:/Users/Marcelo/Repositories/Cube/projects/horoscope/Drivers/BSP/STM32F769I-Discovery/Inc" -I"C:/Users/Marcelo/Repositories/Cube/projects/horoscope/Application/Common/Middlewares/ST/STM32_USB_Device_Library/Class/AUDIO_10/Inc" -I"C:/Users/Marcelo/Repositories/Cube/projects/horoscope/Application/Common/Streaming/Inc" -I"C:/Users/Marcelo/Repositories/Cube/projects/horoscope/Application/Extension/Drivers/BSP/Components/wm8994/Inc" -I"C:/Users/Marcelo/Repositories/Cube/projects/horoscope/Application/Extension/Drivers/BSP/STM32F769I-DISCOVERY/Inc" -I"C:/Users/Marcelo/Repositories/Cube/projects/horoscope/Application/User/Inc" -I"C:/Users/Marcelo/Repositories/Cube/projects/horoscope/Drivers/STM32F7xx_HAL_Driver/Inc" -I"C:/Users/Marcelo/Repositories/Cube/projects/horoscope/Drivers/CMSIS/Device/ST/STM32F7xx/Include" -I"C:/Users/Marcelo/Repositories/Cube/projects/horoscope/Drivers/CMSIS/Include" -I"C:/Users/Marcelo/Repositories/Cube/projects/horoscope/Application/Extension/Drivers/STM32F7xx_HAL_Driver" -I"C:/Users/Marcelo/Repositories/Cube/projects/horoscope/Application/Extension/Drivers/BSP/Components/common" -Os -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

