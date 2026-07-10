cmake_minimum_required(VERSION 3.10)

project(Mango)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB mangoSrc
	"${CMAKE_CURRENT_SOURCE_DIR}/src/mango/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/mango/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/mango/*.rc"
)

add_executable(${CMAKE_PROJECT_NAME} WIN32 ${mangoSrc})
add_definitions(-DUNICODE)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/src
)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES
	VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E echo "Copying img directory..."
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		${CMAKE_SOURCE_DIR}/img
		$<TARGET_FILE_DIR:${PROJECT_NAME}>/img
)