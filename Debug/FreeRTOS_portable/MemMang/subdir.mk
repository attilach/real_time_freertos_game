################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FreeRTOS_portable/MemMang/heap_2.c 

OBJS += \
./FreeRTOS_portable/MemMang/heap_2.o 

C_DEPS += \
./FreeRTOS_portable/MemMang/heap_2.d 


# Each subdirectory must supply rules for building sources it contributes
FreeRTOS_portable/MemMang/%.o: ../FreeRTOS_portable/MemMang/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -D__USE_CMSIS=CMSISv2p00_LPC17xx -DDEBUG -D__CODE_RED -DPACK_STRUCT_END=__attribute\(\(packed\)\) -DGCC_ARMCM3 -I../src -I"/home/attila/LPCXpresso/workspace/CMSISv2p00_LPC17xx/inc" -I../FreeRTOS_include -I../FreeRTOS_portable -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


