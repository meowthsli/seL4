/*
 * Copyright 2014, General Dynamics C4 Systems
 *
 * This software may be distributed and modified according to the terms of
 * the GNU General Public License version 2. Note that NO WARRANTY is provided.
 * See "LICENSE_GPLv2.txt" for details.
 *
 * @TAG(GD_GPL)
 */

#ifndef __ARCH_MACHINE_PL2_32_H
#define __ARCH_MACHINE_PL2_32_H

#include <config.h>
#include <arch/object/vcpu.h>

#ifdef CONFIG_ARM_HYPERVISOR_SUPPORT

/** MODIFIES: [*] */
/** DONT_TRANSLATE */
static inline void writeContextIDPL2(word_t id)
{
    word_t pd_val, vmid;
    asm volatile("mrrc p15, 4, %0, %1, c2" : "=r"(pd_val), "=r"(vmid));
    asm volatile("mcrr p15, 4, %0, %1, c2" : : "r"(pd_val), "r"(id << (48-32)));
    isb();
}

/** MODIFIES: [*] */
/** DONT_TRANSLATE */
static inline void writeContextIDAndPD(word_t id, word_t pd_val)
{
    asm volatile("mcrr p15, 6, %0, %1, c2"  : : "r"(pd_val), "r"(id << (48-32)));
    isb();
}


/** MODIFIES: [*] */
/** DONT_TRANSLATE */
static inline void setCurrentPDPL2(paddr_t addr)
{
    word_t pd_val, vmid;
    asm volatile("mrrc p15, 6, %0, %1, c2" : "=r"(pd_val), "=r"(vmid));
    dsb();
    asm volatile("mcrr p15, 6, %0, %1, c2" : : "r"(addr), "r"(vmid));
    isb();
}

/** MODIFIES: [*] */
/** DONT_TRANSLATE */
static inline void setCurrentHypPD(paddr_t addr)
{
    word_t zero = 0;
    dsb();
    asm volatile("mcrr p15, 4, %0, %1, c2" : : "r"(addr), "r"(zero));
    isb();
}

/** MODIFIES: [*] */
/** DONT_TRANSLATE */
static inline void setVTCR(word_t r)
{
    dsb();
    asm volatile("mcr p15, 4, %0, c2, c1, 2" : : "r"(r));
    isb();
}

/** MODIFIES: [*] */
/** DONT_TRANSLATE */
static inline void setHCR(word_t r)
{
    dsb();
    asm volatile("mcr p15, 4, %0, c1, c1, 0" : : "r"(r));
    isb();
}

/** MODIFIES: [*] */
/** DONT_TRANSLATE */
static inline void setHMAIR(word_t hmair0, word_t hmair1)
{
    asm volatile("mcr p15, 4, %0, c10, c2, 0" : : "r"(hmair0));
    asm volatile("mcr p15, 4, %0, c10, c2, 1" : : "r"(hmair1));
    isb();
}

/** MODIFIES: [*] */
/** DONT_TRANSLATE */
static inline void setMAIR(word_t hmair0, word_t hmair1)
{
    asm volatile("mcr p15, 0, %0, c10, c2, 0" : : "r"(hmair0));
    asm volatile("mcr p15, 0, %0, c10, c2, 1" : : "r"(hmair1));
    isb();
}

/** MODIFIES: [*] */
/** DONT_TRANSLATE */
static inline void invalidateHypTLB(void)
{
    dsb();
    asm volatile("mcr p15, 4, %0, c8, c7, 0" : : "r"(0));
    dsb();
    isb();
}

/** MODIFIES: [*] */
/** DONT_TRANSLATE */
static inline paddr_t PURE addressTranslateS1CPR(vptr_t vaddr)
{
    uint64_t ipa;
    asm volatile ("mcr  p15, 0, %0, c7, c8, 0" :: "r"(vaddr));
    isb();
    asm volatile ("mrrc p15, 0, %Q0, %R0, c7"   : "=r"(ipa));

    return ipa;
}

/** MODIFIES: [*] */
/** DONT_TRANSLATE */
static inline word_t PURE getHSR(void)
{
    word_t HSR;
    asm volatile("mrc p15, 4, %0, c5, c2, 0" : "=r"(HSR));
    return HSR;
}

/** MODIFIES: [*] */
/** DONT_TRANSLATE */
static inline word_t PURE getHDFAR(void)
{
    word_t HDFAR;
    asm volatile("mrc p15, 4, %0, c6, c0, 0" : "=r"(HDFAR));
    return HDFAR;
}

/** MODIFIES: [*] */
/** DONT_TRANSLATE */
static inline word_t PURE getHIFAR(void)
{
    word_t HIFAR;
    asm volatile("mrc p15, 4, %0, c6, c0, 2" : "=r"(HIFAR));
    return HIFAR;
}

/** MODIFIES: [*] */
/** DONT_TRANSLATE */
static inline word_t PURE getHPFAR(void)
{
    word_t HPFAR;
    asm volatile("mrc p15, 4, %0, c6, c0, 4" : "=r"(HPFAR));
    return HPFAR;
}

/** MODIFIES: [*] */
/** DONT_TRANSLATE */
static inline word_t getSCTLR(void)
{
    word_t SCTLR;
    asm volatile ("mrc p15, 0, %0, c1, c0, 0" : "=r"(SCTLR));
    return SCTLR;
}

/** MODIFIES: [*] */
/** DONT_TRANSLATE */
static inline void setSCTLR(word_t sctlr)
{
    asm volatile ("mcr p15, 0, %0, c1, c0, 0" :: "r"(sctlr));
}

/** MODIFIES: [*] */
/** DONT_TRANSLATE */
static inline word_t getACTLR(void)
{
    word_t ACTLR;
    asm volatile ("mrc p15, 0, %0, c1, c0, 1" : "=r"(ACTLR));
    return ACTLR;
}

/** MODIFIES: [*] */
/** DONT_TRANSLATE */
static inline void setACTLR(word_t actlr)
{
    asm volatile ("mcr p15, 0, %0, c1, c0, 1" :: "r"(actlr));
}

#else

/* used in other files without guards */
static inline void setCurrentPDPL2(paddr_t pa) {}
static inline void invalidateHypTLB(void) {}
static inline void writeContextIDPL2(word_t pd_val) {}
static inline void writeContextIDAndPD(word_t id, word_t pd_val) {}
static inline paddr_t addressTranslateS1CPR(vptr_t vaddr)
{
    return vaddr;
}

#endif /* !CONFIG_ARM_HYPERVISOR_SUPPORT */
#endif /* __ARCH_MACHINE_PL2_32_H */
