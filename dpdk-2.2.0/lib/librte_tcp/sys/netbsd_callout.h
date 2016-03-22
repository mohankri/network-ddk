/*	$NetBSD: callout.h,v 1.31 2008/04/28 20:24:10 martin Exp $	*/

/*-
 * Copyright (c) 2000, 2003, 2006, 2007, 2008 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Jason R. Thorpe of the Numerical Aerospace Simulation Facility,
 * NASA Ames Research Center, and by Andrew Doran.
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

#ifndef NETBSD_SYS_CALLOUT_H
#define NETBSD_SYS_CALLOUT_H

#include <netbsd_types.h>
#include <sys/netbsd_queue.h>

#define CALLOUT_BOUND		0x01
#define CALLOUT_PENDING		0x02
#define CALLOUT_FIRED		0x04
#define CALLOUT_INVOKING	0x08

typedef struct callout {
	uint64_t c_time;
	void *c_arg;
	void (*c_func)(void *);
	TAILQ_ENTRY(callout) c_list;
	uint16_t c_flags;
} callout_t;

// struct cpu_info;

void	callout_startup(void);
// void	callout_init_cpu(struct cpu_info *);
void	callout_hardclock(void);

void	callout_init(callout_t *, u_int);
void	callout_destroy(callout_t *);
void	callout_setfunc(callout_t *, void (*)(void *), void *);
void	callout_reset(callout_t *, unsigned long long, void (*)(void *), void *);
void	callout_schedule(callout_t *, unsigned long long);
bool	callout_stop(callout_t *);
bool	callout_halt(callout_t *, void *);
bool	callout_pending(callout_t *);
bool	callout_expired(callout_t *);
bool	callout_active(callout_t *);
bool	callout_invoking(callout_t *);
void	callout_ack(callout_t *);
void 	callout_run(void);
// void	callout_bind(callout_t *, struct cpu_info *);

#endif /* !NETBSD_SYS_CALLOUT_H */
