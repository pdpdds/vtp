////////////////////////////////////////////////////////////
/// @file   DebugOut.h
///         output string to debug window
/// @author seojt@kogsoft.com
/// @note   I refered to 'afx.h(MFC standard header file)'
///

#if !defined(_DebugOut_Defined_)
#define _DebugOut_Defined_

#include <stdio.h>
#include <stdarg.h>


#if defined(DebugOut)
    #undef DebugOut
#endif // defined(DebugOut)


#if defined(_DEBUG)
    #define DebugOut _DebugOut
#else
    #define DebugOut 1 ? (void)0 :
#endif // defined(_DEBUG)

/** @example    DebugOut("value=%d",i);

  0) on Debug Mode: _DebugOut("value=%d",i);
  1) on Release Mode: 1 ? (void)0 : ("value=%d",i);

*/


/// faster version than 'DebugOut'
#if defined(DebugOutFast)
    #undef DebugOutFast
#endif // defined(DebugOutFast)

#if defined(_DEBUG)
    #define DebugOutFast(a) _DebugOut a
#else
    #define DebugOutFast(a)
#endif // defined(_DEBUG)

/** @example    DebugOutFast(("value=%d",i));

  0) on Debug Mode: _Debug ("value=%d",i);
  1) on Release Mode: (empty statement)

*/


inline void _DebugOut( const char *msg, ... ) 
{
    char    buffer[512];
    WCHAR wFileName[MAX_PATH];   
    va_list argList;    
    va_start(argList, msg);        
    vsprintf(buffer, msg, argList);    
    va_end(argList);
    MultiByteToWideChar( CP_ACP, 0, buffer, -1,                          
									wFileName, MAX_PATH );
    OutputDebugString( wFileName );
}//_DebugOut()

#endif // !defined(_DebugOut_Defined_)
