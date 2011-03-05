#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_CONF=Debug
CND_DISTDIR=dist

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/log/html_logger.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/log/console_logger.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/log/logger.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-Debug.mk dist/../../../../lib/linux/libdjah_log_d.a

dist/../../../../lib/linux/libdjah_log_d.a: ${OBJECTFILES}
	${MKDIR} -p dist/../../../../lib/linux
	${RM} dist/../../../../lib/linux/libdjah_log_d.a
	${AR} rv ${CND_DISTDIR}/../../../../lib/linux/libdjah_log_d.a ${OBJECTFILES} 
	$(RANLIB) dist/../../../../lib/linux/libdjah_log_d.a

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/log/html_logger.o: nbproject/Makefile-${CND_CONF}.mk ../../../sources/log/html_logger.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/log
	${RM} $@.d
	$(COMPILE.cc) -g -I../../../include/djah -I../../../../extlibs/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/log/html_logger.o ../../../sources/log/html_logger.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/log/console_logger.o: nbproject/Makefile-${CND_CONF}.mk ../../../sources/log/console_logger.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/log
	${RM} $@.d
	$(COMPILE.cc) -g -I../../../include/djah -I../../../../extlibs/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/log/console_logger.o ../../../sources/log/console_logger.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/log/logger.o: nbproject/Makefile-${CND_CONF}.mk ../../../sources/log/logger.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/log
	${RM} $@.d
	$(COMPILE.cc) -g -I../../../include/djah -I../../../../extlibs/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/log/logger.o ../../../sources/log/logger.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Debug
	${RM} dist/../../../../lib/linux/libdjah_log_d.a

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
