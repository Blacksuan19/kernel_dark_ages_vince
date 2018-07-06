/*
 * Touchscreen State Notifier
 *
 * Copyright (C) 2013-2017, Pranav Vashi <neobuddy89@gmail.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef __TOUCHSCEEEN_STATE_NOTIFIER__
#define __TOUCHSCEEEN_STATE_NOTIFIER__

#include <linux/notifier.h>

#define STATE_NOTIFIER_ACTIVE		0x01
#define STATE_NOTIFIER_SUSPEND		0x02

struct state_event {
	void *data;
};

extern bool state_suspended;
extern void state_suspend(void);
extern void state_resume(void);
int state_register_client(struct notifier_block *nb);
int state_unregister_client(struct notifier_block *nb);
int state_notifier_call_chain(unsigned long val, void *v);

#endif /* __TOUCHSCEEEN_STATE_NOTIFIER__ */