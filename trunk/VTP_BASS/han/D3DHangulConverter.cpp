//////////////////////////////////////////////////////////////////////
/// @file   D3DHangulConverter.cpp
///         implementation of the CD3DHangulConverter class.
///

#include "D3DHangulConverter.h"
#include "FileEx.h"


// Combi Hangul File Size
#define CD3DHC_TABLESIZE        18620


CD3DHangulConverter::CD3DHangulConverter()
{
    m_pCombiTable = NULL;
}//CD3DHangulConverter::CD3DHangulConverter()


CD3DHangulConverter::~CD3DHangulConverter()
{
    SafeDelete();
}//CD3DHangulConverter::~CD3DHangulConverter()


void CD3DHangulConverter::SafeDelete()
{
    if ( m_pCombiTable )
    {
        delete[] m_pCombiTable;
        m_pCombiTable = NULL;
    }//if
}//CD3DHangulConverter::SafeDelete()


BOOL CD3DHangulConverter::LoadTable(LPCTSTR lpszCombiTableFile)
{
    // delete previous buffer if exist
    SafeDelete();

    CFileEx file;
    // open Combi Hangul Table file
	char buffer[_MAX_PATH];
		WideCharToMultiByte(CP_ACP, NULL,
			lpszCombiTableFile, -1,
			buffer, sizeof(buffer),
			NULL, NULL);

    if ( -1 == file.Open(buffer) )
    {
        return FALSE;
    }//if

    // allocate buffer
    ASSERT( sizeof(WORD) == 2 );
    m_pCombiTable = new WORD[CD3DHC_TABLESIZE/2];
    ASSERT( m_pCombiTable );
    if ( m_pCombiTable == NULL )
    {
        file.Close();
        return FALSE;
    }//if

    // read to buffer
    int nByteRead;
    nByteRead = file.Read( m_pCombiTable, CD3DHC_TABLESIZE );
    ASSERT( nByteRead == CD3DHC_TABLESIZE );

    file.Close();

    return TRUE;
}//CD3DHangulConverter::LoadTable()
