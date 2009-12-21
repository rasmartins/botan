/*
* Byte Swapping Operations
* (C) 1999-2008 Jack Lloyd
* (C) 2007 Yves Jerschow
*
* Distributed under the terms of the Botan license
*/

#ifndef BOTAN_BYTE_SWAP_H__
#define BOTAN_BYTE_SWAP_H__

#include <botan/types.h>
#include <botan/rotate.h>

namespace Botan {

/*
* Byte Swapping Functions
*/
inline u16bit reverse_bytes(u16bit input)
   {
   return rotate_left(input, 8);
   }

inline u32bit reverse_bytes(u32bit input)
   {
#if BOTAN_USE_GCC_INLINE_ASM && (defined(BOTAN_TARGET_ARCH_IS_IA32) || \
                                 defined(BOTAN_TARGET_ARCH_IS_AMD64))

   // GCC-style inline assembly for x86 or x86-64
   asm("bswapl %0" : "=r" (input) : "0" (input));
   return input;

#elif defined(_MSC_VER) && defined(BOTAN_TARGET_ARCH_IS_IA32)
   // Visual C++ inline asm for 32-bit x86, by Yves Jerschow
   __asm mov eax, input;
   __asm bswap eax;

#else
   // Generic implementation
   return (rotate_right(input, 8) & 0xFF00FF00) |
          (rotate_left (input, 8) & 0x00FF00FF);
#endif
   }

inline u64bit reverse_bytes(u64bit input)
   {
#if BOTAN_USE_GCC_INLINE_ASM && defined(BOTAN_TARGET_ARCH_IS_AMD64)
   // GCC-style inline assembly for x86-64
   asm("bswapq %0" : "=r" (input) : "0" (input));
   return input;

#else
   /* Generic implementation. Defined in terms of 32-bit bswap so any
    * optimizations in that version can help here (particularly
    * useful for 32-bit x86).
    */

   u32bit hi = static_cast<u32bit>(input >> 32);
   u32bit lo = static_cast<u32bit>(input);

   hi = reverse_bytes(hi);
   lo = reverse_bytes(lo);

   return (static_cast<u64bit>(lo) << 32) | hi;
#endif
   }

}

#endif
