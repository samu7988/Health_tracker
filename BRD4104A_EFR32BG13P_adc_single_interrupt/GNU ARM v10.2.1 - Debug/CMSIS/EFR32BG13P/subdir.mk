################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
I:/Software_Install/SiliconLabs/SimplicityStudio/v5/developer/sdks/gecko_sdk_suite/v3.2/platform/Device/SiliconLabs/EFR32BG13P/Source/system_efr32bg13p.c 

S_UPPER_SRCS += \
I:/Software_Install/SiliconLabs/SimplicityStudio/v5/developer/sdks/gecko_sdk_suite/v3.2/platform/Device/SiliconLabs/EFR32BG13P/Source/GCC/startup_efr32bg13p.S 

OBJS += \
./CMSIS/EFR32BG13P/startup_efr32bg13p.o \
./CMSIS/EFR32BG13P/system_efr32bg13p.o 

C_DEPS += \
./CMSIS/EFR32BG13P/system_efr32bg13p.d 


# Each subdirectory must supply rules for building sources it contributes
CMSIS/EFR32BG13P/startup_efr32bg13p.o: I:/Software_Install/SiliconLabs/SimplicityStudio/v5/developer/sdks/gecko_sdk_suite/v3.2/platform/Device/SiliconLabs/EFR32BG13P/Source/GCC/startup_efr32bg13p.S
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Assembler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -II:/Software_Install/SiliconLabs/SimplicityStudio/v5/developer/repos/hardware/kit/EFR32BG13_BRD4104A/config -I"I:/Software_Install/SiliconLabs/SimplicityStudio/v5/developer/sdks/gecko_sdk_suite/v3.2//hardware/kit/EFR32BG13_BRD4104A/config" -I"I:/Software_Install/SiliconLabs/SimplicityStudio/v5/developer/sdks/gecko_sdk_suite/v3.2//platform/CMSIS/Include" -I"I:/Software_Install/SiliconLabs/SimplicityStudio/v5/developer/sdks/gecko_sdk_suite/v3.2//platform/emlib/inc" -I"I:/Software_Install/SiliconLabs/SimplicityStudio/v5/developer/sdks/gecko_sdk_suite/v3.2//hardware/kit/common/bsp" -I"I:/Software_Install/SiliconLabs/SimplicityStudio/v5/developer/sdks/gecko_sdk_suite/v3.2//hardware/kit/common/drivers" -I"I:/Software_Install/SiliconLabs/SimplicityStudio/v5/developer/sdks/gecko_sdk_suite/v3.2//platform/Device/SiliconLabs/EFR32BG13P/Include" '-DEFR32BG13P632F512GM48=1' -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -c -x assembler-with-cpp -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

CMSIS/EFR32BG13P/system_efr32bg13p.o: I:/Software_Install/SiliconLabs/SimplicityStudio/v5/developer/sdks/gecko_sdk_suite/v3.2/platform/Device/SiliconLabs/EFR32BG13P/Source/system_efr32bg13p.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -std=c99 '-DEFR32BG13P632F512GM48=1' '-DDEBUG=1' -II:/Software_Install/SiliconLabs/SimplicityStudio/v5/developer/repos/hardware/kit/EFR32BG13_BRD4104A/config -I"I:/Software_Install/SiliconLabs/SimplicityStudio/v5/developer/sdks/gecko_sdk_suite/v3.2//hardware/kit/EFR32BG13_BRD4104A/config" -I"I:/Software_Install/SiliconLabs/SimplicityStudio/v5/developer/sdks/gecko_sdk_suite/v3.2//platform/CMSIS/Include" -I"I:/Software_Install/SiliconLabs/SimplicityStudio/v5/developer/sdks/gecko_sdk_suite/v3.2//platform/emlib/inc" -I"I:/Software_Install/SiliconLabs/SimplicityStudio/v5/developer/sdks/gecko_sdk_suite/v3.2//hardware/kit/common/bsp" -I"I:/Software_Install/SiliconLabs/SimplicityStudio/v5/developer/sdks/gecko_sdk_suite/v3.2//hardware/kit/common/drivers" -I"I:/Software_Install/SiliconLabs/SimplicityStudio/v5/developer/sdks/gecko_sdk_suite/v3.2//platform/Device/SiliconLabs/EFR32BG13P/Include" -O0 -Wall -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -c -fmessage-length=0 -MMD -MP -MF"CMSIS/EFR32BG13P/system_efr32bg13p.d" -MT"CMSIS/EFR32BG13P/system_efr32bg13p.o" -o "$@" "$<" && "I:/Software_Install/SiliconLabs/SimplicityStudio/v5/configuration/org.eclipse.osgi/446/0/.cp/scripts/path_fixup.sh" "CMSIS/EFR32BG13P/system_efr32bg13p.d"
	@echo 'Finished building: $<'
	@echo ' '


