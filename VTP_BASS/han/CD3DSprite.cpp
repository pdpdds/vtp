#include <d3dx9.h>
#include "CD3DSprite.h"

void CD3DSprite::Create( int nX , int nY , int nWidth , int nHeight )
{
	m_nX = nX;
	m_nY = nY;
	m_nWidth = nWidth;
	m_nHeight = nHeight;
	
	m_vVertex[0].x = nX - 0.5f;
	m_vVertex[0].y = nY - 0.5f;
	m_vVertex[0].z = 0.0f;
	m_vVertex[0].color = 0xFFFFFFFF;
	m_vVertex[0].tu = 0.0f;
	m_vVertex[0].tv = 0.0f;

	m_vVertex[1].x = (nX - 0.5f) + nWidth;
	m_vVertex[1].y = nY - 0.5f;
	m_vVertex[1].z = 0.0f;
	m_vVertex[1].color = 0xFFFFFFFF;
	m_vVertex[1].tu = 1.0f;
	m_vVertex[1].tv = 0.0f;

	m_vVertex[2].x = nX - 0.5f;
	m_vVertex[2].y = (nY - 0.5f) + nHeight;
	m_vVertex[2].z = 0.0f;
	m_vVertex[2].color = 0xFFFFFFFF;
	m_vVertex[2].tu = 0.0f;
	m_vVertex[2].tv = 1.0f;

	m_vVertex[3].x = (nX - 0.5f) + nWidth;
	m_vVertex[3].y = (nY - 0.5f) + nHeight;
	m_vVertex[3].z = 0.0f;
	m_vVertex[3].color = 0xFFFFFFFF;
	m_vVertex[3].tu = 1.0f;
	m_vVertex[3].tv = 1.0f;

	
}
