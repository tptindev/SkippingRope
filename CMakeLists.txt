cmake_minimum_required(VERSION 3.14)
set(CMAKE_C_STANDARD 11)

set(ENVSDK $ENV{PLAYDATE_SDK_PATH})

if (NOT ${ENVSDK} STREQUAL "")
	# Convert path from Windows
	file(TO_CMAKE_PATH ${ENVSDK} SDK)
else()
	execute_process(
			COMMAND bash -c "egrep '^\\s*SDKRoot' $HOME/.Playdate/config"
			COMMAND head -n 1
			COMMAND cut -c9-
			OUTPUT_VARIABLE SDK
			OUTPUT_STRIP_TRAILING_WHITESPACE
	)
endif()

if (NOT EXISTS ${SDK})
	message(FATAL_ERROR "SDK Path not found; set ENV value PLAYDATE_SDK_PATH")
	return()
endif()

set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
set(CMAKE_XCODE_GENERATE_SCHEME TRUE)

# Game Name Customization
set(PLAYDATE_GAME_NAME SkippingRole)
set(PLAYDATE_GAME_DEVICE SkippingRole_DEVICE)

project(${PLAYDATE_GAME_NAME} LANGUAGES C ASM)

set(INCLUDE_DIR
	${CMAKE_SOURCE_DIR}/include/box2d
)

set(LIB_DIR
    ${CMAKE_SOURCE_DIR}/libs
)

file(GLOB IMAGES
	"Source/images/*"
)

File(GLOB_RECURSE SOURCE_FILES "src/*.h" "src/*.c")

include_directories(${INCLUDE_DIR})
link_directories(${LIB_DIRS})

if (TOOLCHAIN STREQUAL "armgcc")
	add_executable(${PLAYDATE_GAME_DEVICE} ${SDK}/C_API/buildsupport/setup.c ${SOURCE_FILES})
else()
	add_library(${PLAYDATE_GAME_NAME} SHARED ${SOURCE_FILES} ${IMAGES})
	target_link_libraries(${PLAYDATE_GAME_NAME} PRIVATE "${LIB_DIR}/box2d.lib")
endif()
	

include(${SDK}/C_API/buildsupport/playdate_game.cmake)