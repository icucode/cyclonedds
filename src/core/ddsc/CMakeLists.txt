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

add_library(ddsc)
add_library(ddsc_interface INTERFACE)

include(GenerateExportHeader)
generate_export_header(
  ddsc BASE_NAME DDS EXPORT_FILE_NAME include/dds/export.h)

target_sources(ddsc
  PRIVATE src/dds__builtin.h
          src/dds__domain.h
          src/dds__entity.h
          src/dds__init.h
          src/dds__listener.h
          src/dds__participant.h
          src/dds__publisher.h
          src/dds__qos.h
          src/dds__querycond.h
          src/dds__readcond.h
          src/dds__guardcond.h
          src/dds__reader.h
          src/dds__rhc_default.h
          src/dds__statistics.h
          src/dds__subscriber.h
          src/dds__topic.h
          src/dds__write.h
          src/dds__writer.h
          src/dds__whc_builtintopic.h
          src/dds__serdata_builtintopic.h
          src/dds__get_status.h
          src/dds_alloc.c
          src/dds_builtin.c
          src/dds_coherent.c
          src/dds_participant.c
          src/dds_reader.c
          src/dds_writer.c
          src/dds_init.c
          src/dds_publisher.c
          src/dds_rhc.c
          src/dds_rhc_default.c
          src/dds_domain.c
          src/dds_instance.c
          src/dds_qos.c
          src/dds_handles.c
          src/dds_entity.c
          src/dds_matched.c
          src/dds_querycond.c
          src/dds_topic.c
          src/dds_listener.c
          src/dds_read.c
          src/dds_waitset.c
          src/dds_readcond.c
          src/dds_guardcond.c
          src/dds_statistics.c
          src/dds_subscriber.c
          src/dds_write.c
          src/dds_whc.c
          src/dds_whc_builtintopic.c
          src/dds_serdata_builtintopic.c
          src/dds_sertopic_builtintopic.c
  PUBLIC  include/dds__alloc.h
          include/dds__handles.h
          include/dds__types.h
          include/dds__whc.h
          include/dds/dds.h
          include/dds/ddsc/dds_public_error.h
          include/dds/ddsc/dds_public_impl.h
          include/dds/ddsc/dds_public_listener.h
          include/dds/ddsc/dds_public_qos.h
          include/dds/ddsc/dds_public_qosdefs.h
          include/dds/ddsc/dds_public_status.h
          include/dds/ddsc/dds_statistics.h
          include/dds/ddsc/dds_rhc.h
          include/dds/ddsc/dds_internal_api.h
          $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>$<INSTALL_INTERFACE:include>/dds/export.h)

if (DDS_HAS_SHM)
  target_sources(ddsc
    PRIVATE src/shm__monitor.h
            src/shm_monitor.c)
endif()

target_include_directories(ddsc
  PUBLIC include
         $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/src/core/include>)

target_include_directories(ddsc_interface
  INTERFACE include
            $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>)

target_link_libraries(ddsc PUBLIC ddsi ddsrt)

if("${CMAKE_C_COMPILER_ID}" STREQUAL "MSVC")
  target_link_libraries(ddsc PRIVATE dbghelp)
endif()

if(ENABLE_SSL AND OPENSSL_FOUND)
  target_link_libraries(ddsc PRIVATE OpenSSL::SSL)
  if(CMAKE_GENERATOR MATCHES "Visual Studio")
    set_target_properties(ddsc PROPERTIES LINK_FLAGS "/ignore:4099")
  endif()
endif()

if(iceoryx_binding_c_FOUND)
  target_link_libraries(ddsc PRIVATE iceoryx_binding_c::iceoryx_binding_c)
endif()

# SOVERSION should increase on incompatible ABI change
set_target_properties(ddsc PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION ${PROJECT_VERSION_MAJOR})

# define target property to indicate if Cyclone DDS is compiled with SHM support
define_property(TARGET
    PROPERTY SHM_SUPPORT_IS_AVAILABLE
    BRIEF_DOCS "Indicator whether SHM support is available with current Cyclone DDS build configuration."
    FULL_DOCS "Indicator whether SHM support is available with current Cyclone DDS build configuration."
    )
set_target_properties(ddsc PROPERTIES
    SHM_SUPPORT_IS_AVAILABLE "${DDS_HAS_SHM}")
set_target_properties(ddsc PROPERTIES EXPORT_PROPERTIES
    "SHM_SUPPORT_IS_AVAILABLE")

# Create a pseudo-target that other targets (i.e. examples, tests) can depend
# on and can also be provided as import-target by a package-file when building
# those targets outside the regular Cyclone build-tree (i.e. the installed tree)
add_library(${PROJECT_NAME}::ddsc ALIAS ddsc)

install(
  DIRECTORY
      "${CMAKE_CURRENT_LIST_DIR}/include/"
      "${CMAKE_CURRENT_BINARY_DIR}/include/"
  TYPE INCLUDE
  COMPONENT dev)

install(TARGETS ddsc TYPE LIBRARY)

if (MSVC)
  install(FILES $<TARGET_PDB_FILE:ddsc>
    DESTINATION "${CMAKE_INSTALL_BINDIR}"
    COMPONENT dev
    OPTIONAL
  )
endif()

if(BUILD_DOCS)
  set(DOXYGEN_GENERATE_HTML NO)
  set(DOXYGEN_GENERATE_XML YES)
  set(DOXYGEN_STRIP_FROM_PATH "ddsc/include/dds/")
  set(DOXYGEN_EXCLUDE_PATTERNS "*/tests/*")
  set(DOXYGEN_MACRO_EXPANSION YES)
  set(DOXYGEN_PREDEFINED
    "__restrict="
    "__attribute__="
    "__declspec(x)="
    "DDS_EXPORT="
    "DDS_INLINE_EXPORT="
    "DDS_DEPRECATED_EXPORT="
    "DDSRT_STATIC_ASSERT(x)=")
  find_package(Doxygen REQUIRED)
  doxygen_add_docs(ddsc_api_docs "ddsc/include/dds/")
endif()

# TODO: improve test inclusion.
if((BUILD_TESTING) AND (BUILD_IDLC) AND ((NOT DEFINED MSVC_VERSION) OR (MSVC_VERSION GREATER "1800")))
  add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/tests")
endif()
