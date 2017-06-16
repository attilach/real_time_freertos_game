################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/cat.c \
../src/commons.c \
../src/cr_startup_lpc17.c \
../src/debug.c \
../src/dtmf_detection.c \
../src/mouse.c \
../src/objects.c \
../src/rendering.c \
../src/tp2_cat_and_mouse.c \
../src/trace_mgt.c 

OBJS += \
./src/cat.o \
./src/commons.o \
./src/cr_startup_lpc17.o \
./src/debug.o \
./src/dtmf_detection.o \
./src/mouse.o \
./src/objects.o \
./src/rendering.o \
./src/tp2_cat_and_mouse.o \
./src/trace_mgt.o 

C_DEPS += \
./src/cat.d \
./src/commons.d \
./src/cr_startup_lpc17.d \
./src/debug.d \
./src/dtmf_detection.d \
./src/mouse.d \
./src/objects.d \
./src/rendering.d \
./src/tp2_cat_and_mouse.d \
./src/trace_mgt.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -D__USE_CMSIS=CMSISv2p00_LPC17xx -DDEBUG -D__CODE_RED -DPACK_STRUCT_END=__attribute\(\(packed\)\) -DGCC_ARMCM3 -I../src -I"/home/attila/LPCXpresso/workspace/CMSISv2p00_LPC17xx/inc" -I../FreeRTOS_include -I../FreeRTOS_portable -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


