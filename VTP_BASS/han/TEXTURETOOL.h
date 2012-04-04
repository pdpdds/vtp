//////////////////////////////////////////////////////////////////////////
/// @file   TEXTURETOOL.h
///         interface for the TEXTURETOOL class of 'ERally' project.
/// @author seojt@kogsoft.com
/// @since  22.08.2002
///

#if !defined(_TEXTURETOOL_Defined_)
#define _TEXTURETOOL_Defined_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KogGen.h"


//////////////////////////////////////////////////////////////////////
/// @class  TEXTURETOOL
///         texture tool utility.
///
class TEXTURETOOL
{
public:
    LPDIRECT3DTEXTURE9  m_pTextr;           ///< working texture
    D3DLOCKED_RECT      m_rect;             ///< locked rectangle info
    BYTE*               m_pBits;            ///< pointer to data when texture locked
    int                 m_numBytePerPixel;  ///< number of bytes required for pixel

    /// default constructor.
    TEXTURETOOL()
    {
        m_pTextr            = NULL;
        m_pBits             = NULL;
        m_numBytePerPixel   = 0;
    }//TEXTURETOOL()

    /// constructor.
    /// @param  pTextr: [in] working texture
	TEXTURETOOL(LPDIRECT3DTEXTURE9 pTextr)
    {
        m_pTextr            = NULL;
        m_pBits             = NULL;
        m_numBytePerPixel   = 0;

        SafeLock(pTextr);
    }//TEXTURETOOL()

    /// destructor.
	~TEXTURETOOL()
    {
        SafeUnlock();
    }//~TEXTURETOOL()

    /// put pixel to locked texture
    /// @param  x,y: pixel position\n
    ///         we assume left-top is (0,0)
    /// @param  color: (format dependent) pixel color
    inline void PutPixel(int x, int y, D3DCOLOR color)
    {
        ASSERT( m_pBits );
        memcpy( &m_pBits[y*m_rect.Pitch + x*m_numBytePerPixel], &color, m_numBytePerPixel );
    }//PutPixel()

    /// safely lock texture
    /// @param  pTextr: [in] working texture
    /// @return FALSE when locking failed
    inline BOOL SafeLock(LPDIRECT3DTEXTURE9 pTextr)
    {
        if ( pTextr == NULL ) 
            return FALSE;

        // release previous texture if exist
        SafeUnlock();

        // set texture surface pointer
        m_pTextr = pTextr;

        // lock entire rectangle
        HRESULT hr = m_pTextr->LockRect(0, &m_rect, NULL, 0);
        if ( FAILED(hr) )
        {
            m_pTextr = NULL;
            m_pBits  = NULL;
            return FALSE;
        }//if

        // check texture format
        D3DSURFACE_DESC desc;
        m_pTextr->GetLevelDesc(0, &desc);
        if ( desc.Format == D3DFMT_A8R8G8B8 )
        {
            m_numBytePerPixel = 4;
        }
        else if ( desc.Format == D3DFMT_A4R4G4B4 )
        {
            m_numBytePerPixel = 2;
        }
        else if ( desc.Format == D3DFMT_A1R5G5B5 )
        {
            m_numBytePerPixel = 2;
        }
        else
        {
            m_pTextr->UnlockRect(0);
            m_pTextr = NULL;
            m_pBits  = NULL;
            return FALSE;
        }//if

        // set pointer to data
        m_pBits = (BYTE*)m_rect.pBits;

        return TRUE;
    }//SafeLock()

    /// safely unlock texture.
    inline void SafeUnlock()
    {
        if ( m_pTextr )
        {
            m_pTextr->UnlockRect(0);
            m_pTextr = NULL;
            m_pBits  = NULL;
        }//if
    }//SafeUnlock()
};//class TEXTURETOOL

#endif // !defined(_TEXTURETOOL_Defined_)
