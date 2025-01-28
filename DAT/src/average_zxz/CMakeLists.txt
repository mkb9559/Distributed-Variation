cmake_minimum_required(VERSION 3.0)
project(average_zxz)
#set(OpenCV_DIR /usr/local/opencv341/share/OpenCV)

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Release)
ENDIF()

# Check C++11 or C++0x support
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
   add_definitions(-DCOMPILEDWITHC11)
   message(STATUS "Using flag -std=c++11.")
elseif(COMPILER_SUPPORTS_CXX0X)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
   add_definitions(-DCOMPILEDWITHC0X)
   message(STATUS "Using flag -std=c++0x.")
else()
   message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

find_package (catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  tf
  geometry_msgs
)

find_package(Boost REQUIRED COMPONENTS system)
find_package(OsqpEigen REQUIRED)
find_package(Eigen3)
catkin_package(
    INCLUDE_DIRS
    include
    LIBRARIES
    CATKIN_DEPENDS geometry_msgs roscpp
    DEPENDS Boost
)
include_directories(${catkin_INCLUDE_DIRS} ${EIGEN3_INCLUDE_DIR} include)


add_executable(swarm_cmder
    src/swarm_cmder.cpp
    src/DATnode/Point3D.cpp
)
target_link_libraries(swarm_cmder
    ${catkin_LIBRARIES}
    boost_system
    boost_filesystem
)


add_executable(dat_node
    src/dat_single.cpp
    src/DATnode/DATnode.cpp
    src/DATnode/inits.cpp
    src/DATnode/methods.cpp
    src/DATnode/communication.cpp
    src/DATnode/Point3D.cpp
    src/DATnode/Netzxz.cpp
)
target_link_libraries(dat_node
    ${catkin_LIBRARIES}
    boost_system
    boost_filesystem
    OsqpEigen::OsqpEigen
)

add_executable(dat_sim
    src/dat_sim.cpp
    src/DATnode/DATnode.cpp
    src/DATnode/inits.cpp
    src/DATnode/methods.cpp
    src/DATnode/communication.cpp
    src/DATnode/Point3D.cpp
    src/DATnode/Netzxz.cpp
)
target_link_libraries(dat_sim
    ${catkin_LIBRARIES}
    boost_system
    boost_filesystem
    OsqpEigen::OsqpEigen
)


install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
  FILES_MATCHING PATTERN "*.h"
)

## Mark other files for installation (e.g. launch and bag files, etc.)
install(DIRECTORY launch/
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/launch
)

