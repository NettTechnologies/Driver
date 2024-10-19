################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HCI_AS/Src/arrays.c \
../HCI_AS/Src/errors.c \
../HCI_AS/Src/fuel_mass_calculation.c \
../HCI_AS/Src/hci.c \
../HCI_AS/Src/linear_approximation.c \
../HCI_AS/Src/rt_look.c \
../HCI_AS/Src/rt_look1d.c \
../HCI_AS/Src/rt_look2d_normal.c \
../HCI_AS/Src/sensors.c \
../HCI_AS/Src/soot.c 

OBJS += \
./HCI_AS/Src/arrays.o \
./HCI_AS/Src/errors.o \
./HCI_AS/Src/fuel_mass_calculation.o \
./HCI_AS/Src/hci.o \
./HCI_AS/Src/linear_approximation.o \
./HCI_AS/Src/rt_look.o \
./HCI_AS/Src/rt_look1d.o \
./HCI_AS/Src/rt_look2d_normal.o \
./HCI_AS/Src/sensors.o \
./HCI_AS/Src/soot.o 

C_DEPS += \
./HCI_AS/Src/arrays.d \
./HCI_AS/Src/errors.d \
./HCI_AS/Src/fuel_mass_calculation.d \
./HCI_AS/Src/hci.d \
./HCI_AS/Src/linear_approximation.d \
./HCI_AS/Src/rt_look.d \
./HCI_AS/Src/rt_look1d.d \
./HCI_AS/Src/rt_look2d_normal.d \
./HCI_AS/Src/sensors.d \
./HCI_AS/Src/soot.d 


# Each subdirectory must supply rules for building sources it contributes
HCI_AS/Src/%.o HCI_AS/Src/%.su HCI_AS/Src/%.cyclo: ../HCI_AS/Src/%.c HCI_AS/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F205xx -c -I../Core/Inc -I../FATFS/Target -I../FATFS/App -I../Drivers/STM32F2xx_HAL_Driver/Inc -I../Drivers/STM32F2xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Device/ST/STM32F2xx/Include -I../Drivers/CMSIS/Include -I"../HCI_AS/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-HCI_AS-2f-Src

clean-HCI_AS-2f-Src:
	-$(RM) ./HCI_AS/Src/arrays.cyclo ./HCI_AS/Src/arrays.d ./HCI_AS/Src/arrays.o ./HCI_AS/Src/arrays.su ./HCI_AS/Src/errors.cyclo ./HCI_AS/Src/errors.d ./HCI_AS/Src/errors.o ./HCI_AS/Src/errors.su ./HCI_AS/Src/fuel_mass_calculation.cyclo ./HCI_AS/Src/fuel_mass_calculation.d ./HCI_AS/Src/fuel_mass_calculation.o ./HCI_AS/Src/fuel_mass_calculation.su ./HCI_AS/Src/hci.cyclo ./HCI_AS/Src/hci.d ./HCI_AS/Src/hci.o ./HCI_AS/Src/hci.su ./HCI_AS/Src/linear_approximation.cyclo ./HCI_AS/Src/linear_approximation.d ./HCI_AS/Src/linear_approximation.o ./HCI_AS/Src/linear_approximation.su ./HCI_AS/Src/rt_look.cyclo ./HCI_AS/Src/rt_look.d ./HCI_AS/Src/rt_look.o ./HCI_AS/Src/rt_look.su ./HCI_AS/Src/rt_look1d.cyclo ./HCI_AS/Src/rt_look1d.d ./HCI_AS/Src/rt_look1d.o ./HCI_AS/Src/rt_look1d.su ./HCI_AS/Src/rt_look2d_normal.cyclo ./HCI_AS/Src/rt_look2d_normal.d ./HCI_AS/Src/rt_look2d_normal.o ./HCI_AS/Src/rt_look2d_normal.su ./HCI_AS/Src/sensors.cyclo ./HCI_AS/Src/sensors.d ./HCI_AS/Src/sensors.o ./HCI_AS/Src/sensors.su ./HCI_AS/Src/soot.cyclo ./HCI_AS/Src/soot.d ./HCI_AS/Src/soot.o ./HCI_AS/Src/soot.su

.PHONY: clean-HCI_AS-2f-Src

