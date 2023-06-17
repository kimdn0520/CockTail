# Generated by Boost 1.80.0

# address-model=32

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  _BOOST_SKIPPED("libboost_filesystem-vc142-mt-sgd-x32-1_80.lib" "32 bit, need 64")
  return()
endif()

# layout=versioned

# toolset=vc142

if(Boost_COMPILER)
  if(NOT Boost_COMPILER STREQUAL "vc142" AND NOT Boost_COMPILER STREQUAL "-vc142")
    _BOOST_SKIPPED("libboost_filesystem-vc142-mt-sgd-x32-1_80.lib" "vc142, Boost_COMPILER=${Boost_COMPILER}")
    return()
  endif()
else()
  if(BOOST_DETECTED_TOOLSET AND NOT BOOST_DETECTED_TOOLSET STREQUAL "vc142")
    _BOOST_SKIPPED("libboost_filesystem-vc142-mt-sgd-x32-1_80.lib" "vc142, detected ${BOOST_DETECTED_TOOLSET}, set Boost_COMPILER to override")
    return()
  endif()
endif()

# link=static

if(DEFINED Boost_USE_STATIC_LIBS)
  if(NOT Boost_USE_STATIC_LIBS)
    _BOOST_SKIPPED("libboost_filesystem-vc142-mt-sgd-x32-1_80.lib" "static, Boost_USE_STATIC_LIBS=${Boost_USE_STATIC_LIBS}")
    return()
  endif()
else()
  if(NOT WIN32 AND NOT _BOOST_SINGLE_VARIANT)
    _BOOST_SKIPPED("libboost_filesystem-vc142-mt-sgd-x32-1_80.lib" "static, default is shared, set Boost_USE_STATIC_LIBS=ON to override")
    return()
  endif()
endif()

# runtime-link=static

if(NOT Boost_USE_STATIC_RUNTIME)
  _BOOST_SKIPPED("libboost_filesystem-vc142-mt-sgd-x32-1_80.lib" "static runtime, Boost_USE_STATIC_RUNTIME not ON")
  return()
endif()

# runtime-debugging=on

if(NOT "${Boost_USE_DEBUG_RUNTIME}" STREQUAL "" AND NOT Boost_USE_DEBUG_RUNTIME)
  _BOOST_SKIPPED("libboost_filesystem-vc142-mt-sgd-x32-1_80.lib" "debug runtime, Boost_USE_DEBUG_RUNTIME=${Boost_USE_DEBUG_RUNTIME}")
  return()
endif()

# threading=multi

if(DEFINED Boost_USE_MULTITHREADED AND NOT Boost_USE_MULTITHREADED)
  _BOOST_SKIPPED("libboost_filesystem-vc142-mt-sgd-x32-1_80.lib" "multithreaded, Boost_USE_MULTITHREADED=${Boost_USE_MULTITHREADED}")
  return()
endif()

# variant=debug

if(NOT "${Boost_USE_DEBUG_LIBS}" STREQUAL "" AND NOT Boost_USE_DEBUG_LIBS)
  _BOOST_SKIPPED("libboost_filesystem-vc142-mt-sgd-x32-1_80.lib" "debug, Boost_USE_DEBUG_LIBS=${Boost_USE_DEBUG_LIBS}")
  return()
endif()

if(Boost_VERBOSE OR Boost_DEBUG)
  message(STATUS "  [x] libboost_filesystem-vc142-mt-sgd-x32-1_80.lib")
endif()

# Create imported target Boost::filesystem

if(NOT TARGET Boost::filesystem)
  add_library(Boost::filesystem STATIC IMPORTED)

  set_target_properties(Boost::filesystem PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${_BOOST_INCLUDEDIR}"
    INTERFACE_COMPILE_DEFINITIONS "BOOST_FILESYSTEM_NO_LIB"
  )
endif()

# Target file name: libboost_filesystem-vc142-mt-sgd-x32-1_80.lib

get_target_property(__boost_imploc Boost::filesystem IMPORTED_LOCATION_DEBUG)
if(__boost_imploc)
  message(SEND_ERROR "Target Boost::filesystem already has an imported location '${__boost_imploc}', which is being overwritten with '${_BOOST_LIBDIR}/libboost_filesystem-vc142-mt-sgd-x32-1_80.lib'")
endif()
unset(__boost_imploc)

set_property(TARGET Boost::filesystem APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)

set_target_properties(Boost::filesystem PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG CXX
  IMPORTED_LOCATION_DEBUG "${_BOOST_LIBDIR}/libboost_filesystem-vc142-mt-sgd-x32-1_80.lib"
  )

list(APPEND _BOOST_FILESYSTEM_DEPS atomic headers)

if(CMAKE_CONFIGURATION_TYPES)
  set_property(TARGET Boost::filesystem APPEND PROPERTY INTERFACE_LINK_LIBRARIES
    "$<$<CONFIG:debug>:bcrypt;synchronization>")
else()
  set_property(TARGET Boost::filesystem APPEND PROPERTY INTERFACE_LINK_LIBRARIES
    bcrypt synchronization)
endif()