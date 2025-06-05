# CMake entry point
cmake_minimum_required (VERSION 3.30)

# Define the project 
project (Chess3D)

############################################### 
# Add the necessary dependencies
###############################################

# OpenGL
find_package(OpenGL REQUIRED)
find_package(OpenMP REQUIRED)

# Look at /external folder CMakeLists.txt for the dependencies 
add_subdirectory (external)

############################################### 
# Select the directories to compile
###############################################

include_directories(
	external/AntTweakBar-1.16/include/
	external/glfw-3.1.2/include/
	external/glm-0.9.7.1/
	external/glew-1.13.0/include/
	external/assimp-3.0.1270/include/
	external/bullet-2.81-rev2613/src/
	external/SFML/include
	./
  	include/
)

# Link the executable to the SFML libraries
link_directories(external/SFML/lib)

# Name the libraries
set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	GLEW_1130
	assimp
	sfml-graphics 
	sfml-system 
	sfml-window
	OpenMP::OpenMP_CXX
)

# Libraries definitions
add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

############################################### 
# Select the sources to compile
###############################################

# Define sources
file(GLOB SOURCES src/*.cpp)

# main
add_executable(main
  	${SOURCES}							# .cpp source files in /src
	src/shaders/vertexShader.glsl		# Vertex shader
	src/shaders/fragmentShader.glsl		# Fragment shader
)

# Link the libraries to the target
target_link_libraries(main					# Target executable 
	${ALL_LIBS}								
)

# Set the executable into src folder
add_custom_command(
   TARGET main POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/main${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/src/"
)