#include <math.h>
#include "resource.h"
#include "curtain.h"
#include <stdio.h>
#include <atlbase.h>
#include <dshow.h>
#include "han/D3DHangul.h"
#include "han/CD3DSprite.h"
#include "CD3DSprite2.h"
//#include "SoundManager.h"
#include "bass.h"
#include <math.h>


#define BANDS 29

#define ECHBUFLEN 1200	// buffer length
#define FLABUFLEN 350	// buffer length

#define SPECWIDTH 320	// display width
#define SPECHEIGHT 127	// height (changing requires palette adjustments too)

#define EFFECT_FLANGER       0		
#define EFFECT_ROTATE        1                        					
#define EFFECT_ECHO          2               
#define UNEFFECT_FLANGER	 3	
#define UNEFFECT_ROTATE      4                     					
#define UNEFFECT_ECHO        5   
			

	DWORD m_volume;
	DWORD floatable; // floating-point channel support?
	DWORD chan;	// the channel... HMUSIC or HSTREAM	
	int specmode;
	int specpos; // spectrum mode (and marker pos for 2nd mode)	
	BYTE specbuf[SPECWIDTH*SPECHEIGHT];
	BYTE specbuf2[SPECWIDTH];
	float specbuf2timeset[SPECWIDTH*2];

//#define DEBUG_VS   // Uncomment this line to debug vertex shaders 
//#define DEBUG_PS   // Uncomment this line to debug pixel shaders 
CD3DSprite2 AAAA;
struct AAA
{
    float x, y, z, h;
    D3DCOLOR color;
    float tu, tv;

    static DWORD FVF;
};
DWORD AAA::FVF=D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;

#define ALPHA_BLENDING      0
#define IMAGE_SPRITE        1
#define IMAGE_BOOK          2
#define ALL_SPRITE          10
#define IMAGE_PRESENT       11
#define CURTAIN_SPRITE      12
#define DEFALUT_SPRITE      13
int gScreen_Mode=IMAGE_PRESENT;
bool g_bCurtain=false;
//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
ID3DXFont*              g_pFont = NULL;         // Font for drawing text
ID3DXFont*              g_pFont2 = NULL;         // Font for drawing text
ID3DXSprite*            g_pTextSprite = NULL;   // Sprite for batching draw text calls
ID3DXSprite*            g_pTextSprite2 = NULL;   // Sprite for batching draw text calls

D3DXMATRIXA16           g_mView;
D3DXMATRIXA16           g_mWorld;

CModelViewerCamera      g_Camera;               // A model viewing camera
CDXUTDialog             g_HUD;                  // dialog for standard controls
CDXUTDialog             g_MP3_UI;             // dialog for sample specific controls
IDirect3DDevice9* g_pd3dDevice;
HWND g_Hwnd;	

//한글폰트를 이용한 한글출력
CD3DHangul  g_d3dHangul;
BYTE bkRed = 239;
BYTE bkGrn = 84;
BYTE bkBlu = 137;
//--------------------------------------------------------------------------------------
// UI control IDs
//--------------------------------------------------------------------------------------
#define IDC_TOGGLEFULLSCREEN    1
#define IDC_TOGGLEREF           3
#define IDC_CHANGEDEVICE        4
#define IDC_START               5
#define IDC_LOAD                6
#define IDC_EDITBOX1            7
#define IDC_EDITBOX2            8
#define IDC_COMBOBOX            9
#define IDC_STATIC              10
#define IDC_OUTPUT              12
#define IDC_SLIDER              13
#define IDC_CHECKLOOP           14
#define IDC_CHECKIMAGE          15
#define IDC_FLANGER         26
#define IDC_ECHO         27
#define IDC_ROTATE         29
#define IDC_CURTAIN             28
#define IDC_RADIO1A             16
#define IDC_RADIO1B             17
#define IDC_RADIO1C             18
#define IDC_RADIO2A             19
#define IDC_RADIO2B             20
#define IDC_RADIO2C             21
#define IDC_LISTBOX             22
#define IDC_LISTBOXM            23

#define IDC_CONFIG              24
#define IDC_EXIT                25
#define WM_GRAPHNOTIFY  WM_USER+13
#define WM_GRAPHNOTIFY2  WM_USER+14
#define WM_Pos_Change  WM_USER+100
#define MAX_IMAGE 10

// global game data
// very important, don't crack me...

int g_GameStart=0;
int g_BgMusic=0;
float g_width=358.f;
float g_height=322.f;
float g_sum=150.f;
float g_Defaut_Width=640.0f;
int g_temp2=0;


float g_temp=-320.0f;

//--------------------------------------------------------------------------------------
// Forward declarations 
//--------------------------------------------------------------------------------------
bool    CALLBACK IsDeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, bool bWindowed );
void    CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, const D3DCAPS9* pCaps );
HRESULT CALLBACK OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc );
HRESULT CALLBACK OnResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc );
void    CALLBACK OnFrameMove( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime );
void    CALLBACK OnFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime );
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing );
void    CALLBACK KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown  );
void    CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl );
void    CALLBACK OnLostDevice();
void    CALLBACK OnDestroyDevice();

void    InitApp();
void    RenderText();

//DirectShow Code
HRESULT GraphInit(void);
HRESULT SwapSourceFilter(LPCTSTR wstr);
HRESULT SwapSourceFilter2(int i);

HRESULT HandleGraphEvent(void);
HRESULT HandleGraphEvent2(void);
int g_effectRunning=1;
int gPlaying=0;
TCHAR          g_szCurrentFile[128];

LPCTSTR pstrFiles[] =
{
    TEXT("music/The Sign.mp3\0"),
    TEXT("music/The Sign.smi\0"),
    TEXT("music/scene2.mp3\0"),
	TEXT("music/scene3.mp3\0"),
	TEXT("music/scene4.mp3\0"),
};

WCHAR g_wFileName[MAX_PATH];          
WCHAR g_wFileName2[MAX_PATH];         
WCHAR g_wFileName3[MAX_PATH];         
WCHAR g_wFileName4[MAX_PATH];         


LPCTSTR g_pstrScene[] =
{
    TEXT("image\\image1.jpg"),
	TEXT("image\\image2.jpg"),
	TEXT("image\\image3.jpg"),
	TEXT("image\\image4.jpg"),	
	TEXT("image\\image5.jpg"),	
	TEXT("image\\image6.jpg"),	
	TEXT("image\\image7.jpg"),	
	TEXT("image\\image8.jpg"),	
	TEXT("image\\image9.jpg"),	
	TEXT("image\\image10.jpg"),		
	TEXT("image\\title.jpg"),	
};


//implemented for 2d image sprite.. alpha blending fade, and so on
// vertex structure definition, function....

LPDIRECT3DVERTEXBUFFER9 g_pVB_Fade[MAX_IMAGE]        = {NULL}; // Buffer to hold vertices
LPDIRECT3DTEXTURE9      g_pTexture_Fade[MAX_IMAGE]   = {NULL}; // Our texture
LPDIRECT3DVERTEXBUFFER9 g_pVB_Opening        = {NULL}; // Buffer to hold vertices
LPDIRECT3DTEXTURE9      g_pTexture_Opening   = {NULL}; // Our texture
LPDIRECT3DVERTEXBUFFER9 g_pVB_Loading        = {NULL}; // Buffer to hold vertices
LPDIRECT3DTEXTURE9      g_pTexture_Loading   = {NULL}; // Our texture
LPDIRECT3DVERTEXBUFFER9 g_pVB_Book        = {NULL}; // Buffer to hold vertices.. Book Sprite
LPDIRECT3DVERTEXBUFFER9 g_pVB_Book2        = {NULL}; // Buffer to hold vertices.. Book Sprite
LPDIRECT3DVERTEXBUFFER9 g_pVB_Book3        = {NULL}; // Buffer to hold vertices.. Book Sprite
LPDIRECT3DVERTEXBUFFER9 g_pVB_Book4        = {NULL}; // Buffer to hold vertices.. Book Sprite
 


LPDIRECT3DVERTEXBUFFER9 g_pVB_Curtain=NULL;//커텐구현
LPDIRECT3DINDEXBUFFER9 g_pIB_Curtain=NULL;
int g_Rand=0;
int g_Rand2=1;
float g_time=0.0f;

struct CUSTOMVERTEX
{
  D3DXVECTOR3 position; // The position
  D3DCOLOR    color;    // The color
  FLOAT       tu, tv;   // The texture coordinates
};
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)
D3DXCOLOR g_FadeToColor;
bool FadeQuadVerts(D3DXCOLOR FadeToColor, float fFadeTime, 
                   float fEffectTime, LPDIRECT3DVERTEXBUFFER9 pVB,int effect_num);
HRESULT Create_3DSprite( IDirect3DDevice9* pd3dDevice,LPDIRECT3DTEXTURE9 &Test_Texture,LPDIRECT3DVERTEXBUFFER9 &Test_Buffer,LPCTSTR lpstr,float x, float y, float width, float height,int alpha);
HRESULT Create_Book_VertexBuffer( IDirect3DDevice9* pd3dDevice,float x, float y, float width, float height,int alpha);
bool BookQuadVerts( IDirect3DDevice9* pd3dDevice,float fElapsedTime, float fEffectTime, LPDIRECT3DVERTEXBUFFER9 pVB,int BookQuadVerts,D3DXMATRIX* matWorld);
HRESULT DisplayFileDuration(void);
HRESULT InitCurtainGeometry();

void ReadMediaPosition(float a, float b);
void SearchFileList2(IDirect3DDevice9* pd3dDevice,float x, float y); //플리핑 되는 배경 이미지를 랜덤하게 생성
void NEXT_PLAY_MP3(DXUTListBoxItem *pItem,int chg);

// DirectShow Graph, Filter & Pins used
IGraphBuilder *g_pGraphBuilder  = NULL;
IMediaControl *g_pMediaControl  = NULL;
IMediaSeeking *g_pMediaSeeking  = NULL;
IBaseFilter   *g_pSourceCurrent = NULL;
IBaseFilter   *g_pSourceNext    = NULL;
IMediaEventEx *g_pME   = NULL;

IGraphBuilder *g_pGraphBuilder2  = NULL;
IMediaControl *g_pMediaControl2  = NULL;
IMediaSeeking *g_pMediaSeeking2  = NULL;
IBaseFilter   *g_pSourceCurrent2 = NULL;
IBaseFilter   *g_pSourceNext2    = NULL;
IMediaEventEx *g_pME2   = NULL;

HRESULT Render_Caption(LPCTSTR wFileName);
char	m_szCaption[MAX_PATH];
void Image_Mode_Change();
bool g_bSMI_exist=false;
bool g_bLoop=false;
bool g_bimage_change=false;
int g_Image_Mode=ALL_SPRITE;
int g_current_mode=ALPHA_BLENDING;
bool g_bimage_effect=false;
bool g_bgui_hide=false;
LPCTSTR g_pstrNO_SMI[] =
{
    TEXT("자막이 존재하지 않습니다"),
};
int g_strlen=0;
int g_strlen2=0;
int g_strlen3=0;
bool g_bfile_str=false;
int g_text_mode=0;//몇 라인이 존재하는지 확인
int g_text_mode2=0;  //원문만 출력 또는 번역 및 발음까지 출력할 것인지를 세팅
int g_Sound_Play=0;  //현재 음악이 재생중인지를 체크		
char	g_szFileName[MAX_PATH];
    
//SlideBar Update, Media Position Change....
REFERENCE_TIME g_rtTotalTime;
DWORD g_dwtime=0;
bool b_sliderbar_check=false;
bool g_click=false;

//리스트 박스관련
void SearchFileList(); //파일을 리스트 박스에 추가
int g_Num=0; // 파일의 수
int g_start=0;
int g_start2=0;
int g_Music_Start=0;
int g_wait=1;
float g_image_pos_x=-3.0f;
float g_image_pos_y=1.0f;
float g_rotate_angle=0.0f;


//Duplicated code... For Opening....
D3DXCOLOR g_FadeToColor2;
bool FadeQuadVerts2(D3DXCOLOR FadeToColor, float fFadeTime, 
                   float fEffectTime, LPDIRECT3DVERTEXBUFFER9 pVB);

// BASS!!

QWORD gMedia_Len=0;		
QWORD gMedia_Cur=0;
#define SPECWIDTH 320	// display width
#define SPECHEIGHT 127	// height (changing requires palette adjustments too)

BOOL g_bSpec_hide=false;

void UpdateSpectrum()
{
	HDC dc;
	int x,y,y1;
	float fft[1024]; // get the FFT data
	BASS_ChannelGetData(chan,fft,BASS_DATA_FFT2048);

	if (!specmode) { // "normal" FFT
		memset(specbuf,0,SPECWIDTH*SPECHEIGHT);
		
		for (x=0;x<SPECWIDTH/2;x++) {
#if 1
			y=sqrt(fft[x+1])*3*SPECHEIGHT-4; // scale it (sqrt to make low values more visible)
#else
			y=fft[x+1]*10*SPECHEIGHT; // scale it (linearly)
#endif
			if (y>SPECHEIGHT) y=SPECHEIGHT; // cap it
			if (x && (y1=(y+y1)/2)) // interpolate from previous to make the display smoother
				while (--y1>=0) specbuf[y1*SPECWIDTH+x*2-1]=y1+1;
			y1=y;
			while (--y>=0) {				
				specbuf[y*SPECWIDTH+x*2]=y+1; // draw level
			}
		}
	} else if (specmode==1) { // logarithmic, acumulate & average bins
		int b0=0;
		memset(specbuf,0,SPECWIDTH*SPECHEIGHT);
		
#define BANDS 29
		for (x=0;x<BANDS;x++) {
			float sum=0;
			int sc,jk;
			jk=x*10.0/(BANDS-1);
			int b1=pow(2.0,jk);
			if (b1>1023) b1=1023;
			if (b1<=b0) b1=b0+1; // make sure it uses at least 1 FFT bin
			sc=10+b1-b0;
			for (;b0<b1;b0++) sum+=fft[1+b0];
			y=(sqrt(sum/(log10((double)sc)))*1.7*SPECHEIGHT)-4; // scale it
			if (y>SPECHEIGHT) y=SPECHEIGHT; // cap it
			while (--y>=0)
				memset(specbuf+y*SPECWIDTH+x*(SPECWIDTH/BANDS),y+1,0.9*(SPECWIDTH/BANDS)); // draw bar
		}
	} else { // "3D"
		for (x=0;x<SPECHEIGHT;x++) {
			y=sqrt(fft[x+1])*3*127; // scale it (sqrt to make low values more visible)
			if (y>127) y=127; // cap it
			specbuf[x*SPECWIDTH+specpos]=128+y; // plot it
		}
		// move marker onto next position
		specpos=(specpos+1)%SPECWIDTH;
		for (x=0;x<SPECHEIGHT;x++) specbuf[x*SPECWIDTH+specpos]=255;
	}

}

float g_ratio=0;
DWORD g_dVol=0;

/* "rotate" */
	HDSP rotdsp=0;	// DSP handle
	float rotpos;	// cur.pos
	
	/* "echo" */
	HDSP echdsp=0;	// DSP handle
	float echbuf[ECHBUFLEN][2];	// buffer
	int echpos;	// cur.pos

	/* "flanger" */
	HDSP fladsp=0;	// DSP handle
	float flabuf[FLABUFLEN][2];	// buffer
	int flapos;	// cur.pos
	float flas,flasinc;	// sweep pos/increment
void CALLBACK Rotate(HDSP handle, DWORD channel, void *buffer, DWORD length, DWORD user)
{
	float *d=(float*)buffer;
	DWORD a;
	for (a=0;a<length/4;a+=2) {
		d[a]*=fabs(sin(rotpos));
		d[a+1]*=fabs(cos(rotpos));
		rotpos=fmod(rotpos+0.00003,3.1415927);
	}
}




void CALLBACK Flange(HDSP handle, DWORD channel, void *buffer, DWORD length, DWORD user)
{
	float *d=(float*)buffer;
	DWORD a;

	for (a=0;a<length/4;a+=2) {
		int p1=(flapos+(int)flas)%FLABUFLEN;
		int p2=(p1+1)%FLABUFLEN;
		float f=fmod(flas,1);
		float s;

		s=d[a]+((flabuf[p1][0]*(1-f))+(flabuf[p2][0]*f));
		flabuf[flapos][0]=d[a];
		d[a]=s;

		s=d[a+1]+((flabuf[p1][1]*(1-f))+(flabuf[p2][1]*f));
		flabuf[flapos][1]=d[a+1];
		d[a+1]=s;

		flapos++;
		if (flapos==FLABUFLEN) flapos=0;
		flas+=flasinc;
		if (flas<0.0 || flas>FLABUFLEN)
			flasinc=-flasinc;
	}
}

void CALLBACK Echo(HDSP handle, DWORD channel, void *buffer, DWORD length, DWORD user)
{
	float *d=(float*)buffer;
	DWORD a;

	for (a=0;a<length/4;a+=2) {
		float l=d[a]+(echbuf[echpos][1]/2);
		float r=d[a+1]+(echbuf[echpos][0]/2);
#if 1 // 0=echo, 1=basic "bathroom" reverb
		echbuf[echpos][0]=d[a]=l;
		echbuf[echpos][1]=d[a+1]=r;
#else
		echbuf[echpos][0]=d[a];
		echbuf[echpos][1]=d[a+1];
		d[a]=l;
		d[a+1]=r;
#endif
		echpos++;
		if (echpos==ECHBUFLEN) echpos=0;
	}
}