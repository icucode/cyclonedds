#
# Copyright(c) 2006 to 2021 ADLINK Technology Limited and others
#
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License v. 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0, or the Eclipse Distribution License
# v. 1.0 which is available at
# http://www.eclipse.org/org/documents/edl-v10.php.
#
# SPDX-License-Identifier: EPL-2.0 OR BSD-3-Clause
#

add_library(security_core)

target_sources(security_core
  PRIVATE src/dds_security_serialize.c
          src/dds_security_utils.c
          src/dds_security_plugins.c
          src/shared_secret.c
          src/dds_security_fsm.c
          src/dds_security_timed_cb.c
  PUBLIC  include/dds/security/core/dds_security_fsm.h
          include/dds/security/core/dds_security_plugins.h
          include/dds/security/core/dds_security_serialize.h
          include/dds/security/core/dds_security_timed_cb.h
          include/dds/security/core/dds_security_types.h
          include/dds/security/core/dds_security_utils.h
          include/dds/security/core/shared_secret.h)

target_include_directories(security_core
  PUBLIC include
         $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>)

if(NOT WIN32)
  target_compile_options(security_core
    PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-fPIC>
            $<$<COMPILE_LANGUAGE:C>:-fPIC>)
endif()

target_link_libraries(security_core
  PUBLIC security_api ddsi_interface ddsc_interface ddsrt)

install(TARGETS security_core TYPE LIBRARY)

if(BUILD_TESTING AND BUILD_IDLC)
  add_subdirectory(tests)
endif()