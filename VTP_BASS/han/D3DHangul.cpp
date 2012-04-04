//////////////////////////////////////////////////////////////////////
/// @file   D3DHangul.cpp
///         implementation of the CD3DHangul class.
///

#include "D3DHangul.h"


#define HAN_CHOSUNG_SIZE        19
#define HAN_JUNGSUNG_SIZE       21
#define HAN_JONGSUNG_SIZE       27

#define HAN_CHOSUNG_SET         10
#define HAN_JUNGSUNG_SET        2
#define HAN_JONGSUNG_SET        3


// 5bits JoHap code to index mapping table
static BYTE HanTable[3][32] =
{
    { 0, 0, 1, 2, 3, 4, 5, 6,
      7, 8, 9,10,11,12,13,14,  // First index:
     15,16,17,18,19, 0, 0, 0,  //    0 : First consonant
      0, 0, 0, 0, 0, 0, 0, 0}, //    1 : Second vowel
    { 0, 0, 0, 1, 2, 3, 4, 5,  //    2 : Third consonant
      0, 0, 6, 7, 8, 9,10,11,  // Second index:
      0, 0,12,13,14,15,16,17,  //    Combi 5 bits Hangul code
      0, 0,18,19,20,21, 0, 0}, // contents:
    { 0, 0, 1, 2, 3, 4, 5, 6,  //    index to HAN_FONT?
      7, 8, 9,10,11,12,13,14,
     15,16, 0,17,18,19,20,21,
     22,23,24,25,26,27, 0, 0}
};//HanTable[][]


CD3DHangul::CD3DHangul()
{
    _Init();
}//CD3DHangul::CD3DHangul()


CD3DHangul::~CD3DHangul()
{
    SafeRelease();
}//CD3DHangul::~CD3DHangul()


void CD3DHangul::_Init()
{
    m_buffer[0] = NULL;
    strcpy( m_lpszFntFile, ".\\HanFont\\AHNSS.FNT" );  // set default Hangul font
    strcpy( m_lpszEftFile, ".\\HanFont\\ITALIC.EFT" ); // set default ASCII font
}//CD3DHangul::_Init()


HRESULT CD3DHangul::RestoreDeviceObjects(LPCTSTR lpszFntFile, LPCTSTR lpszEftFile)
{
    char buffer[MAX_PATH];
    if ( lpszFntFile )
	{
		WideCharToMultiByte(CP_ACP, NULL,
			lpszFntFile, -1,
			buffer, sizeof(buffer),
			NULL, NULL);
        strcpy( m_lpszFntFile, buffer );
	}
    if ( lpszEftFile )
	{
		WideCharToMultiByte(CP_ACP, NULL,
			lpszEftFile, -1,
			buffer, sizeof(buffer),
			NULL, NULL);
        strcpy( m_lpszEftFile, buffer );
	}

    SafeRelease();

    BOOL b1, b2, b3, b4;
	WCHAR wFileName[MAX_PATH];   
    




    b1 = m_font.CreateFontTextures();
	MultiByteToWideChar( CP_ACP, 0, m_lpszFntFile, -1,                          
									wFileName, MAX_PATH );
    b2 = m_font.LoadFnt( wFileName );
	MultiByteToWideChar( CP_ACP, 0, m_lpszEftFile, -1,                          
									wFileName, MAX_PATH );
    b3 = m_font.LoadEft( wFileName );

    b4 = m_converter.LoadTable();

    if ( b1 && b2 && b3 && b4)
        return S_OK;     
    //ASSERT( !"[Hangul] can not initilize Hangul resources(Textures, Fonts, or Table)" );
    return E_FAIL;
}//CD3DHangul::RestoreDeviceObjects()


void CD3DHangul::SafeRelease()
{
    m_font.SafeRelease(); // release font object

    m_converter.SafeDelete(); // release convert table object
}//CD3DHangul::SafeRelease()


/// Output 10*2*3 Combi Hangul character, 16x16 bitmapped image.
/// @param  b1: First consonant index
/// @param  b2: Middle vowel index
/// @param  b3: Last consonant index
void CD3DHangul::HAN_FONT_TO_BUF(int x, int y, BYTE b1, BYTE b2, BYTE b3)
{
    int     _f;
    int     _s;
    int     _t;
    int     id1;
    int     id2;
    int     id3;

    // when JongSung is empty
    if (b3 == 0)
    {
        // determine ChoSung by JungSung
       switch (b2)
       {
           case  0: case  1: case  2: case  3: case  4: case  5:
           case  6: case  7: case  8: case 21: 
               _f = 0;
               break;
           case  9: case 13: case 19:
               _f = 1;
               break;
           case 14: case 18:
               _f = 2;
               break;
           case 10: case 11: case 12: case 20:
               _f = 3;
               break;
           case 15: case 16: case 17:
               _f = 4;
               break;
       }// switch

       // when JungSung is empty
       if (b2 == 0)
       {
           // ChoSung
           id1 = _f*HAN_CHOSUNG_SIZE + b1-1;
           m_font.OutHanCharXY( x, y, id1 );
           return;
       }
       else // when JungSung isn't empty
       {
           _s = 0; // set JungSung index

           // ChoSung
           // JungSung
           if ( b1 == 0 )
               id1 = 319; // set empty font
           else
                id1 = _f*HAN_CHOSUNG_SIZE + b1-1;
           id2 = HAN_CHOSUNG_SET*HAN_CHOSUNG_SIZE + _s*HAN_JUNGSUNG_SIZE + b2-1;
           m_font.OutHanCharXY( x, y, id1, id2 );
           return;
       }//if.. else..
    }
    else // when JongSung isn't empty
    {
        // determine ChoSung by JungSung
       switch (b2)
       {
           case  1: case  2: case  3: case  4: case  5: case  6:
           case  7: case  8: case 21:
               _f = 5;
               break;
           case  9: case 13: case 19:
               _f = 6;
               break;
           case 14: case 18:
               _f = 7;
               break;
           case 10: case 11: case 12: case 20:
               _f = 8;
               break;
           case 15: case 16: case 17:
               _f = 9;
               break;
       }//switch

       _s = 1; // set JungSung index

       // determine JongSung by JungSung
       switch (b2)
       {
           case  1: case  2: case  3: case  4: case  5: case  6:
           case  7: case  8: case 10: case 11: case 12: case 20:
           case 21:
               _t = 0;
               break;
           case 15: case 16: case 17:
               _t = 1;
               break;
           case  9: case 13: case 14: case 18: case 19:
               _t = 2;
               break;
       }//switch

       // ChoSung
       // JungSung
       // JongSung
       if ( b1 == 0 )
            id1 = 319; // set empty font
       else
            id1 = _f*HAN_CHOSUNG_SIZE + b1-1;
       id2 = HAN_CHOSUNG_SET*HAN_CHOSUNG_SIZE + _s*HAN_JUNGSUNG_SIZE + b2-1;
       id3 = HAN_CHOSUNG_SET*HAN_CHOSUNG_SIZE 
           + HAN_JUNGSUNG_SET*HAN_JUNGSUNG_SIZE + _t*HAN_JONGSUNG_SIZE + b3-1;
       m_font.OutHanCharXY( x, y, id1, id2, id3 );
       return;
    }//if.. else..
}//CD3DHangul::HAN_FONT_TO_BUF()


/// Output ASCII & Hangul mixed string
/// with 8x16, 16x16 bitmapped image for each.
/// @param  x,y: screen position
/// @param  lpszText: [in] Hangul string
void CD3DHangul::_OutHanXY(int x, int y, char * lpszText)
{
    BYTE    byte1;
    BYTE    byte2;
    BYTE    b1;
    BYTE    b2;
    BYTE    b3;
    int     i = 0;	 
	char buffer[MAX_PATH];
	//WideCharToMultiByte(CP_ACP, NULL,
	//		lpszText, -1,
	//		buffer, sizeof(buffer),
	//		NULL, NULL);
    int     len = strlen(lpszText);

    // set render state and vertex shader
    m_font.m_spriteHangul.OutSpriteBarePrepare();

    while ( i < len )
    {
        // get first byte
        byte1 = lpszText[i];

        // if this Hangul code
        if ( byte1 & 0x80 )
        {
            // get second byte
            byte2 = lpszText[i+1];

            // code separation, convert code to array index
            b1 = HanTable[0][ (byte1 & 0x7c) >> 2 ];
            b2 = HanTable[1][ ((byte1 & 0x03) << 3) | (byte2 >> 5) ];
            b3 = HanTable[2][ byte2 & 0x1f ];

            // render a character
            HAN_FONT_TO_BUF( x, y, b1, b2, b3 );
            x += m_font.m_spriteHangul.m_outWidth;

            ++i;
        }
        else
        {
            m_font.OutEngCharXY( x, y, byte1 );
            x += m_font.m_spriteASCII.m_outWidth;
        }// if.. else..

        ++i;

    }//while

    // restore render state
    m_font.m_spriteHangul.OutSpriteBareRestore();
}//CD3DHangul::_OutHanXY()


void CD3DHangul::SetWidthHeight(int width, int height)
{
    m_font.m_spriteHangul.SetOutSize(width, height);
    m_font.m_spriteASCII.SetOutSize(width/2, height);
}//CD3DHangul::SetWidthHeight()


void CD3DHangul::SetColor(D3DCOLOR color)
{
    m_font.m_spriteHangul.SetColor(color);
    m_font.m_spriteASCII.SetColor(color);
}//CD3DHangul::SetColor()
