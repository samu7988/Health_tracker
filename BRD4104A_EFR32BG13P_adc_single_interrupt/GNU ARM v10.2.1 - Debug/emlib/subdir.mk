################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
I:/Software_Install/SiliconLabs/SimplicityStudio/v5/developer/sdks/gecko_sdk_suite/v3.2/platform/emlib/src/em_adc.c \
I:/Software_Install/SiliconLabs/SimplicityStudio/v5/developer/sdks/gecko_sdk_suite/v3.2/platform/emlib/src/em_cmu.c \
I:/Software_Install/SiliconLabs/SimplicityStudio/v5/developer/sdks/gecko_sdk_suite/v3.2/platform/emlib/src/em_core.c \
I:/Software_Install/SiliconLabs/SimplicityStudio/v5/developer/sdks/gecko_sdk_suite/v3.2/platform/emlib/src/em_system.c 

OBJS += \
./emlib/em_adc.o \
./emlib/em_cmu.o \
./emlib/em_core.o \
./emlib/em_system.o 

C_DEPS += \
./emlib/em_adc.d \
./emlib/em_cmu.d \
./emlib/em_core.d \
./emlib/em_system.d 


# Each subdirectory must supply rules for building sources it contributes
emlib/em_adc.o: I:/Software_Install/SiliconLabs/SimplicityStudio/v5/developer/sdks/gecko_sdk_suite/v3.2/platform/emlib/src/em_adc.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -std=c99 '-DEFR32BG13P632F512GM48=1' '-DDEBUG=1' -II:/Software_Install/SiliconLabs/SimplicityStudio/v5/developer/repos/hardware/kit/EFR32BG13_BRD4104A/config -I"I:/Software_Install/SiliconLabs/SimplicityStudio/v5/developer/sdks/gecko_sdk_suite/v3.2//hardware/kit/EFR32BG13_BRD4104A/config" -I"I:/Software_Install/SiliconLabs/SimplicityStudio/v5/developer/sdks/gecko_sdk_suite/v3.2//platform/CMSIS/Include" -I"I:/Software_Install/SiliconLabs/SimplicityStudio/v5/developer/sdks/gecko_sdk_suite/v3.2//platform/emlib/inc" -I"I:/Software_Install/SiliconLabs/SimplicityStudio/v5/developer/sdks/gecko_sdk_suite/v3.2//hardware/kit/common/bsp" -I"I:/Software_Install/SiliconLabs/SimplicityStudio/v5/developer/sdks/gecko_sdk_suite/v3.2//hardware/kit/common/drivers" -I"I:/Software_Install/SiliconLabs/SimplicityStudio/v5/developer/sdks/gecko_sdk_suite/v3.2//platform/Device/SiliconLabs/EFR32BG13P/Include" -O0 -Wall -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -c -fmessage-length=0 -MMD -MP -MF"emlib/em_adc.d" -MT"emlib/em_adc.o" -o "$@" "$<" && "I:/Software_Install/SiliconLabs/SimplicityStudio/v5/configuration/org.eclipse.osgi/446/0/.cp/scripts/path_fixup.sh" "emlib/em_adc.d"
	@echo 'Finished building: $<'
	@echo ' '

emlib/em_cmu.o: I:/Software_Install/SiliconLabs/SimplicityStudio/v5/developer/sdks/gecko_sdk_suite/v3.2/platform/emlib/src/em_cmu.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -std=c99 '-DEFR32BG13P632F512GM48=1' '-DDEBUG=1' -II:/Software_Install/SiliconLabs/SimplicityStudio/v5/developer/repos/hardware/kit/EFR32BG13_BRD4104A/config -I"I:/Software_Install/SiliconLabs/SimplicityStudio/v5/developer/sdks/gecko_sdk_suite/v3.2//hardware/kit/EFR32BG13_BRD4104A/config" -I"I:/Software_Install/SiliconLabs/SimplicityStudio/v5/developer/sdks/gecko_sdk_suite/v3.2//platform/CMSIS/Include" -I"I:/Software_Install/SiliconLabs/SimplicityStudio/v5/developer/sdks/gecko_sdk_suite/v3.2//platform/emlib/inc" -I"I:/Software_Install/SiliconLabs/SimplicityStudio/v5/developer/sdks/gecko_sdk_suite/v3.2//hardware/kit/common/bsp" -I"I:/Software_Install/SiliconLabs/SimplicityStudio/v5/developer/sdks/gecko_sdk_suite/v3.2//hardware/kit/common/drivers" -I"I:/Software_Install/SiliconLabs/SimplicityStudio/v5/developer/sdks/gecko_sdk_suite/v3.2//platform/Device/SiliconLabs/EFR32BG13P/Include" -O0 -Wall -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -c -fmessage-length=0 -MMD -MP -MF"emlib/em_cmu.d" -MT"emlib/em_cmu.o" -o "$@" "$<" && "I:/Software_Install/SiliconLabs/SimplicityStudio/v5/configuration/org.eclipse.osgi/446/0/.cp/scripts/path_fixup.sh" "emlib/em_cmu.d"
	@echo 'Finished building: $<'
	@echo ' '

emlib/em_core.o: I:/Software_Install/SiliconLabs/SimplicityStudio/v5/developer/sdks/gecko_sdk_suite/v3.2/platform/emlib/src/em_core.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -std=c99 '-DEFR32BG13P632F512GM48=1' '-DDEBUG=1' -II:/Software_Install/SiliconLabs/SimplicityStudio/v5/developer/repos/hardware/kit/EFR32BG13_BRD4104A/config -I"I:/Software_Install/SiliconLabs/SimplicityStudio/v5/developer/sdks/gecko_sdk_suite/v3.2//hardware/kit/EFR32BG13_BRD4104A/config" -I"I:/Software_Install/SiliconLabs/SimplicityStudio/v5/developer/sdks/gecko_sdk_suite/v3.2//platform/CMSIS/Include" -I"I:/Software_Install/SiliconLabs/SimplicityStudio/v5/developer/sdks/gecko_sdk_suite/v3.2//platform/emlib/inc" -I"I:/Software_Install/SiliconLabs/SimplicityStudio/v5/developer/sdks/gecko_sdk_suite/v3.2//hardware/kit/common/bsp" -I"I:/Software_Install/SiliconLabs/SimplicityStudio/v5/developer/sdks/gecko_sdk_suite/v3.2//hardware/kit/common/drivers" -I"I:/Software_Install/SiliconLabs/SimplicityStudio/v5/developer/sdks/gecko_sdk_suite/v3.2//platform/Device/SiliconLabs/EFR32BG13P/Include" -O0 -Wall -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -c -fmessage-length=0 -MMD -MP -MF"emlib/em_core.d" -MT"emlib/em_core.o" -o "$@" "$<" && "I:/Software_Install/SiliconLabs/SimplicityStudio/v5/configuration/org.eclipse.osgi/446/0/.cp/scripts/path_fixup.sh" "emlib/em_core.d"
	@echo 'Finished building: $<'
	@echo ' '

emlib/em_system.o: I:/Software_Install/SiliconLabs/SimplicityStudio/v5/developer/sdks/gecko_sdk_suite/v3.2/platform/emlib/src/em_system.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -std=c99 '-DEFR32BG13P632F512GM48=1' '-DDEBUG=1' -II:/Software_Install/SiliconLabs/SimplicityStudio/v5/developer/repos/hardware/kit/EFR32BG13_BRD4104A/config -I"I:/Software_Install/SiliconLabs/SimplicityStudio/v5/developer/sdks/gecko_sdk_suite/v3.2//hardware/kit/EFR32BG13_BRD4104A/config" -I"I:/Software_Install/SiliconLabs/SimplicityStudio/v5/developer/sdks/gecko_sdk_suite/v3.2//platform/CMSIS/Include" -I"I:/Software_Install/SiliconLabs/SimplicityStudio/v5/developer/sdks/gecko_sdk_suite/v3.2//platform/emlib/inc" -I"I:/Software_Install/SiliconLabs/SimplicityStudio/v5/developer/sdks/gecko_sdk_suite/v3.2//hardware/kit/common/bsp" -I"I:/Software_Install/SiliconLabs/SimplicityStudio/v5/developer/sdks/gecko_sdk_suite/v3.2//hardware/kit/common/drivers" -I"I:/Software_Install/SiliconLabs/SimplicityStudio/v5/developer/sdks/gecko_sdk_suite/v3.2//platform/Device/SiliconLabs/EFR32BG13P/Include" -O0 -Wall -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -c -fmessage-length=0 -MMD -MP -MF"emlib/em_system.d" -MT"emlib/em_system.o" -o "$@" "$<" && "I:/Software_Install/SiliconLabs/SimplicityStudio/v5/configuration/org.eclipse.osgi/446/0/.cp/scripts/path_fixup.sh" "emlib/em_system.d"
	@echo 'Finished building: $<'
	@echo ' '


