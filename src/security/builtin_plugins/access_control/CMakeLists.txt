#
# Copyright(c) 2006 to 2019 ADLINK Technology Limited and others
#
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License v. 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0, or the Eclipse Distribution License
# v. 1.0 which is available at
# http://www.eclipse.org/org/documents/edl-v10.php.
#
# SPDX-License-Identifier: EPL-2.0 OR BSD-3-Clause
#
include(GenerateExportHeader)

add_library(dds_security_ac)

target_sources(dds_security_ac
  PRIVATE src/access_control_objects.c
          src/access_control_parser.c
          src/access_control_utils.c
          src/access_control.c
  PUBLIC  src/access_control_objects.h
          src/access_control_parser.h
          src/access_control_utils.h
          src/access_control.h
          ../include/ac_tokens.h)

generate_export_header(
    dds_security_ac
        BASE_NAME SECURITY
        EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/include/dds/security/export.h"
)

target_link_libraries(dds_security_ac
  PRIVATE security_openssl
  PUBLIC ddsc OpenSSL::SSL)

if(CMAKE_GENERATOR MATCHES "Visual Studio")
  set_target_properties(dds_security_ac PROPERTIES LINK_FLAGS "/ignore:4099")
endif()

add_coverage(dds_security_ac)

target_include_directories(dds_security_ac
    PUBLIC
        "$<BUILD_INTERFACE:$<TARGET_PROPERTY:security_api,INTERFACE_INCLUDE_DIRECTORIES>>"
        "$<BUILD_INTERFACE:$<TARGET_PROPERTY:security_core,INTERFACE_INCLUDE_DIRECTORIES>>"
        "$<BUILD_INTERFACE:$<TARGET_PROPERTY:security_openssl,INTERFACE_INCLUDE_DIRECTORIES>>"
        "$<BUILD_INTERFACE:$<TARGET_PROPERTY:ddsrt,INTERFACE_INCLUDE_DIRECTORIES>>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../include>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"
)

if (MSVC)
  install(FILES $<TARGET_PDB_FILE:dds_security_ac>
    DESTINATION "${CMAKE_INSTALL_BINDIR}"
    COMPONENT dev
    OPTIONAL
  )
endif()
