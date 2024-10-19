################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/L99sd_new/l99sd.c 

OBJS += \
./Core/Src/L99sd_new/l99sd.o 

C_DEPS += \
./Core/Src/L99sd_new/l99sd.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/L99sd_new/%.o Core/Src/L99sd_new/%.su Core/Src/L99sd_new/%.cyclo: ../Core/Src/L99sd_new/%.c Core/Src/L99sd_new/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F205xx -c -I../Core/Inc -I../FATFS/Target -I../FATFS/App -I../Drivers/STM32F2xx_HAL_Driver/Inc -I../Drivers/STM32F2xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Device/ST/STM32F2xx/Include -I../Drivers/CMSIS/Include -I"../HCI_AS/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-L99sd_new

clean-Core-2f-Src-2f-L99sd_new:
	-$(RM) ./Core/Src/L99sd_new/l99sd.cyclo ./Core/Src/L99sd_new/l99sd.d ./Core/Src/L99sd_new/l99sd.o ./Core/Src/L99sd_new/l99sd.su

.PHONY: clean-Core-2f-Src-2f-L99sd_new

