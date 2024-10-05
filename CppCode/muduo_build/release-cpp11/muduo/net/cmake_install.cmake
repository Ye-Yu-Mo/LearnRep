# Install script for directory: /root/download/muduo/muduo/net

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/root/download/build/release-install-cpp11")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/root/download/build/release-cpp11/lib/libmuduo_net.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/muduo/net" TYPE FILE FILES
    "/root/download/muduo/muduo/net/Buffer.h"
    "/root/download/muduo/muduo/net/Callbacks.h"
    "/root/download/muduo/muduo/net/Channel.h"
    "/root/download/muduo/muduo/net/Endian.h"
    "/root/download/muduo/muduo/net/EventLoop.h"
    "/root/download/muduo/muduo/net/EventLoopThread.h"
    "/root/download/muduo/muduo/net/EventLoopThreadPool.h"
    "/root/download/muduo/muduo/net/InetAddress.h"
    "/root/download/muduo/muduo/net/TcpClient.h"
    "/root/download/muduo/muduo/net/TcpConnection.h"
    "/root/download/muduo/muduo/net/TcpServer.h"
    "/root/download/muduo/muduo/net/TimerId.h"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/root/download/build/release-cpp11/muduo/net/http/cmake_install.cmake")
  include("/root/download/build/release-cpp11/muduo/net/inspect/cmake_install.cmake")
  include("/root/download/build/release-cpp11/muduo/net/tests/cmake_install.cmake")
  include("/root/download/build/release-cpp11/muduo/net/protobuf/cmake_install.cmake")
  include("/root/download/build/release-cpp11/muduo/net/protorpc/cmake_install.cmake")

endif()

