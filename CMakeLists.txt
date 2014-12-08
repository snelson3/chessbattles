cmake_minimum_required(VERSION 2.8.12.1)

PROJECT(chessbattles)
SET(VTK_DIR /Users/snelson/CIS441/VTK-6.1.0)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

add_executable(chessbattles MACOSX_BUNDLE chessbattles)

if(VTK_LIBRARIES)
  target_link_libraries(chessbattles ${VTK_LIBRARIES})
else()
  target_link_libraries(chessbattles vtkHybrid)
endif()

