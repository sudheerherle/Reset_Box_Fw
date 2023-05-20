#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/RESET_UNIT_FW.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/RESET_UNIT_FW.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../Source/USB_STACK/usb_descriptors.c ../Source/USB_STACK/usb_device.c ../Source/USB_STACK/USB_DRV.c ../Source/USB_STACK/usb_function_cdc.c ../Source/USB_STACK/usb_hal_pic24.c ../Source/COMM_SMC.c ../Source/CPU_SM_MAIN.c ../Source/CRC16.c ../Source/CRC32.c ../Source/DRV_A2D.c ../Source/DRV_I2C.c ../Source/DRV_MEM.c ../Source/DRV_RTC.c ../Source/EEPROM.c ../Source/EVENTS.c ../Source/comm_host.c ../Source/command_proc.c ../Source/DRV_CNT.c ../Source/DRV_DI.c ../Source/DRV_LED.c ../Source/PRINT.c ../Source/DRV_GLCD_SPI.c ../Source/AUTO_DRV_CNT.c ../Source/COMM_GSM.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1669077864/usb_descriptors.o ${OBJECTDIR}/_ext/1669077864/usb_device.o ${OBJECTDIR}/_ext/1669077864/USB_DRV.o ${OBJECTDIR}/_ext/1669077864/usb_function_cdc.o ${OBJECTDIR}/_ext/1669077864/usb_hal_pic24.o ${OBJECTDIR}/_ext/1728301206/COMM_SMC.o ${OBJECTDIR}/_ext/1728301206/CPU_SM_MAIN.o ${OBJECTDIR}/_ext/1728301206/CRC16.o ${OBJECTDIR}/_ext/1728301206/CRC32.o ${OBJECTDIR}/_ext/1728301206/DRV_A2D.o ${OBJECTDIR}/_ext/1728301206/DRV_I2C.o ${OBJECTDIR}/_ext/1728301206/DRV_MEM.o ${OBJECTDIR}/_ext/1728301206/DRV_RTC.o ${OBJECTDIR}/_ext/1728301206/EEPROM.o ${OBJECTDIR}/_ext/1728301206/EVENTS.o ${OBJECTDIR}/_ext/1728301206/comm_host.o ${OBJECTDIR}/_ext/1728301206/command_proc.o ${OBJECTDIR}/_ext/1728301206/DRV_CNT.o ${OBJECTDIR}/_ext/1728301206/DRV_DI.o ${OBJECTDIR}/_ext/1728301206/DRV_LED.o ${OBJECTDIR}/_ext/1728301206/PRINT.o ${OBJECTDIR}/_ext/1728301206/DRV_GLCD_SPI.o ${OBJECTDIR}/_ext/1728301206/AUTO_DRV_CNT.o ${OBJECTDIR}/_ext/1728301206/COMM_GSM.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1669077864/usb_descriptors.o.d ${OBJECTDIR}/_ext/1669077864/usb_device.o.d ${OBJECTDIR}/_ext/1669077864/USB_DRV.o.d ${OBJECTDIR}/_ext/1669077864/usb_function_cdc.o.d ${OBJECTDIR}/_ext/1669077864/usb_hal_pic24.o.d ${OBJECTDIR}/_ext/1728301206/COMM_SMC.o.d ${OBJECTDIR}/_ext/1728301206/CPU_SM_MAIN.o.d ${OBJECTDIR}/_ext/1728301206/CRC16.o.d ${OBJECTDIR}/_ext/1728301206/CRC32.o.d ${OBJECTDIR}/_ext/1728301206/DRV_A2D.o.d ${OBJECTDIR}/_ext/1728301206/DRV_I2C.o.d ${OBJECTDIR}/_ext/1728301206/DRV_MEM.o.d ${OBJECTDIR}/_ext/1728301206/DRV_RTC.o.d ${OBJECTDIR}/_ext/1728301206/EEPROM.o.d ${OBJECTDIR}/_ext/1728301206/EVENTS.o.d ${OBJECTDIR}/_ext/1728301206/comm_host.o.d ${OBJECTDIR}/_ext/1728301206/command_proc.o.d ${OBJECTDIR}/_ext/1728301206/DRV_CNT.o.d ${OBJECTDIR}/_ext/1728301206/DRV_DI.o.d ${OBJECTDIR}/_ext/1728301206/DRV_LED.o.d ${OBJECTDIR}/_ext/1728301206/PRINT.o.d ${OBJECTDIR}/_ext/1728301206/DRV_GLCD_SPI.o.d ${OBJECTDIR}/_ext/1728301206/AUTO_DRV_CNT.o.d ${OBJECTDIR}/_ext/1728301206/COMM_GSM.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1669077864/usb_descriptors.o ${OBJECTDIR}/_ext/1669077864/usb_device.o ${OBJECTDIR}/_ext/1669077864/USB_DRV.o ${OBJECTDIR}/_ext/1669077864/usb_function_cdc.o ${OBJECTDIR}/_ext/1669077864/usb_hal_pic24.o ${OBJECTDIR}/_ext/1728301206/COMM_SMC.o ${OBJECTDIR}/_ext/1728301206/CPU_SM_MAIN.o ${OBJECTDIR}/_ext/1728301206/CRC16.o ${OBJECTDIR}/_ext/1728301206/CRC32.o ${OBJECTDIR}/_ext/1728301206/DRV_A2D.o ${OBJECTDIR}/_ext/1728301206/DRV_I2C.o ${OBJECTDIR}/_ext/1728301206/DRV_MEM.o ${OBJECTDIR}/_ext/1728301206/DRV_RTC.o ${OBJECTDIR}/_ext/1728301206/EEPROM.o ${OBJECTDIR}/_ext/1728301206/EVENTS.o ${OBJECTDIR}/_ext/1728301206/comm_host.o ${OBJECTDIR}/_ext/1728301206/command_proc.o ${OBJECTDIR}/_ext/1728301206/DRV_CNT.o ${OBJECTDIR}/_ext/1728301206/DRV_DI.o ${OBJECTDIR}/_ext/1728301206/DRV_LED.o ${OBJECTDIR}/_ext/1728301206/PRINT.o ${OBJECTDIR}/_ext/1728301206/DRV_GLCD_SPI.o ${OBJECTDIR}/_ext/1728301206/AUTO_DRV_CNT.o ${OBJECTDIR}/_ext/1728301206/COMM_GSM.o

# Source Files
SOURCEFILES=../Source/USB_STACK/usb_descriptors.c ../Source/USB_STACK/usb_device.c ../Source/USB_STACK/USB_DRV.c ../Source/USB_STACK/usb_function_cdc.c ../Source/USB_STACK/usb_hal_pic24.c ../Source/COMM_SMC.c ../Source/CPU_SM_MAIN.c ../Source/CRC16.c ../Source/CRC32.c ../Source/DRV_A2D.c ../Source/DRV_I2C.c ../Source/DRV_MEM.c ../Source/DRV_RTC.c ../Source/EEPROM.c ../Source/EVENTS.c ../Source/comm_host.c ../Source/command_proc.c ../Source/DRV_CNT.c ../Source/DRV_DI.c ../Source/DRV_LED.c ../Source/PRINT.c ../Source/DRV_GLCD_SPI.c ../Source/AUTO_DRV_CNT.c ../Source/COMM_GSM.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/RESET_UNIT_FW.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=24FJ256GB210
MP_LINKER_FILE_OPTION=,--script="..\Source\linker_script.gld"
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1669077864/usb_descriptors.o: ../Source/USB_STACK/usb_descriptors.c  .generated_files/flags/default/ecbc2f86cac6806cfca302a49d6097975a2fc3f4 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1669077864" 
	@${RM} ${OBJECTDIR}/_ext/1669077864/usb_descriptors.o.d 
	@${RM} ${OBJECTDIR}/_ext/1669077864/usb_descriptors.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/USB_STACK/usb_descriptors.c  -o ${OBJECTDIR}/_ext/1669077864/usb_descriptors.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1669077864/usb_descriptors.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1669077864/usb_device.o: ../Source/USB_STACK/usb_device.c  .generated_files/flags/default/b89d51a82d84e209068a937d766a5056c0de63cb .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1669077864" 
	@${RM} ${OBJECTDIR}/_ext/1669077864/usb_device.o.d 
	@${RM} ${OBJECTDIR}/_ext/1669077864/usb_device.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/USB_STACK/usb_device.c  -o ${OBJECTDIR}/_ext/1669077864/usb_device.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1669077864/usb_device.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1669077864/USB_DRV.o: ../Source/USB_STACK/USB_DRV.c  .generated_files/flags/default/9535d76b176086c203bf5a66901b80a1ae7adbb8 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1669077864" 
	@${RM} ${OBJECTDIR}/_ext/1669077864/USB_DRV.o.d 
	@${RM} ${OBJECTDIR}/_ext/1669077864/USB_DRV.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/USB_STACK/USB_DRV.c  -o ${OBJECTDIR}/_ext/1669077864/USB_DRV.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1669077864/USB_DRV.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1669077864/usb_function_cdc.o: ../Source/USB_STACK/usb_function_cdc.c  .generated_files/flags/default/aeeac2a52c5085060c684336bfc9c46549c607d1 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1669077864" 
	@${RM} ${OBJECTDIR}/_ext/1669077864/usb_function_cdc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1669077864/usb_function_cdc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/USB_STACK/usb_function_cdc.c  -o ${OBJECTDIR}/_ext/1669077864/usb_function_cdc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1669077864/usb_function_cdc.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1669077864/usb_hal_pic24.o: ../Source/USB_STACK/usb_hal_pic24.c  .generated_files/flags/default/ffb979cc591937b5a81c4028a8b9a322f6a2365d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1669077864" 
	@${RM} ${OBJECTDIR}/_ext/1669077864/usb_hal_pic24.o.d 
	@${RM} ${OBJECTDIR}/_ext/1669077864/usb_hal_pic24.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/USB_STACK/usb_hal_pic24.c  -o ${OBJECTDIR}/_ext/1669077864/usb_hal_pic24.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1669077864/usb_hal_pic24.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/COMM_SMC.o: ../Source/COMM_SMC.c  .generated_files/flags/default/a2e966ba13ff4d5e687b2bf83625f5bcb2513bfc .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/COMM_SMC.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/COMM_SMC.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/COMM_SMC.c  -o ${OBJECTDIR}/_ext/1728301206/COMM_SMC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/COMM_SMC.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/CPU_SM_MAIN.o: ../Source/CPU_SM_MAIN.c  .generated_files/flags/default/8b4ab24ef0d48badd8ec05a2a524b4df9be1e343 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/CPU_SM_MAIN.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/CPU_SM_MAIN.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/CPU_SM_MAIN.c  -o ${OBJECTDIR}/_ext/1728301206/CPU_SM_MAIN.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/CPU_SM_MAIN.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/CRC16.o: ../Source/CRC16.c  .generated_files/flags/default/daccc99a9db968f59e239358b6e055e01f06e606 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/CRC16.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/CRC16.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/CRC16.c  -o ${OBJECTDIR}/_ext/1728301206/CRC16.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/CRC16.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/CRC32.o: ../Source/CRC32.c  .generated_files/flags/default/120d53db93e4aa35b8889d1fa99a7accd5f0dff4 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/CRC32.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/CRC32.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/CRC32.c  -o ${OBJECTDIR}/_ext/1728301206/CRC32.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/CRC32.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/DRV_A2D.o: ../Source/DRV_A2D.c  .generated_files/flags/default/d79fb13042eb72ee6817b64fdc9cf6f48a63a3d8 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_A2D.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_A2D.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/DRV_A2D.c  -o ${OBJECTDIR}/_ext/1728301206/DRV_A2D.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/DRV_A2D.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/DRV_I2C.o: ../Source/DRV_I2C.c  .generated_files/flags/default/f21dbfd4d9f47d70b75327c9e920e715670f7cf7 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_I2C.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_I2C.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/DRV_I2C.c  -o ${OBJECTDIR}/_ext/1728301206/DRV_I2C.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/DRV_I2C.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/DRV_MEM.o: ../Source/DRV_MEM.c  .generated_files/flags/default/9110b564150aa238c7954f089f9ed249f27aeb4d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_MEM.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_MEM.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/DRV_MEM.c  -o ${OBJECTDIR}/_ext/1728301206/DRV_MEM.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/DRV_MEM.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/DRV_RTC.o: ../Source/DRV_RTC.c  .generated_files/flags/default/49165dc33d6dfa5915a57784b3c79218b5426024 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_RTC.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_RTC.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/DRV_RTC.c  -o ${OBJECTDIR}/_ext/1728301206/DRV_RTC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/DRV_RTC.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/EEPROM.o: ../Source/EEPROM.c  .generated_files/flags/default/cc2380b5b789604937a8f97beff4730d123f7d99 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/EEPROM.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/EEPROM.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/EEPROM.c  -o ${OBJECTDIR}/_ext/1728301206/EEPROM.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/EEPROM.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/EVENTS.o: ../Source/EVENTS.c  .generated_files/flags/default/369ffc77e082f07311965423a00403d42a6d9a4d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/EVENTS.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/EVENTS.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/EVENTS.c  -o ${OBJECTDIR}/_ext/1728301206/EVENTS.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/EVENTS.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/comm_host.o: ../Source/comm_host.c  .generated_files/flags/default/dc1898054a4e753b528c40b4655da10060be8b7a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/comm_host.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/comm_host.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/comm_host.c  -o ${OBJECTDIR}/_ext/1728301206/comm_host.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/comm_host.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/command_proc.o: ../Source/command_proc.c  .generated_files/flags/default/587b6d90e354480878ea5c370b1927fd0dd2e5f6 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/command_proc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/command_proc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/command_proc.c  -o ${OBJECTDIR}/_ext/1728301206/command_proc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/command_proc.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/DRV_CNT.o: ../Source/DRV_CNT.c  .generated_files/flags/default/a07be77317495fc75d4e2ea70981c9cbe1de978f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_CNT.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_CNT.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/DRV_CNT.c  -o ${OBJECTDIR}/_ext/1728301206/DRV_CNT.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/DRV_CNT.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/DRV_DI.o: ../Source/DRV_DI.c  .generated_files/flags/default/87013053f1e86af6f6102d09cb4b43b18470b87b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_DI.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_DI.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/DRV_DI.c  -o ${OBJECTDIR}/_ext/1728301206/DRV_DI.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/DRV_DI.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/DRV_LED.o: ../Source/DRV_LED.c  .generated_files/flags/default/e905858b7dee95fd4539ee7679b8cce2422bd476 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_LED.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_LED.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/DRV_LED.c  -o ${OBJECTDIR}/_ext/1728301206/DRV_LED.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/DRV_LED.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/PRINT.o: ../Source/PRINT.c  .generated_files/flags/default/ffa587d47518e9ceb2543a89a6b3e676f129c7d5 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/PRINT.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/PRINT.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/PRINT.c  -o ${OBJECTDIR}/_ext/1728301206/PRINT.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/PRINT.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/DRV_GLCD_SPI.o: ../Source/DRV_GLCD_SPI.c  .generated_files/flags/default/9db31cead2fe317cace8dbf4bc5e467aaef3ec37 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_GLCD_SPI.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_GLCD_SPI.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/DRV_GLCD_SPI.c  -o ${OBJECTDIR}/_ext/1728301206/DRV_GLCD_SPI.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/DRV_GLCD_SPI.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/AUTO_DRV_CNT.o: ../Source/AUTO_DRV_CNT.c  .generated_files/flags/default/e6906eeaeb9211ce559cfcc4d018c629c447e473 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/AUTO_DRV_CNT.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/AUTO_DRV_CNT.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/AUTO_DRV_CNT.c  -o ${OBJECTDIR}/_ext/1728301206/AUTO_DRV_CNT.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/AUTO_DRV_CNT.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/COMM_GSM.o: ../Source/COMM_GSM.c  .generated_files/flags/default/108c1dc0b3a23159b12424796a87d9d79412a70c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/COMM_GSM.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/COMM_GSM.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/COMM_GSM.c  -o ${OBJECTDIR}/_ext/1728301206/COMM_GSM.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/COMM_GSM.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
else
${OBJECTDIR}/_ext/1669077864/usb_descriptors.o: ../Source/USB_STACK/usb_descriptors.c  .generated_files/flags/default/8d29138d47d48e36cc600249dcd06e4c4c222150 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1669077864" 
	@${RM} ${OBJECTDIR}/_ext/1669077864/usb_descriptors.o.d 
	@${RM} ${OBJECTDIR}/_ext/1669077864/usb_descriptors.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/USB_STACK/usb_descriptors.c  -o ${OBJECTDIR}/_ext/1669077864/usb_descriptors.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1669077864/usb_descriptors.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1669077864/usb_device.o: ../Source/USB_STACK/usb_device.c  .generated_files/flags/default/a175bbacacd482a5b0b345d8a6d5114ddd83a5f8 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1669077864" 
	@${RM} ${OBJECTDIR}/_ext/1669077864/usb_device.o.d 
	@${RM} ${OBJECTDIR}/_ext/1669077864/usb_device.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/USB_STACK/usb_device.c  -o ${OBJECTDIR}/_ext/1669077864/usb_device.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1669077864/usb_device.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1669077864/USB_DRV.o: ../Source/USB_STACK/USB_DRV.c  .generated_files/flags/default/eac78c7df2fb40d485b29e299695963c6df2f6b1 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1669077864" 
	@${RM} ${OBJECTDIR}/_ext/1669077864/USB_DRV.o.d 
	@${RM} ${OBJECTDIR}/_ext/1669077864/USB_DRV.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/USB_STACK/USB_DRV.c  -o ${OBJECTDIR}/_ext/1669077864/USB_DRV.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1669077864/USB_DRV.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1669077864/usb_function_cdc.o: ../Source/USB_STACK/usb_function_cdc.c  .generated_files/flags/default/385fbf59b04fb1b9fa7d31c2ebad19ad4cea6f79 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1669077864" 
	@${RM} ${OBJECTDIR}/_ext/1669077864/usb_function_cdc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1669077864/usb_function_cdc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/USB_STACK/usb_function_cdc.c  -o ${OBJECTDIR}/_ext/1669077864/usb_function_cdc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1669077864/usb_function_cdc.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1669077864/usb_hal_pic24.o: ../Source/USB_STACK/usb_hal_pic24.c  .generated_files/flags/default/e924aaaf00b18a91e0d76d11ede846e0bdd051bc .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1669077864" 
	@${RM} ${OBJECTDIR}/_ext/1669077864/usb_hal_pic24.o.d 
	@${RM} ${OBJECTDIR}/_ext/1669077864/usb_hal_pic24.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/USB_STACK/usb_hal_pic24.c  -o ${OBJECTDIR}/_ext/1669077864/usb_hal_pic24.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1669077864/usb_hal_pic24.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/COMM_SMC.o: ../Source/COMM_SMC.c  .generated_files/flags/default/5631b2b834d7a7e5c3a3d20a85e776325fd124c5 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/COMM_SMC.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/COMM_SMC.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/COMM_SMC.c  -o ${OBJECTDIR}/_ext/1728301206/COMM_SMC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/COMM_SMC.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/CPU_SM_MAIN.o: ../Source/CPU_SM_MAIN.c  .generated_files/flags/default/da83273d3ff6c6b72d71cef9aaa7e8265e98ef76 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/CPU_SM_MAIN.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/CPU_SM_MAIN.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/CPU_SM_MAIN.c  -o ${OBJECTDIR}/_ext/1728301206/CPU_SM_MAIN.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/CPU_SM_MAIN.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/CRC16.o: ../Source/CRC16.c  .generated_files/flags/default/7f7210fc8155ea748bfd24ff8c620066df2384b7 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/CRC16.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/CRC16.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/CRC16.c  -o ${OBJECTDIR}/_ext/1728301206/CRC16.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/CRC16.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/CRC32.o: ../Source/CRC32.c  .generated_files/flags/default/9a36473d08d6cf8055b44f2dda324310114dd3b4 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/CRC32.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/CRC32.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/CRC32.c  -o ${OBJECTDIR}/_ext/1728301206/CRC32.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/CRC32.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/DRV_A2D.o: ../Source/DRV_A2D.c  .generated_files/flags/default/f9e4bbb9ced6dbaf81735dcc6bba599824bd62d5 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_A2D.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_A2D.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/DRV_A2D.c  -o ${OBJECTDIR}/_ext/1728301206/DRV_A2D.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/DRV_A2D.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/DRV_I2C.o: ../Source/DRV_I2C.c  .generated_files/flags/default/468048608abfe8b9d020cd789cb6fdb106cdb2f8 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_I2C.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_I2C.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/DRV_I2C.c  -o ${OBJECTDIR}/_ext/1728301206/DRV_I2C.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/DRV_I2C.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/DRV_MEM.o: ../Source/DRV_MEM.c  .generated_files/flags/default/4a4d68d09f8fd5075a1513be14dae2fd84e11b0f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_MEM.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_MEM.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/DRV_MEM.c  -o ${OBJECTDIR}/_ext/1728301206/DRV_MEM.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/DRV_MEM.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/DRV_RTC.o: ../Source/DRV_RTC.c  .generated_files/flags/default/f9b7ed0b41a27a5ad480471320407503d54d12e4 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_RTC.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_RTC.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/DRV_RTC.c  -o ${OBJECTDIR}/_ext/1728301206/DRV_RTC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/DRV_RTC.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/EEPROM.o: ../Source/EEPROM.c  .generated_files/flags/default/aa8d4eb88be36165fa251e2bb5395d4ae204279b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/EEPROM.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/EEPROM.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/EEPROM.c  -o ${OBJECTDIR}/_ext/1728301206/EEPROM.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/EEPROM.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/EVENTS.o: ../Source/EVENTS.c  .generated_files/flags/default/a50ca1a35d36477e268f3f56ed46fa418c89fbd .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/EVENTS.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/EVENTS.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/EVENTS.c  -o ${OBJECTDIR}/_ext/1728301206/EVENTS.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/EVENTS.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/comm_host.o: ../Source/comm_host.c  .generated_files/flags/default/5bd0c2ef4f5b7fe44427aa5fa29ac4e620765db5 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/comm_host.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/comm_host.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/comm_host.c  -o ${OBJECTDIR}/_ext/1728301206/comm_host.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/comm_host.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/command_proc.o: ../Source/command_proc.c  .generated_files/flags/default/d010240ba6f405de6e95e8f4138a88d7815cbb2b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/command_proc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/command_proc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/command_proc.c  -o ${OBJECTDIR}/_ext/1728301206/command_proc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/command_proc.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/DRV_CNT.o: ../Source/DRV_CNT.c  .generated_files/flags/default/2539f32c5221425c938b9100f618afd867fdefac .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_CNT.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_CNT.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/DRV_CNT.c  -o ${OBJECTDIR}/_ext/1728301206/DRV_CNT.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/DRV_CNT.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/DRV_DI.o: ../Source/DRV_DI.c  .generated_files/flags/default/b6cabc25d2d04f65a3d94b0cc3cc50022ca2aecc .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_DI.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_DI.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/DRV_DI.c  -o ${OBJECTDIR}/_ext/1728301206/DRV_DI.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/DRV_DI.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/DRV_LED.o: ../Source/DRV_LED.c  .generated_files/flags/default/bab8bf55191c757debc7bd4504348e54fe8aec53 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_LED.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_LED.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/DRV_LED.c  -o ${OBJECTDIR}/_ext/1728301206/DRV_LED.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/DRV_LED.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/PRINT.o: ../Source/PRINT.c  .generated_files/flags/default/14e50ca151348b692aaf041d8b671e75de7daa8a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/PRINT.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/PRINT.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/PRINT.c  -o ${OBJECTDIR}/_ext/1728301206/PRINT.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/PRINT.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/DRV_GLCD_SPI.o: ../Source/DRV_GLCD_SPI.c  .generated_files/flags/default/f89ca096b6ae678960dfed704dba1b606c4cd721 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_GLCD_SPI.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_GLCD_SPI.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/DRV_GLCD_SPI.c  -o ${OBJECTDIR}/_ext/1728301206/DRV_GLCD_SPI.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/DRV_GLCD_SPI.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/AUTO_DRV_CNT.o: ../Source/AUTO_DRV_CNT.c  .generated_files/flags/default/90996a54c405499d046e7dfa8ba6cda0ffbc3d5b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/AUTO_DRV_CNT.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/AUTO_DRV_CNT.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/AUTO_DRV_CNT.c  -o ${OBJECTDIR}/_ext/1728301206/AUTO_DRV_CNT.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/AUTO_DRV_CNT.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/COMM_GSM.o: ../Source/COMM_GSM.c  .generated_files/flags/default/60a7a23cab555ffb9ec2b98227b1c9de9f5aea92 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/COMM_GSM.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/COMM_GSM.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/COMM_GSM.c  -o ${OBJECTDIR}/_ext/1728301206/COMM_GSM.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/COMM_GSM.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/RESET_UNIT_FW.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    ../Source/linker_script.gld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/RESET_UNIT_FW.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG   -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  --fill=0xFFDEAD  -mreserve=data@0x800:0x81B -mreserve=data@0x81C:0x81D -mreserve=data@0x81E:0x81F -mreserve=data@0x820:0x821 -mreserve=data@0x822:0x823 -mreserve=data@0x824:0x827 -mreserve=data@0x82A:0x84F   -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--no-handles,--no-isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--cref,--memorysummary,${DISTDIR}/memoryfile.xml,--no-ivt$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	
else
${DISTDIR}/RESET_UNIT_FW.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   ../Source/linker_script.gld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/RESET_UNIT_FW.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  --fill=0xFFDEAD -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--no-handles,--no-isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--cref,--memorysummary,${DISTDIR}/memoryfile.xml,--no-ivt$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	${MP_CC_DIR}\\xc16-bin2hex ${DISTDIR}/RESET_UNIT_FW.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf   -mdfp="${DFP_DIR}/xc16" 
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(wildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
