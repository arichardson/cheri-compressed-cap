/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2018-2020 Alex Richardson
 * All rights reserved.
 *
 * This software was developed by SRI International and the University of
 * Cambridge Computer Laboratory under DARPA/AFRL contract FA8750-10-C-0237
 * ("CTSRD"), as part of the DARPA CRASH research programme.
 *
 * This software was developed by SRI International and the University of
 * Cambridge Computer Laboratory (Department of Computer Science and
 * Technology) under DARPA contract HR0011-18-C-0016 ("ECATS"), as part of the
 * DARPA SSITH research programme.
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
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
#ifndef _cc_debug_assert
#ifdef cheri_debug_assert
#define _cc_debug_assert(cond) cheri_debug_assert(cond)
#else
#include <assert.h>
#define _cc_debug_assert(cond) assert(cond)
#endif
#endif

#ifndef _CC_CONCAT
#define _CC_CONCAT1(x, y) x##y
#define _CC_CONCAT(x, y) _CC_CONCAT1(x, y)
#define _CC_EXPAND1(x) x
#define _CC_EXPAND(x) _CC_EXPAND1(x)

#ifdef __cplusplus
// Some versions of GCC dont't like _Static_assert() in C++ mode
#define _CC_STATIC_ASSERT(cond, msg) static_assert(cond, msg)
#else
#define _CC_STATIC_ASSERT(cond, msg) _Static_assert(cond, msg)
#endif

#define _cc_N(name) _CC_CONCAT(_CC_CONCAT(_CC_CONCAT(cc, CC_BITS), _), name)
#define _CC_N(name) _CC_CONCAT(_CC_CONCAT(_CC_CONCAT(CC, CC_BITS), _), name)

#define _CC_BITMASK64(nbits) ((UINT64_C(1) << (nbits)) - UINT64_C(1))

#define _CC_MIN(a, b)                                                                                                  \
    ({                                                                                                                 \
        __typeof__(a) _a = (a);                                                                                        \
        __typeof__(b) _b = (b);                                                                                        \
        _a < _b ? _a : _b;                                                                                             \
    })
#define _CC_MAX(a, b)                                                                                                  \
    ({                                                                                                                 \
        __typeof__(a) _a = (a);                                                                                        \
        __typeof__(b) _b = (b);                                                                                        \
        _a > _b ? _a : _b;                                                                                             \
    })

#define _CC_FIELD(name, last, start)                                                                                   \
    _CC_N(FIELD_##name##_START) = (start - 64), _CC_N(FIELD_##name##_LAST) = (last - 64),                              \
    _CC_N(FIELD_##name##_SIZE) = _CC_N(FIELD_##name##_LAST) - _CC_N(FIELD_##name##_START) + 1,                         \
    _CC_N(FIELD_##name##_MASK_NOT_SHIFTED) = _CC_BITMASK64(_CC_N(FIELD_##name##_SIZE)),                                \
    _CC_N(FIELD_##name##_MASK64) = (uint64_t)_CC_N(FIELD_##name##_MASK_NOT_SHIFTED) << _CC_N(FIELD_##name##_START),    \
    _CC_N(FIELD_##name##_MAX_VALUE) = _CC_N(FIELD_##name##_MASK_NOT_SHIFTED)

#define _CC_ENCODE_FIELD(value, name)                                                                                  \
    ((uint64_t)((value)&_CC_N(FIELD_##name##_MAX_VALUE)) << _CC_N(FIELD_##name##_START))

#define _CC_EXTRACT_FIELD(value, name) cc128_getbits((value), _CC_N(FIELD_##name##_START), _CC_N(FIELD_##name##_SIZE))

#define _CC_ENCODE_EBT_FIELD(value, name)                                                                              \
    ((uint64_t)((value)&_CC_N(FIELD_##name##_MAX_VALUE)) << (_CC_N(FIELD_##name##_START) + _CC_N(FIELD_EBT_START)))

#define _CC_SPECIAL_OTYPE(name, subtract)                                                                              \
    _CC_N(name) = (_CC_N(MAX_REPRESENTABLE_OTYPE) - subtract##u), _CC_N(name##_SIGNED) = (((int64_t)-1) - subtract##u)
#endif // _CC_CONCAT
