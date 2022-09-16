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
MKDIR=mkdir -p
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

ifdef SUB_IMAGE_ADDRESS
SUB_IMAGE_ADDRESS_COMMAND=--image-address $(SUB_IMAGE_ADDRESS)
else
SUB_IMAGE_ADDRESS_COMMAND=
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
MP_LINKER_FILE_OPTION=,--script="../Source/linker_script.gld"
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1669077864/usb_descriptors.o: ../Source/USB_STACK/usb_descriptors.c  .generated_files/flags/default/2cf6143d7ed5c023741cd4fd8fa040fd05087b89 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1669077864" 
	@${RM} ${OBJECTDIR}/_ext/1669077864/usb_descriptors.o.d 
	@${RM} ${OBJECTDIR}/_ext/1669077864/usb_descriptors.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/USB_STACK/usb_descriptors.c  -o ${OBJECTDIR}/_ext/1669077864/usb_descriptors.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1669077864/usb_descriptors.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1669077864/usb_device.o: ../Source/USB_STACK/usb_device.c  .generated_files/flags/default/27b31a5ae1db13b9bb07374c9d1719ff86826e8d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1669077864" 
	@${RM} ${OBJECTDIR}/_ext/1669077864/usb_device.o.d 
	@${RM} ${OBJECTDIR}/_ext/1669077864/usb_device.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/USB_STACK/usb_device.c  -o ${OBJECTDIR}/_ext/1669077864/usb_device.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1669077864/usb_device.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1669077864/USB_DRV.o: ../Source/USB_STACK/USB_DRV.c  .generated_files/flags/default/506292c6e999926915fa9045507915700e9d053d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1669077864" 
	@${RM} ${OBJECTDIR}/_ext/1669077864/USB_DRV.o.d 
	@${RM} ${OBJECTDIR}/_ext/1669077864/USB_DRV.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/USB_STACK/USB_DRV.c  -o ${OBJECTDIR}/_ext/1669077864/USB_DRV.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1669077864/USB_DRV.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1669077864/usb_function_cdc.o: ../Source/USB_STACK/usb_function_cdc.c  .generated_files/flags/default/c0b0d800af0386b54fbb4ead400f6da5e723143f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1669077864" 
	@${RM} ${OBJECTDIR}/_ext/1669077864/usb_function_cdc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1669077864/usb_function_cdc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/USB_STACK/usb_function_cdc.c  -o ${OBJECTDIR}/_ext/1669077864/usb_function_cdc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1669077864/usb_function_cdc.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1669077864/usb_hal_pic24.o: ../Source/USB_STACK/usb_hal_pic24.c  .generated_files/flags/default/d10297a533473e7934320a58f05f164a3b6e041 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1669077864" 
	@${RM} ${OBJECTDIR}/_ext/1669077864/usb_hal_pic24.o.d 
	@${RM} ${OBJECTDIR}/_ext/1669077864/usb_hal_pic24.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/USB_STACK/usb_hal_pic24.c  -o ${OBJECTDIR}/_ext/1669077864/usb_hal_pic24.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1669077864/usb_hal_pic24.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/COMM_SMC.o: ../Source/COMM_SMC.c  .generated_files/flags/default/c1256a0a7ff994c49e1c8e1531cbd002da56beb3 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/COMM_SMC.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/COMM_SMC.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/COMM_SMC.c  -o ${OBJECTDIR}/_ext/1728301206/COMM_SMC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/COMM_SMC.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/CPU_SM_MAIN.o: ../Source/CPU_SM_MAIN.c  .generated_files/flags/default/f9ffdd026b093b5844443bc2b967386ffac3d6bc .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/CPU_SM_MAIN.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/CPU_SM_MAIN.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/CPU_SM_MAIN.c  -o ${OBJECTDIR}/_ext/1728301206/CPU_SM_MAIN.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/CPU_SM_MAIN.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/CRC16.o: ../Source/CRC16.c  .generated_files/flags/default/67866e78acb95d35241abf6d0be93f56331866e8 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/CRC16.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/CRC16.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/CRC16.c  -o ${OBJECTDIR}/_ext/1728301206/CRC16.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/CRC16.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/CRC32.o: ../Source/CRC32.c  .generated_files/flags/default/c6d82f33be0b4fcb158d09d65dd95c2e0cdcd4e9 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/CRC32.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/CRC32.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/CRC32.c  -o ${OBJECTDIR}/_ext/1728301206/CRC32.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/CRC32.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/DRV_A2D.o: ../Source/DRV_A2D.c  .generated_files/flags/default/57b6909699aefc85026644783ac859be55bbfa1e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_A2D.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_A2D.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/DRV_A2D.c  -o ${OBJECTDIR}/_ext/1728301206/DRV_A2D.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/DRV_A2D.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/DRV_I2C.o: ../Source/DRV_I2C.c  .generated_files/flags/default/575920c383cb761b631c2e29d591ad9fe47d887c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_I2C.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_I2C.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/DRV_I2C.c  -o ${OBJECTDIR}/_ext/1728301206/DRV_I2C.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/DRV_I2C.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/DRV_MEM.o: ../Source/DRV_MEM.c  .generated_files/flags/default/1af7eb65a2739339b9b5bf22084b40084c34a1ee .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_MEM.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_MEM.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/DRV_MEM.c  -o ${OBJECTDIR}/_ext/1728301206/DRV_MEM.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/DRV_MEM.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/DRV_RTC.o: ../Source/DRV_RTC.c  .generated_files/flags/default/aa3e5043e6a2d41d69ce966bf6875628e759b088 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_RTC.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_RTC.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/DRV_RTC.c  -o ${OBJECTDIR}/_ext/1728301206/DRV_RTC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/DRV_RTC.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/EEPROM.o: ../Source/EEPROM.c  .generated_files/flags/default/707f745992f835744c04d9d5e9b50276985d74ed .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/EEPROM.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/EEPROM.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/EEPROM.c  -o ${OBJECTDIR}/_ext/1728301206/EEPROM.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/EEPROM.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/EVENTS.o: ../Source/EVENTS.c  .generated_files/flags/default/9c4b378bac29dd491b7660e83660fbbd53abdd2d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/EVENTS.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/EVENTS.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/EVENTS.c  -o ${OBJECTDIR}/_ext/1728301206/EVENTS.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/EVENTS.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/comm_host.o: ../Source/comm_host.c  .generated_files/flags/default/866a6c52c24c55ce9e6c99676dd9bee11beac6f7 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/comm_host.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/comm_host.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/comm_host.c  -o ${OBJECTDIR}/_ext/1728301206/comm_host.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/comm_host.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/command_proc.o: ../Source/command_proc.c  .generated_files/flags/default/757a8219f262e9a82899e5383cedd0fcae5bb11e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/command_proc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/command_proc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/command_proc.c  -o ${OBJECTDIR}/_ext/1728301206/command_proc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/command_proc.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/DRV_CNT.o: ../Source/DRV_CNT.c  .generated_files/flags/default/7dc0dcbfaf618e6c0a97332b9fb253f3bebe767f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_CNT.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_CNT.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/DRV_CNT.c  -o ${OBJECTDIR}/_ext/1728301206/DRV_CNT.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/DRV_CNT.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/DRV_DI.o: ../Source/DRV_DI.c  .generated_files/flags/default/a73cfe6fd29b06e6db4e2b0546c3b185c247fd02 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_DI.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_DI.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/DRV_DI.c  -o ${OBJECTDIR}/_ext/1728301206/DRV_DI.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/DRV_DI.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/DRV_LED.o: ../Source/DRV_LED.c  .generated_files/flags/default/aa0dd22a0dc00d1097e126b848cca0187d032da2 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_LED.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_LED.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/DRV_LED.c  -o ${OBJECTDIR}/_ext/1728301206/DRV_LED.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/DRV_LED.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/PRINT.o: ../Source/PRINT.c  .generated_files/flags/default/24fbd47309d3779f0db3e77d27913e57ead57406 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/PRINT.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/PRINT.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/PRINT.c  -o ${OBJECTDIR}/_ext/1728301206/PRINT.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/PRINT.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/DRV_GLCD_SPI.o: ../Source/DRV_GLCD_SPI.c  .generated_files/flags/default/fc103916d336f7b4b62a0d1abee4f5e52e560e5a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_GLCD_SPI.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_GLCD_SPI.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/DRV_GLCD_SPI.c  -o ${OBJECTDIR}/_ext/1728301206/DRV_GLCD_SPI.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/DRV_GLCD_SPI.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/AUTO_DRV_CNT.o: ../Source/AUTO_DRV_CNT.c  .generated_files/flags/default/1ddc1ae3e3ed547b9adfc2de3ea711aba891a78b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/AUTO_DRV_CNT.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/AUTO_DRV_CNT.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/AUTO_DRV_CNT.c  -o ${OBJECTDIR}/_ext/1728301206/AUTO_DRV_CNT.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/AUTO_DRV_CNT.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/COMM_GSM.o: ../Source/COMM_GSM.c  .generated_files/flags/default/f0f9c56dc1959c6727153c6ca2e38ca7ebf75781 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/COMM_GSM.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/COMM_GSM.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/COMM_GSM.c  -o ${OBJECTDIR}/_ext/1728301206/COMM_GSM.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/COMM_GSM.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
else
${OBJECTDIR}/_ext/1669077864/usb_descriptors.o: ../Source/USB_STACK/usb_descriptors.c  .generated_files/flags/default/dde41eb13d35ecc41fd1729f1f16298556fef3d5 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1669077864" 
	@${RM} ${OBJECTDIR}/_ext/1669077864/usb_descriptors.o.d 
	@${RM} ${OBJECTDIR}/_ext/1669077864/usb_descriptors.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/USB_STACK/usb_descriptors.c  -o ${OBJECTDIR}/_ext/1669077864/usb_descriptors.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1669077864/usb_descriptors.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1669077864/usb_device.o: ../Source/USB_STACK/usb_device.c  .generated_files/flags/default/f1811452aefa66237dd240351e7dd6fe8845b1e1 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1669077864" 
	@${RM} ${OBJECTDIR}/_ext/1669077864/usb_device.o.d 
	@${RM} ${OBJECTDIR}/_ext/1669077864/usb_device.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/USB_STACK/usb_device.c  -o ${OBJECTDIR}/_ext/1669077864/usb_device.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1669077864/usb_device.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1669077864/USB_DRV.o: ../Source/USB_STACK/USB_DRV.c  .generated_files/flags/default/229baadb18f9626c4f77ba8f5af4ba5a8ede369 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1669077864" 
	@${RM} ${OBJECTDIR}/_ext/1669077864/USB_DRV.o.d 
	@${RM} ${OBJECTDIR}/_ext/1669077864/USB_DRV.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/USB_STACK/USB_DRV.c  -o ${OBJECTDIR}/_ext/1669077864/USB_DRV.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1669077864/USB_DRV.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1669077864/usb_function_cdc.o: ../Source/USB_STACK/usb_function_cdc.c  .generated_files/flags/default/a653cb768e6bec7f0e2bb275f7813c93f2218b1f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1669077864" 
	@${RM} ${OBJECTDIR}/_ext/1669077864/usb_function_cdc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1669077864/usb_function_cdc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/USB_STACK/usb_function_cdc.c  -o ${OBJECTDIR}/_ext/1669077864/usb_function_cdc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1669077864/usb_function_cdc.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1669077864/usb_hal_pic24.o: ../Source/USB_STACK/usb_hal_pic24.c  .generated_files/flags/default/fd9778643a61437d20fd6f2bebfbfbaaa1e0551c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1669077864" 
	@${RM} ${OBJECTDIR}/_ext/1669077864/usb_hal_pic24.o.d 
	@${RM} ${OBJECTDIR}/_ext/1669077864/usb_hal_pic24.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/USB_STACK/usb_hal_pic24.c  -o ${OBJECTDIR}/_ext/1669077864/usb_hal_pic24.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1669077864/usb_hal_pic24.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/COMM_SMC.o: ../Source/COMM_SMC.c  .generated_files/flags/default/d8731d5337539401fcc4f4bae166b801ba7b68f1 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/COMM_SMC.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/COMM_SMC.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/COMM_SMC.c  -o ${OBJECTDIR}/_ext/1728301206/COMM_SMC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/COMM_SMC.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/CPU_SM_MAIN.o: ../Source/CPU_SM_MAIN.c  .generated_files/flags/default/9ab4e98bb3874bad7a7628221056e2edbe2818e7 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/CPU_SM_MAIN.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/CPU_SM_MAIN.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/CPU_SM_MAIN.c  -o ${OBJECTDIR}/_ext/1728301206/CPU_SM_MAIN.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/CPU_SM_MAIN.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/CRC16.o: ../Source/CRC16.c  .generated_files/flags/default/85606bc47c255885b9508a13589bbfb4dd4f230 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/CRC16.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/CRC16.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/CRC16.c  -o ${OBJECTDIR}/_ext/1728301206/CRC16.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/CRC16.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/CRC32.o: ../Source/CRC32.c  .generated_files/flags/default/1e27bb0c0604c4de7adbe7a2a4a47467470de00f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/CRC32.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/CRC32.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/CRC32.c  -o ${OBJECTDIR}/_ext/1728301206/CRC32.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/CRC32.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/DRV_A2D.o: ../Source/DRV_A2D.c  .generated_files/flags/default/ac58fccadc695d66f3b0a2f3833944705ab5f160 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_A2D.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_A2D.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/DRV_A2D.c  -o ${OBJECTDIR}/_ext/1728301206/DRV_A2D.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/DRV_A2D.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/DRV_I2C.o: ../Source/DRV_I2C.c  .generated_files/flags/default/895f937a3f39e7be35f9201dcd049df6d7c2c54a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_I2C.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_I2C.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/DRV_I2C.c  -o ${OBJECTDIR}/_ext/1728301206/DRV_I2C.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/DRV_I2C.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/DRV_MEM.o: ../Source/DRV_MEM.c  .generated_files/flags/default/f764dbba1946a6c9720fb852f3c70f718653287e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_MEM.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_MEM.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/DRV_MEM.c  -o ${OBJECTDIR}/_ext/1728301206/DRV_MEM.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/DRV_MEM.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/DRV_RTC.o: ../Source/DRV_RTC.c  .generated_files/flags/default/6d3b44e6180f83b7ec78477a09a8ee71127dee0f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_RTC.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_RTC.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/DRV_RTC.c  -o ${OBJECTDIR}/_ext/1728301206/DRV_RTC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/DRV_RTC.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/EEPROM.o: ../Source/EEPROM.c  .generated_files/flags/default/7166ed58b06b0c3ce385e7eb6bd173bd38f1b9f5 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/EEPROM.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/EEPROM.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/EEPROM.c  -o ${OBJECTDIR}/_ext/1728301206/EEPROM.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/EEPROM.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/EVENTS.o: ../Source/EVENTS.c  .generated_files/flags/default/d39ab5fa44890a875086701a7b9221c4a5c4c68d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/EVENTS.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/EVENTS.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/EVENTS.c  -o ${OBJECTDIR}/_ext/1728301206/EVENTS.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/EVENTS.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/comm_host.o: ../Source/comm_host.c  .generated_files/flags/default/81be968b90eb25674d774bb6e0e529a13f20ad6a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/comm_host.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/comm_host.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/comm_host.c  -o ${OBJECTDIR}/_ext/1728301206/comm_host.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/comm_host.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/command_proc.o: ../Source/command_proc.c  .generated_files/flags/default/1476bfe4c26c833f7bb2d26ee3afb49796997d38 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/command_proc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/command_proc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/command_proc.c  -o ${OBJECTDIR}/_ext/1728301206/command_proc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/command_proc.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/DRV_CNT.o: ../Source/DRV_CNT.c  .generated_files/flags/default/accbdf9a3b42df3e76b718de2409856da60d9c1e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_CNT.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_CNT.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/DRV_CNT.c  -o ${OBJECTDIR}/_ext/1728301206/DRV_CNT.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/DRV_CNT.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/DRV_DI.o: ../Source/DRV_DI.c  .generated_files/flags/default/dfda75e4efd282a65cfc93c8e2cc9af302855344 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_DI.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_DI.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/DRV_DI.c  -o ${OBJECTDIR}/_ext/1728301206/DRV_DI.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/DRV_DI.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/DRV_LED.o: ../Source/DRV_LED.c  .generated_files/flags/default/36f3ba495e2c09adf0c572cf3c237bd832f0d425 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_LED.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_LED.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/DRV_LED.c  -o ${OBJECTDIR}/_ext/1728301206/DRV_LED.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/DRV_LED.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/PRINT.o: ../Source/PRINT.c  .generated_files/flags/default/a2c4ce77275d2f5f5b9d228dce75f317a525faff .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/PRINT.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/PRINT.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/PRINT.c  -o ${OBJECTDIR}/_ext/1728301206/PRINT.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/PRINT.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/DRV_GLCD_SPI.o: ../Source/DRV_GLCD_SPI.c  .generated_files/flags/default/e45e17d816c1ba8b62f2a3d3a7217273dbac1859 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_GLCD_SPI.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/DRV_GLCD_SPI.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/DRV_GLCD_SPI.c  -o ${OBJECTDIR}/_ext/1728301206/DRV_GLCD_SPI.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/DRV_GLCD_SPI.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/AUTO_DRV_CNT.o: ../Source/AUTO_DRV_CNT.c  .generated_files/flags/default/3eb9d79edc2a947f42158dac743392e2aaaf6234 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1728301206" 
	@${RM} ${OBJECTDIR}/_ext/1728301206/AUTO_DRV_CNT.o.d 
	@${RM} ${OBJECTDIR}/_ext/1728301206/AUTO_DRV_CNT.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Source/AUTO_DRV_CNT.c  -o ${OBJECTDIR}/_ext/1728301206/AUTO_DRV_CNT.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1728301206/AUTO_DRV_CNT.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Header" -I"../Header/USB_STACK" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1728301206/COMM_GSM.o: ../Source/COMM_GSM.c  .generated_files/flags/default/c7740317f22c6913c7ede353500651a96cd2d4da .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
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
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/RESET_UNIT_FW.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG   -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  --fill=0xFFDEAD     -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--no-handles,--no-isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--cref,--memorysummary,${DISTDIR}/memoryfile.xml,--no-ivt$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	
else
${DISTDIR}/RESET_UNIT_FW.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   ../Source/linker_script.gld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/RESET_UNIT_FW.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  --fill=0xFFDEAD -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--no-handles,--no-isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--cref,--memorysummary,${DISTDIR}/memoryfile.xml,--no-ivt$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	${MP_CC_DIR}/xc16-bin2hex ${DISTDIR}/RESET_UNIT_FW.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf   -mdfp="${DFP_DIR}/xc16" 
	
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

DEPFILES=$(shell "${PATH_TO_IDE_BIN}"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
