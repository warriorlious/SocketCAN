/*
 * socketcan/can/dev.h
 *
 * Definitions for the CAN network device driver interface
 *
 * $Id$
 *
 * Copyright (C) 2006 Andrey Volkov <avolkov@varma-el.com>
 *               Varma Electronics Oy
 *
 * Copyright (C) 2008 Wolfgang Grandegger <wg@grandegger.com>
 *
 * Send feedback to <socketcan-users@lists.berlios.de>
 */

#ifndef CAN_DEV_H
#define CAN_DEV_H

#include <linux/version.h>
#include <socketcan/can/netlink.h>
#include <socketcan/can/error.h>

/*
 * CAN mode
 */
enum can_mode {
	CAN_MODE_STOP = 0,
	CAN_MODE_START,
	CAN_MODE_SLEEP
};

/*
 * CAN common private data
 */
struct can_priv {
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,23)
	struct net_device_stats net_stats;
#endif
	struct can_device_stats can_stats;

	struct can_bittiming bittiming;
	struct can_bittiming_const *bittiming_const;
	struct can_clock clock;

	enum can_state state;
	u32 ctrlmode;

	int restart_ms;
	struct timer_list restart_timer;

	int (*do_set_bittiming)(struct net_device *dev);
	int (*do_set_mode)(struct net_device *dev, enum can_mode mode);
	int (*do_get_state)(const struct net_device *dev,
			    enum can_state *state);

	unsigned int echo_skb_max;
	struct sk_buff **echo_skb;
};

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,21)
#define ND2D(_ndev)	(_ndev->class_dev.dev)
#else
#define ND2D(_ndev)	(_ndev->dev.parent)
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,25)
#define IFF_ECHO IFF_LOOPBACK
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,23)
struct net_device_stats *can_get_stats(struct net_device *dev);
#endif

struct net_device *alloc_candev(int sizeof_priv, unsigned int echo_skb_max);
void free_candev(struct net_device *dev);

int open_candev(struct net_device *dev);
void close_candev(struct net_device *dev);

int register_candev(struct net_device *dev);
void unregister_candev(struct net_device *dev);

int can_restart_now(struct net_device *dev);
void can_bus_off(struct net_device *dev);

void can_put_echo_skb(struct sk_buff *skb, struct net_device *dev,
		      unsigned int idx);
void can_get_echo_skb(struct net_device *dev, unsigned int idx);
void can_free_echo_skb(struct net_device *dev, unsigned int idx);

#endif /* CAN_DEV_H */
