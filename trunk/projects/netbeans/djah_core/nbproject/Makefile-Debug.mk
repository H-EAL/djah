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
	${OBJECTDIR}/_ext/media/Windows_Data__OS_/Development/djah/trunk/sources/time/clock.o \
	${OBJECTDIR}/_ext/media/Windows_Data__OS_/Development/djah/trunk/sources/utils/randomizer.o

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
	${MAKE}  -f nbproject/Makefile-Debug.mk dist/../../../../lib/linux/libdjah_core_d.a

dist/../../../../lib/linux/libdjah_core_d.a: ${OBJECTFILES}
	${MKDIR} -p dist/../../../../lib/linux
	${RM} dist/../../../../lib/linux/libdjah_core_d.a
	${AR} rv ${CND_DISTDIR}/../../../../lib/linux/libdjah_core_d.a ${OBJECTFILES} 
	$(RANLIB) dist/../../../../lib/linux/libdjah_core_d.a

${OBJECTDIR}/_ext/media/Windows_Data__OS_/Development/djah/trunk/sources/time/clock.o: nbproject/Makefile-${CND_CONF}.mk /media/Windows\ Data\ \(OS\)/Development/djah/trunk/sources/time/clock.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/media/Windows_Data__OS_/Development/djah/trunk/sources/time
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -I../../../include/djah -I../../../../extlibs/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/media/Windows_Data__OS_/Development/djah/trunk/sources/time/clock.o /media/Windows\ Data\ \(OS\)/Development/djah/trunk/sources/time/clock.cpp

${OBJECTDIR}/_ext/media/Windows_Data__OS_/Development/djah/trunk/sources/utils/randomizer.o: nbproject/Makefile-${CND_CONF}.mk /media/Windows\ Data\ \(OS\)/Development/djah/trunk/sources/utils/randomizer.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/media/Windows_Data__OS_/Development/djah/trunk/sources/utils
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -I../../../include/djah -I../../../../extlibs/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/media/Windows_Data__OS_/Development/djah/trunk/sources/utils/randomizer.o /media/Windows\ Data\ \(OS\)/Development/djah/trunk/sources/utils/randomizer.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Debug
	${RM} dist/../../../../lib/linux/libdjah_core_d.a

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
