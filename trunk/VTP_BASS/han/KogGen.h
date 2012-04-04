/////////////////////////////////////////////////////////////////////
/// @file   kogGen.h
/// Desc: KOG general helpful macros.
///
/// @author edited by seojt@kogsoft.com
/// @since  on 2 April, 2001
///
/// @date   on November 30, 2001
///         - DebugOut.h
///
/// @date   on December 5, 2001
///         - SafeCast.h
///
/// @date   June 10, 2002
///         - VERIFY()
/// @date   August 1, 2002
///         - TRACE()
///

#if !defined(_kogGen_Defined_)
#define _kogGen_Defined_

// keep a lot of unnecessary code out of our project builds
#define WIN32_LEAN_AND_MEAN 

#if !defined(_MSC_VER)
    #error kogGen.h needs Visual C++
#endif // !defined(_MSC_VER)

#pragma once

#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <assert.h>
#include <d3dx9.h>      ///<< DirectX 8.x header
#include "MemoryLeak.h" ///< memory leak detection macros
#include "Todo.h"       ///< display TODO list to output window
#include "Fastlib.h"    ///< some fast math functions
#include "DebugOut.h"   ///< ASSERT(), TRACE() and etc.
#include "SafeCast.h"


// user break point macro
#if defined(_DEBUG)
    #define BREAK(cond)         if (cond) DebugBreak()
    #define ASSERT(exp)         assert(exp)
    #define VERIFY(exp)         assert(exp)
    void _trace(char* fmt, ...);
    #define TRACE               _trace
#else
    #define BREAK(cond)
    #define ASSERT(exp)
    #define VERIFY(exp)         exp
    inline void _trace(char* fmt, ...) {}
    #define TRACE               1 ? (void)0 : _trace
#endif // defined(_DEBUG)

/** @example    KogGen.h

    void main()
    {
        int n;

        scanf("%d", &n);
    
        // user break point
        // you must run this program using F5 in debug mode
        BREAK(n==0);
    
        printf("%d\n", 100/n);
    }
*/


// local function
#define BEGIN_FUNCTION(name)    struct name {
#define END_FUNCTION(name)      };

/** @example    BEGIN_FUNCTION(name)

    void Test()
    {
        BEGIN_FUNCTION(Test)

            static long Factorial(long n)
            {
                if (n==0) 
                    return 1L;
                else
                    return n*Factorial(n-1);
            }

        END_FUNCTION(Test)

        long l;

        l = Test::Factorial(4);
        printf("Factorial 4=%ld\n", l);
    }
*/

#endif // !defined(_kogGen_Defined_)
