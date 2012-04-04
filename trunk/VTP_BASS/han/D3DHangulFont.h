//////////////////////////////////////////////////////////////////////////
/// @file   D3DHangulFont.h
///         interface for the CD3DHangulFont class of 'ERally' project.
/// @author seojt@kogsoft.com
/// @since  22.08.2002
///

#if !defined(_CD3DHangulFont_Defined_)
#define _CD3DHangulFont_Defined_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KogGen.h"
#include "SpriteEx.h"
#include "TextureTool.h"


#define HAN_NUMCOMPONENTPERTEXTURE  64 ///< number of component per a Hangul texture
    // 128(texture width)/16(a font width) = 8, so total component is 8x8 = 64


//////////////////////////////////////////////////////////////////////
/// @class  CD3DHangulFont
///         a class for loading fonts(combination Hangul and Extended ASCII)
///
/// @see    CSpriteEx, TEXTURETOOL
///
class CD3DHangulFont
{
public:
    D3DFORMAT           m_d3dFormat;        ///< texture format
    D3DCOLOR            m_colorPixel;       ///< font pixel color
    LPDIRECT3DTEXTURE9  m_ppTextrHangul[5]; ///< Hangul texture
    LPDIRECT3DTEXTURE9  m_pTextrASCII;      ///< ASCII texture

    CSpriteEx           m_spriteHangul;     ///< first sound sprite
    CSpriteEx           m_spriteASCII;      ///< ASCII sprite

private:
    /// onetime initialize some members.
    void _Init();

    /// copy one componets(16x16 bitmap) of .FNT data to texture
    /// @param  pSrc: [in] 32 byte source buffer
    /// @param  pTT: [in,out] target texture wrapper
    /// @param  x: left-top pos of a component in the texture
    /// @param  y: left-top pos of a component in the texture
    void _CopyBits16x16ToTexture(const BYTE* pSrc, TEXTURETOOL* pTT, int x, int y);

    /// copy one componets(8x16 bitmap) of .FNT data to texture
    /// @param  pSrc: [in] 16 byte source buffer
    /// @param  pTT: [in,out] target texture wrapper
    /// @param  x: left-top pos of a component in the texture
    /// @param  y: left-top pos of a component in the texture
    void _CopyBits8x16ToTexture(const BYTE* pSrc, TEXTURETOOL* pTT, int x, int y);

public:
    /// constructor.
	CD3DHangulFont();
    /// destructor.
	~CD3DHangulFont();

    /// create font textures.
    /// @return TRUE when textures are created
    BOOL CreateFontTextures();

    /// save created font textures to file.
    /// @return number of files created
    int SaveFontTextures();

    /// load Hangul .FNT file.
    /// @param  lpszFntFile: .FNT filename
    /// @return TRUE when font loaded
    BOOL LoadFnt(LPCTSTR lpszFntFile);

    /// load ASCII .EFT file.
    /// @param  lpszEftFile: .EFT filename
    /// @return TRUE when font loaded
    BOOL LoadEft(LPCTSTR lpszEftFile);

    /// safely release font textures.
    void SafeRelease();

    //////////////////////////////////////////////////////////
    // render functions
public:
    /// get texture index and offset.
    /// @param  pOutTextr: [out] texture index which ranges from 0 to 4
    /// @param  pOutOffsetId: [out] sprite id in the texture(pOutTextr)
    /// @param  id: sequential id from first font
    inline void GetHanTextureIndices(int* pOutTextr, int* pOutOffsetId, int id)
    {
        *pOutTextr    = id / HAN_NUMCOMPONENTPERTEXTURE;
        *pOutOffsetId = id % HAN_NUMCOMPONENTPERTEXTURE;
    }//GetHanTextureIndices()

    /// render a Hangul character.
    /// @param  x,y: screen position
    /// @param  pIndices: [in] component indices
    /// @param  size: pIndices[] array size
    /// @NOTE   you must call m_spriteHangul.OutSpriteBarePrepare()
    ///         before call this function\n
    ///         and call m_spriteHangul.OutSpriteBareRestore() after.
    inline void OutHanCharXY(int x, int y, int id1)
    {
        m_spriteHangul.OutSpriteBare( m_ppTextrHangul[id1 / HAN_NUMCOMPONENTPERTEXTURE],
            x, y, id1 % HAN_NUMCOMPONENTPERTEXTURE );
    }//OutHanCharXY()
    inline void OutHanCharXY(int x, int y, int id1, int id2)
    {
        m_spriteHangul.OutSpriteBare( m_ppTextrHangul[id1 / HAN_NUMCOMPONENTPERTEXTURE],
            x, y, id1 % HAN_NUMCOMPONENTPERTEXTURE );
        m_spriteHangul.OutSpriteBare( m_ppTextrHangul[id2 / HAN_NUMCOMPONENTPERTEXTURE],
            x, y, id2 % HAN_NUMCOMPONENTPERTEXTURE );
    }//OutHanCharXY()
    inline void OutHanCharXY(int x, int y, int id1, int id2, int id3)
    {
        m_spriteHangul.OutSpriteBare( m_ppTextrHangul[id1 / HAN_NUMCOMPONENTPERTEXTURE],
            x, y, id1 % HAN_NUMCOMPONENTPERTEXTURE );
        m_spriteHangul.OutSpriteBare( m_ppTextrHangul[id2 / HAN_NUMCOMPONENTPERTEXTURE],
            x, y, id2 % HAN_NUMCOMPONENTPERTEXTURE );
        m_spriteHangul.OutSpriteBare( m_ppTextrHangul[id3 / HAN_NUMCOMPONENTPERTEXTURE],
            x, y, id3 % HAN_NUMCOMPONENTPERTEXTURE );
    }//OutHanCharXY()


    /// render a ASCII character.
    /// @param  x,y: screen position
    /// @param  id: ASCII code
    /// @NOTE   you must call m_spriteASCII.OutSpriteBarePrepare()
    ///         before call this function\n
    ///         and call m_spriteASCII.OutSpriteBareRestore() after.
    inline void OutEngCharXY(int x, int y, int id)
    {
        m_spriteASCII.OutSpriteBare( m_pTextrASCII, x, y, id-1 );
    }//OutEngCharXY()
};//class CD3DHangulFont

#endif // !defined(_CD3DHangulFont_Defined_)
