################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Utilities/STM32F429I-Discovery/stm32f429i_discovery.c \
../Utilities/STM32F429I-Discovery/stm32f429i_discovery_ioe.c \
../Utilities/STM32F429I-Discovery/stm32f429i_discovery_l3gd20.c \
../Utilities/STM32F429I-Discovery/stm32f429i_discovery_lcd.c \
../Utilities/STM32F429I-Discovery/stm32f429i_discovery_sdram.c 

OBJS += \
./Utilities/STM32F429I-Discovery/stm32f429i_discovery.o \
./Utilities/STM32F429I-Discovery/stm32f429i_discovery_ioe.o \
./Utilities/STM32F429I-Discovery/stm32f429i_discovery_l3gd20.o \
./Utilities/STM32F429I-Discovery/stm32f429i_discovery_lcd.o \
./Utilities/STM32F429I-Discovery/stm32f429i_discovery_sdram.o 

C_DEPS += \
./Utilities/STM32F429I-Discovery/stm32f429i_discovery.d \
./Utilities/STM32F429I-Discovery/stm32f429i_discovery_ioe.d \
./Utilities/STM32F429I-Discovery/stm32f429i_discovery_l3gd20.d \
./Utilities/STM32F429I-Discovery/stm32f429i_discovery_lcd.d \
./Utilities/STM32F429I-Discovery/stm32f429i_discovery_sdram.d 


# Each subdirectory must supply rules for building sources it contributes
Utilities/STM32F429I-Discovery/%.o: ../Utilities/STM32F429I-Discovery/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32 -DARM_MATH_CM4 '-D__FPU_USED = 1' '-D__FPU_PRESENT = 1' -DSTM32F4 -DSTM32F429ZITx -DSTM32F429I_DISCO -DDEBUG -DSTM32F429_439xx -DUSE_STDPERIPH_DRIVER -I"C:/Users/FLL1GA/Documents/Luis Javier Flores/Tec de Monterrey/8vo Semestre/DSPs/HRTF_project/stm32f4/HRTF_test/Utilities/Common" -I"C:/Users/FLL1GA/Documents/Luis Javier Flores/Tec de Monterrey/8vo Semestre/DSPs/HRTF_project/stm32f4/HRTF_test/StdPeriph_Driver/inc" -I"C:/Users/FLL1GA/Documents/Luis Javier Flores/Tec de Monterrey/8vo Semestre/DSPs/HRTF_project/stm32f4/HRTF_test/inc" -I"C:/Users/FLL1GA/Documents/Luis Javier Flores/Tec de Monterrey/8vo Semestre/DSPs/HRTF_project/stm32f4/HRTF_test/CMSIS/device" -I"C:/Users/FLL1GA/Documents/Luis Javier Flores/Tec de Monterrey/8vo Semestre/DSPs/HRTF_project/stm32f4/HRTF_test/CMSIS/core" -I"C:/Users/FLL1GA/Documents/Luis Javier Flores/Tec de Monterrey/8vo Semestre/DSPs/HRTF_project/stm32f4/HRTF_test/Utilities/STM32F429I-Discovery" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


