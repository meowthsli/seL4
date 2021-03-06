/*
 * Copyright 2016, General Dynamics C4 Systems
 *
 * This software may be distributed and modified according to the terms of
 * the GNU General Public License version 2. Note that NO WARRANTY is provided.
 * See "LICENSE_GPLv2.txt" for details.
 *
 * @TAG(GD_GPL)
 */

#ifndef __KERNEL_TRAPS_H
#define __KERNEL_TRAPS_H

#include <config.h>
#include <util.h>
#include <arch/kernel/traps.h>

/* This C function should be the first thing called from C after entry from
 * assembly. It provides a single place to do any entry work that is not
 * done in assembly for various reasons */
static inline void c_entry_hook(void)
{
    arch_c_entry_hook();
#if defined(CONFIG_BENCHMARK_TRACK_KERNEL_ENTRIES) || defined(CONFIG_BENCHMARK_TRACK_UTILISATION)
    ksEnter = timestamp();
#endif
}

/* This C function should be the last thing called from C before exiting
 * the kernel (be it to assembly or returning to user space). It provides
 * a place to provide any additional instrumentation or functionality
 * in C before leaving the kernel */
static inline void c_exit_hook(void)
{
#ifdef CONFIG_BENCHMARK_TRACK_KERNEL_ENTRIES
    benchmark_track_exit();
#endif /* CONFIG_BENCHMARK_TRACK_KERNEL_ENTRIES */
    arch_c_exit_hook();
}

#endif /* __KERNEL_TRAPS_H */
