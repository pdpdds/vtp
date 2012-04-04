//////////////////////////////////////////////////////////////////////
/// @file   D3DVertex.h
///         interface for the CD3DVertex class.
/// @author programmed by seojt@kogsoft.com
/// @since  22 August, 2001
///
/// @date   23 August, 2001
///
/// @date   18 September, 2001
///         - D3DVERTEXV type added
///
/// @date   19 September, 2001
///         - operator=() is added for D3DVERTEX and D3DVERTEXV
///
/// @date   21 September, 2001
///         - BEGIN_VERTEXBUFFER(), END_VERTEXBUFFER() macros are added
///
/// @date   24 September, 2001
///         - D3DFVF_VERTEX macros are added
///
/// @date   October 9, 2001
///         - struct MESHTOOL
///
/// @date   October 25, 2001
///         - struct MESHTOOLEX
///
/// @date   August 22, 2002
///         - Doxygen JAVADOC style comment added
///

#if !defined(_D3DVertex_Defined_)
#define _D3DVertex_Defined_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdlib.h>
#include <d3dx9.h>
#include <assert.h>


//////////////////////////////////////////////////////////////////////////
// macro functions

/// declare vertex buffer and lock.
#define BEGIN_VERTEXBUFFER(m_pCMesh, pVB, VERTEXTYPE, pVertices)    VERTEXTYPE* pVertices;\
    LPDIRECT3DVERTEXBUFFER8 pVB;\
    m_pCMesh->GetVertexBuffer(&pVB);\
    pVB->Lock( 0, 0, (BYTE**)&pVertices, 0 );

/// unlock vertex buffer and release.
#define END_VERTEXBUFFER(pVB)      pVB->Unlock();\
    pVB->Release();


//////////////////////////////////////////////////////////////////////////
// FVF vertex types

#define D3DVERTEX_FVF   (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)
#define D3DFVF_VERTEX   D3DVERTEX_FVF

typedef struct _D3DVERTEX
{
    FLOAT x, y, z;
    FLOAT nx, ny, nz;
    FLOAT tu, tv;

    _D3DVERTEX() {}
    _D3DVERTEX(const D3DXVECTOR3& v, const D3DXVECTOR3& n, float _tu, float _tv)
    { 
        x  = v.x; 
        y  = v.y; 
        z  = v.z;
        nx = n.x; 
        ny = n.y; 
        nz = n.z;
        tu = _tu; 
        tv = _tv;
    }//_D3DVERTEX
    inline _D3DVERTEX& operator=(_D3DVERTEX& v)
    {
        x  = v.x;
        y  = v.y;
        z  = v.z;
        nx = v.nx;
        ny = v.ny;
        nz = v.nz;
        tu = v.tu;
        tv = v.tv;

        return *this;
    }//operator=()
} D3DVERTEX, *LPD3DVERTEX;


#define D3DVERTEXV_FVF      (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)
#define D3DFVF_VERTEXV      D3DVERTEXV_FVF

typedef struct _D3DVERTEXV
{
    D3DXVECTOR3 p;
    D3DXVECTOR3 n;
    FLOAT       tu, tv;

    _D3DVERTEXV() {}
    _D3DVERTEXV(const D3DXVECTOR3& _pos, const D3DXVECTOR3& _n, float _tu, float _tv)
    { 
        p  = _pos;
        n  = _n;
        tu = _tu; 
        tv = _tv;
    }//_D3DVERTEXV()
    inline _D3DVERTEXV& operator=(_D3DVERTEXV& v)
    {
        p  = v.p;
        n  = v.n;
        tu = v.tu;
        tv = v.tv;

        return *this;
    }//operator=()
} D3DVERTEXV, *LPD3DVERTEXV;


#define D3DLVERTEX_FVF      (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)
#define D3DFVF_LVERTEX      D3DLVERTEX_FVF

typedef struct _D3DLVERTEX
{
    FLOAT       x, y, z;
    D3DCOLOR    color;
    FLOAT       tu, tv;

    _D3DLVERTEX() { }
    _D3DLVERTEX(const D3DXVECTOR3& v, D3DCOLOR _color, float _tu, float _tv)
    { 
        x       = v.x; 
        y       = v.y; 
        z       = v.z; 
        color   = _color; 
        tu      = _tu; 
        tv      = _tv;
    }//_D3DLVERTEX()
} D3DLVERTEX, *LPD3DLVERTEX;


#define D3DTLVERTEX_FVF     (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)
#define D3DFVF_TLVERTEX     D3DTLVERTEX_FVF

typedef struct _D3DTLVERTEX
{
    FLOAT       sx, sy, sz, rhw;
    D3DCOLOR    color;
    FLOAT       tu, tv;

    _D3DTLVERTEX() {}
    _D3DTLVERTEX(const D3DXVECTOR3& v, D3DCOLOR _color, float _tu, float _tv)
    { 
        sx    = v.x; 
        sy    = v.y; 
        sz    = v.z; 
        rhw   = 1.f;
        color = _color; 
        tu    = _tu; 
        tv    = _tv;
    }//_D3DTLVERTEX()
} D3DTLVERTEX, *LPD3DTLVERTEX;


//////////////////////////////////////////////////////////////////////
/// @struct MESHTOOL
///         get vertex pointer from mesh
///    
struct MESHTOOL
{
    LPDIRECT3DVERTEXBUFFER9 pVB;
    DWORD                   dwNumVertices;
    LPD3DVERTEXV            pVertices;

    MESHTOOL( LPD3DXMESH pMesh )
    {
        pMesh->GetVertexBuffer( &pVB );
        pVB->Lock( 0, 0, (void**)&pVertices, 0 );
        dwNumVertices = pMesh->GetNumVertices();
    }//MESHTOOL()

    ~MESHTOOL()
    {
        pVB->Unlock();
        pVB->Release();
    }//~MESHTOOL()
};//struct MESHTOOL


//////////////////////////////////////////////////////////////////////
/// @struct MESHTOOLEX
///         get vertex and index pointer from LPD3DXMESH
///    
struct MESHTOOLEX
{
    LPDIRECT3DVERTEXBUFFER9 pVB;
    DWORD                   dwNumVertices;
    LPD3DVERTEX             pVertices;
    LPDIRECT3DINDEXBUFFER9  pIB;
    DWORD                   dwNumFaces;
    WORD*                   pIndices;

    MESHTOOLEX( LPD3DXMESH pMesh )
    {
#ifdef _DEBUG
        assert( pMesh );
#endif // _DEBUG

        HRESULT hr;

        // lock vertex buffer
        hr = pMesh->GetVertexBuffer( &pVB );
#ifdef _DEBUG
        assert( !FAILED(hr) );
#endif // _DEBUG
        hr = pVB->Lock( 0, 0, (void**)&pVertices, 0 );
#ifdef _DEBUG
        assert( !FAILED(hr) );
#endif // _DEBUG
        dwNumVertices = pMesh->GetNumVertices();

        // lock index buffer
        hr = pMesh->GetIndexBuffer( &pIB );
#ifdef _DEBUG
        assert( !FAILED(hr) );
#endif // _DEBUG
        hr = pIB->Lock( 0, 0, (void**)&pIndices, D3DLOCK_READONLY );
#ifdef _DEBUG
        assert( !FAILED(hr) );
#endif // _DEBUG
        dwNumFaces = pMesh->GetNumFaces();

    }//MESHTOOLEX()

    ~MESHTOOLEX()
    {
        // unlock index buffer
        pIB->Unlock();
        pIB->Release();

        // unlock vertex buffer
        pVB->Unlock();
        pVB->Release();
    }//~MESHTOOLEX()
};//struct MESHTOOLEX


/// @struct RECT_FLOAT
struct RECT_FLOAT
{
    float left;
    float top;
    float right;
    float bottom;
};//struct RECT_FLOAT


//////////////////////////////////////////////////////////////////////
// global functions

/// get 0 to n-1 random float.
inline float Random(float n) 
{
    return ( (float)rand()/(float)RAND_MAX ) * n;
}//Random()


/// get 0 to n-1 random integer.
inline int Random(int n) 
{
    return rand() % n;
}//Random()


#endif // !defined(_D3DVertex_Defined_)
