cmake_minimum_required(VERSION 2.8)
project(test_atom)

set(CMAKE_CXX_COMPILER ${CROSS_ATOM}/cross/bin/g++)
set(CMAKE_C_COMPILER ${CROSS_ATOM}/cross/bin/gcc)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath-link,${CROSS_ATOM}/libnaoqi/lib")

set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_EXE_LINKER_FLAGS "-static")

set(CMAKE_EXE_LINKER_FLAGS "-Wl,-rpath-link,/home/vboxuser/Downloads/ctc-linux64-atom-2.1.4.13/libnaoqi/lib/libopencv_highgui.so")


link_directories(/opt/aldebaran/lib)


find_package(qibuild)
find_package(Boost REQUIRED)
find_package(OpenCV REQUIRED)

set(CROSS_ATOM "/home/vboxuser/Downloads/ctc-linux64-atom-2.1.4.13")

include(${CROSS_ATOM}/cross-config.cmake)

#include_directories(${CROSS_ATOM}/libnaoqi/include)
include_directories(
    ${CROSS_ATOM}/libnaoqi/include
    ${Boost_INCLUDE_DIRS}
)
link_directories(${CROSS_ATOM}/libnaoqi/lib)

add_definitions(-DBOOST_SIGNALS_NO_DEPRECATION_WARNING)

set(_srcs
  main.cpp
)

qi_create_bin(test_atom ${_srcs} DEPENDS QI)
qi_use_lib(test_atom ALCOMMON)
qi_use_lib(test_atom OPENCV2_CORE OPENCV2_IMGPROC OPENCV2_OBJDETECT)


target_link_libraries(test_atom
    alcommon
    alproxies
    ${CROSS_ATOM}/libnaoqi/lib/libalcommon.so
    ${CROSS_ATOM}/libnaoqi/lib/libqi.so
    ${CROSS_ATOM}/libnaoqi/lib/libqitype.so
    #${CROSS_ATOM}/libnaoqi/lib/libboost_system-mt-1_55.so
    #${CROSS_ATOM}/libnaoqi/lib/libboost_signals-mt-1_55.so.1.55.0
    ${CROSS_ATOM}/libnaoqi/lib/libalvalue.so
    ${CROSS_ATOM}/libnaoqi/lib/libalerror.so
    ${CROSS_ATOM}/libnaoqi/lib/libqimessaging.so
    ${CROSS_ATOM}/libnaoqi/lib/libalvision.so
    #${OpenCV_LIBS}
    opencv_core
    opencv_imgproc
    opencv_objdetect
    #opencv_highgui
)
