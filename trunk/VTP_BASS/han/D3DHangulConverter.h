//////////////////////////////////////////////////////////////////////////
/// @file   D3DHangulConverter.h
///         interface for the CD3DHangulConverter class of 'ERally' project.
/// @author seojt@kogsoft.com
/// @since  24.08.2002
///

#if !defined(_CD3DHangulConverter_Defined_)
#define _CD3DHangulConverter_Defined_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KogGen.h"


//////////////////////////////////////////////////////////////////////
/// @class  CD3DHangulConverter
///         convert Completion(WanSung) Hangul to Combination(JoHap) Hangul.
///
/// @see    CD3DHangul, CD3DHangulFont
///
class CD3DHangulConverter
{
private:
    WORD*   m_pCombiTable;  ///< WanSung to Combi convert table

public:
    /// constructor.
	CD3DHangulConverter();
    /// destructor.
	~CD3DHangulConverter();

    /// safely delete used buffers.
    void SafeDelete();

    /// load Combi Hangul Table from disk.
    /// @param  lpszCombiTableFile: [in] Combi Hangul Table filename\n
    ///         default is "han_c_table.bin"
    /// @return TRUE when table loaded
    BOOL LoadTable(LPCTSTR lpszCombiTableFile = L".\\HanFont\\HanCombiTable.bin");

    /// convert a WanSung Char to a JoHap Char.
    /// convert a Completion Code to Combination Code
    /// @param  nHanCompleteCode: WanSung code\n
    ///         nHanCompleteCode is BYTE reversed original code
    /// @param  pOutCombiCode: [out] JoHap code(don't reverse return code)
    inline void ConvertAWord(WORD* pOutCombiCode, WORD nHanCompleteCode)
    {
        ASSERT( m_pCombiTable );
        if ( nHanCompleteCode >= 42145 && nHanCompleteCode <= 51454 )
        {
            *pOutCombiCode = m_pCombiTable[nHanCompleteCode-42145];
        }
        else
        {
            *pOutCombiCode = 0xffff; // Han flag and Invalid 5Bits codes for each component
        }//if.. else..
    }//ConvertAWord()

    /// convert WanSung string to JoHap string.
    /// @param  lpszWanSung: [in,out] WanSung Hangul string
    inline LPSTR ConvertString(LPSTR lpszWanSung)
    {
        int             len = strlen(lpszWanSung);
        int             i = 0;
        WORD            word;

        while ( i < len )
        {
            if ( BYTE(lpszWanSung[i]) < 0x80 )
            {
                ++i;
            }
            else
            {
                word  = BYTE(lpszWanSung[i])<<8;
                word |= BYTE(lpszWanSung[i+1]);
                ConvertAWord((WORD*)(&lpszWanSung[i]), word);
                i += 2;
            }//if
        }//for

        return lpszWanSung;
    }//ConvertString()
};//class CD3DHangulConverter

#endif // !defined(_CD3DHangulConverter_Defined_)
