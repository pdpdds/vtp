//////////////////////////////////////////////////////////////////////////
/// @file   D3DHangul.h
///         interface for the CD3DHangul class of 'ERally' project.
/// @author seojt@kogsoft.com
/// @since  23.08.2002
///

#if !defined(_CD3DHangul_Defined_)
#define _CD3DHangul_Defined_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KogGen.h"
#include "D3DHangulFont.h"
#include "D3DHangulConverter.h"


//////////////////////////////////////////////////////////////////////
/// @class  CD3DHangul
///         a class for combo Hangul IO
///
class CD3DHangul
{
public:
    CD3DHangulFont      m_font; ///< Hangul & ASCII font object
    CHAR                m_lpszFntFile[_MAX_PATH]; ///< .FNT filename
    CHAR                m_lpszEftFile[_MAX_PATH]; ///< .EFT filename

    CD3DHangulConverter m_converter;   ///< WanSung to JoHap converter

private:
    void _Init();

public:
    /// constructor.
	CD3DHangul();
    /// destructor.
	~CD3DHangul();

    // build Hangul resources.
    HRESULT RestoreDeviceObjects(LPCTSTR lpszFntFile = NULL, LPCTSTR lpszEftFile = NULL);

    /// safely delete all resources.
    void SafeRelease();

    /// render a Hangul character.
    /// @param  x: screen x
    /// @param  y: screen y
    /// @param  b1: ChoSung index(not code)
    /// @param  b2: JungSung index
    /// @param  b3: JongSung index
    void HAN_FONT_TO_BUF(int x, int y, BYTE b1, BYTE b2, BYTE b3);

    /// draw JoHap Hangul string.
    /// @param  x: screen x
    /// @param  y: screen y
    /// @param  lpszText: [in] JoHap Hangul string
    void _OutHanXY(int x, int y, char * lpszText);

private:
    CHAR    m_buffer[_MAX_PATH]; ///< temporary string buffer
   
public:
    /// draw WanSung Hangul string.
    /// @param  x: screen x
    /// @param  y: screen y
    /// @param  lpszText: [in] WanSung Hangul string.
    /// @note   use _OutHanXY() to draw a JoHap Hangul string.
    /// @see    _OutHanXY(), SetWidthHeight(), SetColor()
    inline void OutHanXY(int x, int y, LPCTSTR lpszText)
    {

		WCHAR wFileName[MAX_PATH];   
		char buffer[_MAX_PATH];

		WideCharToMultiByte(CP_ACP, NULL,
		lpszText, -1,
			buffer, sizeof(buffer),
			NULL, NULL);
    //    strcpy(buffer,m_converter.ConvertString(m_buffer));
    strcpy(m_buffer,m_converter.ConvertString(buffer));
	//MultiByteToWideChar( CP_ACP, 0, m_buffer, -1,                          
	//								wFileName, MAX_PATH );
	
        _OutHanXY( x, y, m_buffer);

        
    }//OutHanXY()

    ////////////////////////////////////////////////////////////////
    // helpers
public:
    /// set output width and height.
    /// @param  width: Hangul width in pixel
    /// @param  height: Hangul height in pixel
    void SetWidthHeight(int width, int height);

    /// set output color.
    /// @param  color: AARRGGBB format color
    void SetColor(D3DCOLOR color);
};//class CD3DHangul

#endif // !defined(_CD3DHangul_Defined_)
