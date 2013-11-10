cmake_minimum_required (VERSION 2.6)

project (bots)

Find_Package ( SDL REQUIRED )
#Find_Package ( SDL_gfx REQUIRED )
find_package(OpenGL REQUIRED )

add_definitions(-std=c++11)

include_directories(${SDL_INCLUDE_DIR})

add_executable(bots playground.cpp bot.cpp bots.cpp)

target_link_libraries(bots ${SDL_LIBRARY})
target_link_libraries(bots ${OPENGL_LIBRARY})

FIND_PACKAGE(Doxygen)
if (NOT DOXYGEN_FOUND)
    message(FATAL_ERROR 
      "Doxygen is needed to build the documentation. Please install it correctly")
  endif()

  configure_file(Doxyfile
                 ${PROJECT_BINARY_DIR}/Doxyfile  @ONLY IMMEDIATE)
  add_custom_target (Docs ALL 
                                          COMMAND ${DOXYGEN_EXECUTABLE} ${PROJECT_BINARY_DIR}/Doxyfile
                                          SOURCES ${PROJECT_BINARY_DIR}/Doxyfile)
