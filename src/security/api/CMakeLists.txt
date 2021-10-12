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

add_library(security_api INTERFACE)

target_sources(security_api
  INTERFACE include/dds/security/dds_security_api.h
            include/dds/security/dds_security_api_access_control.h
            include/dds/security/dds_security_api_authentication.h
            include/dds/security/dds_security_api_cryptography.h
            include/dds/security/dds_security_api_defs.h
            include/dds/security/dds_security_api_err.h
            include/dds/security/dds_security_api_types.h)

target_include_directories(security_api INTERFACE include)