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
CND_CONF=Release
CND_DISTDIR=dist

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/video/ogl/buffers/vertex_array.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/video/ogl/buffers/texture.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/video/ogl/buffers/frame_buffer.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/video/ogl/buffers/render_buffer.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/video/ogl/shaders/shader_program.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/video/primitives/triangle.o \
	${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/video/ogl/buffers/vertex_format.o

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
	${MAKE}  -f nbproject/Makefile-Release.mk dist/../../../../lib/linux/libdjah_video.a

dist/../../../../lib/linux/libdjah_video.a: ${OBJECTFILES}
	${MKDIR} -p dist/../../../../lib/linux
	${RM} dist/../../../../lib/linux/libdjah_video.a
	${AR} rv ${CND_DISTDIR}/../../../../lib/linux/libdjah_video.a ${OBJECTFILES} 
	$(RANLIB) dist/../../../../lib/linux/libdjah_video.a

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/video/ogl/buffers/vertex_array.o: nbproject/Makefile-${CND_CONF}.mk ../../../sources/video/ogl/buffers/vertex_array.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/video/ogl/buffers
	${RM} $@.d
	$(COMPILE.cc) -O2 -I../../../../extlibs/include -I../../../include/djah -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/video/ogl/buffers/vertex_array.o ../../../sources/video/ogl/buffers/vertex_array.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/video/ogl/buffers/texture.o: nbproject/Makefile-${CND_CONF}.mk ../../../sources/video/ogl/buffers/texture.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/video/ogl/buffers
	${RM} $@.d
	$(COMPILE.cc) -O2 -I../../../../extlibs/include -I../../../include/djah -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/video/ogl/buffers/texture.o ../../../sources/video/ogl/buffers/texture.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/video/ogl/buffers/frame_buffer.o: nbproject/Makefile-${CND_CONF}.mk ../../../sources/video/ogl/buffers/frame_buffer.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/video/ogl/buffers
	${RM} $@.d
	$(COMPILE.cc) -O2 -I../../../../extlibs/include -I../../../include/djah -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/video/ogl/buffers/frame_buffer.o ../../../sources/video/ogl/buffers/frame_buffer.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/video/ogl/buffers/render_buffer.o: nbproject/Makefile-${CND_CONF}.mk ../../../sources/video/ogl/buffers/render_buffer.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/video/ogl/buffers
	${RM} $@.d
	$(COMPILE.cc) -O2 -I../../../../extlibs/include -I../../../include/djah -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/video/ogl/buffers/render_buffer.o ../../../sources/video/ogl/buffers/render_buffer.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/video/ogl/shaders/shader_program.o: nbproject/Makefile-${CND_CONF}.mk ../../../sources/video/ogl/shaders/shader_program.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/video/ogl/shaders
	${RM} $@.d
	$(COMPILE.cc) -O2 -I../../../../extlibs/include -I../../../include/djah -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/video/ogl/shaders/shader_program.o ../../../sources/video/ogl/shaders/shader_program.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/video/primitives/triangle.o: nbproject/Makefile-${CND_CONF}.mk ../../../sources/video/primitives/triangle.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/video/primitives
	${RM} $@.d
	$(COMPILE.cc) -O2 -I../../../../extlibs/include -I../../../include/djah -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/video/primitives/triangle.o ../../../sources/video/primitives/triangle.cpp

${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/video/ogl/buffers/vertex_format.o: nbproject/Makefile-${CND_CONF}.mk ../../../sources/video/ogl/buffers/vertex_format.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/video/ogl/buffers
	${RM} $@.d
	$(COMPILE.cc) -O2 -I../../../../extlibs/include -I../../../include/djah -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/_DOTDOT/_DOTDOT/sources/video/ogl/buffers/vertex_format.o ../../../sources/video/ogl/buffers/vertex_format.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Release
	${RM} dist/../../../../lib/linux/libdjah_video.a

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
