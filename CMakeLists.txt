#
# Shade Model CMake Configuration Script
#
# 2015.03.31 Jared D. Baker <jared.baker@uwyo.edu>
#
# ChangeLog
#
# 2015.03.31 JDB Initial File Creation
#

cmake_minimum_required (VERSION 2.8)

project(ShadeModel)

set(CMAKE_C_COMPILER mpicc)
set(CMAKE_CXX_COMPILER mpicxx)
add_definitions(-DPAR_MPI=1)

include_directories(include)

set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_MACOSX_RPATH 1)

add_executable(
	shadem
	src/azimuth.c
	src/landReader.c
	src/localHourAngle.c
	src/parlib.c
	src/solarAltitude.c
	src/sunDeclination.c
	src/test.c
	src/tilt.c
	src/timeDifference.c
	)

install(TARGETS shadem DESTINATION bin)

