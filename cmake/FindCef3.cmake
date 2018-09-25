# - Try to find cef3
# Set CEF3_ROOT for search-hint
# Set CEF3_NO_DEBUG for only optimized cef library
#
# Once done, this will define
# 
#  CEF3_FOUND 
#  CEF3_INCLUDE_DIRS 
#  CEF3_LIBRARIES - link these to use cef3
#  CEF3_LIBRARY_DEBUG - link these to use cef3 debug
#  CEF3_LIBRARY_RELEASE - link these to use cef3 release
#  CEF3_LIBRARY_WRAPPER_RELEASE - link these to use cef3-wrapper release
#  CEF3_LIBRARY_WRAPPER_DEBUG - link these to use cef3-wrapper debug

# TODO markus: script out dated - cef3 seems to change build system to cmake!


IF(NOT EXISTS ${CEF3_ROOT})
  message(WARNING "Could not find cef3 root: ${CEF3_ROOT}")
endif ()

find_path(CEF3_INCLUDE_DIRS NAMES include PATHS ${CEF3_ROOT} DOC "include path of cef3")

set (CEF3_INCLUDE_DIRS ${CEF3_INCLUDE_DIRS} ${CEF3_INCLUDE_DIRS}/include )

find_library(CEF3_LIBRARY_RELEASE NAMES cef PATHS ${CEF3_ROOT} PATH_SUFFIXES Release DOC "libcef release path")
find_library(CEF3_LIBRARY_DEBUG NAMES cef PATHS ${CEF3_ROOT} PATH_SUFFIXES Debug DOC "libcef debug path")

set(CEF3_LIBRARY 
  optimized ${CEF3_LIBRARY_RELEASE}
  debug ${CEF3_LIBRARY_DEBUG}
)
find_library(CEF3_LIBRARY_WRAPPER_RELEASE NAMES libcef_dll PATHS ${CEF3_ROOT} PATH_SUFFIXES Release DOC "libcef-wrapper release path")
find_library(CEF3_LIBRARY_WRAPPER_DEBUG NAMES libcef_dll PATHS ${CEF3_ROOT} PATH_SUFFIXES Debug DOC "libcef-wrapper debug path")
set(CEF3_LIBRARY_WRAPPER
  optimized ${CEF3_LIBRARY_WRAPPER_RELEASE}
  debug ${CEF3_LIBRARY_WRAPPER_DEBUG}
)

find_package(GTK2 REQUIRED gtk)
set (CEF3_INCLUDE_DIRS
      ${CEF3_INCLUDE_DIRS}
      ${GTK2_INCLUDE_DIRS}
)
set (CEF3_LIBRARIES 
  ${CEF3_LIBRARY_WRAPPER}
#  ${UDEV_LIBRARIES}
)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
  CEF3
  REQUIRED_VARS CEF3_LIBRARY CEF3_LIBRARY_WRAPPER CEF3_INCLUDE_DIRS
)