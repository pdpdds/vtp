////////////////////////////////////////////////////////////////////////////
/// @file   SpriteEx.cpp
///         implementation for class CSpriteEx
///

#include "SpriteEx.h"


CSpriteEx::CSpriteEx()
{
    m_width      = 0; // original width of a sprite
    m_height     = 0; // original height of a sprite
    m_count      = 0; // number of sprites in a row

    _InitData();
}//CSpriteEx::CSpriteEx()


CSpriteEx::CSpriteEx(int _count, int _width, int _height)
{
    m_count  = _count;
    m_width  = _width;
    m_height = _height;

    _InitData();
}//CSpriteEx::CSpriteEx()


CSpriteEx::~CSpriteEx()
{
}//CSpriteEx::~CSpriteEx()


void CSpriteEx::_InitData()
{
    m_outWidth   = m_width; // output width of a sprite
    m_outHeight  = m_height; // output height of a sprite
    m_modeRender = CSE_TRANSPARENT; // default rendering mode

    m_widthImage  = 256; // image size is always 256 by 256
    m_heightImage = 256;
    m_bBorder     = FALSE;

    // 1--3
    // |  |
    // 0--2
    D3DXVECTOR3 v = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
    m_mesh[0] = D3DTLVERTEX( v, 0xffffffff, 0.0f, 1.0f );
    m_mesh[1] = D3DTLVERTEX( v, 0xffffffff, 0.0f, 0.0f );
    m_mesh[2] = D3DTLVERTEX( v, 0xffffffff, 1.0f, 1.0f );
    m_mesh[3] = D3DTLVERTEX( v, 0xffffffff, 1.0f, 0.0f );
}//CSpriteEx::_InitData()


D3DTLVERTEX* CSpriteEx::GetMesh(int i /*= -1 */)
{
    // 1------3
    // |      |
    // 0------2
    if (i != -1)
        SetTextrCoords(i);
    return m_mesh;
}//CSpriteEx::GetMesh()


void CSpriteEx::SetOutSize(int _width, int _height)
{
    if (_width == -1) 
        m_outWidth = m_width; // set original width
    else 
        m_outWidth = _width;

    if (_height == -1) 
        m_outHeight = m_height; // set original height
    else 
        m_outHeight = _height;
}//CSpriteEx::SetOutSize()


void CSpriteEx::SetInfo(int _count, int _width, int _height, 
                        float _widthImage, float _heightImage, BOOL bBorder)
{
    SetCount(_count);
    SetSize(_width, _height);
    m_widthImage  = _widthImage;
    m_heightImage = _heightImage;
    m_bBorder     = bBorder;
}//CSpriteEx::SetInfo()


void CSpriteEx::UVGet(D3DTLVERTEX vertex[])
{
    for (register int i=0; i<4; ++i)
    {
        vertex[i].tu = m_mesh[i].tu;
        vertex[i].tv = m_mesh[i].tv;
    }//for
}//CSpriteEx::UVGet()


void CSpriteEx::UVGet(D3DVERTEX vertex[])
{
    for (register int i=0; i<4; ++i)
    {
        vertex[i].tu = m_mesh[i].tu;
        vertex[i].tv = m_mesh[i].tv;
    }//for
}//CSpriteEx::UVGet()


void CSpriteEx::UVGet(float uv[])
{
    for (register int i=0, j=0; i<8; i+=2, ++j)
    {
        uv[i]   = m_mesh[j].tu;
        uv[i+1] = m_mesh[j].tv;
    }//for
}//CSpriteEx::UVGet()
