/*	$NetBSD: if_tun.h,v 1.16 2008/04/24 15:35:31 ad Exp $	*/

/*
 * Copyright (c) 1988, Julian Onions <jpo@cs.nott.ac.uk>
 * Nottingham University 1987.
 *
 * This source may be freely distributed, however I would be interested
 * in any changes that are made.
 *
 * This driver takes packets off the IP i/f and hands them up to a
 * user process to have it's wicked way with. This driver has it's
 * roots in a similar driver written by Phil Cockcroft (formerly) at
 * UCL. This driver is based much more on read/write/select mode of
 * operation though.
 *
 * from: Header: if_tnreg.h,v 1.1.2.1 1992/07/16 22:39:16 friedl Exp
 */

#ifndef _NET_IF_TUN_H
#define _NET_IF_TUN_H

#ifdef _KERNEL
struct tun_softc {
	struct	ifnet tun_if;		/* the interface */

	u_short	tun_flags;		/* misc flags */
#define	TUN_OPEN	0x0001
#define	TUN_INITED	0x0002
#define	TUN_RCOLL	0x0004
#define	TUN_IASET	0x0008
#define	TUN_DSTADDR	0x0010
#define	TUN_RWAIT	0x0040
#define	TUN_ASYNC	0x0080
#define	TUN_NBIO	0x0100
#define	TUN_PREPADDR	0x0200
#define	TUN_IFHEAD	0x0400

#define	TUN_READY	(TUN_OPEN | TUN_INITED | TUN_IASET)

	pid_t	tun_pgid;		/* PID or process group ID */
	struct	selinfo	tun_rsel;	/* read select */
	struct	selinfo	tun_wsel;	/* write select (not used) */
	int	tun_unit;		/* the tunnel unit number */
	struct	simplelock tun_lock;	/* lock for this tunnel */
	LIST_ENTRY(tun_softc) tun_list;	/* list of all tuns */
	void	*tun_osih;		/* soft interrupt handle */
	void	*tun_isih;		/* soft interrupt handle */
};
#endif	/* _KERNEL */

/* Maximum packet size */
#define	TUNMTU		1500

/* ioctl's for get/set debug */
#define	TUNSDEBUG	_IOW('t', 90, int)
#define	TUNGDEBUG	_IOR('t', 89, int)
#define	TUNSIFMODE	_IOW('t', 88, int)
#define	TUNSLMODE	_IOW('t', 87, int)
#define	TUNSIFHEAD	_IOW('t', 66, int)
#define	TUNGIFHEAD	_IOR('t', 65, int)

#endif /* !_NET_IF_TUN_H */
