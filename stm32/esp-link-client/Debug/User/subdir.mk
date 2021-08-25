################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/ELClient.c \
../User/ELClientCRC.c \
../User/ELClientMqtt.c \
../User/ELClientRequest.c \
../User/ELClientResponse.c \
../User/ELClientTransport.c \
../User/debug_log.c \
../User/mqtt.c 

OBJS += \
./User/ELClient.o \
./User/ELClientCRC.o \
./User/ELClientMqtt.o \
./User/ELClientRequest.o \
./User/ELClientResponse.o \
./User/ELClientTransport.o \
./User/debug_log.o \
./User/mqtt.o 

C_DEPS += \
./User/ELClient.d \
./User/ELClientCRC.d \
./User/ELClientMqtt.d \
./User/ELClientRequest.d \
./User/ELClientResponse.d \
./User/ELClientTransport.d \
./User/debug_log.d \
./User/mqtt.d 


# Each subdirectory must supply rules for building sources it contributes
User/%.o: ../User/%.c User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../User -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../User/platform -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

