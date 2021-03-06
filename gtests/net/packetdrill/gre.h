/*
 * Copyright 2013 Google Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */
/*
 * Author: ncardwell@google.com (Neal Cardwell)
 *
 * Our own GRE header declarations, so we have something that's
 * portable and somewhat more readable than a typical system header
 * file.
 *
 * We cannot include the kernel's GRE .h files because this tool tries
 * to compile and work for basically any Linux/BSD kernel version. So
 * we declare our own version of various GRE-related definitions here.
 */

#ifndef __GRE_HEADERS_H__
#define __GRE_HEADERS_H__

#include "types.h"

/* GRE header. See RFC 1701. */
struct gre {
#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
	__u16	recursion_control:3,
		strict_route:1,
		has_seq:1,
		has_key:1,
		has_routing:1,
		has_checksum:1,
		version:3,
		reserved:4,
		ack:1;
#elif defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
	__u16	has_checksum:1,
		has_routing:1,
		has_key:1,
		has_seq:1,
		strict_route:1,
		recursion_control:3,
		ack:1,
		reserved:4,
		version:3;
#else
# error "Please fix endianness defines"
#endif
	__be16 protocol;
	/* bytes 4+ (optional) not defined here... */
};

/* Return the length in bytes of a GRE header. */
static inline int gre_len(const struct gre *gre)
{
	int bytes = sizeof(*gre);

	assert(gre->version == 0);	/* we only support v0 */
	assert(!gre->has_routing);	/* routing info is variable-length! */

	if (gre->has_checksum || gre->has_routing)
		bytes += 4;
	if (gre->has_key)
		bytes += 4;
	if (gre->has_seq)
		bytes += 4;

	return bytes;
}

#endif /* __GRE_HEADERS_H__ */
