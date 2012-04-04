// Another crc12

#ifndef __CD3DSPRITE_HEADER__
#define __CD3DSPRITE_HEADER__

typedef struct tagSpriteVertex
{
	float x , y , z;
	DWORD color;
	float tu , tv;
} SpriteVertex;

#define D3DFVF_SPRITEVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)

class CD3DSprite
{
public:
	int						m_nX , m_nY;
	int						m_nWidth , m_nHeight;
	SpriteVertex			m_vVertex[4];
	LPDIRECT3DTEXTURE9		m_Texture;
	LPDIRECT3DVERTEXBUFFER9	VertexBuffer;

public:
	void Create( int nX , int nY , int nWidth , int nHeight );
};


#endif __CD3DSPRITE_HEADER__
