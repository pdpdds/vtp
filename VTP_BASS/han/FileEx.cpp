/////////////////////////////////////////////////////////////////////
/// @file   FileEx.cpp
///         wrapper class for low-level file operation which is based
///         on handle
///

#ifdef __AFXWIN_H__
    #include "stdafx.h"
#endif
#include "FileEx.h"


// initialize some static members.
int CFileEx::READ      = CFileEx::modeRead      | CFileEx::typeBinary;
int CFileEx::WRITE     = CFileEx::modeWrite     | CFileEx::typeBinary | CFileEx::modeCreate;
int CFileEx::READWRITE = CFileEx::modeReadWrite | CFileEx::typeBinary | CFileEx::modeCreate;


void CFileEx::_InitMembers()
{
    m_hFile       = -1;
    m_nOpenFlags  = 0;
    m_nShareFlags = 0;
    strcpy(m_szaFullPath, "");
}//CFileEx::_InitMembers()


CFileEx::CFileEx()
{
    _InitMembers();
}//CFileEx::CFileEx()


CFileEx::CFileEx(const char* lpszFileName, int nOpenFlags /*= CFileEx::READ*/)
{
    Open(lpszFileName, nOpenFlags);
}//CFileEx::CFileEx()


CFileEx::~CFileEx()
{
    Close();
}//CFileEx::~CFileEx()


int CFileEx::Open( const char* lpszFileName, int nOpenFlags /*= CFileEx::READ*/ )
{
    _InitMembers();

    // parse filename and set information
    _fullpath(m_szaFullPath, lpszFileName, sizeof(m_szaFullPath)-1);
    _splitpath(m_szaFullPath, m_szaDrive, m_szaPath, m_szaName, m_szaExt);

    // set working flag
    m_nOpenFlags = nOpenFlags;
    // set creation flags
    if (nOpenFlags & modeCreate)
    {
        m_nShareFlags= _S_IREAD | _S_IWRITE;
    }//if

    // open file
    m_hFile = _open(m_szaFullPath, m_nOpenFlags, m_nShareFlags);

    return m_hFile;
}//CFileEx::Open()


void CFileEx::Close()
{
    // Note: -1 means invalid file handle
    if (m_hFile != -1)
    {
        _close(m_hFile);
        m_hFile = -1;
        strcpy(m_szaFullPath, "");
    }//if
}//CFileEx::Close()
