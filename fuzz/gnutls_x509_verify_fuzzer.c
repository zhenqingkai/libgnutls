/*
 * Copyright (C) 2019 Red Hat, Inc.
 *
 * Author: Nikos Mavrogiannopoulos
 *
 * This file is part of GnuTLS.
 *
 * The GnuTLS is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>
 *
 */

#include <assert.h>
#include <stdint.h>

#include <gnutls/gnutls.h>
#include <gnutls/x509.h>
#include <stdio.h>
#include <stdlib.h>

#include "fuzzer.h"

unsigned char _ca_der[] = {
  0x30, 0x82, 0x05, 0xe2, 0x30, 0x82, 0x04, 0xca, 0xa0, 0x03, 0x02, 0x01,
  0x02, 0x02, 0x10, 0x27, 0x53, 0xf8, 0x06, 0xfe, 0x91, 0x37, 0x32, 0x2f,
  0x5b, 0x40, 0x9f, 0x1c, 0x31, 0x87, 0x5c, 0x30, 0x0d, 0x06, 0x09, 0x2a,
  0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x05, 0x05, 0x00, 0x30, 0x81,
  0xca, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02,
  0x55, 0x53, 0x31, 0x17, 0x30, 0x15, 0x06, 0x03, 0x55, 0x04, 0x0a, 0x13,
  0x0e, 0x56, 0x65, 0x72, 0x69, 0x53, 0x69, 0x67, 0x6e, 0x2c, 0x20, 0x49,
  0x6e, 0x63, 0x2e, 0x31, 0x1f, 0x30, 0x1d, 0x06, 0x03, 0x55, 0x04, 0x0b,
  0x13, 0x16, 0x56, 0x65, 0x72, 0x69, 0x53, 0x69, 0x67, 0x6e, 0x20, 0x54,
  0x72, 0x75, 0x73, 0x74, 0x20, 0x4e, 0x65, 0x74, 0x77, 0x6f, 0x72, 0x6b,
  0x31, 0x3a, 0x30, 0x38, 0x06, 0x03, 0x55, 0x04, 0x0b, 0x13, 0x31, 0x28,
  0x63, 0x29, 0x20, 0x32, 0x30, 0x30, 0x36, 0x20, 0x56, 0x65, 0x72, 0x69,
  0x53, 0x69, 0x67, 0x6e, 0x2c, 0x20, 0x49, 0x6e, 0x63, 0x2e, 0x20, 0x2d,
  0x20, 0x46, 0x6f, 0x72, 0x20, 0x61, 0x75, 0x74, 0x68, 0x6f, 0x72, 0x69,
  0x7a, 0x65, 0x64, 0x20, 0x75, 0x73, 0x65, 0x20, 0x6f, 0x6e, 0x6c, 0x79,
  0x31, 0x45, 0x30, 0x43, 0x06, 0x03, 0x55, 0x04, 0x03, 0x13, 0x3c, 0x56,
  0x65, 0x72, 0x69, 0x53, 0x69, 0x67, 0x6e, 0x20, 0x43, 0x6c, 0x61, 0x73,
  0x73, 0x20, 0x33, 0x20, 0x50, 0x75, 0x62, 0x6c, 0x69, 0x63, 0x20, 0x50,
  0x72, 0x69, 0x6d, 0x61, 0x72, 0x79, 0x20, 0x43, 0x65, 0x72, 0x74, 0x69,
  0x66, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x20, 0x41, 0x75, 0x74,
  0x68, 0x6f, 0x72, 0x69, 0x74, 0x79, 0x20, 0x2d, 0x20, 0x47, 0x35, 0x30,
  0x1e, 0x17, 0x0d, 0x31, 0x30, 0x30, 0x32, 0x30, 0x38, 0x30, 0x30, 0x30,
  0x30, 0x30, 0x30, 0x5a, 0x17, 0x0d, 0x32, 0x30, 0x30, 0x32, 0x30, 0x37,
  0x32, 0x33, 0x35, 0x39, 0x35, 0x39, 0x5a, 0x30, 0x81, 0xb4, 0x31, 0x0b,
  0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x55, 0x53, 0x31,
  0x17, 0x30, 0x15, 0x06, 0x03, 0x55, 0x04, 0x0a, 0x13, 0x0e, 0x56, 0x65,
  0x72, 0x69, 0x53, 0x69, 0x67, 0x6e, 0x2c, 0x20, 0x49, 0x6e, 0x63, 0x2e,
  0x31, 0x1f, 0x30, 0x1d, 0x06, 0x03, 0x55, 0x04, 0x0b, 0x13, 0x16, 0x56,
  0x65, 0x72, 0x69, 0x53, 0x69, 0x67, 0x6e, 0x20, 0x54, 0x72, 0x75, 0x73,
  0x74, 0x20, 0x4e, 0x65, 0x74, 0x77, 0x6f, 0x72, 0x6b, 0x31, 0x3b, 0x30,
  0x39, 0x06, 0x03, 0x55, 0x04, 0x0b, 0x13, 0x32, 0x54, 0x65, 0x72, 0x6d,
  0x73, 0x20, 0x6f, 0x66, 0x20, 0x75, 0x73, 0x65, 0x20, 0x61, 0x74, 0x20,
  0x68, 0x74, 0x74, 0x70, 0x73, 0x3a, 0x2f, 0x2f, 0x77, 0x77, 0x77, 0x2e,
  0x76, 0x65, 0x72, 0x69, 0x73, 0x69, 0x67, 0x6e, 0x2e, 0x63, 0x6f, 0x6d,
  0x2f, 0x72, 0x70, 0x61, 0x20, 0x28, 0x63, 0x29, 0x31, 0x30, 0x31, 0x2e,
  0x30, 0x2c, 0x06, 0x03, 0x55, 0x04, 0x03, 0x13, 0x25, 0x56, 0x65, 0x72,
  0x69, 0x53, 0x69, 0x67, 0x6e, 0x20, 0x43, 0x6c, 0x61, 0x73, 0x73, 0x20,
  0x33, 0x20, 0x43, 0x6f, 0x64, 0x65, 0x20, 0x53, 0x69, 0x67, 0x6e, 0x69,
  0x6e, 0x67, 0x20, 0x32, 0x30, 0x31, 0x30, 0x20, 0x43, 0x41, 0x30, 0x82,
  0x01, 0x22, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d,
  0x01, 0x01, 0x01, 0x05, 0x00, 0x03, 0x82, 0x01, 0x0f, 0x00, 0x30, 0x82,
  0x01, 0x0a, 0x02, 0x82, 0x01, 0x01, 0x00, 0xf5, 0x23, 0x4b, 0x5e, 0xa5,
  0xd7, 0x8a, 0xbb, 0x32, 0xe9, 0xd4, 0x57, 0xf7, 0xef, 0xe4, 0xc7, 0x26,
  0x7e, 0xad, 0x19, 0x98, 0xfe, 0xa8, 0x9d, 0x7d, 0x94, 0xf6, 0x36, 0x6b,
  0x10, 0xd7, 0x75, 0x81, 0x30, 0x7f, 0x04, 0x68, 0x7f, 0xcb, 0x2b, 0x75,
  0x1e, 0xcd, 0x1d, 0x08, 0x8c, 0xdf, 0x69, 0x94, 0xa7, 0x37, 0xa3, 0x9c,
  0x7b, 0x80, 0xe0, 0x99, 0xe1, 0xee, 0x37, 0x4d, 0x5f, 0xce, 0x3b, 0x14,
  0xee, 0x86, 0xd4, 0xd0, 0xf5, 0x27, 0x35, 0xbc, 0x25, 0x0b, 0x38, 0xa7,
  0x8c, 0x63, 0x9d, 0x17, 0xa3, 0x08, 0xa5, 0xab, 0xb0, 0xfb, 0xcd, 0x6a,
  0x62, 0x82, 0x4c, 0xd5, 0x21, 0xda, 0x1b, 0xd9, 0xf1, 0xe3, 0x84, 0x3b,
  0x8a, 0x2a, 0x4f, 0x85, 0x5b, 0x90, 0x01, 0x4f, 0xc9, 0xa7, 0x76, 0x10,
  0x7f, 0x27, 0x03, 0x7c, 0xbe, 0xae, 0x7e, 0x7d, 0xc1, 0xdd, 0xf9, 0x05,
  0xbc, 0x1b, 0x48, 0x9c, 0x69, 0xe7, 0xc0, 0xa4, 0x3c, 0x3c, 0x41, 0x00,
  0x3e, 0xdf, 0x96, 0xe5, 0xc5, 0xe4, 0x94, 0x71, 0xd6, 0x55, 0x01, 0xc7,
  0x00, 0x26, 0x4a, 0x40, 0x3c, 0xb5, 0xa1, 0x26, 0xa9, 0x0c, 0xa7, 0x6d,
  0x80, 0x8e, 0x90, 0x25, 0x7b, 0xcf, 0xbf, 0x3f, 0x1c, 0xeb, 0x2f, 0x96,
  0xfa, 0xe5, 0x87, 0x77, 0xc6, 0xb5, 0x56, 0xb2, 0x7a, 0x3b, 0x54, 0x30,
  0x53, 0x1b, 0xdf, 0x62, 0x34, 0xff, 0x1e, 0xd1, 0xf4, 0x5a, 0x93, 0x28,
  0x85, 0xe5, 0x4c, 0x17, 0x4e, 0x7e, 0x5b, 0xfd, 0xa4, 0x93, 0x99, 0x7f,
  0xdf, 0xcd, 0xef, 0xa4, 0x75, 0xef, 0xef, 0x15, 0xf6, 0x47, 0xe7, 0xf8,
  0x19, 0x72, 0xd8, 0x2e, 0x34, 0x1a, 0xa6, 0xb4, 0xa7, 0x4c, 0x7e, 0xbd,
  0xbb, 0x4f, 0x0c, 0x3d, 0x57, 0xf1, 0x30, 0xd6, 0xa6, 0x36, 0x8e, 0xd6,
  0x80, 0x76, 0xd7, 0x19, 0x2e, 0xa5, 0xcd, 0x7e, 0x34, 0x2d, 0x89, 0x02,
  0x03, 0x01, 0x00, 0x01, 0xa3, 0x82, 0x01, 0xd6, 0x30, 0x82, 0x01, 0xd2,
  0x30, 0x12, 0x06, 0x03, 0x55, 0x1d, 0x13, 0x01, 0x01, 0xff, 0x04, 0x08,
  0x30, 0x06, 0x01, 0x01, 0xff, 0x02, 0x01, 0x00, 0x30, 0x70, 0x06, 0x03,
  0x55, 0x1d, 0x20, 0x04, 0x69, 0x30, 0x67, 0x30, 0x65, 0x06, 0x0b, 0x60,
  0x86, 0x48, 0x01, 0x86, 0xf8, 0x45, 0x01, 0x07, 0x17, 0x03, 0x30, 0x56,
  0x30, 0x28, 0x06, 0x08, 0x2b, 0x06, 0x01, 0x05, 0x05, 0x07, 0x02, 0x01,
  0x16, 0x1c, 0x68, 0x74, 0x74, 0x70, 0x73, 0x3a, 0x2f, 0x2f, 0x77, 0x77,
  0x77, 0x2e, 0x76, 0x65, 0x72, 0x69, 0x73, 0x69, 0x67, 0x6e, 0x2e, 0x63,
  0x6f, 0x6d, 0x2f, 0x63, 0x70, 0x73, 0x30, 0x2a, 0x06, 0x08, 0x2b, 0x06,
  0x01, 0x05, 0x05, 0x07, 0x02, 0x02, 0x30, 0x1e, 0x1a, 0x1c, 0x68, 0x74,
  0x74, 0x70, 0x73, 0x3a, 0x2f, 0x2f, 0x77, 0x77, 0x77, 0x2e, 0x76, 0x65,
  0x72, 0x69, 0x73, 0x69, 0x67, 0x6e, 0x2e, 0x63, 0x6f, 0x6d, 0x2f, 0x72,
  0x70, 0x61, 0x30, 0x0e, 0x06, 0x03, 0x55, 0x1d, 0x0f, 0x01, 0x01, 0xff,
  0x04, 0x04, 0x03, 0x02, 0x01, 0x06, 0x30, 0x6d, 0x06, 0x08, 0x2b, 0x06,
  0x01, 0x05, 0x05, 0x07, 0x01, 0x0c, 0x04, 0x61, 0x30, 0x5f, 0xa1, 0x5d,
  0xa0, 0x5b, 0x30, 0x59, 0x30, 0x57, 0x30, 0x55, 0x16, 0x09, 0x69, 0x6d,
  0x61, 0x67, 0x65, 0x2f, 0x67, 0x69, 0x66, 0x30, 0x21, 0x30, 0x1f, 0x30,
  0x07, 0x06, 0x05, 0x2b, 0x0e, 0x03, 0x02, 0x1a, 0x04, 0x14, 0x8f, 0xe5,
  0xd3, 0x1a, 0x86, 0xac, 0x8d, 0x8e, 0x6b, 0xc3, 0xcf, 0x80, 0x6a, 0xd4,
  0x48, 0x18, 0x2c, 0x7b, 0x19, 0x2e, 0x30, 0x25, 0x16, 0x23, 0x68, 0x74,
  0x74, 0x70, 0x3a, 0x2f, 0x2f, 0x6c, 0x6f, 0x67, 0x6f, 0x2e, 0x76, 0x65,
  0x72, 0x69, 0x73, 0x69, 0x67, 0x6e, 0x2e, 0x63, 0x6f, 0x6d, 0x2f, 0x76,
  0x73, 0x6c, 0x6f, 0x67, 0x6f, 0x2e, 0x67, 0x69, 0x66, 0x30, 0x34, 0x06,
  0x03, 0x55, 0x1d, 0x1f, 0x04, 0x2d, 0x30, 0x2b, 0x30, 0x29, 0xa0, 0x27,
  0xa0, 0x25, 0x86, 0x23, 0x68, 0x74, 0x74, 0x70, 0x3a, 0x2f, 0x2f, 0x63,
  0x72, 0x6c, 0x2e, 0x76, 0x65, 0x72, 0x69, 0x73, 0x69, 0x67, 0x6e, 0x2e,
  0x63, 0x6f, 0x6d, 0x2f, 0x70, 0x63, 0x61, 0x33, 0x2d, 0x67, 0x35, 0x2e,
  0x63, 0x72, 0x6c, 0x30, 0x28, 0x06, 0x03, 0x55, 0x1d, 0x11, 0x04, 0x21,
  0x30, 0x1f, 0xa4, 0x1d, 0x30, 0x1b, 0x31, 0x19, 0x30, 0x17, 0x06, 0x03,
  0x55, 0x04, 0x03, 0x13, 0x10, 0x56, 0x65, 0x72, 0x69, 0x53, 0x69, 0x67,
  0x6e, 0x4d, 0x50, 0x4b, 0x49, 0x2d, 0x32, 0x2d, 0x38, 0x30, 0x1d, 0x06,
  0x03, 0x55, 0x1d, 0x0e, 0x04, 0x16, 0x04, 0x14, 0xcf, 0x99, 0xa9, 0xea,
  0x7b, 0x26, 0xf4, 0x4b, 0xc9, 0x8e, 0x8f, 0xd7, 0xf0, 0x05, 0x26, 0xef,
  0xe3, 0xd2, 0xa7, 0x9d, 0x30, 0x2b, 0x06, 0x03, 0x55, 0x1d, 0x25, 0x04,
  0x24, 0x30, 0x22, 0x06, 0x08, 0x2b, 0x06, 0x01, 0x05, 0x05, 0x07, 0x03,
  0x02, 0x06, 0x08, 0x2b, 0x06, 0x01, 0x05, 0x05, 0x07, 0x03, 0x03, 0x06,
  0x0c, 0x2a, 0x86, 0x3a, 0x00, 0x01, 0x83, 0x8f, 0x89, 0x0d, 0x01, 0x01,
  0x01, 0x30, 0x1f, 0x06, 0x03, 0x55, 0x1d, 0x23, 0x04, 0x18, 0x30, 0x16,
  0x80, 0x14, 0x7f, 0xd3, 0x65, 0xa7, 0xc2, 0xdd, 0xec, 0xbb, 0xf0, 0x30,
  0x09, 0xf3, 0x43, 0x39, 0xfa, 0x02, 0xaf, 0x33, 0x31, 0x33, 0x30, 0x0d,
  0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x05, 0x05,
  0x00, 0x03, 0x82, 0x01, 0x01, 0x00, 0x5b, 0x8e, 0x9f, 0xd3, 0xba, 0xbe,
  0xa9, 0xaf, 0x1a, 0x3e, 0x65, 0x81, 0xb7, 0xc1, 0x64, 0xf6, 0xa2, 0x78,
  0xeb, 0x4c, 0x80, 0x06, 0x3a, 0xac, 0x22, 0x6e, 0x8c, 0x2b, 0xbc, 0x5d,
  0x85, 0x7f, 0xc0, 0x8f, 0x10, 0x2a, 0xca, 0xc4, 0xad, 0x96, 0x24, 0x01,
  0x68, 0xff, 0x65, 0xb5, 0xd1, 0x14, 0x2c, 0x2a, 0xa5, 0xda, 0x49, 0x96,
  0xaf, 0xe9, 0xd4, 0xff, 0x71, 0x9c, 0x93, 0x09, 0x4a, 0x6a, 0x61, 0xf4,
  0x4b, 0xd1, 0xcb, 0xb6, 0x5f, 0x29, 0xfc, 0x68, 0x11, 0xe5, 0x72, 0xff,
  0x46, 0x0f, 0x36, 0x29, 0x99, 0x43, 0x9e, 0xb3, 0xd9, 0xac, 0x47, 0x93,
  0x7b, 0xef, 0xc4, 0xeb, 0x66, 0x86, 0xa7, 0xd6, 0x89, 0x54, 0x3e, 0x94,
  0x9b, 0x0d, 0x91, 0xf0, 0xd2, 0x23, 0x6a, 0xc0, 0x0d, 0x37, 0xce, 0x46,
  0xdf, 0x95, 0xae, 0xa4, 0x65, 0xc6, 0x18, 0xbf, 0xa4, 0x69, 0x73, 0x3f,
  0xe8, 0x85, 0xd0, 0x48, 0x7d, 0x28, 0xef, 0xf3, 0x5f, 0xb0, 0x01, 0xb4,
  0x75, 0xb4, 0xd1, 0x53, 0x4b, 0xf4, 0x6f, 0xf6, 0xa4, 0x8b, 0x66, 0xb6,
  0xef, 0x78, 0x39, 0xc5, 0x88, 0x2b, 0x67, 0x32, 0x1a, 0xef, 0x40, 0xc0,
  0x94, 0xf6, 0xb7, 0xd2, 0x8b, 0x84, 0xa0, 0x26, 0x6f, 0x8a, 0x4b, 0xc5,
  0xd8, 0xdc, 0x54, 0xee, 0xe2, 0x4a, 0x56, 0xaa, 0xbc, 0x72, 0xe5, 0x93,
  0xe6, 0x3f, 0xb6, 0xde, 0x6d, 0x6c, 0x7b, 0x25, 0x5b, 0x01, 0xec, 0x6f,
  0x29, 0x64, 0xb7, 0xf3, 0x2e, 0x23, 0xf6, 0x1f, 0xae, 0x11, 0x2d, 0xc6,
  0xba, 0xb9, 0xeb, 0x08, 0x60, 0xb1, 0x2d, 0x1b, 0x2b, 0x9e, 0xbe, 0x85,
  0x74, 0x0a, 0x1d, 0x2e, 0x2a, 0x52, 0x91, 0x18, 0x20, 0xd7, 0xc9, 0xb2,
  0x97, 0x84, 0x8f, 0x16, 0x2c, 0x32, 0xca, 0x18, 0x12, 0xaa, 0xc5, 0x20,
  0x59, 0xca, 0xfb, 0x6d, 0x37, 0x32, 0xe3, 0xc5, 0xa3, 0x84
};
unsigned int _ca_der_len = 1510;

static time_t mytime(time_t * t)
{
	time_t then = 1553835274;
	if (t)
		*t = then;

	return then;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
	gnutls_datum_t raw;
	gnutls_datum_t ca;
	gnutls_x509_crt_t crt, ca_crt;
	int ret;
	unsigned status = 0;

	gnutls_global_set_time_function(mytime);

	raw.data = (unsigned char *)data;
	raw.size = size;

	ca.data = _ca_der;
	ca.size = _ca_der_len;

	ret = gnutls_x509_crt_init(&ca_crt);
	assert(ret >= 0);

	ret = gnutls_x509_crt_init(&crt);
	assert(ret >= 0);

	ret = gnutls_x509_crt_import(ca_crt, &ca, GNUTLS_X509_FMT_DER);
	if (ret < 0)
		goto fail;

	ret = gnutls_x509_crt_import(crt, &raw, GNUTLS_X509_FMT_DER);
	if (ret < 0)
		goto fail;

	ret = gnutls_x509_crt_list_verify(&crt, 1, &ca_crt, 1, NULL, 0,
					  GNUTLS_VERIFY_ALLOW_UNSORTED_CHAIN|GNUTLS_VERIFY_ALLOW_BROKEN|
					  GNUTLS_VERIFY_DISABLE_TRUSTED_TIME_CHECKS,
					  &status);
	assert(ret < 0 || status != 0);

 fail:
	gnutls_x509_crt_deinit(crt);
	gnutls_x509_crt_deinit(ca_crt);
	return 0;
}
