#include <d3dx9.h>
#include "CD3DSprite2.h"

#define BANDS 29
void CD3DSprite2::Create( float nX , float nY , float nWidth , float nHeight,BYTE color,float g_ratio )
{
	m_nX = nX;
	m_nY = nY;
	m_nWidth = nWidth;
	m_nHeight = nHeight;
	
	m_vVertex[0].x = nX;
	m_vVertex[0].y = nY;
	m_vVertex[0].z = 0.0f;
	m_vVertex[0].color = 0xFFFFFFFF;
	m_vVertex[0].tu = ((1.0f+nX)/(2.0f));
	m_vVertex[0].tv = 1.0f-((1.0f+nY-g_ratio)/(0.5f));

	m_vVertex[1].x = (nX) + nWidth;
	m_vVertex[1].y = nY;
	m_vVertex[1].z = 0.0f;
	m_vVertex[1].color = 0xFFFFFFFF;
	m_vVertex[1].tu = ((1.0f+nX)/(2.0f))+((1.0f)/(float)BANDS);
	m_vVertex[1].tv = 1.0f-((1.0f+nY-g_ratio)/(0.5f));

	m_vVertex[2].x = nX ;
	m_vVertex[2].y = (nY) + nHeight;
	m_vVertex[2].z = 0.0f;
	m_vVertex[2].color = 0xFFFFFFFF;
	m_vVertex[2].tu = ((1.0f+nX)/(2.0f));
	m_vVertex[2].tv = 1.0f-((1.0f+nY-g_ratio)/(0.5f))-((1.0f)/((float)(BANDS)));


	m_vVertex[3].x = (nX) + nWidth;
	m_vVertex[3].y = nY;
	m_vVertex[3].z = 0.0f;
	m_vVertex[3].color = 0xFFFFFFFF;
	m_vVertex[3].tu = ((1.0f+nX)/(2.0f))+((1.0f)/(float)BANDS);
	m_vVertex[3].tv = 1.0f-((1.0f+nY-g_ratio)/(0.5));

m_vVertex[4].x = (nX);
	m_vVertex[4].y = (nY) + nHeight;
	m_vVertex[4].z = 0.0f;
	m_vVertex[4].color = 0xFFFFFFFF;
	m_vVertex[4].tu = ((1.0f+nX)/(2.0f));
 	m_vVertex[4].tv = 1.0f-((1.0f+nY-g_ratio)/(0.5f))-((1.0f)/((float)(BANDS)));

	m_vVertex[5].x = nX + nWidth ;
	m_vVertex[5].y = (nY) + nHeight;
	m_vVertex[5].z = 0.0f;
	m_vVertex[5].color = 0xFFFFFFFF;
	m_vVertex[5].tu = ((1.0f+nX)/(2.0f))+((1.0f)/(float)BANDS);
   	m_vVertex[5].tv = 1.0f-((1.0f+nY-g_ratio)/(0.5f))-((1.0f)/((float)(BANDS)));

	

	
}
/*void CD3DSprite::Create( int nX , int nY , int nWidth , int nHeight )
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

	
}*/
