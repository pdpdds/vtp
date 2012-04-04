//////////////////////////////////////////////////////////////////////
/// @file   D3DHangulFont.cpp
///         implementation of the CD3DHangulFont class.
///

#include "D3DHangulFont.h"
#include "FileEx.h"


// redefine SAFE_RELEASE() for reduce dependency
#define HAN_SAFE_RELEASE(p)         { if(p) { (p)->Release(); (p)=NULL; } }
#define HAN_SAFE_DELETE(p)          { if(p) { delete (p); (p)=NULL; } }
#define HAN_SAFE_DELETE_ARRAY(p)    { if(p) { delete[] (p); (p)=NULL; } }

#define HAN_FNTSIZE                 10464   ///< .FNT file size
#define HAN_EFTSIZE                 2032    ///< .EFT file size

#define HAN_TEXTUREWIDTH            128     ///< font texture width


CD3DHangulFont::CD3DHangulFont()
{
    _Init();
}//CD3DHangulFont::CD3DHangulFont()


CD3DHangulFont::~CD3DHangulFont()
{
    SafeRelease();
}//CD3DHangulFont::~CD3DHangulFont()


void CD3DHangulFont::_Init()
{
    // set default texture format and pixel color
    m_d3dFormat  = D3DFMT_A8R8G8B8;
    m_colorPixel = 0xffffffff;

    // initialize textures
    for (int i=0; i<5; ++i)
    {
        m_ppTextrHangul[i] = NULL;
    }//for
    m_pTextrASCII = NULL;
}//CD3DHangulFont::_Init()


BOOL CD3DHangulFont::CreateFontTextures()
{
    ASSERT( m_ppTextrHangul[0] == NULL && m_pTextrASCII == NULL );
    
    HRESULT hr;

    // create Hangul texture
    for (int i=0; i<5; ++i)
    {
        hr = D3DXCreateTexture(g_pd3dDevice, HAN_TEXTUREWIDTH, HAN_TEXTUREWIDTH,
            1, 0, m_d3dFormat, D3DPOOL_MANAGED, &m_ppTextrHangul[i] );
       // ASSERT( !FAILED(hr) );
        //if ( FAILED(hr) )
        //{
          //  SafeRelease();
           // return FALSE;
        //}//if
    }//for

    // create ASCII texture
    hr = D3DXCreateTexture(g_pd3dDevice, HAN_TEXTUREWIDTH, HAN_TEXTUREWIDTH,
        1, 0, m_d3dFormat, D3DPOOL_MANAGED, &m_pTextrASCII );
    //ASSERT( !FAILED(hr) );
    //if ( FAILED(hr) )
   // {
      //  SafeRelease();
        //return FALSE;
    //}//if

    // set Hangul(16x16) sprite
    m_spriteHangul.SetInfo(8, 16,16, HAN_TEXTUREWIDTH,HAN_TEXTUREWIDTH);
    //m_spriteHangul.SetTexture( m_pTextrHangul[i] );

    // set ASCII(8x16) sprite
    m_spriteASCII.SetInfo(16, 8,16, HAN_TEXTUREWIDTH,HAN_TEXTUREWIDTH);
    //m_spriteASCII.SetTexture( m_pTextrASCII );
    
    return TRUE;
}//CD3DHangulFont::CreateFontTextures()


int CD3DHangulFont::SaveFontTextures()
{
    int     retCounter = 0;
    HRESULT hr;
    char    buffer[80];
	WCHAR wFileName[MAX_PATH];          

    for (int i=0; i<5; ++i)
    {
        if ( m_ppTextrHangul[i] )
        {
            sprintf( buffer, "Fnt%02d.tga", i+1);
			MultiByteToWideChar( CP_ACP, 0, buffer, -1,                          
									wFileName, MAX_PATH );			
            hr = D3DXSaveTextureToFile(wFileName, D3DXIFF_TGA, m_ppTextrHangul[i], NULL );
            if ( SUCCEEDED(hr) )
                ++retCounter;
        }//for
    }//for

    if ( m_pTextrASCII )
    {
        hr = D3DXSaveTextureToFile( L"Eft01.tga", D3DXIFF_TGA, m_pTextrASCII, NULL );
        if ( SUCCEEDED(hr) )
            ++retCounter;
    }//if

    return retCounter;
}//CD3DHangulFont::SaveFontTextures()


void CD3DHangulFont::_CopyBits16x16ToTexture(const BYTE* pSrc, TEXTURETOOL* pTT, int x, int y)
{
    BYTE    shiftMask;  // used to mask .FNT bit array
    int     xSaved = x; // save starting x-pos

    // iterate all 32 bytes of .FNT data
    for (register int i=1; i<=32; ++i)
    {
        shiftMask = 0x80; // setup mask
        // iterate 8 bits
        for (register int j=0; j<8; ++j)
        {
            // build up texture
            pTT->PutPixel(x,y, shiftMask & pSrc[i-1] ? m_colorPixel : 0 );

            shiftMask >>= 1;
            ++x;
        }//for

        // line feed for every two bytes
        if ( i%2 == 0 )
        {
            x  = xSaved;
            y += 1;
        }//if
    }//for
}//CD3DHangulFont::_CopyBits16x16ToTexture()


void CD3DHangulFont::_CopyBits8x16ToTexture(const BYTE* pSrc, TEXTURETOOL* pTT, int x, int y)
{
    BYTE    shiftMask;  // used to mask .FNT bit array
    int     xSaved = x; // save starting x-pos

    // iterate all 16 bytes of .FNT data
    for (register int i=1; i<=16; ++i)
    {
        shiftMask = 0x80; // setup mask

        // iterate 8 bits
        for (register int j=0; j<8; ++j)
        {
            // build up texture
            pTT->PutPixel(x,y, shiftMask & pSrc[i-1] ? m_colorPixel : 0 );

            shiftMask >>= 1;
            ++x;
        }//for

        x  = xSaved;
        y += 1;
    }//for
}//CD3DHangulFont::_CopyBits8x16ToTexture()


BOOL CD3DHangulFont::LoadFnt(LPCTSTR lpszFntFile)
{
    if ( lpszFntFile == NULL || m_ppTextrHangul[0] == NULL )
        return FALSE;

    CFileEx     file;        // file object to open .FNT file
    BYTE*       pBuf = NULL; // temporary buffer for data of .FNT file
    char buffer[_MAX_PATH];





		
		WideCharToMultiByte(CP_ACP, NULL,
			lpszFntFile, -1,
			buffer, sizeof(buffer),
			NULL, NULL);
    // open .FNT file
    if ( -1 == file.Open( buffer ) )
        return FALSE;

    // allocate buffer
    pBuf = new BYTE[HAN_FNTSIZE];
    ASSERT( pBuf );
    if ( pBuf == NULL )
    {
        file.Close();
        return FALSE;
    }//if
    
    // read to buffer
    long nNumRead = file.Read( pBuf, HAN_FNTSIZE );
    ASSERT( nNumRead == HAN_FNTSIZE ); // validate read size
    if ( nNumRead != HAN_FNTSIZE )
    {
        HAN_SAFE_DELETE_ARRAY( pBuf );
        file.Close();
        return FALSE;
    }//if

    // write to texture
    TEXTURETOOL tt;                 // working texture tool object
    int         iBlock;             // component block loop index
    int         iComponent;         // each component index
    int         iTexture     = 0;   // destination texture index
    int         x            = 0;   // texture destination coords
    int         y            = 0;
    int         offset       = 0;   // component offset in .FNT data
    const int   pSizeComponent[15] = 
    {
        19,19,19,19,19,19,19,19,19,19,  // # of component of ChoSung
        21,21,                          // # of component of JungSung
        27,27,27                        // # of component of JongSung
    };

    // lock first texture
    tt.SafeLock( m_ppTextrHangul[iTexture] );

    // iterate all component blocks
    for (iBlock=0; iBlock<15; ++iBlock)
    {
        // read font bits and write to texture
        // 32 means 2(bytes per row)*16, 1024 means 16*16*4(bytes per pixel)
        for (iComponent=0; iComponent<pSizeComponent[iBlock]; ++iComponent)
        {
            // build a Hangul font
            _CopyBits16x16ToTexture( &pBuf[offset*32], &tt, x, y );

            // 16x16 font logic
            x += 16;
            if ( x >= HAN_TEXTUREWIDTH )
            {
                x  = 0;
                y += 16;

                // when working texture fully filled
                if ( y >= HAN_TEXTUREWIDTH )
                {
                    y = 0;
                    tt.SafeUnlock(); // unlock used(old) texture

                    // lock new working texture
                    ++iTexture; // move to next texture
                    tt.SafeLock( m_ppTextrHangul[iTexture] );
                }//if
            }//if

            ++offset; // move to next component
        }//for

        ++offset; // skip component separator
    }//for (iBlock..

    // unlock last texture
    tt.SafeUnlock();

    // done with buffer
    HAN_SAFE_DELETE_ARRAY( pBuf );

    // close file
    file.Close();

    return TRUE;
}//CD3DHangulFont::LoadFnt()


BOOL CD3DHangulFont::LoadEft(LPCTSTR lpszEftFile)
{
    if ( lpszEftFile == NULL || m_pTextrASCII == NULL )
        return FALSE;

    CFileEx     file;        // file object to open .EFT file
    BYTE*       pBuf = NULL; // temporary buffer for data of .EFT file
char buffer[_MAX_PATH];
		WideCharToMultiByte(CP_ACP, NULL,
			lpszEftFile, -1,
			buffer, sizeof(buffer),
			NULL, NULL);

    // open .EFT file
    if ( -1 == file.Open( buffer ) )
        return FALSE;

    // allocate buffer
    pBuf = new BYTE[HAN_EFTSIZE];
    ASSERT( pBuf );
    if ( pBuf == NULL )
    {
        file.Close();
        return FALSE;
    }//if
    
    // read to buffer
    long nNumRead = file.Read( pBuf, HAN_EFTSIZE );
    ASSERT( nNumRead == HAN_EFTSIZE ); // validate read size
    if ( nNumRead != HAN_EFTSIZE )
    {
        HAN_SAFE_DELETE_ARRAY( pBuf );
        file.Close();
        return FALSE;
    }//if

    // write to texture
    TEXTURETOOL tt( m_pTextrASCII );                 // working texture tool object
    int         iComponent;         // each component index
    int         x            = 0;   // texture destination coords
    int         y            = 0;
    int         offset       = 0;   // component offset in .FNT data

    // iterate all ASCII characters
    for (iComponent=0; iComponent<128; ++iComponent)
    {
        // build a ASCII character
        _CopyBits8x16ToTexture( &pBuf[offset*16], &tt, x, y );

        // 8x16 font logic
        x += 8;
        if ( x >= HAN_TEXTUREWIDTH )
        {
            x  = 0;
            y += 16;
        }//if

        ++offset; // move to next component
    }//for

    // done with buffer
    HAN_SAFE_DELETE_ARRAY( pBuf );

    // close file
    file.Close();

    return TRUE;
}//CD3DHangulFont::LoadEft()


void CD3DHangulFont::SafeRelease()
{
    // release all textures
    for (int i=0; i<5; ++i)
    {
        HAN_SAFE_RELEASE( m_ppTextrHangul[i] );
    }//for
    HAN_SAFE_RELEASE( m_pTextrASCII );
}//CD3DHangulFont::SafeRelease()
