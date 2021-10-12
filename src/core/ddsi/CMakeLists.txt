#
# Copyright(c) 2006 to 2018 ADLINK Technology Limited and others
#
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License v. 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0, or the Eclipse Distribution License
# v. 1.0 which is available at
# http://www.eclipse.org/org/documents/edl-v10.php.
#
# SPDX-License-Identifier: EPL-2.0 OR BSD-3-Clause
#

add_library(ddsi)
add_library(ddsi_interface INTERFACE)

target_sources(ddsi
  PRIVATE src/ddsi_eth.c
          src/ddsi_ssl.c
          src/ddsi_tcp.c
          src/ddsi_tran.c
          src/ddsi_udp.c
          src/ddsi_raweth.c
          src/ddsi_ipaddr.c
          src/ddsi_mcgroup.c
          src/ddsi_security_util.c
          src/ddsi_security_omg.c
          src/ddsi_portmapping.c
          src/ddsi_handshake.c
          src/ddsi_serdata.c
          src/ddsi_serdata_default.c
          src/ddsi_serdata_pserop.c
          src/ddsi_serdata_plist.c
          src/ddsi_sertopic.c
          src/ddsi_sertopic_default.c
          src/ddsi_sertopic_plist.c
          src/ddsi_sertopic_pserop.c
          src/ddsi_statistics.c
          src/ddsi_iid.c
          src/ddsi_tkmap.c
          src/ddsi_vendor.c
          src/ddsi_threadmon.c
          src/ddsi_rhc.c
          src/ddsi_pmd.c
          src/ddsi_entity_index.c
          src/ddsi_deadline.c
          src/ddsi_deliver_locally.c
          src/ddsi_plist.c
          src/ddsi_cdrstream.c
          src/ddsi_time.c
          src/ddsi_ownip.c
          src/ddsi_acknack.c
          src/q_addrset.c
          src/q_bitset_inlines.c
          src/q_bswap.c
          src/q_config.c
          src/q_ddsi_discovery.c
          src/q_debmon.c
          src/q_entity.c
          src/q_gc.c
          src/q_init.c
          src/q_lat_estim.c
          src/q_lease.c
          src/q_misc.c
          src/q_pcap.c
          src/q_qosmatch.c
          src/q_radmin.c
          src/q_receive.c
          src/q_sockwaitset.c
          src/q_thread.c
          src/q_transmit.c
          src/q_inverse_uint32_set.c
          src/q_whc.c
          src/q_xevent.c
          src/q_xmsg.c
          src/q_freelist.c
          src/sysdeps.c
  PUBLIC  include/dds/ddsi/ddsi_ssl.h
          include/dds/ddsi/ddsi_tcp.h
          include/dds/ddsi/ddsi_tran.h
          include/dds/ddsi/ddsi_udp.h
          include/dds/ddsi/ddsi_raweth.h
          include/dds/ddsi/ddsi_ipaddr.h
          include/dds/ddsi/ddsi_mcgroup.h
          include/dds/ddsi/ddsi_plist_generic.h
          include/dds/ddsi/ddsi_security_util.h
          include/dds/ddsi/ddsi_security_omg.h
          include/dds/ddsi/ddsi_portmapping.h
          include/dds/ddsi/ddsi_handshake.h
          include/dds/ddsi/ddsi_serdata.h
          include/dds/ddsi/ddsi_serdata_default.h
          include/dds/ddsi/ddsi_serdata_pserop.h
          include/dds/ddsi/ddsi_serdata_plist.h
          include/dds/ddsi/ddsi_sertopic.h
          include/dds/ddsi/ddsi_statistics.h
          include/dds/ddsi/ddsi_iid.h
          include/dds/ddsi/ddsi_tkmap.h
          include/dds/ddsi/ddsi_vendor.h
          include/dds/ddsi/ddsi_threadmon.h
          include/dds/ddsi/ddsi_builtin_topic_if.h
          include/dds/ddsi/ddsi_rhc.h
          include/dds/ddsi/ddsi_guid.h
          include/dds/ddsi/ddsi_keyhash.h
          include/dds/ddsi/ddsi_entity_index.h
          include/dds/ddsi/ddsi_deadline.h
          include/dds/ddsi/ddsi_deliver_locally.h
          include/dds/ddsi/ddsi_domaingv.h
          include/dds/ddsi/ddsi_plist.h
          include/dds/ddsi/ddsi_xqos.h
          include/dds/ddsi/ddsi_cdrstream.h
          include/dds/ddsi/ddsi_time.h
          include/dds/ddsi/ddsi_ownip.h
          include/dds/ddsi/ddsi_cfgunits.h
          include/dds/ddsi/ddsi_cfgelems.h
          include/dds/ddsi/ddsi_acknack.h
          include/dds/ddsi/q_addrset.h
          include/dds/ddsi/q_bitset.h
          include/dds/ddsi/q_bswap.h
          include/dds/ddsi/q_config.h
          include/dds/ddsi/q_ddsi_discovery.h
          include/dds/ddsi/q_debmon.h
          include/dds/ddsi/q_entity.h
          include/dds/ddsi/q_feature_check.h
          include/dds/ddsi/q_freelist.h
          include/dds/ddsi/q_gc.h
          include/dds/ddsi/q_hbcontrol.h
          include/dds/ddsi/q_lat_estim.h
          include/dds/ddsi/q_lease.h
          include/dds/ddsi/q_log.h
          include/dds/ddsi/q_misc.h
          include/dds/ddsi/q_pcap.h
          include/dds/ddsi/q_protocol.h
          include/dds/ddsi/q_qosmatch.h
          include/dds/ddsi/q_radmin.h
          include/dds/ddsi/q_receive.h
          include/dds/ddsi/q_rtps.h
          include/dds/ddsi/q_sockwaitset.h
          include/dds/ddsi/q_thread.h
          include/dds/ddsi/q_transmit.h
          include/dds/ddsi/q_inverse_uint32_set.h
          include/dds/ddsi/q_unused.h
          include/dds/ddsi/q_whc.h
          include/dds/ddsi/q_xevent.h
          include/dds/ddsi/q_xmsg.h
          include/dds/ddsi/sysdeps.h)

if(ENABLE_LIFESPAN)
  target_sources(ddsi
    PRIVATE src/ddsi_lifespan.c
    PUBLIC  include/dds/ddsi/ddsi_lifespan.h)
endif()

if(ENABLE_DEADLINE_MISSED)
  target_sources(ddsi
    PRIVATE src/ddsi_deadline.c
    PUBLIC  include/dds/ddsi/ddsi_deadline.h)
endif()

if(ENABLE_TYPE_DISCOVERY)
  target_sources(ddsi
    PRIVATE src/ddsi_typelookup.c
            src/ddsi_typeid.c
    PUBLIC  include/dds/ddsi/ddsi_typeid.h
            include/dds/ddsi/ddsi_typelookup.h)
endif()

if(ENABLE_SECURITY)
  target_sources(ddsi
    PRIVATE src/ddsi_security_msg.c
            src/ddsi_security_exchange.c
    PUBLIC  include/dds/ddsi/ddsi_security_msg.h
            include/dds/ddsi/ddsi_security_exchange.h)

  target_compile_definitions(ddsi PUBLIC DDSI_INCLUDE_SECURITY)
endif()

if(iceoryx_binding_c_FOUND)
  target_sources(ddsi
    PRIVATE src/shm_init.c
            src/shm_sync.c
    PUBLIC  include/dds/ddsi/shm_sync.h)
endif()

target_include_directories(ddsi PUBLIC include)

target_include_directories(ddsi_interface INTERFACE include)

target_link_libraries(ddsi
  PRIVATE ddsrt security_core
  PUBLIC  ddsc_interface security_api)

set(defconfig_src "${CMAKE_CURRENT_BINARY_DIR}/defconfig.c")

add_custom_command(
  OUTPUT ${defconfig_src}
  COMMAND ddsconf-native -f defconfig -o ${defconfig_src}
  DEPENDS ddsconf-native)

install(
  DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/include/"
  TYPE INCLUDE
  COMPONENT dev)

install(TARGETS ddsi TYPE LIBRARY)

# TODO: improve test inclusion.
if((BUILD_TESTING) AND ((NOT DEFINED MSVC_VERSION) OR (MSVC_VERSION GREATER "1800")))
  add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/tests")
endif()
