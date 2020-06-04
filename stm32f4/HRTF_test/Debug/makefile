################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include startup/subdir.mk
-include src/subdir.mk
-include Utilities/STM32F429I-Discovery/subdir.mk
-include Utilities/Common/subdir.mk
-include StdPeriph_Driver/src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: UART_FULL_DUPLEX.elf

# Tool invocations
UART_FULL_DUPLEX.elf: $(OBJS) $(USER_OBJS) C:\Users\FLL1GA\Documents\Luis\ Javier\ Flores\Tec\ de\ Monterrey\8vo\ Semestre\DSPs\HRTF_project\stm32f4\UART_FULL_DUPLEX\LinkerScript.ld
	@echo 'Building target: $@'
	@echo 'Invoking: MCU GCC Linker'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -L"C:\Users\FLL1GA\Documents\Luis Javier Flores\Tec de Monterrey\8vo Semestre\DSPs\HRTF_project\stm32f4\UART_FULL_DUPLEX\lib" -T"C:\Users\FLL1GA\Documents\Luis Javier Flores\Tec de Monterrey\8vo Semestre\DSPs\HRTF_project\stm32f4\UART_FULL_DUPLEX\LinkerScript.ld" -Wl,-Map=output.map -Wl,--gc-sections -o "UART_FULL_DUPLEX.elf" @"objects.list" $(USER_OBJS) $(LIBS) -lm
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

post-build:
	-@echo 'Generating binary and Printing size information:'
	arm-none-eabi-objcopy -O binary "UART_FULL_DUPLEX.elf" "UART_FULL_DUPLEX.bin"
	arm-none-eabi-size "UART_FULL_DUPLEX.elf"
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
