################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/App/Variables.c 

OBJS += \
./Core/App/Variables.o 

C_DEPS += \
./Core/App/Variables.d 


# Each subdirectory must supply rules for building sources it contributes
Core/App/%.o Core/App/%.su Core/App/%.cyclo: ../Core/App/%.c Core/App/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F205xx -c -I../Core/Inc -I../FATFS/Target -I../FATFS/App -I../Drivers/STM32F2xx_HAL_Driver/Inc -I../Drivers/STM32F2xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Device/ST/STM32F2xx/Include -I../Drivers/CMSIS/Include -I"../HCI_AS/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-App

clean-Core-2f-App:
	-$(RM) ./Core/App/Variables.cyclo ./Core/App/Variables.d ./Core/App/Variables.o ./Core/App/Variables.su

.PHONY: clean-Core-2f-App

