################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
../startup/startup_stm32f429_439xx.S 

OBJS += \
./startup/startup_stm32f429_439xx.o 

S_UPPER_DEPS += \
./startup/startup_stm32f429_439xx.d 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32 -DARM_MATH_CM4 '-D__FPU_USED = 1' '-D__FPU_PRESENT = 1' -DSTM32F4 -DSTM32F429ZITx -DSTM32F429I_DISCO -DDEBUG -DSTM32F429_439xx -DUSE_STDPERIPH_DRIVER -I"C:/Users/FLL1GA/Documents/Luis Javier Flores/Tec de Monterrey/8vo Semestre/DSPs/HRTF_project/stm32f4/ADC_test/Utilities/Common" -I"C:/Users/FLL1GA/Documents/Luis Javier Flores/Tec de Monterrey/8vo Semestre/DSPs/HRTF_project/stm32f4/ADC_test/StdPeriph_Driver/inc" -I"C:/Users/FLL1GA/Documents/Luis Javier Flores/Tec de Monterrey/8vo Semestre/DSPs/HRTF_project/stm32f4/ADC_test/inc" -I"C:/Users/FLL1GA/Documents/Luis Javier Flores/Tec de Monterrey/8vo Semestre/DSPs/HRTF_project/stm32f4/ADC_test/CMSIS/device" -I"C:/Users/FLL1GA/Documents/Luis Javier Flores/Tec de Monterrey/8vo Semestre/DSPs/HRTF_project/stm32f4/ADC_test/CMSIS/core" -I"C:/Users/FLL1GA/Documents/Luis Javier Flores/Tec de Monterrey/8vo Semestre/DSPs/HRTF_project/stm32f4/ADC_test/Utilities/STM32F429I-Discovery" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


