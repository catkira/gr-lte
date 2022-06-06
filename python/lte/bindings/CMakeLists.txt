# Copyright 2020 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

########################################################################
# Check if there is C++ code at all
########################################################################
if(NOT lte_sources)
    MESSAGE(STATUS "No C++ sources... skipping python bindings")
    return()
endif(NOT lte_sources)

########################################################################
# Check for pygccxml
########################################################################
GR_PYTHON_CHECK_MODULE_RAW(
    "pygccxml"
    "import pygccxml"
    PYGCCXML_FOUND
    )

include(GrPybind)

########################################################################
# Python Bindings
########################################################################
list(APPEND lte_python_files
    pss_tagger_python.cc
    pss_symbol_selector_cvc_python.cc
    pss_calculator_vcm_python.cc python_bindings.cc)

GR_PYBIND_MAKE_OOT(lte
   ../../..
   gr::lte
   "${lte_python_files}")

# copy bindings extension for use in QA test module
add_custom_command(TARGET lte_python POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:lte_python>
    ${CMAKE_BINARY_DIR}/test_modules/gnuradio/lte/
)

install(TARGETS lte_python DESTINATION ${GR_PYTHON_DIR}/gnuradio/lte COMPONENT pythonapi)
