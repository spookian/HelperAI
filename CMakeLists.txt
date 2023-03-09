cmake_minimum_required(VERSION 3.16)
project(HelperProject)

#If you did not install CMake in the default location, may God have mercy on your soul.
if(WIN32)
set(DEVKITPATH C:/devkitPro) 
endif(WIN32)

if(UNIX)
set(DEVKITPATH /opt/devkitpro)
endif(UNIX)

set(DKP_C_COMP ${DEVKITPATH}/devkitPPC/bin/powerpc-eabi-gcc)
set(DKP_DOL_TOOL ${DEVKITPATH}/tools/bin/elf2dol)
set(GOLEM_PATCHER "./golem")

add_custom_target(Help ALL)
add_custom_command(
	TARGET Help 
	COMMAND ${DKP_C_COMP} -g -O0 -Wall -DGEKKO -mrvl -mcpu=750 -meabi -mhard-float -fomit-frame-pointer -I ./include/ -c ./source/HelperAI.c -o ./build/Helper.o
	COMMAND ${DKP_C_COMP} -g -DGEKKO -mrvl -mcpu=750 -meabi -mhard-float -nodefaultlibs -nostartfiles -Wl,--gc-sections ./build/Helper.o -o ./build/Helper.elf
	COMMAND ${DKP_DOL_TOOL} ./build/Helper.elf ./build/Helper.dol
	COMMAND python quickpatcher.py
	COMMAND ${GOLEM_PATCHER} ./build/Helper.dol
)

file(MAKE_DIRECTORY ./build)