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
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/video/ogl/extensions.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/system/driver_base.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/system/opengl_driver.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/system/device_base.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/video/ogl/gl_caps.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/system/video_config.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/system/linux/x11device.o

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
	${MAKE}  -f nbproject/Makefile-Debug.mk dist/../../../../lib/linux/libdjah_system_d.a

dist/../../../../lib/linux/libdjah_system_d.a: ${OBJECTFILES}
	${MKDIR} -p dist/../../../../lib/linux
	${RM} dist/../../../../lib/linux/libdjah_system_d.a
	${AR} rv ${CND_DISTDIR}/../../../../lib/linux/libdjah_system_d.a ${OBJECTFILES} 
	$(RANLIB) dist/../../../../lib/linux/libdjah_system_d.a

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/video/ogl/extensions.o: nbproject/Makefile-${CND_CONF}.mk ../../../sources/video/ogl/extensions.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/video/ogl
	${RM} $@.d
	$(COMPILE.cc) -g -I../../../include/djah -I../../../../extlibs/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/video/ogl/extensions.o ../../../sources/video/ogl/extensions.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/system/driver_base.o: nbproject/Makefile-${CND_CONF}.mk ../../../sources/system/driver_base.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/system
	${RM} $@.d
	$(COMPILE.cc) -g -I../../../include/djah -I../../../../extlibs/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/system/driver_base.o ../../../sources/system/driver_base.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/system/opengl_driver.o: nbproject/Makefile-${CND_CONF}.mk ../../../sources/system/opengl_driver.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/system
	${RM} $@.d
	$(COMPILE.cc) -g -I../../../include/djah -I../../../../extlibs/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/system/opengl_driver.o ../../../sources/system/opengl_driver.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/system/device_base.o: nbproject/Makefile-${CND_CONF}.mk ../../../sources/system/device_base.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/system
	${RM} $@.d
	$(COMPILE.cc) -g -I../../../include/djah -I../../../../extlibs/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/system/device_base.o ../../../sources/system/device_base.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/video/ogl/gl_caps.o: nbproject/Makefile-${CND_CONF}.mk ../../../sources/video/ogl/gl_caps.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/video/ogl
	${RM} $@.d
	$(COMPILE.cc) -g -I../../../include/djah -I../../../../extlibs/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/video/ogl/gl_caps.o ../../../sources/video/ogl/gl_caps.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/system/video_config.o: nbproject/Makefile-${CND_CONF}.mk ../../../sources/system/video_config.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/system
	${RM} $@.d
	$(COMPILE.cc) -g -I../../../include/djah -I../../../../extlibs/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/system/video_config.o ../../../sources/system/video_config.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/system/linux/x11device.o: nbproject/Makefile-${CND_CONF}.mk ../../../sources/system/linux/x11device.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/system/linux
	${RM} $@.d
	$(COMPILE.cc) -g -I../../../include/djah -I../../../../extlibs/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/system/linux/x11device.o ../../../sources/system/linux/x11device.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Debug
	${RM} dist/../../../../lib/linux/libdjah_system_d.a

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
