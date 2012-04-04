////////////////////////////////////////////////////////////////////////
//                                                                    //
// Fast Code Library                                                  //
// -----------------                                                  //
//                                                                    // 
// This is a collection of highly optimized routines. Most of the     //
// code has been taken out of various open source projects and        //
// message boards. Some code is from myself ;-) Sice all the code     //
// included here is available on the web for people to learn, I       //
// assume that nobody dislikes the fact that I have collected it      //
// here. If you are author of the code and don't want to see it in    //
// this collection, please contact me !                               //
//                                                                    //
// Tim C. Schröder (tcs_web@gmx.de, tcs_web@hotmail.com)               //
// http://glvelocity.gamedev.net                                      //
//                                                                    //
// Credits                                                            //
// -------                                                            //
//                                                                    //
// - Titan Engine (http://talika.fie.us.es/~titan/)                   //
// - Fast Game Programming (http://members.aol.com/form1/index.html)  //
// - GameDev.net Forums (http://www.gamedev.net)                      //
// - nVidia Developer Section (http://www.nvidia.com/developer)       //
// - Paul Hsieh's sqrt (www.azillionmonkeys.com/qed/sqroot.html)      //
// - Golgotha Engine											      //
// - Unreal Engine (http://www.unreal.com/index2.html)                //
// - Quake 3 Engine (www.idsoftware.com/archives/quake3arc.html       //
//                                                                    //
////////////////////////////////////////////////////////////////////////
//
// commented and modified by seojt@kogsoft.com
//

#ifndef __FAST_CODE_H_INCLUDED__
#define __FAST_CODE_H_INCLUDED__

// Disable warning when a function returns a value inside an __asm block
//#pragma warning (disable : 4035)

#include <windows.h>
//#include <math.h> // for build up sqrt-table

// Needed to control floating-point pecision
#include <float.h>

////////////////////////////////////////////////////////////////////////
// Function prototypes
////////////////////////////////////////////////////////////////////////

// fast log function from flip code
__inline float fast_log2(float val);
__inline float fast_log(const float &val);

__inline int FixMul(int a, int b);

// DON'T USE: memset() is faster!
__inline void FastMemZero(void *Dest, int Count); 

// DON'T USE: memcpy() is faster
__inline void FastMemCpy(void *Dest, const void *Src, int Count);

__inline float __fastcall InverseSqrt(float a);
__inline int isqrt0 (unsigned long r);
__inline float RSqrt(float number);
//__forceinline float fastsqrt(float n); // use look-up table
__inline double __fastcall Inv_Sqrt(double x);
__forceinline float __cdecl DotProduct(const float v1[3], const float v2[3]);
__inline void FastNormVect2(float *v);
__inline void FastNormVect3(float *v);
__inline DWORD Log2(DWORD val);
__inline DWORD NextPowerOfTwo(DWORD N);
__forceinline float __fastcall FastCos(float a);
__forceinline float __fastcall FastSin(float a);
__forceinline float __fastcall FastAbs(float a);
__forceinline int asmifloor(float f);
__forceinline int __stdcall ifloor(float  x);

__forceinline BYTE __stdcall FloatToByte (float  x);

// Caution: it does round on
__forceinline int __stdcall FloatToIntRet(float  x); // fastest!

__forceinline int RoundFloatToInt (float f);
__forceinline void FloatToInt(int *int_pointer, float f);

//void BuildSqrtTable();

////////////////////////////////////////////////////////////////////////
// Casting - Taken from the www.gamedev.net forums and www.nvidia.com
////////////////////////////////////////////////////////////////////////

// Fast float to int conversion, NEVER cast with (int) when
// performance has any importance
__forceinline void FloatToInt(int *int_pointer, float f) 
{
	__asm  fld f
	__asm  mov edx, int_pointer
	__asm  FRNDINT
	__asm  fistp dword ptr [edx];
}

// Round a floating point number to an integer. Note that (int + .5)
// is rounded to (int + 1).
__forceinline int RoundFloatToInt (float f)
{
	int i;
	__asm fld [f]
	__asm fistp [i]
	return i;
}

// Doesn't take the pointer, is a bit faster
// Caution: it does round on
__forceinline int __stdcall FloatToIntRet(float  x)
{
	int	   t;
	__asm  fld   x  
	__asm  fistp t
	return t;
}
 
// Casting floats to unsigned chars is also very expensive, just
// NEVER cast with (unsigned char)
__forceinline BYTE __stdcall FloatToByte(float  x) 
{ 
	float  t = x + (float) 0xC00000;
	return * (BYTE *) &t; 
}

////////////////////////////////////////////////////////////////////////
// Floating point arithmetic - Taken from the www.gamedev.net forums and
// the Titan engine
////////////////////////////////////////////////////////////////////////

// Fast floor() for (x >= 0) && (x < 2^31). MUCH faster than the normal
// floor()
__forceinline int __stdcall ifloor(float  x)
{
	DWORD e = (0x7F + 31) - ((* (DWORD *) &x & 0x7F800000) >> 23);
	DWORD m = 0x80000000 | (* (DWORD *) &x << 8);
	return (m >> e) & -(e < 32); 
}

// Converts to integer equal to or less than, asm version
__forceinline int asmifloor(float f)
{
	static float Half = 0.5;
	int i; 
	__asm fld [f]
	__asm fsub [Half]
	__asm fistp [i]
	return i;
}

// Asm version of fabs()
__forceinline float __fastcall FastAbs(float a) 
{
	__asm
	{
		fld 	DWORD PTR [esp+4] 
		fabs
		ret 4
	}
    return 0.0f;
}

// Asm version of sinf()
__forceinline float __fastcall FastSin(float a) 
{
	__asm 
	{
		fld		DWORD PTR [esp+4] 
		fsin
		ret 4
	}
    return 0.0f;
}

// Asm version of cosf()
__forceinline float __fastcall FastCos(float a) 
{
	__asm 
	{
		fld		DWORD PTR [esp+4] 
		fcos
		ret 4
	}
    return 0.0f;
}

// Allows you to set your FPU to single precision mode and back. 
// This especially speeds up divisions and square roots. Be careful
// with this instructions, some of the optimized functions won't work
// in single precision mode
#define SET_TO_SINGLE_PRECISION _controlfp(_PC_24, MCW_PC);
#define SET_TO_DOUBLE_PRECISION _controlfp(_CW_DEFAULT, 0xfffff);

////////////////////////////////////////////////////////////////////////
// Integer point arithmetic - Credits to the Titan and Unreal Engine
// (http://talika.fie.us.es/~titan/)
////////////////////////////////////////////////////////////////////////

// Find the closest power of 2 that is >= N. (Unreal engine)
__inline DWORD NextPowerOfTwo(DWORD N)
{
	if (N<=0L		) return 0L;
	if (N<=1L		) return 1L;
	if (N<=2L		) return 2L;
	if (N<=4L		) return 4L;
	if (N<=8L		) return 8L;
	if (N<=16L	    ) return 16L;
	if (N<=32L	    ) return 32L;
	if (N<=64L 	    ) return 64L;
	if (N<=128L     ) return 128L;
	if (N<=256L     ) return 256L;
	if (N<=512L     ) return 512L;
	if (N<=1024L    ) return 1024L;
	if (N<=2048L    ) return 2048L;
	if (N<=4096L    ) return 4096L;
	if (N<=8192L    ) return 8192L;
	if (N<=16384L   ) return 16384L;
	if (N<=32768L   ) return 32768L;
	if (N<=65536L	) return 65536L;
	else			  return 0;
}

// Fast logarithm
__inline DWORD Log2(DWORD val)
{
	DWORD answer = 0;

	while (val >>= 1)
		answer++;

	return answer;
}

////////////////////////////////////////////////////////////////////////
// Vector math
////////////////////////////////////////////////////////////////////////

// Take the dot product of the two vectors. Is sometimes faster than
// the optimized asm dot product
#define DOT(v1, v2) (v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2])

// Take the cross of the two vectors
#define CROSS(out, v1, v2) \
	out[x] = v1[y] * v2[z] - v1[z] * v2[y]; \
	out[y] = v1[z] * v2[x] - v1[x] * v2[z]; \
	out[z] = v1[x] * v2[y] - v1[y] * v2[x]; \

// Substract the two vectors
#define SUB(dest, v1, v2) \
	dest[0] = v1[0] - v2[0]; \
    dest[1] = v1[1] - v2[1]; \
    dest[2] = v1[2] - v2[2]; 

// Add the two vectors
#define ADD(dest, v1, v2) \
	dest[0] = v1[0] + v2[0]; \
    dest[1] = v1[1] + v2[1]; \
    dest[2] = v1[2] + v2[2];

// Fast normalization of 3 component vector.
// Does not test if the vector has 0 length
__inline void FastNormVect3(float *v)
{
	float ilength;

	ilength = RSqrt(DotProduct(v, v));

	v[0] *= ilength;
	v[1] *= ilength;
	v[2] *= ilength;
}

// Fast normalization of 2 component vector. 
// Does not test if the vector has 0 length
__inline void FastNormVect2(float *v)
{
	float ilength;

	ilength = RSqrt(v[0] * v[0] + v[1] * v[1]);

	v[0] *= ilength;
	v[1] *= ilength;
}

// Fast 15 cycle asm dot product, credits to Golgotha
__forceinline float __cdecl DotProduct(const float v1[3], const float v2[3]) 
{
	FLOAT dotret;

	__asm 
	{
		mov ecx, v1
		mov eax, v2

		;optimized dot product		;15 cycles
		fld dword ptr   [eax+0]     ;starts & ends on cycle 0
		fmul dword ptr  [ecx+0]     ;starts on cycle 1
		fld dword ptr   [eax+4]     ;starts & ends on cycle 2
		fmul dword ptr  [ecx+4]     ;starts on cycle 3
		fld dword ptr   [eax+8]     ;starts & ends on cycle 4
		fmul dword ptr  [ecx+8]     ;starts on cycle 5
		fxch            st(1)       ;no cost
		faddp           st(2),st(0) ;starts on cycle 6, stalls for cycles 7-8
		faddp           st(1),st(0) ;starts on cycle 9, stalls for cycles 10-12
		fstp dword ptr  [dotret]    ;starts on cycle 13, ends on cycle 14
	}

	return dotret;
}

////////////////////////////////////////////////////////////////////////
// Square root - Credits to the Titan Engine and the Q3 game code
// (http://talika.fie.us.es/~titan/)
////////////////////////////////////////////////////////////////////////

// Fast reciprocal square root (Quake 3 game code)
// return: 1 / sqrtf(number)
__inline float RSqrt(float number)
{
	long i;
	float x2, y;
	const float threehalfs = 1.5f;

	x2 = number * 0.5f;
	y  = number;
	i  = * (long *) &y;						// evil floating point bit level hacking
	i  = 0x5f3759df - (i >> 1);             // what the fuck?
	y  = * (float *) &i;
	y  = y * (threehalfs - (x2 * y * y));   // 1st iteration

	return y;
}

// Fast inverse square root
__inline float __fastcall InverseSqrt(float a) 
{
	__asm 
	{
		mov		eax, 0be6eb508h
		mov		DWORD PTR [esp-12],03fc00000h ;  1.5 on the stack
		sub		eax, DWORD PTR [esp+4]; a
		sub		DWORD PTR [esp+4], 800000h ; a/2 a=Y0
		shr		eax, 1     ; firs approx in eax=R0
		mov		DWORD PTR [esp-8], eax

		fld		DWORD PTR [esp-8] ;r
		fmul	st, st            ;r*r
		fld		DWORD PTR [esp-8] ;r
		fxch	st(1)
		fmul	DWORD PTR [esp+4];a ;r*r*y0
		fld		DWORD PTR [esp-12];load 1.5
		fld		st(0)
		fsub	st,st(2)			   ;r1 = 1.5 - y1
		;x1 = st(3)
		;y1 = st(2)
		;1.5 = st(1)
		;r1 = st(0)

		fld		st(1)
		fxch	st(1)
		fmul	st(3),st			; y2=y1*r1*...
		fmul	st(3),st			; y2=y1*r1*r1
		fmulp	st(4),st            ; x2=x1*r1
		fsub	st,st(2)               ; r2=1.5-y2
		;x2=st(3)
		;y2=st(2)
		;1.5=st(1)
		;r2 = st(0)

		fmul	st(2),st			;y3=y2*r2*...
		fmul	st(3),st			;x3=x2*r2
		fmulp	st(2),st			;y3=y2*r2*r2
		fxch	st(1)
		fsubp	st(1),st			;r3= 1.5 - y3
		;x3 = st(1)
		;r3 = st(0)
		fmulp	st(1), st
		ret 4
	}
    return 0.0f;
}

// Another fast inverse square root
// Copyright (C) 1997 by Vesa Karvonen. All rights reserved.
// Use freely as long as my copyright is retained.
__inline double __fastcall Inv_Sqrt(double x) 
{
	__asm 
	{
		; I'm assuming that the argument is aligned to a 64-bit boundary.

        mov     eax,0BFCDD6A1h  ; 1u    Constant from James Van Buskirk
        mov     edx,[esp+8]     ; 1v    Potential pms.
        sub     eax,edx         ; 2u
        push    03FC00000h      ; 2v    Constant 1.5, aligns stack
        shr     eax,1           ; 3u
        sub     edx,000100000h  ; 3v    =.5*x, biased exponent must > 1
        mov     [esp+12],edx    ; 4u
        push    eax             ; 4v

		; The lower 32-bits of the estimate come from uninitialized stack.

        fld     QWORD PTR [esp-4]       ; 5     Potential pms
        fmul    st,st                   ; 6-8
        fld     QWORD PTR [esp-4]       ; 7
        fxch    st(1)                   ; 7x
        fmul    QWORD PTR [esp+12]      ; 9-11  Potential pms
        fld     DWORD PTR [esp+4]       ; 10
        add     esp,4                   ; 12    Faster on Pro/PII
        fsub    st,st(1)                ; 12-14

		fmul	st(1),st	; 15-17
		fmul	st(1),st	; 18-20
		fld	DWORD PTR [esp] ; 19
		fxch	st(1)		; 19
		fmulp	st(2),st	; 20-22
		fsub	st,st(1)	; 21-23

		fmul	st(1),st	; 24-26
		fmul	st(1),st	; 27-29
		fld	DWORD PTR [esp] ; 28
		fxch	st(1)		; 28
		fmulp	st(2),st	; 29-31
		fsub	st,st(1)	; 30-32
		
		fmul	st(1),st	; 33-35
		pop	eax		; 34
		fmul	st(1),st	; 36-38
		fld	DWORD PTR [esp] ; 37
		fxch	st(1)		; 37
		fmulp	st(2),st	; 38-40
		fsubrp	st(1),st	; 39-41
		fmulp   st(1),st	; 42-44
	}
    return 0.0;
}

// Fast integer square root. Doesn't seem to be any faster than the
// floating-point fastsqrt() from nVidia. Code from
// http://www.azillionmonkeys.com/qed/sqroot.html
__inline int isqrt0 (unsigned long r)
{
	double x, y;
	float rr;
	float tempf;
	long is;

    rr = (float) r;
    y = rr * 0.5;
    * (unsigned long *) &tempf = (0xbe6f0000 -* (unsigned long *) &rr) >> 1;
    x = tempf;
    x = (1.5 * x) - (x * x) * (x * y);

    if (r > 101123)
        x = (1.5 * x ) - (x * x) * (x * y);

    x *= rr;

    _asm 
	{
        fld x
        fistp is
    }

    is += (((signed int) (r - is * is)) >> 31);

    return is;
}

////////////////////////////////////////////////////////////////////////
// Square root with lookup table (http://www.nvidia.com/developer)
////////////////////////////////////////////////////////////////////////
//#define USE_SQRT_TABLE
#if defined(USE_SQRT_TABLE)

#define FP_BITS(fp) (* (DWORD *) &(fp))

typedef union FastSqrtUnion
{
	float f;
	unsigned int i;
} FastSqrtUnion;

static unsigned int iFastSqrtTable[0x10000];

// Build the square root table
void BuildSqrtTable()
{
	unsigned int i;
	FastSqrtUnion s;

	// Build the fast square root table
	for (i = 0; i <= 0x7FFF; i++)
	{
		// Build a float with the bit pattern i as mantissa
		// and an exponent of 0, stored as 127
		s.i = (i << 8) | (0x7F << 23);
		s.f = (float) sqrtf(s.f);
    
		// Take the square root then strip the first 7 bits of
		// the mantissa into the table
		iFastSqrtTable[i + 0x8000] = (s.i & 0x7FFFFF);
    
		// Repeat the process, this time with an exponent of 1, 
		// stored as 128
		s.i = (i << 8) | (0x80 << 23);
		s.f = (float) sqrt(s.f);
    
		iFastSqrtTable[i] = (s.i & 0x7FFFFF);
	}
}

__forceinline float fastsqrt(float n)
{
	// Check for square root of 0
	if (FP_BITS(n) == 0)
		return 0.0;                 
  
	FP_BITS(n) = iFastSqrtTable[(FP_BITS(n) >> 8) & 0xFFFF] | 
		((((FP_BITS(n) - 0x3F800000) >> 1) +
		0x3F800000) & 0x7F800000);
  
	return n;
}
#endif // #if defined(USE_SQRT_TABLE)

////////////////////////////////////////////////////////////////////////
// Memory operation - Credits to the Unreal Engine 
////////////////////////////////////////////////////////////////////////

// Most of the time, these functions seem to be slower than the one of
// MSVC's runtime library. Compare the speed before you use them !

// Fast memory copy
__inline void FastMemCpy(void *Dest, const void *Src, int Count)
{	
	__asm
	{
		mov		ecx, Count
		mov		esi, Src
		mov		edi, Dest
		mov     ebx, ecx
		shr     ecx, 2
		and     ebx, 3
		rep     movsd
		mov     ecx, ebx
		rep     movsb
	}
}

// Fast memory zero
__inline void FastMemZero(void *Dest, int Count)
{	
	// FIXME: if dest is unaligned, that wont be very fast
	__asm
	{
		mov		ecx, [Count]
		mov		edi, [Dest]
		xor     eax, eax
		mov		ebx, ecx
		shr		ecx, 2
		and		ebx, 3
		rep     stosd
		mov     ecx, ebx
		rep     stosb
	}
}

////////////////////////////////////////////////////////////////////////
// Fixed point math - Credits to Fast Game Programming
// (http://members.aol.com/form1/index.html)
////////////////////////////////////////////////////////////////////////

typedef long fixed;						// Our new fixed point type.

#define itofx(x) ((x) << 8)				// Integer to fixed point
#define ftofx(x) ((x) * 256)			// Float to fixed point
#define dtofx(x) ((x) * 256)			// Double to fixed point
#define fxtoi(x) ((x) >> 8)				// Fixed point to integer
#define fxtof(x) ((float) (x) / 256)	// Fixed point to float
#define fxtod(x) ((double) (x) / 256)	// Fixed point to double
#define mulfx(x, y) (((y) * (x)) >> 8)	// Multiply a fixed by a fixed
#define divfx(x, y) ((y << 8) / (x))   	// Divide a fixed by a fixed

// Taken from the Poly Engine source
__inline int FixMul(int a, int b) 
{
	_asm 
	{
		xor edx, edx
		mov eax, a
		mov ebx, b
		imul ebx
		shrd eax, edx, 16
		mov a, eax
	}

	return a;
}

__inline float fast_log2 (float val)
{
   int * const    exp_ptr = reinterpret_cast <int *> (&val);
   int            x = *exp_ptr;
   const int      log_2 = ((x >> 23) & 255) - 128;
   x &= ~(255 << 23);
   x += 127 << 23;
   *exp_ptr = x;

   val = ((-1.0f/3) * val + 2) * val - 2.0f/3;   // (1)

   return (val + log_2);
}

__inline float fast_log (const float &val)
{
   return (fast_log2 (val) * 0.69314718f);
}

#endif // __FAST_CODE_H_INCLUDED__