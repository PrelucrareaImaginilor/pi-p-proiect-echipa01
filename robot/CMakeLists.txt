cmake_minimum_required(VERSION 2.8)
project(test01)

set(CMAKE_CXX_COMPILER /usr/bin/g++)
set(CMAKE_C_COMPILER /usr/bin/gcc)

find_package(qibuild)

set(NAOQI_SDK "/home/vboxuser/Downloads/naoqi-sdk")

include_directories(${NOQI_SDK}/include)
link_directories(${NOQI_SDK}/lib)

#qi_create_bin(test01 "main.cpp")

# Add a simple test:
#qi_create_test(test_test01 "test.cpp")


set(_srcs
  main.cpp
  EmotionDetection.cpp
  EmotionDetection.hpp
  Behavior.cpp
  Behavior.hpp
)


#qi_create_bin(test01 ${_srcs})
qi_create_bin(main ${_srcs} DEPENDS QI)

