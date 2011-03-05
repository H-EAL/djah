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
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/filesystem/directory_source.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/filesystem/file_stream.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/filesystem/browser.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/filesystem/memory_stream.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/filesystem/compression_techniques.o

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
	${MAKE}  -f nbproject/Makefile-Debug.mk dist/../../../../lib/linux/libdjah_filesystem_d.a

dist/../../../../lib/linux/libdjah_filesystem_d.a: ${OBJECTFILES}
	${MKDIR} -p dist/../../../../lib/linux
	${RM} dist/../../../../lib/linux/libdjah_filesystem_d.a
	${AR} rv ${CND_DISTDIR}/../../../../lib/linux/libdjah_filesystem_d.a ${OBJECTFILES} 
	$(RANLIB) dist/../../../../lib/linux/libdjah_filesystem_d.a

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/filesystem/directory_source.o: nbproject/Makefile-${CND_CONF}.mk ../../../sources/filesystem/directory_source.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/filesystem
	${RM} $@.d
	$(COMPILE.cc) -g -I../../../include/djah -I../../../../extlibs/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/filesystem/directory_source.o ../../../sources/filesystem/directory_source.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/filesystem/file_stream.o: nbproject/Makefile-${CND_CONF}.mk ../../../sources/filesystem/file_stream.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/filesystem
	${RM} $@.d
	$(COMPILE.cc) -g -I../../../include/djah -I../../../../extlibs/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/filesystem/file_stream.o ../../../sources/filesystem/file_stream.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/filesystem/browser.o: nbproject/Makefile-${CND_CONF}.mk ../../../sources/filesystem/browser.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/filesystem
	${RM} $@.d
	$(COMPILE.cc) -g -I../../../include/djah -I../../../../extlibs/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/filesystem/browser.o ../../../sources/filesystem/browser.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/filesystem/memory_stream.o: nbproject/Makefile-${CND_CONF}.mk ../../../sources/filesystem/memory_stream.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/filesystem
	${RM} $@.d
	$(COMPILE.cc) -g -I../../../include/djah -I../../../../extlibs/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/filesystem/memory_stream.o ../../../sources/filesystem/memory_stream.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/filesystem/compression_techniques.o: nbproject/Makefile-${CND_CONF}.mk ../../../sources/filesystem/compression_techniques.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/filesystem
	${RM} $@.d
	$(COMPILE.cc) -g -I../../../include/djah -I../../../../extlibs/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/filesystem/compression_techniques.o ../../../sources/filesystem/compression_techniques.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Debug
	${RM} dist/../../../../lib/linux/libdjah_filesystem_d.a

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
