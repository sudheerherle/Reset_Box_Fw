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
${OBJECTDIR}/_ext/1669077864/usb_descriptors.o: ../Source/USB_STACK/usb_descriptors.c  .generated_files/flags/default/7927980a36b0ea076d8d73baf69050ed2be7a4db .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1669077864" 
	@${RM} ${OBJECTDIR}/_ext/1669077864/usb_descriptors.o.d 
	@${RM} ${OBJECTDIR}/_ext/1669077864/usb_descriptors.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/USB_STACK/usb_descriptors.c  -o ${OBJECTDIR}/_ext/1669077864/usb_descriptors.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1669077864/usb_descriptors.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1669077864/usb_device.o: ../Source/USB_STACK/usb_device.c  .generated_files/flags/default/60c42f939b9f9f98609fc527350c05df2cc24909 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1669077864" 
	@${RM} ${OBJECTDIR}/_ext/1669077864/usb_device.o.d 
	@${RM} ${OBJECTDIR}/_ext/1669077864/usb_device.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/USB_STACK/usb_device.c  -o ${OBJECTDIR}/_ext/1669077864/usb_device.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1669077864/usb_device.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1669077864/USB_DRV.o: ../Source/USB_STACK/USB_DRV.c  .generated_files/flags/default/31c98c0cc38552f91fcf7850db8c9926aaefd13d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1669077864" 
	@${RM} ${OBJECTDIR}/_ext/1669077864/USB_DRV.o.d 
	@${RM} ${OBJECTDIR}/_ext/1669077864/USB_DRV.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/USB_STACK/USB_DRV.c  -o ${OBJECTDIR}/_ext/1669077864/USB_DRV.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1669077864/USB_DRV.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1669077864/usb_function_cdc.o: ../Source/USB_STACK/usb_function_cdc.c  .generated_files/flags/default/8b643f128cb8a5b6a0be9ffb18542da5c901711 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1669077864" 
	@${RM} ${OBJECTDIR}/_ext/1669077864/usb_function_cdc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1669077864/usb_function_cdc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/USB_STACK/usb_function_cdc.c  -o ${OBJECTDIR}/_ext/1669077864/usb_function_cdc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1669077864/usb_function_cdc.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1669077864/usb_hal_pic24.o: ../Source/USB_STACK/usb_hal_pic24.c  .generated_files/flags/default/e193ecae59d82870442e90486923ae0f313d7ca .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1669077864" 
	@${RM} ${OBJECTDIR}/_ext/1669077864/usb_hal_pic24.o.d 
	@${RM} ${OBJECTDIR}/_ext/1669077864/usb_hal_pic24.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/USB_STACK/usb_hal_pic24.c  -o ${OBJECTDIR}/_ext/1669077864/usb_hal_pic24.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1669077864/usb_hal_pic24.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1728301206/COMM_SMC.o: ../Source/COMM_SMC.c  .generated_files/flags/default/5274230d519ecd2d2726a2dbc1dcd83275ad81be .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/COMM_SMC.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/COMM_SMC.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/COMM_SMC.c  -o ${OBJECTDIR}/_ext/1728301206/COMM_SMC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/COMM_SMC.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1728301206/CPU_SM_MAIN.o: ../Source/CPU_SM_MAIN.c  .generated_files/flags/default/f3c98fcc80d3d96b0d7299748da31a6f78803208 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/CPU_SM_MAIN.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/CPU_SM_MAIN.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/CPU_SM_MAIN.c  -o ${OBJECTDIR}/_ext/1728301206/CPU_SM_MAIN.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/CPU_SM_MAIN.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1728301206/CRC16.o: ../Source/CRC16.c  .generated_files/flags/default/64f35bcb3d0d4ff45ecfd86a7981381917da430 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/CRC16.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/CRC16.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/CRC16.c  -o ${OBJECTDIR}/_ext/1728301206/CRC16.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/CRC16.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1728301206/CRC32.o: ../Source/CRC32.c  .generated_files/flags/default/a561359195c5fadd4d738e27b08748c1f7eaee98 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/CRC32.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/CRC32.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/CRC32.c  -o ${OBJECTDIR}/_ext/1728301206/CRC32.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/CRC32.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1728301206/DRV_A2D.o: ../Source/DRV_A2D.c  .generated_files/flags/default/f8cdb7ce5273b60a2377637c5fd0a463b0664c5d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_A2D.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_A2D.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/DRV_A2D.c  -o ${OBJECTDIR}/_ext/1728301206/DRV_A2D.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/DRV_A2D.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1728301206/DRV_I2C.o: ../Source/DRV_I2C.c  .generated_files/flags/default/fe11bad08c1c9275008fc9bc49d55defc4b4ab3b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_I2C.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_I2C.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/DRV_I2C.c  -o ${OBJECTDIR}/_ext/1728301206/DRV_I2C.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/DRV_I2C.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1728301206/DRV_MEM.o: ../Source/DRV_MEM.c  .generated_files/flags/default/11d25390da75a9515e309c8ad891c6b71def254d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_MEM.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_MEM.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/DRV_MEM.c  -o ${OBJECTDIR}/_ext/1728301206/DRV_MEM.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/DRV_MEM.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1728301206/DRV_RTC.o: ../Source/DRV_RTC.c  .generated_files/flags/default/f441cd3307f830848fe28bcf3ec7c7e388e1fcc .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_RTC.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_RTC.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/DRV_RTC.c  -o ${OBJECTDIR}/_ext/1728301206/DRV_RTC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/DRV_RTC.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1728301206/EEPROM.o: ../Source/EEPROM.c  .generated_files/flags/default/2e626479751a0927946d4bcc3cfe67ee6abfdd30 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/EEPROM.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/EEPROM.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/EEPROM.c  -o ${OBJECTDIR}/_ext/1728301206/EEPROM.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/EEPROM.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1728301206/EVENTS.o: ../Source/EVENTS.c  .generated_files/flags/default/ab0de133479e5b0f343501a57b2570810686b1d3 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/EVENTS.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/EVENTS.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/EVENTS.c  -o ${OBJECTDIR}/_ext/1728301206/EVENTS.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/EVENTS.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1728301206/comm_host.o: ../Source/comm_host.c  .generated_files/flags/default/5c5c5e34654050ce601743efa36f0a36691f742f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/comm_host.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/comm_host.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/comm_host.c  -o ${OBJECTDIR}/_ext/1728301206/comm_host.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/comm_host.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1728301206/command_proc.o: ../Source/command_proc.c  .generated_files/flags/default/4adf673eb7b821e632c5b1a7e98782e3a3b7b096 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/command_proc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/command_proc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/command_proc.c  -o ${OBJECTDIR}/_ext/1728301206/command_proc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/command_proc.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1728301206/DRV_CNT.o: ../Source/DRV_CNT.c  .generated_files/flags/default/72189668ee0c61b6d8a98aebc121a1e13cf77cf2 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_CNT.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_CNT.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/DRV_CNT.c  -o ${OBJECTDIR}/_ext/1728301206/DRV_CNT.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/DRV_CNT.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1728301206/DRV_DI.o: ../Source/DRV_DI.c  .generated_files/flags/default/db657a61fd0cf6cd56d14ffd2d98b97dd94006b6 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_DI.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_DI.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/DRV_DI.c  -o ${OBJECTDIR}/_ext/1728301206/DRV_DI.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/DRV_DI.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1728301206/DRV_LED.o: ../Source/DRV_LED.c  .generated_files/flags/default/c9a794ba397add51289a01d158d7d5f1b65e95b9 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_LED.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_LED.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/DRV_LED.c  -o ${OBJECTDIR}/_ext/1728301206/DRV_LED.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/DRV_LED.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1728301206/PRINT.o: ../Source/PRINT.c  .generated_files/flags/default/6d8bc8a74eb59e5be6ac73aa8d4e0abccd27bf26 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/PRINT.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/PRINT.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/PRINT.c  -o ${OBJECTDIR}/_ext/1728301206/PRINT.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/PRINT.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1728301206/DRV_GLCD_SPI.o: ../Source/DRV_GLCD_SPI.c  .generated_files/flags/default/84897cf76c680b0716c7500a34ab07c72ae4e322 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_GLCD_SPI.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_GLCD_SPI.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/DRV_GLCD_SPI.c  -o ${OBJECTDIR}/_ext/1728301206/DRV_GLCD_SPI.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/DRV_GLCD_SPI.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1728301206/AUTO_DRV_CNT.o: ../Source/AUTO_DRV_CNT.c  .generated_files/flags/default/1995f0b9f480319b7e12b8047d572343c41a8f81 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/AUTO_DRV_CNT.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/AUTO_DRV_CNT.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/AUTO_DRV_CNT.c  -o ${OBJECTDIR}/_ext/1728301206/AUTO_DRV_CNT.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/AUTO_DRV_CNT.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1728301206/COMM_GSM.o: ../Source/COMM_GSM.c  .generated_files/flags/default/fb4c4910352523dac01a2db6f0dab07697d26366 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/COMM_GSM.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/COMM_GSM.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/COMM_GSM.c  -o ${OBJECTDIR}/_ext/1728301206/COMM_GSM.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/COMM_GSM.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off   
	
else
${OBJECTDIR}/_ext/1669077864/usb_descriptors.o: ../Source/USB_STACK/usb_descriptors.c  .generated_files/flags/default/ca2b40547b8f08dc0e18d8d37477db12205c1b98 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1669077864" 
	@${RM} ${OBJECTDIR}/_ext/1669077864/usb_descriptors.o.d 
	@${RM} ${OBJECTDIR}/_ext/1669077864/usb_descriptors.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/USB_STACK/usb_descriptors.c  -o ${OBJECTDIR}/_ext/1669077864/usb_descriptors.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1669077864/usb_descriptors.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1669077864/usb_device.o: ../Source/USB_STACK/usb_device.c  .generated_files/flags/default/3e971dfec8296fdfc2ee513c6dc0c90966015bbe .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1669077864" 
	@${RM} ${OBJECTDIR}/_ext/1669077864/usb_device.o.d 
	@${RM} ${OBJECTDIR}/_ext/1669077864/usb_device.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/USB_STACK/usb_device.c  -o ${OBJECTDIR}/_ext/1669077864/usb_device.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1669077864/usb_device.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1669077864/USB_DRV.o: ../Source/USB_STACK/USB_DRV.c  .generated_files/flags/default/f0f37ed34a2a3b1bb2ec4f796c96a510aa0d0bf1 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1669077864" 
	@${RM} ${OBJECTDIR}/_ext/1669077864/USB_DRV.o.d 
	@${RM} ${OBJECTDIR}/_ext/1669077864/USB_DRV.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/USB_STACK/USB_DRV.c  -o ${OBJECTDIR}/_ext/1669077864/USB_DRV.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1669077864/USB_DRV.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1669077864/usb_function_cdc.o: ../Source/USB_STACK/usb_function_cdc.c  .generated_files/flags/default/d945e7e522ca598d53c82947981e1bc42ad18072 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1669077864" 
	@${RM} ${OBJECTDIR}/_ext/1669077864/usb_function_cdc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1669077864/usb_function_cdc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/USB_STACK/usb_function_cdc.c  -o ${OBJECTDIR}/_ext/1669077864/usb_function_cdc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1669077864/usb_function_cdc.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1669077864/usb_hal_pic24.o: ../Source/USB_STACK/usb_hal_pic24.c  .generated_files/flags/default/4ac37df4781ee2f373dfba1f58d8732922d55e94 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1669077864" 
	@${RM} ${OBJECTDIR}/_ext/1669077864/usb_hal_pic24.o.d 
	@${RM} ${OBJECTDIR}/_ext/1669077864/usb_hal_pic24.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/USB_STACK/usb_hal_pic24.c  -o ${OBJECTDIR}/_ext/1669077864/usb_hal_pic24.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1669077864/usb_hal_pic24.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1728301206/COMM_SMC.o: ../Source/COMM_SMC.c  .generated_files/flags/default/eb8404796db1d9bfa836176211aaf285f90716a3 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/COMM_SMC.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/COMM_SMC.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/COMM_SMC.c  -o ${OBJECTDIR}/_ext/1728301206/COMM_SMC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/COMM_SMC.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1728301206/CPU_SM_MAIN.o: ../Source/CPU_SM_MAIN.c  .generated_files/flags/default/9335feb002271f6cf5a3e4311e3bdccfc3cdc4f5 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/CPU_SM_MAIN.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/CPU_SM_MAIN.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/CPU_SM_MAIN.c  -o ${OBJECTDIR}/_ext/1728301206/CPU_SM_MAIN.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/CPU_SM_MAIN.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1728301206/CRC16.o: ../Source/CRC16.c  .generated_files/flags/default/31ff7396246da2d7e6b8a44fe571bcd6997d4ad5 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/CRC16.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/CRC16.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/CRC16.c  -o ${OBJECTDIR}/_ext/1728301206/CRC16.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/CRC16.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1728301206/CRC32.o: ../Source/CRC32.c  .generated_files/flags/default/9895188aa0ae5f10dd09921514e108ff39179773 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/CRC32.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/CRC32.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/CRC32.c  -o ${OBJECTDIR}/_ext/1728301206/CRC32.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/CRC32.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1728301206/DRV_A2D.o: ../Source/DRV_A2D.c  .generated_files/flags/default/95bdff55862fa06683e06415027be09ca98273f1 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_A2D.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_A2D.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/DRV_A2D.c  -o ${OBJECTDIR}/_ext/1728301206/DRV_A2D.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/DRV_A2D.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1728301206/DRV_I2C.o: ../Source/DRV_I2C.c  .generated_files/flags/default/ae1072169481f764e0fc52d665f37660f6a04b0e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_I2C.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_I2C.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/DRV_I2C.c  -o ${OBJECTDIR}/_ext/1728301206/DRV_I2C.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/DRV_I2C.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1728301206/DRV_MEM.o: ../Source/DRV_MEM.c  .generated_files/flags/default/8c17ff93a5f6f62b7404096ce62354358af5055c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_MEM.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_MEM.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/DRV_MEM.c  -o ${OBJECTDIR}/_ext/1728301206/DRV_MEM.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/DRV_MEM.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1728301206/DRV_RTC.o: ../Source/DRV_RTC.c  .generated_files/flags/default/ce3e14f90c28285d9cf1a3c8f871fd9d2d938771 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_RTC.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_RTC.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/DRV_RTC.c  -o ${OBJECTDIR}/_ext/1728301206/DRV_RTC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/DRV_RTC.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1728301206/EEPROM.o: ../Source/EEPROM.c  .generated_files/flags/default/5ffaebc5c8f7465fddfffaa9ea0c357927690f14 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/EEPROM.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/EEPROM.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/EEPROM.c  -o ${OBJECTDIR}/_ext/1728301206/EEPROM.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/EEPROM.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1728301206/EVENTS.o: ../Source/EVENTS.c  .generated_files/flags/default/5402454aa8d8e02d81e5994b5e8bc06c3aeaf084 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/EVENTS.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/EVENTS.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/EVENTS.c  -o ${OBJECTDIR}/_ext/1728301206/EVENTS.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/EVENTS.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1728301206/comm_host.o: ../Source/comm_host.c  .generated_files/flags/default/e8f80e4a3c03588c4202e768f3af66cbeade9134 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/comm_host.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/comm_host.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/comm_host.c  -o ${OBJECTDIR}/_ext/1728301206/comm_host.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/comm_host.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1728301206/command_proc.o: ../Source/command_proc.c  .generated_files/flags/default/3c25a3f30a56ffba62a0471e501e07e467b3b517 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/command_proc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/command_proc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/command_proc.c  -o ${OBJECTDIR}/_ext/1728301206/command_proc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/command_proc.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1728301206/DRV_CNT.o: ../Source/DRV_CNT.c  .generated_files/flags/default/241281115a4e0762a5542a15099fce0389037b88 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_CNT.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_CNT.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/DRV_CNT.c  -o ${OBJECTDIR}/_ext/1728301206/DRV_CNT.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/DRV_CNT.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1728301206/DRV_DI.o: ../Source/DRV_DI.c  .generated_files/flags/default/eb285fa512bf063439225d10e2407cf5ed12f7e7 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_DI.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_DI.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/DRV_DI.c  -o ${OBJECTDIR}/_ext/1728301206/DRV_DI.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/DRV_DI.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1728301206/DRV_LED.o: ../Source/DRV_LED.c  .generated_files/flags/default/296511479539abd1c4bf556ff8164bd0df1d6389 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_LED.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_LED.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/DRV_LED.c  -o ${OBJECTDIR}/_ext/1728301206/DRV_LED.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/DRV_LED.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1728301206/PRINT.o: ../Source/PRINT.c  .generated_files/flags/default/97b01053258ac802d9c00dddc295642038b8767a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/PRINT.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/PRINT.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/PRINT.c  -o ${OBJECTDIR}/_ext/1728301206/PRINT.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/PRINT.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1728301206/DRV_GLCD_SPI.o: ../Source/DRV_GLCD_SPI.c  .generated_files/flags/default/109d79d5b79c84f2553195fd0057d3870a065e32 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_GLCD_SPI.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_GLCD_SPI.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/DRV_GLCD_SPI.c  -o ${OBJECTDIR}/_ext/1728301206/DRV_GLCD_SPI.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/DRV_GLCD_SPI.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1728301206/AUTO_DRV_CNT.o: ../Source/AUTO_DRV_CNT.c  .generated_files/flags/default/9a8d0fae612cc4b89e3d6592504da82307659a9c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/AUTO_DRV_CNT.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/AUTO_DRV_CNT.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/AUTO_DRV_CNT.c  -o ${OBJECTDIR}/_ext/1728301206/AUTO_DRV_CNT.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/AUTO_DRV_CNT.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off   
	
${OBJECTDIR}/_ext/1728301206/COMM_GSM.o: ../Source/COMM_GSM.c  .generated_files/flags/default/ad7fa765c8528ddc5c61edeb4972ed4800a006b4 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/COMM_GSM.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/COMM_GSM.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/COMM_GSM.c  -o ${OBJECTDIR}/_ext/1728301206/COMM_GSM.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/COMM_GSM.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off   
	
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
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/RESET_UNIT_FW.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  --fill=0xFFDEAD  -mreserve=data@0x800:0x81B -mreserve=data@0x81C:0x81D -mreserve=data@0x81E:0x81F -mreserve=data@0x820:0x821 -mreserve=data@0x822:0x823 -mreserve=data@0x824:0x827 -mreserve=data@0x82A:0x84F   -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,--defsym=__MPLAB_DEBUGGER_ICD3=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--no-handles,--no-isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--cref,--memorysummary,${DISTDIR}/memoryfile.xml,--no-ivt$(MP_EXTRA_LD_POST)  
	
else
${DISTDIR}/RESET_UNIT_FW.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   ../Source/linker_script.gld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/RESET_UNIT_FW.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  --fill=0xFFDEAD -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--no-handles,--no-isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--cref,--memorysummary,${DISTDIR}/memoryfile.xml,--no-ivt$(MP_EXTRA_LD_POST)  
	${MP_CC_DIR}\\xc16-bin2hex ${DISTDIR}/RESET_UNIT_FW.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf   
	
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

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
