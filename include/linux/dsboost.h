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

#ifndef _LINUX_DSBOOST_H
#define _LINUX_DSBOOST_H

#ifdef CONFIG_DYNAMIC_STUNE_BOOST
void do_sched_boost_rem(void);
void do_sched_boost(void);
#else
static inline void do_sched_boost_rem(void)
{
}
static inline void do_sched_boost(void)
{
}
#endif /* CONFIG_DYNAMIC_STUNE_BOOST */

#endif /* _LINUX_DSBOOST_H */
