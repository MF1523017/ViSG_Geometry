# cmake_minimum_required(VERSION 2.8)

project(VISGSFM)

set (CMAKE_BUILD_TYPE "Release")
if(NOT CMAKE_BUILD_TYPE)
	SET(CMAKE_BUILD_TYPE Release)
endif()
message("build type:" ${CMAKE_BUILD_TYPE})


set(CMAKE_CXX_FLAGS "-std=c++11")
include_directories(
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/src
	)

# opencv
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# eigen
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

# sophus
#set(Sophus_INCLUDE_DIR "~/Sophus/")
find_package(Sophus REQUIRED)
include_directories(${Sophus_INCLUDE_DIR})
#target_link_libraries(VISGSFM ${Sophus_LIBRARIES})

# ceres

find_package(Ceres REQUIRED)
include_directories(${CERES_INCLUDE_DIRS})

add_library(feature SHARED	${PROJECT_SOURCE_DIR}/src/feature.cpp)
add_library(matcher SHARED	${PROJECT_SOURCE_DIR}/src/matcher.cpp)
add_library(pose SHARED	${PROJECT_SOURCE_DIR}/src/pose.cpp)
add_library(camera SHARED	${PROJECT_SOURCE_DIR}/src/camera.cpp)
add_library(util SHARED	${PROJECT_SOURCE_DIR}/src/util.cpp)
add_library(map SHARED	${PROJECT_SOURCE_DIR}/src/map.cpp)
add_library(feature_extractor SHARED	${PROJECT_SOURCE_DIR}/src/feature_extractor.cpp)
add_library(geometry SHARED ${PROJECT_SOURCE_DIR}/src/geometry.cpp)
add_library(sfm SHARED ${PROJECT_SOURCE_DIR}/src/sfm.cpp)


add_executable(VISGSFM ${PROJECT_SOURCE_DIR}/test/sfm_test.cpp)

target_link_libraries(VISGSFM ${Sophus_LIBRARIES})
target_link_libraries(VISGSFM ${OpenCV_LIBS})
target_link_libraries(VISGSFM ${CERES_LIBRARIES})
target_link_libraries(VISGSFM feature)
target_link_libraries(VISGSFM matcher)
target_link_libraries(VISGSFM pose)
target_link_libraries(VISGSFM camera)
target_link_libraries(VISGSFM util)
target_link_libraries(VISGSFM map)
target_link_libraries(VISGSFM feature_extractor)
target_link_libraries(VISGSFM geometry)
target_link_libraries(VISGSFM sfm)
