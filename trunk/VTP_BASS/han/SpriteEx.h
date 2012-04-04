////////////////////////////////////////////////////////////////////////////
/// @file   SpriteEx.h
///         2D sprite drawing routine for User Interface and Menu
/// @author seojt@kogsoft.com
/// @since  September 26, 2000
///
/// @date   modified on December 30, 2000
///
/// @date   August 31, 2001
///         - ported for DirectX 8
///
/// @date   December 8, 2001
///         - revised
/// @date   August 24, 2002
///         - Doxygen JAVADOC style comment added
///

#if !defined(_CSpriteEx_DEFINED_)
#define _CSpriteEx_DEFINED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KogGen.h"
#include "D3dVertex.h"


extern LPDIRECT3DDEVICE9 g_pd3dDevice; ///< global external DirectX device object


/// convert degree to radian.
inline static float D2R(float deg) 
{
    return deg*0.017453f; //deg*g_pi/180.0;
}//D2R()

/// rotate 2D (u,v) about origin.
/// @param  (u,v): [in,out] two floats
/// @param  radian: radian
inline static void Rotate(float& u, float& v, float radian)
{
    float tu = u; 
    float tv = v;
    float fSin = sinf(radian);
    float fCos = cosf(radian);

    u = tu*fCos - tv*fSin;
    v = tu*fSin + tv*fCos;
}//Rotate()



////////////////////////////////////////////////////////////////////////////
/// @class  CSpriteEx
///         Draw 2D sprite using D3DTLVERTEX in Direct 3D
///         First designed for UI
///
class CSpriteEx  
{
protected:
    // texture dimension in pixel
    float       m_widthImage;   ///< texture image width in pixel
    float       m_heightImage;  ///< texture image height in pixel

    // sprite dimension
    int         m_width;        ///< original width of a sprite in pixel
    int         m_height;       ///< original height of a sprite in pixel

    int         m_count;        ///< number of sprites in a row

    D3DTLVERTEX m_mesh[4];      ///< mesh to render a sprite

public:
    /// @enum   _RenderMode
    enum _RenderMode
    {
        CSE_OPAQUE      = 0,    ///< Opaque render mode
        CSE_TRANSPARENT = 1     ///< Alpha render mode
    };//enum _RenderMode

    int         m_outWidth;     ///< current output width of a sprite
    int         m_outHeight;    ///< current output height of a sprite
    int         m_modeRender;   ///< rendering mode 0: opaque
                                ///<                1: transparent
    BOOL        m_bBorder;      ///< inter sprite border flag
                                ///< initial value is 'FALSE'

    ////////////////////////////////////////////////////////////////////////
    // newly added members for texture transform

    float       m_textrWidth;   ///< texture's width of currently activated 'm_mesh'
    float       m_textrHeight;  ///< current texture height
    float       m_textrUCenter; ///< current texture u center
    float       m_textrVCenter; ///< current texture v center

    float       m_textrUStep;   ///< 1/100 of 'm_textrWidth' for speed-up
    float       m_textrVStep;   ///< 1/100 of 'm_textrHeight' for speed-up

public:
    LPDIRECT3DTEXTURE9  m_pd3dTexture; ///< sprite texture
        
private:
    /// initialize member variables.
    void _InitData();

    /// set mesh data for a sprite.
    /// @param  _x, _y, _width, and _height: absolute(screen) coords
    inline void _SetMesh(int _x, int _y, int _width, int _height)
    {
        float x      = (float)_x - 0.5f; // biased for one-to-one mapping
        float y      = (float)_y - 0.5f;
        float width  = (float)_width;
        float height = (float)_height;

        // (x,y)
        // 1------3
        // |      |
        // 0------2 (width, height)
        m_mesh[0].sx = x;
        m_mesh[0].sy = y + height;
        m_mesh[1].sx = x;
        m_mesh[1].sy = y;
        m_mesh[2].sx = x + width;
        m_mesh[2].sy = y + height;
        m_mesh[3].sx = x + width;
        m_mesh[3].sy = y;
    }//_SetMesh()

public:
    /// UV local translation
    /// unit is m_textrUStep and m_textrVStep which is 1/100 of width and height
    /// @param  uOffset: 1 to 100
    /// @param  vOffset: 1 to 100
	inline void UVTranslate(float uOffset, float vOffset)
    {
        // loop unrolling for speed-up
//        for (register int i=0; i<4; ++i)
//        {
//            m_mesh[i].tu += uOffset*m_textrUStep;
//            m_mesh[i].tv += vOffset*m_textrVStep;
//        }//for
        m_mesh[0].tu += uOffset*m_textrUStep;
        m_mesh[0].tv += vOffset*m_textrVStep;
        m_mesh[1].tu += uOffset*m_textrUStep;
        m_mesh[1].tv += vOffset*m_textrVStep;
        m_mesh[2].tu += uOffset*m_textrUStep;
        m_mesh[2].tv += vOffset*m_textrVStep;
        m_mesh[3].tu += uOffset*m_textrUStep;
        m_mesh[3].tv += vOffset*m_textrVStep;
    }//CSpriteEx::UVTranslate()

    /// rotate uv about current relative center.
    /// @param  radian: rotation radian
    inline void UVRotate(float radian)
    {
        // translation about m_textr?Center
        m_mesh[0].tu -= m_textrUCenter;
        m_mesh[1].tu -= m_textrUCenter;
        m_mesh[2].tu -= m_textrUCenter;
        m_mesh[3].tu -= m_textrUCenter;

        m_mesh[0].tv -= m_textrVCenter;
        m_mesh[1].tv -= m_textrVCenter;
        m_mesh[2].tv -= m_textrVCenter;
        m_mesh[3].tv -= m_textrVCenter;

        Rotate( m_mesh[0].tu, m_mesh[0].tv, radian);
        Rotate( m_mesh[1].tu, m_mesh[1].tv, radian);
        Rotate( m_mesh[2].tu, m_mesh[2].tv, radian);
        Rotate( m_mesh[3].tu, m_mesh[3].tv, radian);

        // retranslation about m_textr?Center
        m_mesh[0].tu += m_textrUCenter;
        m_mesh[1].tu += m_textrUCenter;
        m_mesh[2].tu += m_textrUCenter;
        m_mesh[3].tu += m_textrUCenter;

        m_mesh[0].tv += m_textrVCenter;
        m_mesh[1].tv += m_textrVCenter;
        m_mesh[2].tv += m_textrVCenter;
        m_mesh[3].tv += m_textrVCenter;
    }//UVRotate()

    /// rotate uv about current relative center.
    /// @param  degree: rotation degree
    inline void UVRotate(int degree) { UVRotate( D2R((float)degree) ); }

    /// local contract UV.
    /// @param  fPercentage: ranges from 0 to 100.f\n
    ///         uv contract hundred ratio
	inline void UVContract(float fPercentage)
    {
        m_mesh[0].tu += m_textrUStep*fPercentage;
        m_mesh[0].tv -= m_textrVStep*fPercentage;
        m_mesh[1].tu += m_textrUStep*fPercentage;
        m_mesh[1].tv += m_textrVStep*fPercentage;
    
        m_mesh[2].tu -= m_textrUStep*fPercentage;
        m_mesh[2].tv -= m_textrVStep*fPercentage;
        m_mesh[3].tu -= m_textrUStep*fPercentage;
        m_mesh[3].tv += m_textrVStep*fPercentage;
    }//UVContract()

    /// local expand UV.
    /// @param  fPercentage: ranges from 0 to 100.f
    ///         uv expand hundres ratio
    inline void UVExpand( float fPercentage)
    {
        m_mesh[0].tu -= m_textrUStep*fPercentage;
        m_mesh[0].tv += m_textrVStep*fPercentage;
        m_mesh[1].tu -= m_textrUStep*fPercentage;
        m_mesh[1].tv -= m_textrVStep*fPercentage;
    
        m_mesh[2].tu += m_textrUStep*fPercentage;
        m_mesh[2].tv += m_textrVStep*fPercentage;
        m_mesh[3].tu += m_textrUStep*fPercentage;
        m_mesh[3].tv -= m_textrVStep*fPercentage;
    }//UVExpand()

    /// local scaling UV.
    /// @param  fPercentage: ranges from -100.f to 100.f.
    inline void UVScale(float fPercentage)
    {
        if (fPercentage < 0.0f)
            UVContract( -fPercentage );
        else
            UVExpand( fPercentage );
    }//UVScale()

    /// get UV.
    /// @param  uv: [out] UV values
	void UVGet(float uv[8]);

    /// get UV to vertices.
    /// @param  vertex: [out] vertices which contain UV
	void UVGet(D3DVERTEX vertex[4]);

    /// get UV to TL vertices.
    /// @param  vertex: [out] TL vertices which contain UV
	void UVGet(D3DTLVERTEX vertex[4]);

    /// update only uv-coordinates of this vertices.
    /// @param  vertex: [in] new vertices
	inline void UVSet(const D3DVERTEX vertex[4])
    {
//        for (register int i=0; i<4; ++i)
//        {
//            m_mesh[i].tu = vertex[i].tu;
//            m_mesh[i].tv = vertex[i].tv;
//        }//for
        m_mesh[0].tu = vertex[0].tu;
        m_mesh[0].tv = vertex[0].tv;
        m_mesh[1].tu = vertex[1].tu;
        m_mesh[1].tv = vertex[1].tv;
        m_mesh[2].tu = vertex[2].tu;
        m_mesh[2].tv = vertex[2].tv;
        m_mesh[3].tu = vertex[3].tu;
        m_mesh[3].tv = vertex[3].tv;
    }//UVSet()

    /// update only uv-coordinates of this vertices.
    /// @param  vertex: [in] new vertices
	inline void UVSet(const D3DTLVERTEX vertex[4])
    {
//        for (register int i=0; i<4; ++i)
//        {
//            m_mesh[i].tu = vertex[i].tu;
//            m_mesh[i].tv = vertex[i].tv;
//        }//for
        m_mesh[0].tu = vertex[0].tu;
        m_mesh[0].tv = vertex[0].tv;
        m_mesh[1].tu = vertex[1].tu;
        m_mesh[1].tv = vertex[1].tv;
        m_mesh[2].tu = vertex[2].tu;
        m_mesh[2].tv = vertex[2].tv;
        m_mesh[3].tu = vertex[3].tu;
        m_mesh[3].tv = vertex[3].tv;
    }//UVSet()


    /// draw a sprite.
    /// @param  id: sprite index\n
    ///         when this is -1 don't set new uv coordinates
    inline void Draw(int x, int y, int id = -1)
    {
        if (id != -1)
            SetTextrCoords(id); // set texture coords

        _SetMesh(x, y, m_outWidth, m_outHeight); // set output coords

        SetRenderState();
        g_pd3dDevice->SetTexture( 0, m_pd3dTexture );
//        g_pd3dDevice->SetVertexShader( D3DFVF_TLVERTEX );
		g_pd3dDevice->SetFVF(D3DFVF_TLVERTEX);
        g_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2,
            m_mesh, sizeof(D3DTLVERTEX) );
        RestoreRenderState();
    }//Draw()

    /// draw a sprite.
    /// @param  id: sprite index\n
    ///         when this is -1 don't set new uv coordinates
    /// @param  width, height: sprite width and height
    inline void Draw(int x, int y, int id, int width, int height)
    {
        SetTextrCoords(id); // set texture coords
        _SetMesh(x, y, width, height); // set output coords

        SetRenderState();
        g_pd3dDevice->SetTexture( 0, m_pd3dTexture );
//        g_pd3dDevice->SetVertexShader( D3DFVF_TLVERTEX );
		g_pd3dDevice->SetVertexShader(NULL);
		g_pd3dDevice->SetFVF(D3DFVF_TLVERTEX );
        g_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2,
            m_mesh, sizeof(D3DTLVERTEX) );
        RestoreRenderState();
    }//Draw()

    /// draw id-th sprite on (x,y).
    /// @param  (x,y): screen coord of sprite center
	inline void DrawCenter(int x,int y, int id)
    {
        Draw( x-(m_outWidth>>1), y-(m_outHeight>>1), id );
    }//DrawCenter()

    /// draw sprite using current texture coords.
    /// you must properly set texture coords 
    /// using SetTextureCoords(left,top,width,height)
    /// ex) SetTextureCoords( 100,100, 20,50)
    ///     DrawDirect( 0,0, 40,50);
    ///     // draw (100,100)-(119,149) portion of image on screen's (0,0)-(39,49)
    ///     // original portion is properly scaled
    inline void DrawDirect(int x, int y, int width, int height)
    {
        _SetMesh(x,y, width,height);

        SetRenderState();
        g_pd3dDevice->SetTexture( 0, m_pd3dTexture );
		//g_pd3dDevice->SetVertexShader( D3DFVF_TLVERTEX );
		g_pd3dDevice->SetVertexShader(NULL);
        g_pd3dDevice->SetFVF( D3DFVF_TLVERTEX );
        g_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2,
            m_mesh, sizeof(D3DTLVERTEX) );
        RestoreRenderState();
    }//DrawDirect()

    /// draw portion of image on (xScreen, yScreen).
    /// @param  (xImage,yImage): image pixel coord
    /// @param  width, height: sprite(or image portion) width and height
    inline void DrawDirect(int xScreen, int yScreen, int xImage, int yImage, int width, int height)
    {
        SetTextrCoords(xImage, yImage, width, height);
        DrawDirect(xScreen, yScreen, width, height);
    }//DrawDirect()

public:
    /// set render state and vertex shader before call OutSpriteBare()
    inline void OutSpriteBarePrepare()
    {
        g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
        //g_pd3dDevice->SetVertexShader( D3DFVF_TLVERTEX );		        
		g_pd3dDevice->SetVertexShader(NULL);		        
		g_pd3dDevice->SetFVF( D3DFVF_TLVERTEX );

    }//OutSpriteBarePrepare()

    /// render a sprite.
    /// designed for onetime set state and many render like a draw a text.
    /// @param  pTextr: textures
    /// @param  x,y: screen coords.
    /// @param  id: sprite id
    inline void OutSpriteBare(LPDIRECT3DTEXTURE9 pTextr, int x, int y, int id)
    {
        SetTextrCoords(id); // set texture coords
        _SetMesh(x, y, m_outWidth, m_outHeight); // set mesh output coords

        g_pd3dDevice->SetTexture( 0, pTextr );
        g_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, m_mesh, sizeof(D3DTLVERTEX) );
    }//OutSpriteBare()

    /// restore rendre state.
    inline void OutSpriteBareRestore()
    {
        g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
    }//OutSpriteBareRestore()

    /// set texture coords and get mesh pointer.
    /// @param  id: sprite id\n
    ///         -1 means current sprite
    /// @return mesh pointer.
    D3DTLVERTEX* GetMesh(int id = -1);

    ///////////////////////////////////////////////////////////////////////
    // texture coordinates setting
    
    /// directly set texture coordinates from image dimension.
    /// image's rectangle area (x,y)-(x+widthPortion,y+heightPortion)
    /// @param  x,y: image coords.
    /// @param  widthPortion, heightPortion: image width and height
    void SetTextrCoords(int x, int y, int widthPortion, int heightPortion)
    {
        // 1------3
        // |      |
        // 0------2
        float   u, v;
        float   width, height;

        u       = (float)x;
        v       = (float)y;
        width   = (float)(widthPortion);
        height  = (float)(heightPortion);

        //ASSERT(x+widthPortion  <= m_widthImage);
        //ASSERT(y+heightPortion <= m_heightImage);

        u       /= m_widthImage;
        v       /= m_heightImage;
        width   /= m_widthImage;
        height  /= m_heightImage;
    
        m_mesh[0].tu = u;
        m_mesh[0].tv = v + height;
        m_mesh[1].tu = u;
        m_mesh[1].tv = v;
        m_mesh[2].tu = u + width;
        m_mesh[2].tv = v + height;
        m_mesh[3].tu = u + width;
        m_mesh[3].tv = v;
    }//SetTextrCoords()
    
    /// set i-th sprite's texture coordinates to 'm_mesh'.
    inline void SetTextrCoords(int i)
    {
        // 1------3
        // |      |
        // 0------2
        float   u, v;
        float   width, height; // width/height of a sprite
        int     row, col;      // sprite position on image

        // get sprite width and height
        width  = (float)(m_width);
        height = (float)(m_height);

        // get sprite position
        col    = (i % m_count);
        row    = (i / m_count);

        // get i-th sprite left-top position on image
        if (m_bBorder) // if image has a border
        {
            u = float(col * (m_width+1))  + 1.f;
            v = float(row * (m_height+1)) + 1.f;
        }
        else
        {
            u = float(col * m_width);
            v = float(row * m_height);
        }// if.. else..

        // convert to texture coords
        u /= m_widthImage;
        v /= m_heightImage;

        // set current texture's real uv width and height
        m_textrWidth   = width  / m_widthImage;
        m_textrHeight  = height / m_heightImage;

        // set uv center for later transform
        // Note: multiplication is slight faster than division
        //m_textrUCenter = u + m_textrWidth  / 2.0f;
        //m_textrVCenter = v + m_textrHeight / 2.0f;
        m_textrUCenter = u + m_textrWidth  * .5f;
        m_textrVCenter = v + m_textrHeight * .5f;

        // set 1/100 uv step for UVxxx() functions
        //m_textrUStep = m_textrWidth  / 100.0f;
        //m_textrVStep = m_textrHeight / 100.0f;
        m_textrUStep = m_textrWidth  * .01f;
        m_textrVStep = m_textrHeight * .01f;
    
        // set final texture coords
        m_mesh[0].tu = u;
        m_mesh[0].tv = v + m_textrHeight;
        m_mesh[1].tu = u;
        m_mesh[1].tv = v;
        m_mesh[2].tu = u + m_textrWidth;
        m_mesh[2].tv = v + m_textrHeight;
        m_mesh[3].tu = u + m_textrWidth;
        m_mesh[3].tv = v;
    }//SetTextrCoords()

    /// set inter sprite border flag.
    void SetBorder(BOOL bBorder = TRUE) { m_bBorder = bBorder; }
    void SetImageHeight(float _height) { m_heightImage = _height; }
    void SetImageWidth(float _width) { m_widthImage = _width; }

    /// constructors.
	CSpriteEx();
    /// constructors.
	CSpriteEx(int _count, int _width, int _height);
    /// destructor.
    virtual ~CSpriteEx();

    ///////////////////////////////////////////////////////////////////////
    // initialize original sprite info.

    /// set number of sprites in a row.
    inline void SetCount(int _count) { m_count = _count; }
	
    /// set original sprite size.
    inline void SetSize(int _width, int _height)
    {
        m_width = _width;
        m_height = _height;

        m_outWidth = m_width;
        m_outHeight = m_height;
    }//SetSize()

    /// set sprite information.
    /// @param  _count: number of sprites in a row
    /// @param  _width: sprite width in pixel
    /// @param  _height: sprite height
    /// @param  _widthImage: image width in pixel
    /// @param  _heightImage: image height in pixel
    /// @param  bBorder: sprite border exist or not
	virtual void SetInfo(int _count, int _width, int _height,
                         float _widthImage, float _heightImage, BOOL bBorder = FALSE);

    
    /// set sprite texture.
    inline void SetTexture(LPDIRECT3DTEXTURE9 _textr)
    {
        m_pd3dTexture = _textr;
    }//SetTexture()

    /// set font color and alpha.
    /// dc's each byte is alpha, red, green, and blue
    inline void SetColor(D3DCOLOR dc)
    {
        m_mesh[0].color = dc;
        m_mesh[1].color = dc;
        m_mesh[2].color = dc;
        m_mesh[3].color = dc;
    }//SetColor()

    /// set vertex alpha only.
    /// @param  fAlpha: from 0 to 1.f
    inline void SetAlpha(float fAlpha)
    {
        DWORD dwAlpha = DWORD(fAlpha*255.f);

        dwAlpha <<= 24;
        m_mesh[0].color = (m_mesh[0].color & 0x00ffffff) | dwAlpha;
        m_mesh[1].color = (m_mesh[1].color & 0x00ffffff) | dwAlpha;
        m_mesh[2].color = (m_mesh[2].color & 0x00ffffff) | dwAlpha;
        m_mesh[3].color = (m_mesh[3].color & 0x00ffffff) | dwAlpha;
    }//SetAlpha()

    /// set rendering sprite size in pixel.
    /// @param  _width: new sprite width in pixel
    /// @param  _height: new sprite height in pixel
	void SetOutSize(int _width = -1, int _height = -1);

    /// set render sprite size by ratio.
    /// @param  _ratio: ratio to original\n
    ///         if this value is 2.f, rendering sprite size will be doubled.
	inline void SetZoom(float _ratio = 1.0f)
    {
        m_outWidth  = int(_ratio*m_width);
        m_outHeight = int(_ratio*m_height);
    }//CSpriteEx::SetZoom()

    /// set render state before calling OutText() etc.
    inline void SetRenderState()
    {
        if (m_modeRender == CSE_TRANSPARENT) // TRANSPARENT
        {
            g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
        }//if
    }//SetRenderState()

    /// must be called after OutText().
    inline void RestoreRenderState()
    {
        g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
    }//RestoreRenderState()

    /// @param  mode 0: Opaque\n
    ///         mode 1: Transparent
    inline void SetRenderMode(int mode = CSE_TRANSPARENT) { m_modeRender = mode; }

};// class CSpriteEx

#endif // !defined(_CSpriteEx_DEFINED_)


/** @example    CSpriteEx

  0) declare

    CSpriteEx       m_sprite;


  1) on constructor or OnetimeSceneInit():



  2) on InitDeviceObjects():

    //.................................................
    // 2D sprite object
    g_d3dManager.CreateTexture( "test.bmp", 0xff000000 );
    m_sprite.SetInfo(2, 128, 128, 256, 256); // 128*128, 2 sprites per a row
      //             ^number of sprites in a row
      //                ^sprite size
      //                        ^image size


  3) on RestoreDeviceObjects():

    //................................................
    // 2D sprite object
    m_sprite.SetTexture( g_d3dManager.GetTexture("test.bmp") );


  4) on FrameMove():



  5) on Render():

        //.....................................................
        // 2D sprite object
        static int i = 0;
        m_sprite.SetRenderState();
        //m_sprite.SetZoom(1.0f);
        //m_sprite.SetOutSize(108,76);
        //m_sprite.SetColor(0x00800000);
        //m_sprite.SetRenderMode(2); // 1: transparent, 2: overlapped
        m_sprite.Draw(50, 50, i);
          //          ^(x,y)
          //                ^sprite index
        m_sprite.RestoreRenderState();
        i = (i+1) % 6;

        //
        m_sprite.SetTextrCoords(0,0,127,127);
        m_sprite.DrawDirect(300,0, 64,64);

        m_sprite.DrawDirect(200,100, 0,0,128,128);

        //-----------------------------------------------------
        // new feature coded on December 2000
        // for texture manupulation
        m_sprite.SetTextrCoords(0);
        m_sprite.UVContract( 25.0f );
        static int d = 0;
        d = (d + 1)%360;
        m_sprite.UVRotate( d );
        //m_sprite.UVTranslate( 15.0f, 0.0f );
        
        m_sprite.Draw(0,0);


  6) on MsgProc():



  7) on InvalidateDeviceObjects():



  8) on DeleteDeviceObjects():



  9) on destructor or FinalCleanup():



*/