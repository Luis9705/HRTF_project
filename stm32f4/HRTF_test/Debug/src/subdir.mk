################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/halFunct.c \
../src/main.c \
../src/stm32f4xx_it.c \
../src/syscalls.c \
../src/system_stm32f4xx.c 

OBJS += \
./src/halFunct.o \
./src/main.o \
./src/stm32f4xx_it.o \
./src/syscalls.o \
./src/system_stm32f4xx.o 

C_DEPS += \
./src/halFunct.d \
./src/main.d \
./src/stm32f4xx_it.d \
./src/syscalls.d \
./src/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32 -DARM_MATH_CM4 '-D__FPU_USED = 1' '-D__FPU_PRESENT = 1' -DSTM32F4 -DSTM32F429ZITx -DSTM32F429I_DISCO -DDEBUG -DSTM32F429_439xx -DUSE_STDPERIPH_DRIVER -I"C:/Users/FLL1GA/Documents/Luis Javier Flores/Tec de Monterrey/8vo Semestre/DSPs/HRTF_project/stm32f4/UART_FULL_DUPLEX/Utilities/Common" -I"C:/Users/FLL1GA/Documents/Luis Javier Flores/Tec de Monterrey/8vo Semestre/DSPs/HRTF_project/stm32f4/UART_FULL_DUPLEX/StdPeriph_Driver/inc" -I"C:/Users/FLL1GA/Documents/Luis Javier Flores/Tec de Monterrey/8vo Semestre/DSPs/HRTF_project/stm32f4/UART_FULL_DUPLEX/inc" -I"C:/Users/FLL1GA/Documents/Luis Javier Flores/Tec de Monterrey/8vo Semestre/DSPs/HRTF_project/stm32f4/UART_FULL_DUPLEX/CMSIS/device" -I"C:/Users/FLL1GA/Documents/Luis Javier Flores/Tec de Monterrey/8vo Semestre/DSPs/HRTF_project/stm32f4/UART_FULL_DUPLEX/CMSIS/core" -I"C:/Users/FLL1GA/Documents/Luis Javier Flores/Tec de Monterrey/8vo Semestre/DSPs/HRTF_project/stm32f4/UART_FULL_DUPLEX/Utilities/STM32F429I-Discovery" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


