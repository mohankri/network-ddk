/*	$NetBSD: ansi.h,v 1.12 2008/04/28 20:24:10 martin Exp $	*/

/*-
 * Copyright (c) 2000, 2001, 2002 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Jun-ichiro itojun Hagino and by Klaus Klein.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef	NETBSD_SYS_ANSI_H
#define	NETBSD_SYS_ANSI_H
#include "netbsd_types.h"

typedef char *		__caddr_t;	/* core address */
typedef __uint32_t	__gid_t;	/* group id */
typedef __uint32_t	__in_addr_t;	/* IP(v4) address */
typedef __uint16_t	__in_port_t;	/* "Internet" port number */
typedef __uint32_t	__mode_t;	/* file permissions */
//typedef __int64_t	__off_t;	/* file offset */
typedef __int32_t	__pid_t;	/* process id */
typedef __uint8_t	__sa_family_t;	/* socket address family */
typedef unsigned int	__socklen_t;	/* socket-related datum length */
typedef __uint32_t	__uid_t;	/* user id */
//typedef	__uint64_t	__fsblkcnt_t;	/* fs block count (statvfs) */
//typedef	__uint64_t	__fsfilcnt_t;	/* fs file count */

#endif	/* !NETBSD_SYS_ANSI_H */
