################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../gecko_sdk_3.2.1/app/bluetooth/common/ota_dfu/sl_ota_dfu.c 

OBJS += \
./gecko_sdk_3.2.1/app/bluetooth/common/ota_dfu/sl_ota_dfu.o 

C_DEPS += \
./gecko_sdk_3.2.1/app/bluetooth/common/ota_dfu/sl_ota_dfu.d 


# Each subdirectory must supply rules for building sources it contributes
gecko_sdk_3.2.1/app/bluetooth/common/ota_dfu/sl_ota_dfu.o: ../gecko_sdk_3.2.1/app/bluetooth/common/ota_dfu/sl_ota_dfu.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g3 -gdwarf-2 -mcpu=cortex-m4 -mthumb -std=c99 '-DEFR32BG13P632F512GM48=1' '-DSL_COMPONENT_CATALOG_PRESENT=1' '-DMBEDTLS_CONFIG_FILE=<mbedtls_config.h>' '-DMBEDTLS_PSA_CRYPTO_CLIENT=1' '-DMBEDTLS_PSA_CRYPTO_CONFIG_FILE=<psa_crypto_config.h>' '-DSL_RAIL_LIB_MULTIPROTOCOL_SUPPORT=0' '-DSL_RAIL_UTIL_PA_CONFIG_HEADER=<sl_rail_util_pa_config.h>' -I"D:\Courses\IOT\Project Guidance\Project\ECEN5823_Project_Health_tracker\Health_tracker" -I"D:\Courses\IOT\Project Guidance\Project\ECEN5823_Project_Health_tracker\Health_tracker\gecko_sdk_3.2.1\platform\Device\SiliconLabs\EFR32BG13P\Include" -I"D:\Courses\IOT\Project Guidance\Project\ECEN5823_Project_Health_tracker\Health_tracker\gecko_sdk_3.2.1\app\common\util\app_assert" -I"D:\Courses\IOT\Project Guidance\Project\ECEN5823_Project_Health_tracker\Health_tracker\gecko_sdk_3.2.1\app\common\util\app_log" -I"D:\Courses\IOT\Project Guidance\Project\ECEN5823_Project_Health_tracker\Health_tracker\gecko_sdk_3.2.1\platform\common\inc" -I"D:\Courses\IOT\Project Guidance\Project\ECEN5823_Project_Health_tracker\Health_tracker\gecko_sdk_3.2.1\protocol\bluetooth\inc" -I"D:\Courses\IOT\Project Guidance\Project\ECEN5823_Project_Health_tracker\Health_tracker\gecko_sdk_3.2.1\hardware\board\inc" -I"D:\Courses\IOT\Project Guidance\Project\ECEN5823_Project_Health_tracker\Health_tracker\gecko_sdk_3.2.1\platform\bootloader" -I"D:\Courses\IOT\Project Guidance\Project\ECEN5823_Project_Health_tracker\Health_tracker\gecko_sdk_3.2.1\platform\bootloader\api" -I"D:\Courses\IOT\Project Guidance\Project\ECEN5823_Project_Health_tracker\Health_tracker\gecko_sdk_3.2.1\platform\CMSIS\Include" -I"D:\Courses\IOT\Project Guidance\Project\ECEN5823_Project_Health_tracker\Health_tracker\gecko_sdk_3.2.1\platform\service\device_init\inc" -I"D:\Courses\IOT\Project Guidance\Project\ECEN5823_Project_Health_tracker\Health_tracker\gecko_sdk_3.2.1\platform\middleware\glib\dmd" -I"D:\Courses\IOT\Project Guidance\Project\ECEN5823_Project_Health_tracker\Health_tracker\gecko_sdk_3.2.1\platform\middleware\glib" -I"D:\Courses\IOT\Project Guidance\Project\ECEN5823_Project_Health_tracker\Health_tracker\gecko_sdk_3.2.1\platform\emlib\inc" -I"D:\Courses\IOT\Project Guidance\Project\ECEN5823_Project_Health_tracker\Health_tracker\gecko_sdk_3.2.1\platform\middleware\glib\glib" -I"D:\Courses\IOT\Project Guidance\Project\ECEN5823_Project_Health_tracker\Health_tracker\gecko_sdk_3.2.1\platform\driver\i2cspm\inc" -I"D:\Courses\IOT\Project Guidance\Project\ECEN5823_Project_Health_tracker\Health_tracker\gecko_sdk_3.2.1\platform\service\iostream\inc" -I"D:\Courses\IOT\Project Guidance\Project\ECEN5823_Project_Health_tracker\Health_tracker\gecko_sdk_3.2.1\hardware\driver\memlcd\src\ls013b7dh03" -I"D:\Courses\IOT\Project Guidance\Project\ECEN5823_Project_Health_tracker\Health_tracker\gecko_sdk_3.2.1\util\third_party\crypto\mbedtls\include" -I"D:\Courses\IOT\Project Guidance\Project\ECEN5823_Project_Health_tracker\Health_tracker\gecko_sdk_3.2.1\util\third_party\crypto\sl_component\sl_mbedtls_support\config" -I"D:\Courses\IOT\Project Guidance\Project\ECEN5823_Project_Health_tracker\Health_tracker\gecko_sdk_3.2.1\util\third_party\crypto\mbedtls\library" -I"D:\Courses\IOT\Project Guidance\Project\ECEN5823_Project_Health_tracker\Health_tracker\gecko_sdk_3.2.1\util\third_party\crypto\sl_component\sl_alt\include" -I"D:\Courses\IOT\Project Guidance\Project\ECEN5823_Project_Health_tracker\Health_tracker\gecko_sdk_3.2.1\util\third_party\crypto\sl_component\sl_mbedtls_support\inc" -I"D:\Courses\IOT\Project Guidance\Project\ECEN5823_Project_Health_tracker\Health_tracker\gecko_sdk_3.2.1\hardware\driver\memlcd\inc" -I"D:\Courses\IOT\Project Guidance\Project\ECEN5823_Project_Health_tracker\Health_tracker\gecko_sdk_3.2.1\hardware\driver\memlcd\inc\memlcd_usart" -I"D:\Courses\IOT\Project Guidance\Project\ECEN5823_Project_Health_tracker\Health_tracker\gecko_sdk_3.2.1\platform\service\mpu\inc" -I"D:\Courses\IOT\Project Guidance\Project\ECEN5823_Project_Health_tracker\Health_tracker\gecko_sdk_3.2.1\hardware\driver\mx25_flash_shutdown\inc\sl_mx25_flash_shutdown_usart" -I"D:\Courses\IOT\Project Guidance\Project\ECEN5823_Project_Health_tracker\Health_tracker\gecko_sdk_3.2.1\app\bluetooth\common\ota_dfu" -I"D:\Courses\IOT\Project Guidance\Project\ECEN5823_Project_Health_tracker\Health_tracker\gecko_sdk_3.2.1\platform\service\power_manager\inc" -I"D:\Courses\IOT\Project Guidance\Project\ECEN5823_Project_Health_tracker\Health_tracker\gecko_sdk_3.2.1\util\third_party\crypto\sl_component\sl_psa_driver\inc" -I"D:\Courses\IOT\Project Guidance\Project\ECEN5823_Project_Health_tracker\Health_tracker\gecko_sdk_3.2.1\platform\radio\rail_lib\common" -I"D:\Courses\IOT\Project Guidance\Project\ECEN5823_Project_Health_tracker\Health_tracker\gecko_sdk_3.2.1\platform\radio\rail_lib\protocol\ble" -I"D:\Courses\IOT\Project Guidance\Project\ECEN5823_Project_Health_tracker\Health_tracker\gecko_sdk_3.2.1\platform\radio\rail_lib\protocol\ieee802154" -I"D:\Courses\IOT\Project Guidance\Project\ECEN5823_Project_Health_tracker\Health_tracker\gecko_sdk_3.2.1\platform\radio\rail_lib\protocol\zwave" -I"D:\Courses\IOT\Project Guidance\Project\ECEN5823_Project_Health_tracker\Health_tracker\gecko_sdk_3.2.1\platform\radio\rail_lib\protocol\mfm" -I"D:\Courses\IOT\Project Guidance\Project\ECEN5823_Project_Health_tracker\Health_tracker\gecko_sdk_3.2.1\platform\radio\rail_lib\chip\efr32\efr32xg1x" -I"D:\Courses\IOT\Project Guidance\Project\ECEN5823_Project_Health_tracker\Health_tracker\gecko_sdk_3.2.1\platform\radio\rail_lib\plugin\pa-conversions" -I"D:\Courses\IOT\Project Guidance\Project\ECEN5823_Project_Health_tracker\Health_tracker\gecko_sdk_3.2.1\platform\radio\rail_lib\plugin\pa-conversions\efr32xg1x" -I"D:\Courses\IOT\Project Guidance\Project\ECEN5823_Project_Health_tracker\Health_tracker\gecko_sdk_3.2.1\platform\radio\rail_lib\plugin\rail_util_pti" -I"D:\Courses\IOT\Project Guidance\Project\ECEN5823_Project_Health_tracker\Health_tracker\gecko_sdk_3.2.1\util\silicon_labs\silabs_core\memory_manager" -I"D:\Courses\IOT\Project Guidance\Project\ECEN5823_Project_Health_tracker\Health_tracker\gecko_sdk_3.2.1\platform\common\toolchain\inc" -I"D:\Courses\IOT\Project Guidance\Project\ECEN5823_Project_Health_tracker\Health_tracker\gecko_sdk_3.2.1\platform\service\system\inc" -I"D:\Courses\IOT\Project Guidance\Project\ECEN5823_Project_Health_tracker\Health_tracker\gecko_sdk_3.2.1\platform\service\sleeptimer\inc" -I"D:\Courses\IOT\Project Guidance\Project\ECEN5823_Project_Health_tracker\Health_tracker\gecko_sdk_3.2.1\util\third_party\crypto\sl_component\sl_protocol_crypto\src" -I"D:\Courses\IOT\Project Guidance\Project\ECEN5823_Project_Health_tracker\Health_tracker\gecko_sdk_3.2.1\platform\service\udelay\inc" -I"D:\Courses\IOT\Project Guidance\Project\ECEN5823_Project_Health_tracker\Health_tracker\autogen" -I"D:\Courses\IOT\Project Guidance\Project\ECEN5823_Project_Health_tracker\Health_tracker\config" -O0 -Wall -Wextra -fno-builtin -fomit-frame-pointer -ffunction-sections -fdata-sections -imacrossl_gcc_preinclude.h -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -c -fmessage-length=0 -MMD -MP -MF"gecko_sdk_3.2.1/app/bluetooth/common/ota_dfu/sl_ota_dfu.d" -MT"gecko_sdk_3.2.1/app/bluetooth/common/ota_dfu/sl_ota_dfu.o" -o "$@" "$<" && "I:/Software_Install/SiliconLabs/SimplicityStudio/v5/configuration/org.eclipse.osgi/446/0/.cp/scripts/path_fixup.sh" "gecko_sdk_3.2.1/app/bluetooth/common/ota_dfu/sl_ota_dfu.d"
	@echo 'Finished building: $<'
	@echo ' '

