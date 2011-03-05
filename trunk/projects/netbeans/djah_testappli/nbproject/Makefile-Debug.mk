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
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/tests/sources/main.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/tests/sources/application_base.o

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
LDLIBSOPTIONS=-L../../../lib/linux -lGL -lX11 -lXext -ldjah_core_d -ldjah_filesystem_d -ldjah_log_d -ldjah_video_d -ldjah_system_d

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-Debug.mk dist/../../../../tests/binaries/djah_testappli_d

dist/../../../../tests/binaries/djah_testappli_d: ${OBJECTFILES}
	${MKDIR} -p dist/../../../../tests/binaries
	${LINK.cc} -o ${CND_DISTDIR}/../../../../tests/binaries/djah_testappli_d ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/tests/sources/main.o: nbproject/Makefile-${CND_CONF}.mk ../../../tests/sources/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/tests/sources
	${RM} $@.d
	$(COMPILE.cc) -g -I../../../include -I../../../../extlibs/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/tests/sources/main.o ../../../tests/sources/main.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/tests/sources/application_base.o: nbproject/Makefile-${CND_CONF}.mk ../../../tests/sources/application_base.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/tests/sources
	${RM} $@.d
	$(COMPILE.cc) -g -I../../../include -I../../../../extlibs/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/tests/sources/application_base.o ../../../tests/sources/application_base.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Debug
	${RM} dist/../../../../tests/binaries/djah_testappli_d

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
