/*
 * Copyright (c) 2019, Tyler Nijmeh <tylernij@gmail.com>.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/dsboost.h>
#include <linux/input.h>
#include <linux/moduleparam.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/time.h>

static struct workqueue_struct *dsboost_wq;

static struct work_struct input_boost_work;
static struct work_struct cooldown_boost_work;
static struct delayed_work input_boost_rem;
static struct delayed_work cooldown_boost_rem;

static __read_mostly unsigned short input_boost_duration = CONFIG_INPUT_BOOST_DURATION;
static __read_mostly unsigned short cooldown_boost_duration = CONFIG_COOLDOWN_BOOST_DURATION;
static __read_mostly unsigned short input_stune_boost = CONFIG_INPUT_STUNE_BOOST;
static __read_mostly unsigned short cooldown_stune_boost = CONFIG_COOLDOWN_STUNE_BOOST;
static __read_mostly unsigned short sched_stune_boost = CONFIG_SCHED_STUNE_BOOST;

module_param(input_boost_duration, ushort, 0644);
module_param(cooldown_boost_duration, ushort, 0644);
module_param(input_stune_boost, ushort, 0644);
module_param(cooldown_stune_boost, ushort, 0644);
module_param(sched_stune_boost, ushort, 0644);

static int input_stune_slot;
static int cooldown_stune_slot;
static int sched_stune_slot;

static bool input_stune_boost_active;
static bool cooldown_stune_boost_active;
static bool sched_stune_boost_active;

static u64 last_input_time;

/* How long after an input before another input boost can be triggered */
#define MIN_INPUT_INTERVAL (input_boost_duration * USEC_PER_MSEC)

static void do_input_boost_rem(struct work_struct *work)
{
	if (cooldown_stune_boost)
		queue_work(dsboost_wq, &cooldown_boost_work);

	if (input_stune_boost_active)
		input_stune_boost_active = reset_stune_boost("top-app",
				input_stune_slot);
}

static void do_input_boost(struct work_struct *work)
{
	if (!cancel_delayed_work_sync(&input_boost_rem)) {
		if (!input_stune_boost_active)
			input_stune_boost_active = !do_stune_boost("top-app",
					input_stune_boost, &input_stune_slot);
	}

	queue_delayed_work(dsboost_wq, &input_boost_rem,
					msecs_to_jiffies(input_boost_duration));
}

static void do_cooldown_boost_rem(struct work_struct *work)
{
	if (cooldown_stune_boost_active)
		cooldown_stune_boost_active = reset_stune_boost("top-app",
				cooldown_stune_slot);
}

static void do_cooldown_boost(struct work_struct *work)
{
	if (!cancel_delayed_work_sync(&cooldown_boost_rem)) {
		if (!cooldown_stune_boost_active)
			cooldown_stune_boost_active = !do_stune_boost("top-app",
					cooldown_stune_boost, &cooldown_stune_slot);
	}

	queue_delayed_work(dsboost_wq, &cooldown_boost_rem,
					msecs_to_jiffies(cooldown_boost_duration));
}

void do_sched_boost_rem(void)
{
	if (sched_stune_boost_active)
		sched_stune_boost_active = reset_stune_boost("top-app",
				sched_stune_slot);
}

void do_sched_boost(void)
{
	if (!sched_stune_boost)
		return;

	if (!sched_stune_boost_active)
		sched_stune_boost_active = !do_stune_boost("top-app",
				sched_stune_boost, &sched_stune_slot);
}

static void dsboost_input_event(struct input_handle *handle,
		unsigned int type, unsigned int code, int value)
{
	u64 now;

	now = ktime_to_us(ktime_get());
	if (now - last_input_time < MIN_INPUT_INTERVAL)
		return;

	if (work_pending(&input_boost_work))
		return;

	if (input_stune_boost)
		queue_work(dsboost_wq, &input_boost_work);
	last_input_time = ktime_to_us(ktime_get());
}

static int dsboost_input_connect(struct input_handler *handler,
		struct input_dev *dev, const struct input_device_id *id)
{
	struct input_handle *handle;
	int error;

	handle = kzalloc(sizeof(struct input_handle), GFP_KERNEL);
	if (!handle)
		return -ENOMEM;

	handle->dev = dev;
	handle->handler = handler;
	handle->name = "cpufreq";

	error = input_register_handle(handle);
	if (error)
		goto err2;

	error = input_open_device(handle);
	if (error)
		goto err1;

	return 0;
err1:
	input_unregister_handle(handle);
err2:
	kfree(handle);
	return error;
}

static void dsboost_input_disconnect(struct input_handle *handle)
{
	input_close_device(handle);
	input_unregister_handle(handle);
	kfree(handle);
}

static const struct input_device_id dsboost_ids[] = {
	/* multi-touch touchscreen */
	{
		.flags = INPUT_DEVICE_ID_MATCH_EVBIT |
			INPUT_DEVICE_ID_MATCH_ABSBIT,
		.evbit = { BIT_MASK(EV_ABS) },
		.absbit = { [BIT_WORD(ABS_MT_POSITION_X)] =
			BIT_MASK(ABS_MT_POSITION_X) |
			BIT_MASK(ABS_MT_POSITION_Y) },
	},
	/* touchpad */
	{
		.flags = INPUT_DEVICE_ID_MATCH_KEYBIT |
			INPUT_DEVICE_ID_MATCH_ABSBIT,
		.keybit = { [BIT_WORD(BTN_TOUCH)] = BIT_MASK(BTN_TOUCH) },
		.absbit = { [BIT_WORD(ABS_X)] =
			BIT_MASK(ABS_X) | BIT_MASK(ABS_Y) },
	},
	/* Keypad */
	{
		.flags = INPUT_DEVICE_ID_MATCH_EVBIT,
		.evbit = { BIT_MASK(EV_KEY) },
	},
	{ },
};

static struct input_handler dsboost_input_handler = {
	.event          = dsboost_input_event,
	.connect        = dsboost_input_connect,
	.disconnect     = dsboost_input_disconnect,
	.name           = "dsboost",
	.id_table       = dsboost_ids,
};

static void dsboost_exit(void)
{
	input_unregister_handler(&dsboost_input_handler);
	destroy_workqueue(dsboost_wq);
}

static int dsboost_init(void)
{
	int ret;

	dsboost_wq = alloc_workqueue("dsboost_wq", WQ_HIGHPRI, 0);
	if (!dsboost_wq) {
		ret = -ENOMEM;
		goto err_wq;
	}

	INIT_WORK(&input_boost_work, do_input_boost);
	INIT_WORK(&cooldown_boost_work, do_cooldown_boost);
	INIT_DELAYED_WORK(&input_boost_rem, do_input_boost_rem);
	INIT_DELAYED_WORK(&cooldown_boost_rem, do_cooldown_boost_rem);

	ret = input_register_handler(&dsboost_input_handler);
	if (ret)
		goto err_input;

	return 0;
err_input:
	input_unregister_handler(&dsboost_input_handler);
err_wq:
	destroy_workqueue(dsboost_wq);

	return ret;
}
late_initcall(dsboost_init);
module_exit(dsboost_exit);
