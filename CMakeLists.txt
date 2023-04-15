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
set(DKP_AS_TOOL ${DEVKITPATH}/devkitPPC/bin/powerpc-eabi-as)
set(DKP_DOL_TOOL ${DEVKITPATH}/tools/bin/elf2dol)
set(GOLEM_PATCHER "./golem")

set(C_FLAGS -g -O0 -Wall -DGEKKO -mrvl -mcpu=750 -meabi -mhard-float -fomit-frame-pointer -I ./include/ -c)
set(LD_FLAGS -g -DGEKKO -mrvl -mcpu=750 -meabi -mhard-float -nodefaultlibs -nostartfiles -T execute.ld)
set(C_FILES ./source/HelperBase.c ./source/enemyCheck.c ./source/piggybackAI.c)

string(REPLACE ".c" ".o" OBJ_FILES "${C_FILES}")
string(REPLACE "/source/" "/" OBJ_FILES "${OBJ_FILES}")

add_custom_target(Help ALL)
add_custom_command(
	TARGET Help 
	COMMAND ${DKP_AS_TOOL} ./asm/main.asm -o ./main.o
	COMMAND ${DKP_C_COMP} ${C_FLAGS} ${C_FILES} 
	COMMAND ${DKP_C_COMP} ${LD_FLAGS} ${OBJ_FILES} ./main.o -o ./build/Helper.elf
	COMMAND ${DKP_DOL_TOOL} ./build/Helper.elf ./build/Helper.dol
	COMMAND ${GOLEM_PATCHER} ./build/Helper.dol
	COMMAND del *.o
)

file(MAKE_DIRECTORY ./build)