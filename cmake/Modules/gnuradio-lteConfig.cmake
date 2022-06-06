find_package(PkgConfig)

PKG_CHECK_MODULES(PC_GR_LTE gnuradio-lte)

FIND_PATH(
    GR_LTE_INCLUDE_DIRS
    NAMES gnuradio/lte/api.h
    HINTS $ENV{LTE_DIR}/include
        ${PC_LTE_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    GR_LTE_LIBRARIES
    NAMES gnuradio-lte
    HINTS $ENV{LTE_DIR}/lib
        ${PC_LTE_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/gnuradio-lteTarget.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GR_LTE DEFAULT_MSG GR_LTE_LIBRARIES GR_LTE_INCLUDE_DIRS)
MARK_AS_ADVANCED(GR_LTE_LIBRARIES GR_LTE_INCLUDE_DIRS)
