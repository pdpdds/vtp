// Another crc12

#ifndef __CD3DSPRITE2_HEADER__
#define __CD3DSPRITE2_HEADER__

typedef struct tagSpriteVertex2
{
	float x , y , z;
	DWORD color;
	float tu , tv;
} SpriteVertex2;

#define D3DFVF_SPRITEVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)

class CD3DSprite2
{
public:
	int						m_nX , m_nY;
	int						m_nWidth , m_nHeight;
	SpriteVertex2			m_vVertex[6];
	LPDIRECT3DTEXTURE9		m_Texture;
	LPDIRECT3DVERTEXBUFFER9	VertexBuffer;

public:
	void Create( float nX , float nY , float nWidth , float nHeight,BYTE a ,float b);
};


#endif __CD3DSPRITE_HEADER2__
