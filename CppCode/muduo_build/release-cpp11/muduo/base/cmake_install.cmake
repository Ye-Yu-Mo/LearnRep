# Install script for directory: /root/download/muduo/muduo/base

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/root/download/build/release-cpp11/lib/libmuduo_base.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/muduo/base" TYPE FILE FILES
    "/root/download/muduo/muduo/base/AsyncLogging.h"
    "/root/download/muduo/muduo/base/Atomic.h"
    "/root/download/muduo/muduo/base/BlockingQueue.h"
    "/root/download/muduo/muduo/base/BoundedBlockingQueue.h"
    "/root/download/muduo/muduo/base/Condition.h"
    "/root/download/muduo/muduo/base/CountDownLatch.h"
    "/root/download/muduo/muduo/base/CurrentThread.h"
    "/root/download/muduo/muduo/base/Date.h"
    "/root/download/muduo/muduo/base/Exception.h"
    "/root/download/muduo/muduo/base/FileUtil.h"
    "/root/download/muduo/muduo/base/GzipFile.h"
    "/root/download/muduo/muduo/base/LogFile.h"
    "/root/download/muduo/muduo/base/LogStream.h"
    "/root/download/muduo/muduo/base/Logging.h"
    "/root/download/muduo/muduo/base/Mutex.h"
    "/root/download/muduo/muduo/base/ProcessInfo.h"
    "/root/download/muduo/muduo/base/Singleton.h"
    "/root/download/muduo/muduo/base/StringPiece.h"
    "/root/download/muduo/muduo/base/Thread.h"
    "/root/download/muduo/muduo/base/ThreadLocal.h"
    "/root/download/muduo/muduo/base/ThreadLocalSingleton.h"
    "/root/download/muduo/muduo/base/ThreadPool.h"
    "/root/download/muduo/muduo/base/TimeZone.h"
    "/root/download/muduo/muduo/base/Timestamp.h"
    "/root/download/muduo/muduo/base/Types.h"
    "/root/download/muduo/muduo/base/WeakCallback.h"
    "/root/download/muduo/muduo/base/copyable.h"
    "/root/download/muduo/muduo/base/noncopyable.h"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/root/download/build/release-cpp11/muduo/base/tests/cmake_install.cmake")

endif()

