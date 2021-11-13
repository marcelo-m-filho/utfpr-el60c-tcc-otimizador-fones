################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Application/Common/Streaming/Src/audio_usb_nodes.c \
../Application/Common/Streaming/Src/audio_usb_playback_session.c \
../Application/Common/Streaming/Src/usbd_audio_10_config_descriptors.c \
../Application/Common/Streaming/Src/usbd_audio_if.c 

OBJS += \
./Application/Common/Streaming/Src/audio_usb_nodes.o \
./Application/Common/Streaming/Src/audio_usb_playback_session.o \
./Application/Common/Streaming/Src/usbd_audio_10_config_descriptors.o \
./Application/Common/Streaming/Src/usbd_audio_if.o 

C_DEPS += \
./Application/Common/Streaming/Src/audio_usb_nodes.d \
./Application/Common/Streaming/Src/audio_usb_playback_session.d \
./Application/Common/Streaming/Src/usbd_audio_10_config_descriptors.d \
./Application/Common/Streaming/Src/usbd_audio_if.d 


# Each subdirectory must supply rules for building sources it contributes
Application/Common/Streaming/Src/%.o: ../Application/Common/Streaming/Src/%.c Application/Common/Streaming/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F769xx -DUSE_STM32F769I_DISCO -DUSE_IOEXPANDER -DUSE_USB_FS -DUSE_USB_FS_INTO_HS -DUSE_USB_AUDIO_PLAYBACK=1 -c -I"C:/Users/Marcelo/Repositories/Cube/projects/horoscope/Middlewares/STM32_USBD_Library/Core/Inc" -I"C:/Users/Marcelo/Repositories/Cube/projects/horoscope/Drivers/BSP/STM32F769I-Discovery/Inc" -I"C:/Users/Marcelo/Repositories/Cube/projects/horoscope/Application/Common/Middlewares/ST/STM32_USB_Device_Library/Class/AUDIO_10/Inc" -I"C:/Users/Marcelo/Repositories/Cube/projects/horoscope/Application/Common/Streaming/Inc" -I"C:/Users/Marcelo/Repositories/Cube/projects/horoscope/Application/Extension/Drivers/BSP/Components/wm8994/Inc" -I"C:/Users/Marcelo/Repositories/Cube/projects/horoscope/Application/Extension/Drivers/BSP/STM32F769I-DISCOVERY/Inc" -I"C:/Users/Marcelo/Repositories/Cube/projects/horoscope/Application/User/Inc" -I"C:/Users/Marcelo/Repositories/Cube/projects/horoscope/Drivers/STM32F7xx_HAL_Driver/Inc" -I"C:/Users/Marcelo/Repositories/Cube/projects/horoscope/Drivers/CMSIS/Device/ST/STM32F7xx/Include" -I"C:/Users/Marcelo/Repositories/Cube/projects/horoscope/Drivers/CMSIS/Include" -I"C:/Users/Marcelo/Repositories/Cube/projects/horoscope/Application/Extension/Drivers/STM32F7xx_HAL_Driver" -I"C:/Users/Marcelo/Repositories/Cube/projects/horoscope/Application/Extension/Drivers/BSP/Components/common" -Os -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

