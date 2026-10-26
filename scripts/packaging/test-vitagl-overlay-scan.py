#!/usr/bin/env python3
"""Functional SIMD checks on host vectors; actual ARM instructions checked at build."""
from pathlib import Path
import subprocess,tempfile
root=Path(__file__).resolve().parents[2]
with tempfile.TemporaryDirectory(prefix='c2-overlay-scan-') as directory:
    p=Path(directory)
    # Same lane-wise load/XOR/OR/reinterpret/store semantics as arm_neon.h.
    # These are host vectors, not an ARM emulator or GPU performance test.
    (p/'arm_neon.h').write_text('''#pragma once
#include <stdint.h>
#include <string.h>
typedef uint16_t uint16x8_t __attribute__((vector_size(16)));
typedef uint32_t uint32x4_t __attribute__((vector_size(16)));
typedef uint32_t uint32x2_t __attribute__((vector_size(8)));
static inline uint16x8_t vdupq_n_u16(uint16_t x) {return (uint16x8_t){x,x,x,x,x,x,x,x};}
static inline uint16x8_t vld1q_u16(const uint16_t* p) {uint16x8_t v;memcpy(&v,p,16);return v;}
static inline void vst1q_u16(uint16_t* p,uint16x8_t v) {memcpy(p,&v,16);}
static inline uint16x8_t veorq_u16(uint16x8_t a,uint16x8_t b) {return a^b;}
static inline uint16x8_t vorrq_u16(uint16x8_t a,uint16x8_t b) {return a|b;}
static inline uint32x4_t vreinterpretq_u32_u16(uint16x8_t a) {return (uint32x4_t)a;}
static inline uint32x2_t vget_low_u32(uint32x4_t a) {return (uint32x2_t){a[0],a[1]};}
static inline uint32x2_t vget_high_u32(uint32x4_t a) {return (uint32x2_t){a[2],a[3]};}
static inline uint32x2_t vorr_u32(uint32x2_t a,uint32x2_t b) {return a|b;}
static inline uint32_t vget_lane_u32(uint32x2_t a,int lane) {return a[lane];}
''')
    for defines in [[],['-D__ARM_NEON=1']]:
        subprocess.run(['gcc','-std=c11','-O2','-g','-fsanitize=address,undefined','-fno-pie','-no-pie',*defines,
            '-I'+str(p),'-I'+str(root/'src/brender/drivers/glrend'),
            str(root/'scripts/packaging/tests/vitagl_overlay_scan_main.c'),'-o',str(p/'test')],check=True)
        subprocess.run([str(p/'test')],check=True)
