///////////////////////////////////////////////////////
/// @file   SafeCast.h
/// @author programmed by seojt@kogsoft.com
/// @since  December 5, 2001
///

#if !defined(_SafeCast_Defined_)
#define _SafeCast_Defined_

#ifdef _DEBUG
    /// must be used for class casting.
    #define SAFE_CAST(type,exp)     dynamic_cast<type>(exp)
#else
    #define SAFE_CAST(type,exp)     (type)(exp)
#endif // _DEBUG

#endif // !defined(_SafeCast_Defined_)
