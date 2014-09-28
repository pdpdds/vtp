//--------------------------------------------------------------------------------------
// File: vtp2005.cpp
//
// Starting point for new Direct3D applications
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
// Created by Park Ju-hang, sogang univ
//--------------------------------------------------------------------------------------



#include "dxstdafx.h"
#include "vtp2005.h"
 #include <atlbase.h> 
#include <atlconv.h>
#include "strmif.h"

//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
INT WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR file_str, int )
{
    // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	if(!strcmp(file_str,""))
	{
		strcpy(g_szFileName,file_str);
		g_bfile_str=TRUE;
		MultiByteToWideChar( CP_ACP, 0, file_str, -1,                          							
			g_wFileName2, MAX_PATH );				
		//MessageBox(NULL,L"L",L"L",MB_OK);
	}
    if (BASS_GetVersion()!=MAKELONG(2,2)) {
		MessageBox(0,L"BASS version 2.2 was not loaded",L"Incorrect BASS.DLL",0);
		return 0;   
	}
	   


	g_ratio=(g_sum/g_width);
    // Set the callback functions. These functions allow the sample framework to notify
    // the application about device changes, user input, and windows messages.  The 
    // callbacks are optional so you need only set callbacks for events you're interested 
    // in. However, if you don't handle the device reset/lost callbacks then the sample 
    // framework won't be able to reset your device since the application must first 
    // release all device resources before resetting.  Likewise, if you don't handle the 
    // device created/destroyed callbacks then the sample framework won't be able to 
    // recreate your device resources.
    DXUTSetCallbackDeviceCreated( OnCreateDevice );
    DXUTSetCallbackDeviceReset( OnResetDevice );
    DXUTSetCallbackDeviceLost( OnLostDevice );
    DXUTSetCallbackDeviceDestroyed( OnDestroyDevice );
    DXUTSetCallbackMsgProc( MsgProc );
    DXUTSetCallbackKeyboard( KeyboardProc );
    DXUTSetCallbackFrameRender( OnFrameRender );
    DXUTSetCallbackFrameMove( OnFrameMove );

    // Show the cursor and clip it when in full screen
    DXUTSetCursorSettings( true, true );

    InitApp();

    // Initialize the sample framework and create the desired Win32 window and Direct3D 
    // device for the application. Calling each of these functions is optional, but they
    // allow you to set several options which control the behavior of the framework.
    DXUTInit( true, false, false ); // Parse the command line, handle the default hotkeys, and show msgboxes
 
    DXUTCreateWindow( L"Visual Text Player 2005" );
    DXUTCreateDevice( D3DADAPTER_DEFAULT, true, 358, g_height+g_sum, IsDeviceAcceptable, ModifyDeviceSettings );
	GraphInit();
    // Pass control to the sample framework for handling the message pump and 
    // dispatching render calls. The sample framework will call your FrameMove 
    // and FrameRender callback when there is idle time between handling window messages.
    DXUTMainLoop();

    // Perform any application-level cleanup here. Direct3D device resources are released within the
    // appropriate callback functions and therefore don't require any cleanup code here.

    return DXUTGetExitCode();
}


//--------------------------------------------------------------------------------------
// Initialize the app 
//--------------------------------------------------------------------------------------
void InitApp()
{
    // Initialize dialogs
    g_HUD.SetCallback( OnGUIEvent ); int iY = 10; 
    //g_HUD.AddButton( IDC_TOGGLEFULLSCREEN, L"Toggle full screen", 35, iY, 125, 22 );
    //g_HUD.AddButton( IDC_TOGGLEREF, L"Toggle REF (F3)", 35, iY += 24, 125, 22 );
    //g_HUD.AddButton( IDC_CHANGEDEVICE, L"Change device (F2)", 35, iY += 24, 125, 22 );

    g_MP3_UI.SetCallback( OnGUIEvent );
    g_MP3_UI.SetFont( 1, L"MD아롱체", 24, FW_NORMAL );
    g_MP3_UI.SetFont( 2, L"MD아롱체", 16, FW_NORMAL );
	memset(specbuf2,0,SPECWIDTH);
	memset(specbuf2timeset,0,SPECWIDTH*2);
    // Static
    /*g_SampleUI.AddStatic( IDC_STATIC, L"This is a static control.", 0, 0, 200, 30 );
    g_SampleUI.AddStatic( IDC_OUTPUT, L"한글이 control provides feedback for your action.  It will change as you interact with the UI controls.", 20, 50, 620, 300 );
    g_SampleUI.GetStatic( IDC_OUTPUT )->SetTextColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) ); // Change color to red
    g_SampleUI.GetStatic( IDC_STATIC )->SetTextColor( D3DCOLOR_ARGB( 255, 0, 255, 0 ) ); // Change color to green
    g_SampleUI.GetControl( IDC_OUTPUT )->GetElement( 0 )->dwTextFormat = DT_LEFT|DT_TOP|DT_WORDBREAK;
    g_SampleUI.GetControl( IDC_OUTPUT )->GetElement( 0 )->iFont = 2;
    g_SampleUI.GetControl( IDC_STATIC )->GetElement( 0 )->dwTextFormat = DT_CENTER|DT_VCENTER|DT_WORDBREAK;*/


    // Buttons
    /*g_SampleUI.AddButton( IDC_START, L"게임시작", 30, 390, 80, 35, L'S' );
    g_SampleUI.AddButton( IDC_LOAD, L"로드", 30, 50, 80, 35, L'L' );
	g_SampleUI.AddButton( IDC_CONFIG, L"환경설정", 30, 390, 80, 35, L'C' );
    g_SampleUI.AddButton( IDC_EXIT, L"끝내기", 30, 50, 80, 35, L'E' );*/

	
    // Edit box
    /*g_SampleUI.AddEditBox( IDC_EDITBOX1, L"Edit control with default styles. Type text here and press Enter", 20, 440, 600, 32 );

    // IME-enabled edit box
    CDXUTIMEEditBox *pIMEEdit;
    if( SUCCEEDED( g_SampleUI.AddIMEEditBox( IDC_EDITBOX2, L"IME-capable edit control with custom styles. Type and press Enter", 20, 390, 600, 45, false, &pIMEEdit ) ) )
    {
        pIMEEdit->GetElement( 0 )->iFont = 1;
        pIMEEdit->GetElement( 1 )->iFont = 1;
        pIMEEdit->GetElement( 9 )->iFont = 1;
        pIMEEdit->GetElement( 0 )->TextureColor.Init( D3DCOLOR_ARGB( 128, 255, 255, 255 ) );  // Transparent center
        pIMEEdit->SetBorderWidth( 7 );
        pIMEEdit->SetTextColor( D3DCOLOR_ARGB( 255, 64, 64, 64 ) );
        pIMEEdit->SetCaretColor( D3DCOLOR_ARGB( 255, 64, 64, 64 ) );
        pIMEEdit->SetSelectedTextColor( D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
        pIMEEdit->SetSelectedBackColor( D3DCOLOR_ARGB( 255, 40, 72, 72 ) );
    }*/

    // Slider
    g_MP3_UI.AddSlider(IDC_SLIDER, 10, 100, g_width-25, 24, 0, 100, 0, false );
	g_MP3_UI.AddListBox( IDC_LISTBOXM, 30, 400, 100, 150, CDXUTListBox::MULTISELECTION );   	
	SearchFileList();
        

    // Checkbox
    g_MP3_UI.AddCheckBox( IDC_CHECKLOOP, L"Loop",
                            10, 30, 150, 14, false, L'C', false );
    g_MP3_UI.AddCheckBox( IDC_CHECKIMAGE, L"Image Change(Random)",
                            10, 45, 150, 14, false, L'T', false );
	g_MP3_UI.AddCheckBox( IDC_FLANGER, L"FLANGER",
                            10, 75, 150, 14, false, L'T', false );
	g_MP3_UI.AddCheckBox( IDC_ECHO, L"ECHO",
                            10, 90, 150, 14, false, L'T', false );
	g_MP3_UI.AddCheckBox( IDC_ROTATE, L"ROTATE",
                            10, 105, 150, 14, false, L'T', false );
	g_MP3_UI.AddCheckBox( IDC_CURTAIN, L"Curtain",
                            10, 60, 150, 14, false, L'T', false );

	g_MP3_UI.AddRadioButton( IDC_RADIO1A, 1, L"원문 출력", 0, 50, 100, 14, true, L'1' );
    g_MP3_UI.AddRadioButton( IDC_RADIO1B, 1, L"원문+번역", 0, 50, 100, 14, false, L'2' );
    g_MP3_UI.AddRadioButton( IDC_RADIO1C, 1, L"원문+번역+발음", 0, 50, 100, 14, false, L'3' );

	/*g_MP3_UI.AddCheckBox( IDC_FLANGER, L"Point Sprite(not yet)",
                            10, 75, 150, 14, false, L'T', false );
	g_MP3_UI.AddCheckBox( IDC_IMAGEEFFECT, L"Image Effect(not yet)",
                            10, 90, 150, 14, false, L'T', false );*/
    
	// Combobox
    /*CDXUTComboBox *pCombo;
    g_SampleUI.AddComboBox( IDC_COMBOBOX, 0, 0, 200, 24, L'O', false, &pCombo );
    if( pCombo )
    {
        pCombo->SetDropHeight( 100 );
        pCombo->AddItem( L"Combobox item (O)", (LPVOID)0x11111111 );
        pCombo->AddItem( L"Placeholder (O)", (LPVOID)0x12121212 );
        pCombo->AddItem( L"One more (O)", (LPVOID)0x13131313 );
        pCombo->AddItem( L"I can't get enough (O)", (LPVOID)0x14141414 );
        pCombo->AddItem( L"Ok, last one, I promise (O)", (LPVOID)0x15151515 );
    }*/

    // Radio buttons
    

   

    // List box
    /*g_SampleUI.AddListBox( IDC_LISTBOX, 30, 400, 200, 150, 0 );
    for( int i = 0; i < 15; ++i )
    {
        WCHAR wszText[50];
        StringCchPrintf( wszText, 50, L"Single-selection listbox item %d", i );
        g_SampleUI.GetListBox( IDC_LISTBOX )->AddItem( wszText, (LPVOID)(size_t)i );
    }
    */
}


//--------------------------------------------------------------------------------------
// Called during device initialization, this code checks the device for some 
// minimum set of capabilities, and rejects those that don't pass by returning false.
//--------------------------------------------------------------------------------------
bool CALLBACK IsDeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, 
                                  D3DFORMAT BackBufferFormat, bool bWindowed )
{
    // Skip backbuffer formats that don't support alpha blending
    IDirect3D9* pD3D = DXUTGetD3DObject(); 
    if( FAILED( pD3D->CheckDeviceFormat( pCaps->AdapterOrdinal, pCaps->DeviceType,
                    AdapterFormat, D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING, 
                    D3DRTYPE_TEXTURE, BackBufferFormat ) ) )
        return false;
/*
    // Must support pixel shader 1.1
    if( pCaps->PixelShaderVersion < D3DPS_VERSION( 1, 1 ) )
        return false;*/

    return true;
}


//--------------------------------------------------------------------------------------
// This callback function is called immediately before a device is created to allow the 
// application to modify the device settings. The supplied pDeviceSettings parameter 
// contains the settings that the framework has selected for the new device, and the 
// application can make any desired changes directly to this structure.  Note however that 
// the sample framework will not correct invalid device settings so care must be taken 
// to return valid device settings, otherwise IDirect3D9::CreateDevice() will fail.  
//--------------------------------------------------------------------------------------
void CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, const D3DCAPS9* pCaps )
{
    // If device doesn't support HW T&L or doesn't support 1.1 vertex shaders in HW 
    // then switch to SWVP.
    if( (pCaps->DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 ||
         pCaps->VertexShaderVersion < D3DVS_VERSION(1,1) )
    {
        pDeviceSettings->BehaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    }
    else
    {
        pDeviceSettings->BehaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;
    }

    // This application is designed to work on a pure device by not using 
    // IDirect3D9::Get*() methods, so create a pure device if supported and using HWVP.
    if ((pCaps->DevCaps & D3DDEVCAPS_PUREDEVICE) != 0 && 
        (pDeviceSettings->BehaviorFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING) != 0 )
        pDeviceSettings->BehaviorFlags |= D3DCREATE_PUREDEVICE;

    // Debugging vertex shaders requires either REF or software vertex processing 
    // and debugging pixel shaders requires REF.  
#ifdef DEBUG_VS
    if( pDeviceSettings->DeviceType != D3DDEVTYPE_REF )
    {
        pDeviceSettings->BehaviorFlags &= ~D3DCREATE_HARDWARE_VERTEXPROCESSING;
        pDeviceSettings->BehaviorFlags &= ~D3DCREATE_PUREDEVICE;                            
        pDeviceSettings->BehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    }
#endif
#ifdef DEBUG_PS
    pDeviceSettings->DeviceType = D3DDEVTYPE_REF;
#endif
}


//--------------------------------------------------------------------------------------
// This callback function will be called immediately after the Direct3D device has been 
// created, which will happen during application initialization and windowed/full screen 
// toggles. This is the best location to create D3DPOOL_MANAGED resources since these 
// resources need to be reloaded whenever the device is destroyed. Resources created  
// here should be released in the OnDestroyDevice callback. 
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc )
{
    HRESULT hr;	
	AAAA.Create(-1.0f,-1.0f,1.0f,0.5f,0,g_ratio);
	BASS_SetConfig(BASS_CONFIG_FLOATDSP,TRUE);
			// initialize - default device, 44100hz, stereo, 16 bits
			if (!BASS_Init(1,44100,0,NULL,NULL)) {
				return 0;
			}
			// check for floating-point capability
			floatable=BASS_StreamCreate(44100,2,BASS_SAMPLE_FLOAT,NULL,0);
			if (floatable) { // woohoo!
				BASS_StreamFree(floatable);
				floatable=BASS_SAMPLE_FLOAT;
			}

	g_width=(float)pBackBufferSurfaceDesc->Width;
	g_height=(float)pBackBufferSurfaceDesc->Height;
	g_height-=g_sum;
    g_temp=(float)(pBackBufferSurfaceDesc->Width/2);
	
    // Initialize the font
    V_RETURN( D3DXCreateFont( pd3dDevice, 12, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, 
                         OUT_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
                         L"HY엽서M", &g_pFont ) );
	
	V_RETURN( D3DXCreateFont( pd3dDevice, 12, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, 
                         OUT_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
                         L"HY엽서M", &g_pFont2 ) );

 
  
    // Setup the camera's view parameters
    D3DXVECTOR3 vecEye(0.0f, 1.5f, -7.0f);
    D3DXVECTOR3 vecAt (0.0f, 0.2f, 0.0f);
    D3DXVECTOR3 vecUp (0.0f, 1.0f, 0.0f );
    g_Camera.SetViewParams( &vecEye, &vecAt );
    D3DXMatrixLookAtLH( &g_mView, &vecEye, &vecAt, &vecUp );
	D3DXMatrixIdentity( &g_mWorld );
	
    return S_OK;
}




//--------------------------------------------------------------------------------------
// This callback function will be called immediately after the Direct3D device has been 
// reset, which will happen after a lost device scenario. This is the best location to 
// create D3DPOOL_DEFAULT resources since these resources need to be reloaded whenever 
// the device is lost. Resources created here should be released in the OnLostDevice 
// callback. 
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnResetDevice( IDirect3DDevice9* pd3dDevice, 
                                const D3DSURFACE_DESC* pBackBufferSurfaceDesc )
{
    HRESULT hr;
	g_pd3dDevice=pd3dDevice;
	g_width=pBackBufferSurfaceDesc->Width;
	g_height=pBackBufferSurfaceDesc->Height;
	g_height=g_height-g_sum;
	D3DXMATRIX  g_matView;

	D3DXVECTOR3 vEye = D3DXVECTOR3( 0.0f, -0.0f, -1.0f );
	D3DXVECTOR3 vAt  = D3DXVECTOR3( 0.0f, -0.0f, 0.0f ); 
	D3DXVECTOR3 vUp  = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );  
	D3DXMatrixLookAtLH( &g_matView, &vEye, &vAt, &vUp );  
	pd3dDevice->SetTransform( D3DTS_VIEW, &g_matView );
    
	pd3dDevice->SetTransform( D3DTS_WORLD, &g_mWorld );
	pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );  
	pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	g_d3dHangul.RestoreDeviceObjects();		
    
	//g_d3dHangul.RestoreDeviceObjects(L"HanFont\\IYAGI.FNT",L"HanFont\\ITALIC.EFT");		

    if( g_pFont )
	{	
        V_RETURN( g_pFont->OnResetDevice() );
	}
	if( g_pFont2 )
	{		
		V_RETURN( g_pFont2->OnResetDevice() );
    }
	int i;	
	for(i=0;i<MAX_IMAGE;i++)		
		{
		SAFE_RELEASE(g_pTexture_Fade[i]);		
		SAFE_RELEASE(g_pVB_Fade[i]);	
		}
	//if(g_Music_Start<=1)	
	//{		
	//	for(i=0;i<MAX_IMAGE;i++)		
	//	{	
	//		Create_3DSprite(pd3dDevice,g_pTexture_Fade[i],g_pVB_Fade[i],g_pstrScene[i],0,0,pBackBufferSurfaceDesc->Width,pBackBufferSurfaceDesc->Height,0);	
	//	}
	//}
	//else
	{
	  SearchFileList2(pd3dDevice,g_width,g_height);
	}
	
	SAFE_RELEASE(g_pTexture_Opening);				
	SAFE_RELEASE(g_pVB_Opening);
	Create_3DSprite(pd3dDevice,g_pTexture_Opening,g_pVB_Opening,g_pstrScene[MAX_IMAGE],0,0,g_width,g_height,0);
	SAFE_RELEASE(g_pVB_Loading);		
	SAFE_RELEASE(g_pTexture_Loading);
	Create_3DSprite(pd3dDevice,g_pTexture_Loading,g_pVB_Loading,L"1.bmp",0,350,640,130,0);



	Create_Book_VertexBuffer(pd3dDevice,-1.0f, 1.0f,g_width,g_height,0);
//     Setup the camera's projection parameters
    float fAspectRatio = pBackBufferSurfaceDesc->Width / (FLOAT)pBackBufferSurfaceDesc->Height;
    g_Camera.SetProjParams( D3DX_PI/4, fAspectRatio, 0.1f, 1000.0f );
    g_Camera.SetWindow( pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height );

    
	g_FadeToColor=D3DCOLOR_ARGB(0,0,0,0);
	g_FadeToColor2=D3DCOLOR_ARGB(255,0,0,0);
	g_time=0;

    g_MP3_UI.SetLocation( 0, 0 );
    g_MP3_UI.SetSize( pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height );
    g_MP3_UI.GetControl( IDC_SLIDER )->SetLocation( 15, 5 );
    g_MP3_UI.GetControl( IDC_LISTBOXM )->SetLocation( 0, 372 );
    g_MP3_UI.GetControl( IDC_LISTBOXM )->SetSize( g_width,g_height-222);
	g_MP3_UI.GetControl( IDC_RADIO1A )->SetLocation( g_width-g_width/3, 30);
    g_MP3_UI.GetControl( IDC_RADIO1B )->SetLocation( g_width-g_width/3, 45);
    g_MP3_UI.GetControl( IDC_RADIO1C )->SetLocation( g_width-g_width/3, 60);

	SAFE_RELEASE(g_pIB_Curtain);				
	SAFE_RELEASE(g_pVB_Curtain);
	
	ResetSimulation();
		
	SetPhysicsWorld();

	InitCurtainGeometry();

    return S_OK;
}


//--------------------------------------------------------------------------------------
// This callback function will be called once at the beginning of every frame. This is the
// best location for your application to handle updates to the scene, but is not 
// intended to contain actual rendering calls, which should instead be placed in the 
// OnFrameRender callback.  
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime )
{    
	
}


//--------------------------------------------------------------------------------------
// This callback function will be called at the end of every frame to perform all the 
// rendering calls for the scene, and it will also be called if the window needs to be 
// repainted. After this function has returned, the sample framework will call 
// IDirect3DDevice9::Present to display the contents of the next buffer in the swap chain
//--------------------------------------------------------------------------------------



void CALLBACK OnFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime )
{   

    HRESULT hr;
    D3DXMATRIXA16 mWorld;
    D3DXMATRIXA16 mView;
    D3DXMATRIXA16 mProj;
    D3DXMATRIXA16 mWorldViewProjection;    
	D3DXMATRIX  g_matView;

	D3DXVECTOR3 vEye = D3DXVECTOR3( 0.0f, -0.0f, -1.0f );
	D3DXVECTOR3 vAt  = D3DXVECTOR3( 0.0f, -0.0f, 0.0f ); 
	D3DXVECTOR3 vUp  = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );  
	D3DXMatrixLookAtLH( &g_matView, &vEye, &vAt, &vUp );  
	pd3dDevice->SetTransform( D3DTS_VIEW, &g_matView );


     // Clear the render target and the zbuffer 
    V( pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 45, 50, 170), 1.0f, 0) );

    // Render the scene
	if( SUCCEEDED( pd3dDevice->BeginScene() ) )
    {	
		if(g_start==0)
		{
			bool hr;  	
			D3DXMATRIX mat;  		
			D3DXMatrixOrthoLH(&mat, (float)g_width, (float)g_height,     		
				0.0, 1000.0);  		
			pd3dDevice->SetTransform( D3DTS_PROJECTION, &mat );    		
			D3DXMATRIX matWorld,matTrans,matScale;  		
			D3DXMatrixScaling(&matScale, (float)g_width/2.0f, (float)g_height/2.0f, 1.0); 		
			D3DXMatrixMultiply(&matWorld, &matScale, D3DXMatrixTranslation(&matTrans, -0.5f, -0.5f, 0));  		
			pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );    		
			pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);		
			pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);		
			pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);		
			pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);     		
			pd3dDevice->SetTexture( 0, g_pTexture_Opening );		
			pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );    		
			pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );    		
			pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );    		
			pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );				
			pd3dDevice->SetStreamSource( 0, g_pVB_Opening,0, sizeof(CUSTOMVERTEX) );    		
			pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );		
			pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 );           		
			pd3dDevice->SetTexture( 0, g_pTexture_Opening );		
			pd3dDevice->SetStreamSource( 0, g_pVB_Opening,0, sizeof(CUSTOMVERTEX) );    		
			// set up texture stage states for blending the 2nd quad on top    		
			pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );		
			pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE );		
			// put the correct colors into the quad's verts      		
			hr=FadeQuadVerts2(g_FadeToColor2,fElapsedTime, 2.0f, g_pVB_Opening);          		
			pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );      		
			pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );      		
			pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );      		
			pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 );      		
			pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );  				
			V( pd3dDevice->EndScene() );    								
			g_time+=fElapsedTime;			
		}
		else if(g_Music_Start>=1 && g_wait==1 && gScreen_Mode==CURTAIN_SPRITE)
		{

			bool hr;  	
			D3DXMATRIX mat;  
			g_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, true );
			/*pd3dDevice->SetTexture( 0, g_pTexture_Loading);
			pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );   
			pd3dDevice->SetStreamSource( 0, g_pVB_Loading,0, sizeof(CUSTOMVERTEX) );  					
		
		
			pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 );	*/					
			FLOAT fAspect = ((FLOAT)g_width) / g_height;
			D3DXMatrixPerspectiveFovLH( &mat, D3DX_PI/4, fAspect, 0.1f, 1000.0f );
			pd3dDevice->SetTransform( D3DTS_PROJECTION, &mat ); 
			D3DXVECTOR3 vEye = D3DXVECTOR3( 200.0f, 0.0f, -500.f );	
			D3DXVECTOR3 vAt  = D3DXVECTOR3( 0.0f, 0.0f, 0.0f ); 	
			D3DXVECTOR3 vUp  = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );  	
			D3DXMatrixLookAtLH( &mat, &vEye, &vAt, &vUp );  	
			pd3dDevice->SetTransform( D3DTS_VIEW, &mat );
			D3DXMatrixIdentity( &mat );
			pd3dDevice->SetTransform( D3DTS_WORLD, &mat );
			Sleep(3);	
			UpdateLocalForce();		
			for ( int i = 16; i < MAXPARTICLE; i ++)			
				MoveParticle(&pa[i],fElapsedTime);
			//DetectCollision();
			InitCurtainGeometry();
			pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
			pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);		
			pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);		
			pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);     		
			pd3dDevice->SetTexture( 0, g_pTexture_Fade[1]);		
			pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );    		
			pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );    		
			pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );    		
			pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );				
			pd3dDevice->SetStreamSource( 0, g_pVB_Curtain,0, sizeof(CUSTOMVERTEX) );    		
			pd3dDevice->SetIndices(g_pIB_Curtain);		
			g_pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0,
				16*16, 0, 15*16*2-2 );		
			//pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 );					
					

			D3DXMatrixOrthoLH(&mat, (float)g_width, (float)g_height,     			
				0.0, 1000.0);  		
			pd3dDevice->SetTransform( D3DTS_PROJECTION, &mat );    		
			D3DXMATRIX matWorld,matTrans,matScale;  		
			vEye = D3DXVECTOR3( 0.0f, -0.0f, -1.0f );	
			vAt  = D3DXVECTOR3( 0.0f, -0.0f, 0.0f ); 	
			vUp  = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );  	
			D3DXMatrixLookAtLH( &g_matView, &vEye, &vAt, &vUp );  	
			pd3dDevice->SetTransform( D3DTS_VIEW, &g_matView );
			D3DXMatrixScaling(&matScale, (float)g_width/2.0f, (float)g_height/2.0f, 1.0); 		
			D3DXMatrixMultiply(&matWorld, &matScale, D3DXMatrixTranslation(&matTrans, -0.5f, -0.5f, 0));  		
			pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );    	
			if(g_Sound_Play==1&&g_bSpec_hide==false)		
			{				
				pd3dDevice->SetTexture( 0, g_pTexture_Fade[4] );
				UpdateSpectrum();									
				for(int x=0;x<BANDS;x++)												
				{											
					if(specbuf2[x]!=0)														
					{													
						//AAAA.Create(-1.0f+(2.0f/SPECWIDTH)*y,-1.0f+(0.5f/SPECHEIGHT)*x,2.0f/SPECWIDTH,(0.5f/SPECHEIGHT),specbuf[x*SPECWIDTH+y]);														
						AAAA.Create(-1.0f+((2.0f/(float)SPECWIDTH)*(float)(x*SPECWIDTH/BANDS)),-1.0f+(0.5f*(specbuf2[x]/float(0x7f)))+g_ratio,((2.0f-0.2f)/(float)BANDS),((0.5f-0.1f)/(float)BANDS),specbuf2[x],g_ratio);
						//memset(specbuf+y*SPECWIDTH+x*(SPECWIDTH/BANDS),y+1,0.9*(SPECWIDTH/BANDS)); // draw bar
						pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLELIST, 2, AAAA.m_vVertex, sizeof(SpriteVertex) );															
						if(specbuf2timeset[SPECWIDTH+x]==1.f)
						{
							if(specbuf2timeset[x]>0.f)
								specbuf2timeset[x]-=fElapsedTime;
							else
							{
 								specbuf2timeset[x]=0.003f;
								specbuf2[x]-=1.0f;
							}
						}
						else
						{
							if(specbuf2timeset[x]>0.f)
								specbuf2timeset[x]-=fElapsedTime;
							else
							{
								specbuf2timeset[SPECWIDTH+x]=1;
								specbuf2timeset[x]=0.003f;
								specbuf2[x]-=1.0f;
							}
						}
					}						
				}
				
				for(int y=0;y<SPECHEIGHT;y+=(SPECHEIGHT/BANDS))		
 					for(int x=0;x<BANDS;x++)								
					{
   						for(int j=0;j<BANDS;j++)							
						{   
 								if(0<specbuf[y*SPECWIDTH+(SPECWIDTH/BANDS)*x+j])															
								{									
									if(specbuf2[x]<y+1)													
									{   specbuf2[x]=y+1;
										specbuf2timeset[x]=0.5f;																						
 										specbuf2timeset[SPECWIDTH+(SPECWIDTH/BANDS)*x]=0.0f;
									}
  								}
						}
					}
			
					//					if(specbuf2[x]<y+1)
				//{
					//specbuf2[x]=y+1;
                  //  specbuf2timeset[x]=0.5f;
				//	specbuf2timeset[SPECWIDTH+x]=0.0f;
				//}

								
				int max_value=0,t=0;
 				for(int y=0;y<SPECHEIGHT;y+=(SPECHEIGHT/BANDS))		
					for(int x=0;x<SPECWIDTH;x+=(SPECWIDTH/BANDS))								
					{	
 						max_value=specbuf[y*SPECWIDTH+x];								
  						for(int j=0;j<BANDS-1;j++)							
						{   
 								if(0<specbuf[y*SPECWIDTH+x+j+1])															
								{
									max_value=y;									
 								}
									
									
 						}
						if(max_value!=0)									
						{								
							//AAAA.Create(-1.0f+(2.0f/SPECWIDTH)*y,-1.0f+(0.5f/SPECHEIGHT)*x,2.0f/SPECWIDTH,(0.5f/SPECHEIGHT),specbuf[x*SPECWIDTH+y]);														
  							AAAA.Create(-1.0f+((2.0f/(float)SPECWIDTH)*(float)x),-1.0f+((0.5f/(float)SPECHEIGHT)*y)+g_ratio,((2.0f-0.2f)/(float)BANDS),((0.5f-0.1f)/(float)BANDS),specbuf[y*SPECWIDTH+x],g_ratio);																					
 							//memset(specbuf+y*SPECWIDTH+x*(SPECWIDTH/BANDS),y+1,0.9*(SPECWIDTH/BANDS)); // draw bar
							pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLELIST, 2, AAAA.m_vVertex, sizeof(SpriteVertex) );															
						}						
					}		
					
														
					RenderText();						
					if(g_bgui_hide!=true)						
						V( g_MP3_UI.OnRender( fElapsedTime ) );										
					V( pd3dDevice->EndScene() );    
					
			}
		}		
		else if(g_Music_Start>=1 && g_wait==1 && gScreen_Mode==DEFALUT_SPRITE)
		{
			RenderText();			
			if(g_bgui_hide!=true)		
				V( g_MP3_UI.OnRender( fElapsedTime ) );				
			V( pd3dDevice->EndScene() );    		
		}
		else if(g_Music_Start>=1 && g_wait==1 && gScreen_Mode==IMAGE_PRESENT)
		{		
			bool hr;  		
			D3DXMATRIX mat;  		
			D3DXMatrixOrthoLH(&mat, (float)g_width, (float)g_height,     			
				0.0, 1000.0);  		
			pd3dDevice->SetTransform( D3DTS_PROJECTION, &mat );    		
			D3DXMATRIX matWorld,matTrans,matScale;  		
			D3DXMatrixScaling(&matScale, (float)g_width/2.0f, (float)g_height/2.0f, 1.0); 		
			D3DXMatrixMultiply(&matWorld, &matScale, D3DXMatrixTranslation(&matTrans, -0.5f, -0.5f, 0));  		
			pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );    		
			pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);		
			pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);		
			pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);		
			pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);     		
			pd3dDevice->SetTexture( 0, g_pTexture_Fade[g_Rand] );		   		
			pd3dDevice->SetStreamSource( 0, g_pVB_Fade[g_Rand],0, sizeof(CUSTOMVERTEX) );		
			pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );    		
			pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );    		
			pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );    		
			pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );												
			pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );		
			pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 );					
		        
			if(g_time>5.0f)		
			{		
				pd3dDevice->SetTexture( 0, g_pTexture_Fade[g_Rand2] );		
				pd3dDevice->SetStreamSource( 0, g_pVB_Fade[g_Rand2],0, sizeof(CUSTOMVERTEX) );    		
				if(g_current_mode==ALPHA_BLENDING)		
				{
					// set up texture stage states for blending the 2nd quad on top    		
					//pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );		
					pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE );		
					// put the correct colors into the quad's verts      		
					hr=FadeQuadVerts(g_FadeToColor,fElapsedTime, 2.0f, g_pVB_Fade[g_Rand2],ALPHA_BLENDING);    		
					pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );      		
					pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );      		
					pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );      		
					pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 );      		
					pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );  		
				}		
				else if(g_current_mode==IMAGE_SPRITE)		
				{			
					hr=FadeQuadVerts(g_FadeToColor,fElapsedTime, 2.0f, g_pVB_Fade[g_Rand2],IMAGE_SPRITE);    			
					pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 );      		
				}		
				else if(g_current_mode==IMAGE_BOOK)		
				{	
					bool hr;  			
					D3DXMATRIX mat;  								
					D3DXMATRIX matWorld,matTrans,matScale;  					
					D3DXMatrixScaling(&matScale, (float)g_width/2.0f, (float)g_height/2.0f, 1.0); 					
					D3DXMatrixMultiply(&matWorld, &matScale, D3DXMatrixTranslation(&matTrans, -0.5f, -0.5f, 0));  										
					pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );			
					pd3dDevice->SetStreamSource( 0, g_pVB_Book3,0, sizeof(CUSTOMVERTEX) );    												
					pd3dDevice->SetTexture( 0, g_pTexture_Fade[g_Rand2] );												
					pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 );								
					if(g_rotate_angle<(D3DX_PI/2))			
					{				
						pd3dDevice->SetStreamSource( 0, g_pVB_Book,0, sizeof(CUSTOMVERTEX) );    												
						hr=BookQuadVerts(pd3dDevice,fElapsedTime, 2.0f, g_pVB_Book,0,&matWorld);    							
						pd3dDevice->SetTexture( 0, g_pTexture_Fade[g_Rand] );																
						pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 );			
					}			
					else			
					{
				
						pd3dDevice->SetStreamSource( 0, g_pVB_Book2,0, sizeof(CUSTOMVERTEX) );    												
						hr=BookQuadVerts(pd3dDevice,fElapsedTime, 2.0f, g_pVB_Book2,1,&matWorld);    							
						pd3dDevice->SetTexture( 0, g_pTexture_Fade[g_Rand2] );											
							
					}				
					pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );																
				}	
			
			}
			pd3dDevice->SetFVF( D3DFVF_SPRITEVERTEX );		
			pd3dDevice->SetTexture( 0, g_pTexture_Fade[1] );
			pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
			pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
		
		
		
			if(g_Sound_Play==1&&g_bSpec_hide==false)		
			{				
				pd3dDevice->SetTexture( 0, g_pTexture_Fade[4] );
				UpdateSpectrum();									
				for(int x=0;x<BANDS;x++)												
				{											
					if(specbuf2[x]!=0)														
					{													
						//AAAA.Create(-1.0f+(2.0f/SPECWIDTH)*y,-1.0f+(0.5f/SPECHEIGHT)*x,2.0f/SPECWIDTH,(0.5f/SPECHEIGHT),specbuf[x*SPECWIDTH+y]);														
 						AAAA.Create(-1.0f+((2.0f/(float)SPECWIDTH)*(float)(x*SPECWIDTH/BANDS)),-1.0f+(0.5f*(specbuf2[x]/float(0x7f)))+g_ratio,((2.0f-0.2f)/(float)BANDS),((0.5f-0.1f)/(float)BANDS),specbuf2[x],g_ratio);
						//memset(specbuf+y*SPECWIDTH+x*(SPECWIDTH/BANDS),y+1,0.9*(SPECWIDTH/BANDS)); // draw bar
						pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLELIST, 2, AAAA.m_vVertex, sizeof(SpriteVertex) );															
						if(specbuf2timeset[SPECWIDTH+x]==1.f)
						{
							if(specbuf2timeset[x]>0.f)
								specbuf2timeset[x]-=fElapsedTime;
							else
							{
 								specbuf2timeset[x]=0.003f;
								specbuf2[x]-=1.0f;
							}
						}
						else
						{
							if(specbuf2timeset[x]>0.f)
								specbuf2timeset[x]-=fElapsedTime;
							else
							{
								specbuf2timeset[SPECWIDTH+x]=1;
								specbuf2timeset[x]=0.003f;
								specbuf2[x]-=1.0f;
							}
						}
					}						
				}
				
				for(int y=0;y<SPECHEIGHT;y+=(SPECHEIGHT/BANDS))		
 					for(int x=0;x<BANDS;x++)								
					{
   						for(int j=0;j<BANDS;j++)							
						{   
 								if(0<specbuf[y*SPECWIDTH+(SPECWIDTH/BANDS)*x+j])															
								{									
									if(specbuf2[x]<y+1)													
									{   specbuf2[x]=y+1;
										specbuf2timeset[x]=0.5f;																						
 										specbuf2timeset[SPECWIDTH+(SPECWIDTH/BANDS)*x]=0.0f;
									}
  								}
						}
					}
			
					//					if(specbuf2[x]<y+1)
				//{
					//specbuf2[x]=y+1;
                  //  specbuf2timeset[x]=0.5f;
				//	specbuf2timeset[SPECWIDTH+x]=0.0f;
				//}

								
				int max_value=0,t=0;
 				for(int y=0;y<SPECHEIGHT;y+=(SPECHEIGHT/BANDS))		
					for(int x=0;x<SPECWIDTH;x+=(SPECWIDTH/BANDS))								
					{	
 						max_value=specbuf[y*SPECWIDTH+x];								
  						for(int j=0;j<BANDS-1;j++)							
						{   
 								if(0<specbuf[y*SPECWIDTH+x+j+1])															
								{
									max_value=y;									
 								}
									
									
 						}
						if(max_value!=0)									
						{								
							//AAAA.Create(-1.0f+(2.0f/SPECWIDTH)*y,-1.0f+(0.5f/SPECHEIGHT)*x,2.0f/SPECWIDTH,(0.5f/SPECHEIGHT),specbuf[x*SPECWIDTH+y]);														
 							AAAA.Create(-1.0f+((2.0f/(float)SPECWIDTH)*(float)x),-1.0f+((0.5f/(float)SPECHEIGHT)*y)+g_ratio,((2.0f-0.2f)/(float)BANDS),((0.5f-0.1f)/(float)BANDS),specbuf[y*SPECWIDTH+x],g_ratio);
 							//memset(specbuf+y*SPECWIDTH+x*(SPECWIDTH/BANDS),y+1,0.9*(SPECWIDTH/BANDS)); // draw bar
							pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLELIST, 2, AAAA.m_vVertex, sizeof(SpriteVertex) );															
						}						
					}		
					
					
					
					
			}
		
			WCHAR wFileName[MAX_PATH];               				
			int  k;		
			char ggbuf[200];			
			WideCharToMultiByte(CP_ACP, NULL,
				g_wFileName3, -1,
				ggbuf, sizeof(ggbuf),			
				NULL, NULL);		
			k=strlen(ggbuf);                   		
			ggbuf[k]=0;		
			MultiByteToWideChar( CP_ACP, 0, ggbuf, -1,                          			
				wFileName, MAX_PATH );		
			RenderText();			
			g_time+=fElapsedTime;			
			if(g_bgui_hide!=true)					
				V( g_MP3_UI.OnRender( fElapsedTime ) );									
			V( pd3dDevice->EndScene() );    									
		}


		else
		{
		
			bool hr;  
		
			D3DXMATRIX mat;  
		
			D3DXMatrixOrthoLH(&mat, (float)g_width, (float)g_height,     
			
				0.0, 1000.0);  
		
			pd3dDevice->SetTransform( D3DTS_PROJECTION, &mat );    
		
			D3DXMATRIX matWorld,matTrans,matScale;  
		
			D3DXMatrixScaling(&matScale, (float)g_width/2.0f, (float)g_height/2.0f, 1.0); 
		


			D3DXMatrixMultiply(&matWorld, &matScale, D3DXMatrixTranslation(&matTrans, -0.5f, -0.5f, 0));  
		
			pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );    
		
			pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		
			pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		
			pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
		
			pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);     
		
			pd3dDevice->SetTexture( 0, g_pTexture_Opening );
		
			pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );    
		
			pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );    
		
			pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );    
		
			pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );		
		
			pd3dDevice->SetStreamSource( 0, g_pVB_Opening,0, sizeof(CUSTOMVERTEX) );    
		
			pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
		
			pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 );				
			if(g_bgui_hide!=true)
				V( g_MP3_UI.OnRender( fElapsedTime ) );			
			V( pd3dDevice->EndScene() );    										
		}
	}	
}






//--------------------------------------------------------------------------------------
// Render the help and statistics text. This function uses the ID3DXFont interface for 
// efficient text rendering.
//--------------------------------------------------------------------------------------
void RenderText()
{
    
	
    CDXUTTextHelper txtHelper( g_pFont, g_pTextSprite, 16 );
 	CDXUTTextHelper txtHelper2( g_pFont2, g_pTextSprite2, 16 );
	RECT rc;
 	rc.right=g_width;
   	rc.bottom=g_height;
	rc.left=0;				
	//txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 0.0f, 0.0f, 1.0f ) );        		
	txtHelper.SetForegroundColor( D3DXCOLOR( ((float)bkRed)/0xff,((float)bkGrn)/0xff,((float)bkBlu)/0xff, 1.0f ) );        		
	txtHelper2.SetForegroundColor( D3DXCOLOR( ((float)bkRed)/0xff,((float)bkGrn)/0xff,((float)bkBlu)/0xff, 1.0f ) );        		

 	if(g_bSMI_exist==true)
	{
		if(g_text_mode==1||g_text_mode2==0)
		{
			
			rc.top=g_height-(g_height/10);
			txtHelper.Begin();	    		
			txtHelper.SetInsertionPos((g_width/2)-(((float)(g_strlen))*2.9f), 0);    					
				
			txtHelper.DrawTextLine(rc,1,g_wFileName);        		
 			txtHelper.End();
		}
		else if(g_text_mode==2||g_text_mode2==1)
		{
			rc.top=g_height-((g_height/15)*2.3);			
			txtHelper.Begin();	    		
			txtHelper.SetInsertionPos((g_width/2)-(((float)(g_strlen))*2.9f), g_height-(g_height/15)*2.3-100);    					
			txtHelper.DrawTextLine(rc,1,g_wFileName);  
			txtHelper.End();

			txtHelper.Begin();	    		
			rc.top=g_height-((g_height/15)*1.5);  
 			txtHelper.SetInsertionPos((g_width/2)-(((float)(g_strlen2))*2.9f), g_height-(g_height/15)*1.5-100);    					
			txtHelper.DrawTextLine(rc,1,g_wFileName3);  
			txtHelper.End();
		}
		else if(g_text_mode==3||g_text_mode2==2)
		{
			rc.top=g_height-((g_height/15)*3.1);		
			txtHelper.Begin();	    		
			txtHelper.SetInsertionPos((g_width/2)-(((float)(g_strlen))*2.9f), g_height-(g_height/15)*3.1-100);    					
			txtHelper.DrawTextLine(rc,1,g_wFileName);  
			txtHelper.End();

			txtHelper.Begin();	    		
			rc.top=g_height-((g_height/15)*2.3);
			txtHelper.SetInsertionPos((g_width/2)-(((float)(g_strlen2))*2.9f), g_height-(g_height/15)*2.3-100);    					
			txtHelper.DrawTextLine(rc,1,g_wFileName3); 			
			txtHelper.End();

			txtHelper.Begin();	    		
			rc.top=g_height-((g_height/15)*1.5);    
			txtHelper.SetInsertionPos((g_width/2)-(((float)(g_strlen3))*2.9f), g_height-(g_height/15)*1.5-100);    					
			txtHelper.DrawTextLine(rc,1,g_wFileName4); 
			txtHelper.End();
		}
	}
	else
	{
		txtHelper.Begin();	    
		char str[MAX_PATH];
		WideCharToMultiByte(CP_ACP, 0, 
					g_pstrNO_SMI[0],
					-1,
					str,
					MAX_PATH,
					NULL, NULL);	
		txtHelper.SetInsertionPos((g_width/2)-(((float)strlen(str))*2.9f), g_height-(g_height/10));    		
		rc.right=g_width;	

		rc.bottom=g_height;	
		rc.left=0;			
		rc.top=g_height-(g_height/10);
		txtHelper.DrawTextLine(rc,1,g_pstrNO_SMI[0]);        
		txtHelper.End();
	}
	QWORD len=BASS_ChannelGetPosition(chan);									
	DWORD time=BASS_ChannelBytes2Seconds(chan,len);		
	char text[10];	
	sprintf(text,"%u:%02u",time/60,time%60);
	WCHAR wFileName[MAX_PATH];
	txtHelper.Begin();	    
	
	    rc.left=0;				
		rc.right=g_width;	
		rc.bottom=g_height;	
		rc.left=0;			
		rc.top=g_height-(g_height/10);
		MultiByteToWideChar(CP_ACP, 0,text, -1, wFileName, MAX_PATH);
		txtHelper.SetInsertionPos((g_width/2)-(((float)(g_strlen3))*2.9f), g_height-(g_height/15)*1.5-100);    					
		txtHelper.DrawTextLine(wFileName);
		txtHelper.End();
		
}



//--------------------------------------------------------------------------------------
// Before handling window messages, the sample framework passes incoming windows 
// messages to the application through this callback function. If the application sets 
// *pbNoFurtherProcessing to TRUE, then the sample framework will not process this message.
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing )
{

	g_Hwnd=hWnd;
	float x = 0,y = 0;
	/*if(uMsg==WM_LBUTTONDOWN)
	{		   
		if(x>=14&& x<=344 &&(y>=5&&y<=29))			
		{
			b_sliderbar_check = true;
		}
		       
		g_click=TRUE;
	}*/
	
	if(uMsg==WM_LBUTTONUP)		   
	{	
		g_click=FALSE;				
	
			x=LOWORD(lParam);									
			y=HIWORD(lParam);
			

		
			if(x>=14&& x<=344 &&(y>=5&&y<=29))			
			{
				b_sliderbar_check = true;
				float z= x-14;
				if(z!=0)
				{
					z = (z / 329)*100;
					ReadMediaPosition(z,0);

				}
				else ReadMediaPosition(0,0);
			}
			
		
	}
		
    if(uMsg==WM_GRAPHNOTIFY)            
		 HandleGraphEvent();        			
	if(uMsg==WM_GRAPHNOTIFY2)            	
	{
		HandleGraphEvent2();        					
	}
	if(uMsg==WM_Pos_Change)            	
	{

		/*WCHAR wszOutput[1024];
		StringCchPrintf( wszOutput, 1024, L"You adjusted the slider control.\nThe new value reported is %d",
                        ((CDXUTSlider*)pControl)->GetValue() );		;*/		
		if(lParam == 0 || (int)lParam == 100)
		ReadMediaPosition((float)lParam,-1);
			/*
		LONGLONG llPos = 0;        			
		hr = g_pMediaSeeking->SetPositions(&llPos, AM_SEEKING_AbsolutePositioning,                               							
			&llPos, AM_SEEKING_NoPositioning);       			
		if(SUCCEEDED(hr))			
		{        
				hr = g_pMediaControl->Run();    			
		}			
		llPos = 0;        		
		hr = g_pMediaSeeking2->SetPositions(&llPos, AM_SEEKING_AbsolutePositioning,                               				
			&llPos, AM_SEEKING_NoPositioning);       			
		if(SUCCEEDED(hr))			
		{        				
			hr = g_pMediaControl2->Run();    			
		}*/
	}
	if(uMsg==WM_SIZE)            
		 return 0;        	
    // Give the dialogs a chance to handle the message first
    *pbNoFurtherProcessing = g_HUD.MsgProc( hWnd, uMsg, wParam, lParam );
    if( *pbNoFurtherProcessing )
        return 0;

    *pbNoFurtherProcessing = g_MP3_UI.MsgProc( hWnd, uMsg, wParam, lParam );
    if( *pbNoFurtherProcessing )
        return 0;
	if(uMsg==WM_TIMER&&g_click==false&&b_sliderbar_check==false)
	{
		
		if (gMedia_Len != 0)    
		{
			float a = g_MP3_UI.GetSlider( IDC_SLIDER )->GetValue();
			if(g_MP3_UI.GetSlider( IDC_SLIDER )->GetValue()<99)		
			{			
				gMedia_Cur=BASS_ChannelGetPosition(chan);						
				QWORD lTick = ((gMedia_Cur*100) / gMedia_Len);							
				g_dwtime=g_MP3_UI.GetSlider( IDC_SLIDER )->GetValue();								
				g_MP3_UI.GetSlider( IDC_SLIDER )->SetValue(lTick);
				
			}
			else if(g_bLoop==true)
			{

				if (g_pMediaControl2 != NULL)		
				{				
					// Run the graph to play the media file				
					g_pMediaControl2->Stop() ;			
					LONGLONG llPos = 0;        
					g_pMediaSeeking2->SetPositions(&llPos, AM_SEEKING_AbsolutePositioning,
                                           &llPos, AM_SEEKING_NoPositioning);
					g_pMediaControl2->Run() ;				
				
					g_MP3_UI.GetSlider( IDC_SLIDER )->SetValue(0);				
					NEXT_PLAY_MP3(NULL,0);
				}
			}
			else
			{
				g_MP3_UI.GetSlider( IDC_SLIDER )->SetValue(0);
				
				NEXT_PLAY_MP3(NULL,0);
			}
	
		}
	}
    // Pass all remaining windows messages to camera so it can respond to user input
    g_Camera.HandleMessages( hWnd, uMsg, wParam, lParam );

    return 0;
}


//--------------------------------------------------------------------------------------
// As a convenience, the sample framework inspects the incoming windows messages for
// keystroke messages and decodes the message parameters to pass relevant keyboard
// messages to the application.  The framework does not remove the underlying keystroke 
// messages, which are still passed to the application's MsgProc callback.
//--------------------------------------------------------------------------------------
void CALLBACK KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown )
{
	 
	if(nChar=='H'&&bKeyDown==false)
	{		
		if(g_bgui_hide==true)
		g_bgui_hide=false;
		else g_bgui_hide=true;
	}
	else if(nChar=='J'&&bKeyDown==false)
	{		
		if(g_bSpec_hide==true)
		g_bSpec_hide=false;
		else g_bSpec_hide=true;
	}
	else if(nChar=='G'&&bKeyDown==false)
	{		
		specmode++;
		if(specmode==3)
			specmode=0;
	}	
	else if(nChar==VK_SPACE&&bKeyDown==false)
	{		
		bkRed = (BYTE)(rand()%0xFF);                
		bkGrn = (BYTE)(rand()%0xFF);                
		bkBlu = (BYTE)(rand()%0xFF);;
	}
		
	else if(nChar=='X')
	{		
		g_dVol++;
		if(g_dVol>100)
			g_dVol=99;
		BASS_SetVolume(g_dVol);    
	}
	else if(nChar=='Z')
	{		
		g_dVol--;
		if(g_dVol==0)
			g_dVol=1;
		BASS_SetVolume(g_dVol);    
	}
	
                    

}


//--------------------------------------------------------------------------------------
// Handles the GUI events
//--------------------------------------------------------------------------------------
void CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl )
{
    WCHAR wszOutput[1024];

    if(g_bgui_hide==true)
		return;
    switch( nControlID )
    {
        case IDC_CONFIG:DXUTSetShowSettingsDialog( !DXUTGetShowSettingsDialog() ); break;
        case IDC_EXIT:        PostQuitMessage(0); break;
	    case IDC_TOGGLEFULLSCREEN: DXUTToggleFullScreen(); break;
        case IDC_TOGGLEREF:        DXUTToggleREF(); break;
        case IDC_CHANGEDEVICE:     DXUTSetShowSettingsDialog( !DXUTGetShowSettingsDialog() ); break;
        case IDC_START:    
            break;
        case IDC_LOAD:
			;
            //CDXUTIMEEditBox::EnableImeSystem( false );
            //g_SampleUI.GetStatic( IDC_OUTPUT )->SetText( L"You clicked the 'Disable IME' button.\nIME text input is disabled for IME-capable edit controls." );
            break;
        case IDC_EDITBOX1:
        case IDC_EDITBOX2:
            switch( nEvent )
            {
               
				case EVENT_EDITBOX_STRING:
                {
                    StringCchPrintf( wszOutput, 1024, L"You have pressed Enter in edit control (ID %u).\nThe content of the edit control is:\n\"%s\"",
                                nControlID, ((CDXUTEditBox*)pControl)->GetText() );
                    g_MP3_UI.GetStatic( IDC_OUTPUT )->SetText( wszOutput );

                    // Clear the text if needed
                    if( g_MP3_UI.GetCheckBox( IDC_CHECKLOOP )->GetChecked() )
                        ((CDXUTEditBox*)pControl)->SetText( L"" );
                    break;
                }

                case EVENT_EDITBOX_CHANGE:
                {
                    StringCchPrintf( wszOutput, 1024, L"You have changed the content of an edit control (ID %u).\nIt is now:\n\"%s\"",
                                nControlID, ((CDXUTEditBox*)pControl)->GetText() );
                    g_MP3_UI.GetStatic( IDC_OUTPUT )->SetText( wszOutput );

                    break;
                }
            }
            break;
        case IDC_SLIDER:

			if(g_Sound_Play==0)
				((CDXUTSlider*)pControl)->SetValue(0);
			else
			{		

				g_dwtime=((CDXUTSlider*)pControl)->GetValue();		
				g_MP3_UI.GetSlider( IDC_SLIDER )->SetValue((LPARAM)((CDXUTSlider*)pControl)->GetValue());					    
				b_sliderbar_check = true;
				long a = (LPARAM)((CDXUTSlider*)pControl)->GetValue();
				if( g_click == true)				
					SendMessage(g_Hwnd,WM_Pos_Change,0,(LPARAM)((CDXUTSlider*)pControl)->GetValue());				            
			}
            break;
        case IDC_CHECKLOOP:                                        
			if(((CDXUTCheckBox*)pControl)->GetChecked()==true)
			{             
				g_bLoop=true;
			}
			else g_bLoop=false;
            break;
        case IDC_CHECKIMAGE:                        
			if(((CDXUTCheckBox*)pControl)->GetChecked()==true)
			{             
				g_bimage_change=true;
			} else g_bimage_change=false;            
            break;
        case IDC_FLANGER:                        
			if(((CDXUTCheckBox*)pControl)->GetChecked()==true)
			{             
				memset(flabuf,0,sizeof(flabuf));						
				flapos=0;				
				flas=FLABUFLEN/2;
				flasinc=0.002f;
				fladsp=BASS_ChannelSetDSP(chan,&Flange,0,0);
			}
			else 
				BASS_ChannelRemoveDSP(chan,fladsp);
            break;
			
		case IDC_ROTATE:                        
			if(((CDXUTCheckBox*)pControl)->GetChecked()==true)
			{             
				rotpos=0.7853981f;						
				rotdsp=BASS_ChannelSetDSP(chan,&Rotate,0,2);				
			} else
				BASS_ChannelRemoveDSP(chan,rotdsp);
			break;        
			
		case IDC_ECHO:                        
			if(((CDXUTCheckBox*)pControl)->GetChecked()==true)
			{             
				memset(echbuf,0,sizeof(echbuf));						
				echpos=0;						
				echdsp=BASS_ChannelSetDSP(chan,&Echo,0,1);					
			} else						
				BASS_ChannelRemoveDSP(chan,echdsp);					
			break;

												
        case IDC_COMBOBOX:
        {
            DXUTComboBoxItem *pItem = ((CDXUTComboBox*)pControl)->GetSelectedItem();
            if( pItem )
            {
                StringCchPrintf( wszOutput, 1024, L"You selected a new item in the combobox.\nThe new item is \"%s\" and the associated data value is 0x%p",
                            pItem->strText, pItem->pData );
               // g_MP3_UI.GetStatic( IDC_OUTPUT )->SetText( wszOutput );
            }
            break;
        }
		case IDC_CURTAIN:        
			if(((CDXUTCheckBox*)pControl)->GetChecked()==true)
			{             
				g_bCurtain=true;
			} 
			else g_bCurtain=false;                        
			break;				
        case IDC_RADIO1A:
			g_text_mode2=0;
			break;
        case IDC_RADIO1B:
			g_text_mode2=1;
			break;
        case IDC_RADIO1C:            
			g_text_mode2=2;
            break;
        case IDC_RADIO2A:
        case IDC_RADIO2B:
        case IDC_RADIO2C:
            StringCchPrintf( wszOutput, 1024, L"You selected a new radio button in the LOWER radio group.\nThe new button is \"%s\"",
                ((CDXUTRadioButton*)pControl)->GetText() );
            g_MP3_UI.GetStatic( IDC_OUTPUT )->SetText( wszOutput );
            break;

        case IDC_LISTBOX:
            switch( nEvent )
            {
                case EVENT_LISTBOX_ITEM_DBLCLK:
                {
                    DXUTListBoxItem *pItem = ((CDXUTListBox *)pControl)->GetItem( ((CDXUTListBox *)pControl)->GetSelectedIndex( -1 ) );

                    StringCchPrintf( wszOutput, 1024, L"You double clicked an item in the left list box.  The item is\n\"%s\"",
                        pItem ? pItem->strText : L"" );
                    g_MP3_UI.GetStatic( IDC_OUTPUT )->SetText( wszOutput );
                    break;
                }

                case EVENT_LISTBOX_SELECTION:
                {
                    StringCchPrintf( wszOutput, 1024, L"You changed the selection in the left list box.  The selected item is %d", ((CDXUTListBox *)pControl)->GetSelectedIndex() );
                    g_MP3_UI.GetStatic( IDC_OUTPUT )->SetText( wszOutput );
                    break;
                }
            }
            break;

        case IDC_LISTBOXM:
            switch( nEvent )
            {
                case EVENT_LISTBOX_ITEM_DBLCLK:
                {
					DXUTListBoxItem *pItem = ((CDXUTListBox *)pControl)->GetItem( ((CDXUTListBox *)pControl)->GetSelectedIndex( -1 ) );                    					
					NEXT_PLAY_MP3(pItem,1);
                    break;
                }

                case EVENT_LISTBOX_SELECTION:
                {
                    /*StringCchPrintf( wszOutput, 1024, L"You changed the selection in the right list box.  The selected item(s) are\n" );
                    int nSelected = -1;
                    while( ( nSelected = ((CDXUTListBox *)pControl)->GetSelectedIndex( nSelected ) ) != -1 )
                    {
                        StringCchPrintf( wszOutput + lstrlenW( wszOutput ), 1024 - lstrlenW( wszOutput ), L"%d,", nSelected );
                    }
                    // Remove the trailing comma if one exists.
                    if( wszOutput[lstrlenW(wszOutput)-1] == L',' )
                        wszOutput[lstrlenW(wszOutput)-1] = L'\0';
                    g_MP3_UI.GetStatic( IDC_OUTPUT )->SetText( wszOutput );*/
                    break;
                }
            }
            break;
    }
}


//--------------------------------------------------------------------------------------
// This callback function will be called immediately after the Direct3D device has 
// entered a lost state and before IDirect3DDevice9::Reset is called. Resources created
// in the OnResetDevice callback should be released here, which generally includes all 
// D3DPOOL_DEFAULT resources. See the "Lost Devices" section of the documentation for 
// information about lost devices. /
//--------------------------------------------------------------------------------------
void CALLBACK OnLostDevice() //여기도 코드를 넣어야 하지만... 귀찮아서
{

    if( g_pFont )
        g_pFont->OnLostDevice();
	if( g_pFont2 )
        g_pFont->OnLostDevice();
    
    SAFE_RELEASE(g_pTextSprite);
	SAFE_RELEASE(g_pTextSprite2);
}


//--------------------------------------------------------------------------------------
// This callback function will be called immediately after the Direct3D device has 
// been destroyed, which generally happens as a result of application termination or 
// windowed/full screen toggles. Resources created in the OnCreateDevice callback 
// should be released here, which generally includes all D3DPOOL_MANAGED resources. 
//--------------------------------------------------------------------------------------
void CALLBACK OnDestroyDevice()
{
    if(g_pMediaControl)
		g_pMediaControl->Stop();
	if(g_pMediaControl2)
		g_pMediaControl2->Stop();

	SAFE_RELEASE(g_pME);
	SAFE_RELEASE(g_pMediaSeeking);
	SAFE_RELEASE(g_pMediaControl);
	SAFE_RELEASE(g_pGraphBuilder);  

	SAFE_RELEASE(g_pME2);
	SAFE_RELEASE(g_pMediaSeeking2);
	SAFE_RELEASE(g_pMediaControl2);
	SAFE_RELEASE(g_pGraphBuilder2);  
			 
			 
	
    SAFE_RELEASE(g_pFont);
	SAFE_RELEASE(g_pFont2);
	g_d3dHangul.SafeRelease();

	for(int i=0;i<MAX_IMAGE;i++)
	{	
		SAFE_RELEASE(g_pTexture_Fade[i]);	
		SAFE_RELEASE(g_pVB_Fade[i]);
	}
	
	SAFE_RELEASE(g_pTexture_Opening);				
	SAFE_RELEASE(g_pVB_Opening);
	SAFE_RELEASE(g_pVB_Book);				
	SAFE_RELEASE(g_pVB_Book2);				
	SAFE_RELEASE(g_pVB_Book3);				
	SAFE_RELEASE(g_pVB_Book4);				
	SAFE_RELEASE(g_pIB_Curtain);				
	SAFE_RELEASE(g_pVB_Curtain);	


	SAFE_RELEASE(g_pVB_Loading);				
	SAFE_RELEASE(g_pTexture_Loading);
	
}


HRESULT GraphInit(void)
{
    HRESULT hr;
    // Initialize COM
    if(FAILED(hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED)))
        return hr;

    // Create DirectShow Graph
    if(FAILED(hr = CoCreateInstance(CLSID_FilterGraph, NULL,
        CLSCTX_INPROC, IID_IGraphBuilder,
        reinterpret_cast<void **>(&g_pGraphBuilder))))
        return hr;


    // Get the IMediaControl Interface
    if(FAILED(hr = g_pGraphBuilder->QueryInterface(IID_IMediaControl,
        reinterpret_cast<void **>(&g_pMediaControl))))
        return hr;

    // Get the IMediaControl Interface
    if(FAILED(hr = g_pGraphBuilder->QueryInterface(IID_IMediaSeeking,
        reinterpret_cast<void **>(&g_pMediaSeeking))))
        return hr;

	if(FAILED(hr = g_pGraphBuilder->QueryInterface(IID_IMediaEventEx,
        reinterpret_cast<void **>(&g_pME))))
        return hr;
	g_pME->SetNotifyWindow((OAHWND)g_Hwnd, WM_GRAPHNOTIFY, 0);

	if(FAILED(hr = CoCreateInstance(CLSID_FilterGraph, NULL,    
		CLSCTX_INPROC, IID_IGraphBuilder,
        reinterpret_cast<void **>(&g_pGraphBuilder2))))
        return hr;
// Get the IMediaControl Interface
    if(FAILED(hr = g_pGraphBuilder2->QueryInterface(IID_IMediaControl,
        reinterpret_cast<void **>(&g_pMediaControl2))))
        return hr;

    // Get the IMediaControl Interface
    if(FAILED(hr = g_pGraphBuilder2->QueryInterface(IID_IMediaSeeking,
        reinterpret_cast<void **>(&g_pMediaSeeking2))))
        return hr;

	if(FAILED(hr = g_pGraphBuilder2->QueryInterface(IID_IMediaEventEx,
        reinterpret_cast<void **>(&g_pME2))))
        return hr;
	

    // Create Source Filter for first file


    // Create the intial graph
    //if(FAILED(hr = SwapSourceFilter()))
      //  return hr;

    return S_OK;
}

int gtimer_start=0;
HRESULT SwapSourceFilter(LPCTSTR wstr)
{
    HRESULT hr = S_OK;
    char str[MAX_PATH];
    TCHAR szFilename[MAX_PATH];
    WCHAR wFileName[MAX_PATH];
	WCHAR wFileName2[MAX_PATH];
    StringCchPrintf( wFileName2, MAX_PATH, L"music/%s",
                                 wstr);
    DWORD dwAttr = GetFileAttributes(wFileName2);
	if(dwAttr!=-1)
	{

    (void)StringCchCopy(g_szCurrentFile, NUMELMS(g_szCurrentFile), wFileName2);    
	(void)StringCchCopy(szFilename, NUMELMS(szFilename),wFileName2);
    USES_CONVERSION;

    (void)StringCchCopyW(wFileName, MAX_PATH, T2W(szFilename));
	WideCharToMultiByte(CP_ACP, 0, 
					wFileName,
					-1,
					str,
					MAX_PATH,
					NULL, NULL);	

	
	}
	else 
	{
		WideCharToMultiByte(CP_ACP, 0, 
					g_wFileName2,
					-1,
					str,
					MAX_PATH,
					NULL, NULL);	
		
	}

	BASS_MusicFree(chan);
							
	BASS_StreamFree(chan);	
	BASS_CHANNELINFO info;
 	if(!(chan=BASS_StreamCreateFile(FALSE,str,0,0,floatable))											
 		&& !(chan=BASS_MusicLoad(FALSE,str,0,0,BASS_MUSIC_RAMPS|floatable|BASS_MUSIC_PRESCAN,0))) //BASS_MUSIC_LOOP|
	{								
			// whatever it is, it ain't playable											
			return 0;													
	}


									
	BASS_ChannelGetInfo(chan,&info);			
		
	if (info.chans!=2) { // only stereo is allowed																			
		BASS_MusicFree(chan);										
		BASS_StreamFree(chan);			
		return 0;
	}									
		
	
	g_Sound_Play=1;	
	Render_Caption(wFileName);
	// Start the graph   	

	//BASS_MusicPlay(chan);	
	BASS_ChannelPlay(chan,FALSE);
//	BASS_StreamPlay(chan,0,0);	//BASS_SAMPLE_LOOP																		
	gMedia_Len=BASS_ChannelGetLength(chan); // length in bytes

	//float time=BASS_ChannelBytes2Seconds(channel,len); // the time length
	//gMedia_Len=BASS_StreamGetLength(chan);    							

	//if(gMedia_Len==0xffffffff)
	//	gMedia_Len=BASS_MusicGetLength(chan,true); // get length


	//gMedia_Cur=BASS_StreamGetFilePosition(chan, 0);	//필요없는 부분 좀 있다 지우자
	g_dVol=BASS_GetVolume();

	BASS_SetVolume(g_dVol); 
	if (g_pMediaControl2 != NULL)	
	{		
		// Run the graph to play the media file		
		g_pMediaControl2->Run() ;				
	}
	
	if(gtimer_start==0)
	{
		SetTimer(g_Hwnd,WM_TIMER,1000,NULL);
		gtimer_start=1;
	}
    return S_OK;
}



HRESULT SwapSourceFilter2(int i) // dummy code.
{
    HRESULT hr = S_OK;
    IPin *pPin = NULL;
    int nNextFileIndex=0;
    TCHAR szFilename[MAX_PATH];
    WCHAR wFileName[MAX_PATH];
	if(gPlaying==0)
	{	
    DWORD dwAttr = GetFileAttributes(pstrFiles[i]);    
    (void)StringCchCopy(g_szCurrentFile, NUMELMS(g_szCurrentFile), pstrFiles[i]);    
	(void)StringCchCopy(szFilename, NUMELMS(szFilename),pstrFiles[i]);
    USES_CONVERSION;

    (void)StringCchCopyW(wFileName, MAX_PATH, T2W(szFilename));
    hr = g_pGraphBuilder2->AddSourceFilter(wFileName, wFileName, &g_pSourceNext2);
    if(SUCCEEDED(hr))
    {
        hr = g_pSourceNext2->FindPin(L"Output", &pPin);
    }

    // Stop the graph
    if(SUCCEEDED(hr))
    {
        hr = g_pMediaControl2->Stop();
    }
    if(SUCCEEDED(hr))
    {
        IEnumFilters *pFilterEnum = NULL;

        if(SUCCEEDED(hr = g_pGraphBuilder2->EnumFilters(&pFilterEnum)))
        {
            int iFiltCount = 0;
            int iPos = 0;

            // Need to know how many filters. If we add/remove filters during the
            // enumeration we'll invalidate the enumerator
            while(S_OK == pFilterEnum->Skip(1))
            {
                iFiltCount++;
            }

            // Allocate space, then pull out all of the
            IBaseFilter **ppFilters = reinterpret_cast<IBaseFilter **>
                                      (_alloca(sizeof(IBaseFilter *) * iFiltCount));
            pFilterEnum->Reset();

            while(S_OK == pFilterEnum->Next(1, &(ppFilters[iPos++]), NULL));

            SAFE_RELEASE(pFilterEnum);

            for(iPos = 0; iPos < iFiltCount; iPos++)
            {
                g_pGraphBuilder2->RemoveFilter(ppFilters[iPos]);

                // Put the filter back, unless it is the old source
                if(ppFilters[iPos] != g_pSourceCurrent2)
                {
                    g_pGraphBuilder2->AddFilter(ppFilters[iPos], NULL);
                }
                SAFE_RELEASE(ppFilters[iPos]);
            }
        }
    }

    // We have the new output pin. Render it
    if(SUCCEEDED(hr))
    {
        // Release the old source filter, if it exists
        SAFE_RELEASE(g_pSourceCurrent2)

        hr = g_pGraphBuilder2->Render(pPin);
        g_pSourceCurrent2 = g_pSourceNext2;
        g_pSourceNext2 = NULL;
    }

    SAFE_RELEASE(pPin);
    SAFE_RELEASE(g_pSourceNext2); // In case of errors

	
	}
    // Re-seek the graph to the beginning
    

    // Start the graph
     
 if(g_effectRunning==1)
 {LONGLONG llPos = 0;
        hr = g_pMediaSeeking2->SetPositions(&llPos, AM_SEEKING_AbsolutePositioning,
                                           &llPos, AM_SEEKING_NoPositioning);
    
		gPlaying=1;
        hr = g_pMediaControl2->Run();
		g_effectRunning=0;
    
 }
    return S_OK;
}


HRESULT HandleGraphEvent(void) //쓰이지 않음
{
    LONG evCode;
	LONG_PTR evParam1, evParam2;
    HRESULT hr=S_OK;

    // Make sure that we don't access the media event interface
    // after it has already been released.
    if (!g_pME)
        return S_OK;    
    // Process all queued events
    while(SUCCEEDED(g_pME->GetEvent(&evCode, &evParam1, &evParam2, 0)))
    {
        // Free memory associated with callback, since we're not using it
        hr = g_pME->FreeEventParams(evCode, evParam1, evParam2);

        // If this is the end of the clip, reset to beginning
        if(EC_COMPLETE == evCode)
        {                           
        
			if(g_bLoop)
			{
			LONGLONG llPos = 0;        
			hr = g_pMediaSeeking->SetPositions(&llPos, AM_SEEKING_AbsolutePositioning,                               
				&llPos, AM_SEEKING_NoPositioning);       
			if(SUCCEEDED(hr))
			{        
				hr = g_pMediaControl->Run();    
			}
			llPos = 0;        
			g_dwtime=0;
			hr = g_pMediaSeeking2->SetPositions(&llPos, AM_SEEKING_AbsolutePositioning,                               
				&llPos, AM_SEEKING_NoPositioning);       

			if(SUCCEEDED(hr))
			{        
				hr = g_pMediaControl2->Run();    
			}
			}
			else
			{
				NEXT_PLAY_MP3(NULL, 0);
			}
        }
    }
	
	if(g_pMediaSeeking2)	
		g_pMediaSeeking2->GetDuration(&g_rtTotalTime);
	DisplayFileDuration();
    return hr;
}

HRESULT HandleGraphEvent2(void)
{
    	
	if (g_pGraphBuilder2 == NULL)			
		return E_ABORT ;		
	
	HRESULT hr ;
	long lEvent, lParam1, lParam2;
	long lTimeOut = 0;
    
	if (!g_pME2)
		return E_FAIL;

	while (SUCCEEDED(g_pME2->GetEvent(&lEvent, &lParam1, &lParam2, lTimeOut)))
	{
		switch (lEvent)
		{
		case EC_USERABORT:
		//	OnStopClip();
			break;

		case EC_COMPLETE:
		//	OnStopClip();
		//	OnPlayClip();
			break;

		// TODO: Place code here.
		case EC_OLE_EVENT:
			{
				int nLength1 = SysStringLen((BSTR)lParam1) * 2 + 1;
				char* pszBuf1 = new char[nLength1];
				WideCharToMultiByte(CP_ACP, 0, 
					(BSTR)lParam1,
					-1,
					pszBuf1,
					nLength1,
					NULL, NULL);

				int nLength2 = SysStringLen((BSTR)lParam2) * 2 + 1;
				char* pszBuf2 = new char[nLength2];
				WideCharToMultiByte(CP_ACP, 0, 
					(BSTR)lParam2,
					-1,
					pszBuf2,
					nLength2,
					NULL, NULL);
				
				TRACE("%s: %s\n", pszBuf1, pszBuf2);

				if (strcmp(pszBuf1, "Text") == 0)
				{
					char *pdest = strrchr(pszBuf2, '>');
					
					if (pdest)
					{
						char seps[]   = "\n"; //seperator입니다
						char *token;
						char temp[MAX_PATH];
						token=strtok(pdest,seps); 						
						strcpy(temp,token);				
						g_strlen=strlen(temp);			
						g_strlen-=1;
						//strcpy(temp1,token);
						//g_strlen=strlen(pdest);		
						//왜 while문을 안썼는지를 이해한다면 아마 시스템 프로그래밍 과목을 들으셨을듯
						WCHAR wFileName[MAX_PATH];							
						MultiByteToWideChar(CP_ACP, 0, strcmp(temp+1, "&nbsp;") == 0 ? " " : temp+1, -1, wFileName, MAX_PATH);
							//MultiByteToWideChar(CP_ACP, 0, strcmp(pdest+1, "&nbsp;") == 0 ? " " : pdest+1, -1, wFileName, MAX_PATH);													
						lstrcpy(g_wFileName,wFileName);		
						g_text_mode=1;
						token=strtok(NULL,seps); 
						if(token!=NULL)							
						{								
																									
							strcpy(temp,token);
							g_strlen2=strlen(temp);							
							MultiByteToWideChar(CP_ACP, 0, strcmp(temp+1, "&nbsp;") == 0 ? " " : temp, -1, wFileName, MAX_PATH);						
							lstrcpy(g_wFileName3,wFileName);	
							g_text_mode=2;
							token=strtok(NULL,seps); 
							if(token!=NULL)
							{
																																	
								strcpy(temp,token);							
								g_strlen3=strlen(temp);														
								MultiByteToWideChar(CP_ACP, 0, strcmp(temp+1, "&nbsp;") == 0 ? " " : temp, -1, wFileName, MAX_PATH);						
								lstrcpy(g_wFileName4,wFileName);							
								g_text_mode=3;
							}
						}

					}

					
				}

				delete[] pszBuf1;
				delete[] pszBuf2;
			}
			break;
		}

		hr = g_pME2->FreeEventParams(lEvent, lParam1, lParam2);
	}
    return hr;
}


HRESULT Create_3DSprite( IDirect3DDevice9* pd3dDevice,LPDIRECT3DTEXTURE9 &Test_Texture,LPDIRECT3DVERTEXBUFFER9 &Test_Buffer,LPCTSTR lpstr,float x, float y, float width, float height,int alpha)
{
	TCHAR szFilename[MAX_PATH];
	WCHAR wFileName[MAX_PATH];
	HRESULT hr;
	if(Test_Buffer==NULL)
	{ 
		pd3dDevice->CreateVertexBuffer(
		6 * sizeof(CUSTOMVERTEX), 
		D3DUSAGE_WRITEONLY,
		D3DFVF_CUSTOMVERTEX,
		D3DPOOL_MANAGED,
		&Test_Buffer,
		0);
	}

	CUSTOMVERTEX* v;	
	if( FAILED( hr = Test_Buffer->Lock( 0, 0, (void **)&v, 0 ) ) )
		return hr;

    // first triangle
    v[0].position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
    v[0].color    = D3DCOLOR_ARGB(alpha,0,0,0);
    v[0].tu       = 0.0f;
    v[0].tv       = 0.0f;

    v[1].position = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
    v[1].color    = D3DCOLOR_ARGB(alpha,0,0,0);
    v[1].tu       = 1.0f;
    v[1].tv       = 0.0f;

    v[2].position = D3DXVECTOR3(1.0f, -1.0f+g_ratio, 0.0f);
    v[2].color    = D3DCOLOR_ARGB(alpha,0,0,0);
    v[2].tu       = 1.0f;
    v[2].tv       = 1.0f;

    // second triangle
    v[3].position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
    v[3].color    = D3DCOLOR_ARGB(alpha,0,0,0);
    v[3].tu       = 0.0f;
    v[3].tv       = 0.0f;

    v[4].position = D3DXVECTOR3(1.0f, -1.0f+g_ratio, 0.0f);
    v[4].color    = D3DCOLOR_ARGB(alpha,0,0,0);
    v[4].tu       = 1.0f;
    v[4].tv       = 1.0f;

    v[5].position = D3DXVECTOR3(-1.0f, -1.0f+g_ratio, 0.0f);
    v[5].color    = D3DCOLOR_ARGB(alpha,0,0,0);
    v[5].tu       = 0.0f;
    v[5].tv       = 1.0f;

    if( FAILED( hr = Test_Buffer->Unlock() ) )	
		return hr;


	(void)StringCchCopy(szFilename, NUMELMS(szFilename),lpstr);
	USES_CONVERSION;

    (void)StringCchCopyW(wFileName, MAX_PATH, T2W(szFilename));

	/*if (FAILED(hr = D3DXCreateTextureFromFileEx(m_pd3dDevice, wFileName, 
    0, 0, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
    0, NULL, NULL, &Test_Texture))) {
    return(hr);

	}*/

	D3DXCreateTextureFromFile(
		pd3dDevice,
	wFileName,
		&Test_Texture);  	

	return hr;
}

float g_ff[6]={0};
float g_ff2[6]={0};
int juhang=0;
bool BookQuadVerts( IDirect3DDevice9* pd3dDevice,float fElapsedTime, float fEffectTime, LPDIRECT3DVERTEXBUFFER9 pVB,int image_num,D3DXMATRIX *matWorld)
{
  
	HRESULT hr;

	
	g_rotate_angle+=(D3DX_PI * fElapsedTime / 2.0f);			
	if(g_rotate_angle>=D3DX_PI)
		g_rotate_angle=D3DX_PI;

	  
	CUSTOMVERTEX* v;	
	if( FAILED( hr = pVB->Lock( 0, 0, (void **)&v, 0 ) ) )
		return hr;
	if(image_num==0)
	{	
		float f=(D3DX_PI * fElapsedTime / 2.0f);
		for(int i=0;i<6;i++)
		{
			v[i].position.x=cosf(f)*v[i].position.x+sinf(f)*g_ff[i];
			v[i].position.z=(-sinf(f)*v[i].position.x)+g_ff[i];
			g_ff[i]=v[i].position.z;
			g_ff2[i]=v[i].position.x;
			v[i].position.z=0;
		}
    	
	}
	if(image_num==1)
	{	
		float f=(D3DX_PI * fElapsedTime / 2.0f);		
		if(juhang==0)
		{
			for(int j=0;j<6;j++)
				v[j].position.x=g_ff2[j];
			juhang=1;
		}
		for(int i=0;i<6;i++)
		{
			v[i].position.x=cosf(f)*v[i].position.x+sinf(f)*g_ff[i];
			v[i].position.z=(-sinf(f)*v[i].position.x)+g_ff[i];
			g_ff[i]=v[i].position.z;
			v[i].position.z=0;
		}
		
	}
		

	if( FAILED( hr = pVB->Unlock() ) )	
		return hr;

	pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 );
	


  if(g_rotate_angle>=D3DX_PI)
	  {
	  g_time=0;
	  g_Rand=g_Rand2;
      g_Rand2=rand()%MAX_IMAGE;
	  while(g_Rand2==g_Rand)
	  {		
		  g_Rand2=rand()%MAX_IMAGE;
	  }
	  g_rotate_angle=0;	  	  
	  juhang=0;
	  

	if( FAILED( hr = g_pVB_Book->Lock( 0, 0, (void **)&v, 0 ) ) )
		return hr;    
    v[0].position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
    v[1].position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    v[2].position = D3DXVECTOR3(0.0f, -1.0f+g_ratio, 0.0f);
    v[3].position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
    v[4].position = D3DXVECTOR3(0.0f, -1.0f+g_ratio, 0.0f);
    v[5].position = D3DXVECTOR3(-1.0f, -1.0f+g_ratio, 0.0f);
	if( FAILED( hr = g_pVB_Book->Unlock() ) )	
		return hr;
	if( FAILED( hr = g_pVB_Book2->Lock( 0, 0, (void **)&v, 0 ) ) )
		return hr;
	v[0].position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
    v[1].position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    v[2].position = D3DXVECTOR3(0.0f, -1.0f+g_ratio, 0.0f);
    v[3].position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
    v[4].position = D3DXVECTOR3(0.0f, -1.0f+g_ratio, 0.0f);
    v[5].position = D3DXVECTOR3(-1.0f, -1.0f+g_ratio, 0.0f);
    if( FAILED( hr = g_pVB_Book2->Unlock() ) )	
		return hr;
	for(int i=0;i<6;i++)
	{	
		g_ff[i]=0;	
		g_ff2[i]=0;
	  }

	  pd3dDevice->SetTransform( D3DTS_WORLD, &(*matWorld) );
	  Image_Mode_Change();
	  }
	
  
  
  
  return false;

}

bool FadeQuadVerts(D3DXCOLOR FadeToColor, float fFadeTime, 
                   float fEffectTime, LPDIRECT3DVERTEXBUFFER9 pVB,int effect_num)
{
  HRESULT hr;
  CUSTOMVERTEX* pVertices;  
  
// got it got it got it got it  
  if( FAILED( hr = pVB->Lock( 0, 6*sizeof(CUSTOMVERTEX), (void**)&pVertices, 0 ) ) )
      return false;
  if(effect_num==ALPHA_BLENDING)
  {
	  // calculate the new color value
  
	  g_FadeToColor.a+=(fFadeTime / fEffectTime);  
	  if (g_FadeToColor.a >= 255) g_FadeToColor.a = 255;  
	  pVertices[0].color    = (DWORD)g_FadeToColor;  
	  pVertices[1].color    = (DWORD)g_FadeToColor;  
	  pVertices[2].color    = (DWORD)g_FadeToColor;  
	  pVertices[3].color    = (DWORD)g_FadeToColor;  
	  pVertices[4].color    = (DWORD)g_FadeToColor;  
	  pVertices[5].color    = (DWORD)g_FadeToColor;

	  if(g_FadeToColor.a >= 1)  
	  {
	  g_time=0;
	  g_Rand=g_Rand2;
      g_Rand2=rand()%MAX_IMAGE;
	  while(g_Rand2==g_Rand)
	  {		
		  g_Rand2=rand()%MAX_IMAGE;
	  }
	  Image_Mode_Change();
	  g_FadeToColor.a=0;	   
	  }
	  if( FAILED( hr = pVB->Unlock() ) ) return false;
	  return true;
  }
  if(effect_num==IMAGE_SPRITE)
  {
	  g_image_pos_x+=(fFadeTime / fEffectTime)*3;  	  
	  if (g_image_pos_x >= -1.0f) g_image_pos_x=-1.0f;
	  pVertices[0].position   = D3DXVECTOR3(g_image_pos_x, g_image_pos_y, 0.0f);
	  pVertices[1].position    = D3DXVECTOR3(g_image_pos_x+2.0f, g_image_pos_y, 0.0f);
	  pVertices[2].position    = D3DXVECTOR3(g_image_pos_x+2.0f, g_image_pos_y-2.0f+g_ratio, 0.0f);
	  pVertices[3].position    = D3DXVECTOR3(g_image_pos_x, g_image_pos_y, 0.0f);
	  pVertices[4].position    = D3DXVECTOR3(g_image_pos_x+2.0f, g_image_pos_y-2.0f+g_ratio, 0.0f);
	  pVertices[5].position    = D3DXVECTOR3(g_image_pos_x, g_image_pos_y-2.0f+g_ratio, 0.0f);

	  if(g_image_pos_x >= -1.0f)  
	  {
	  g_time=0;
	  g_Rand=g_Rand2;
      g_Rand2=rand()%MAX_IMAGE;
	  while(g_Rand2==g_Rand)
	  {		
		  g_Rand2=rand()%MAX_IMAGE;
	  }
	  g_image_pos_x=-3.0f;	  
	  Image_Mode_Change();
	  }
	  if( FAILED( hr = pVB->Unlock() ) ) return false;
	  return true;  
  }
  
  
  
  return false;
}

void Image_Mode_Change()
{
	if(g_Image_Mode==ALL_SPRITE)
	{
      g_current_mode=rand()%3;
	}

}

bool FadeQuadVerts2(D3DXCOLOR FadeToColor, float fFadeTime, 
                   float fEffectTime, LPDIRECT3DVERTEXBUFFER9 pVB)
{
  HRESULT hr;
  
 CUSTOMVERTEX *pVertices;
  if(g_start2==0)
  {
  g_FadeToColor2.a-=(fFadeTime / fEffectTime);
  if (g_FadeToColor2.a <= 0) g_FadeToColor2.a =0;  
  if( FAILED( hr = pVB->Lock( 0, 6*sizeof(CUSTOMVERTEX), (void**)&pVertices, 0 ) ) )
      return false;

  pVertices[0].color    = (DWORD)g_FadeToColor2;
  pVertices[1].color    = (DWORD)g_FadeToColor2;
  pVertices[2].color    = (DWORD)g_FadeToColor2;
 
  pVertices[3].color    = (DWORD)g_FadeToColor2;
  pVertices[4].color    = (DWORD)g_FadeToColor2;
  pVertices[5].color    = (DWORD)g_FadeToColor2;
  
  if( FAILED( hr = pVB->Unlock() ) ) return false;
  if(g_FadeToColor2.a <= 0)
  {   
	  if(g_time>7)
	  {
		  g_start2=1;	  	  
		  g_time=0;	  
	  }
  }
  }
  else
  {
	  g_FadeToColor2.a+=(fFadeTime / fEffectTime);
  if (g_FadeToColor2.a >= 1) g_FadeToColor2.a =1;  
  if( FAILED( hr = pVB->Lock( 0, 6*sizeof(CUSTOMVERTEX), (void**)&pVertices, 0 ) ) )
      return false;

  pVertices[0].color    = (DWORD)g_FadeToColor2;
  pVertices[1].color    = (DWORD)g_FadeToColor2;
  pVertices[2].color    = (DWORD)g_FadeToColor2;
 
  pVertices[3].color    = (DWORD)g_FadeToColor2;
  pVertices[4].color    = (DWORD)g_FadeToColor2;
  pVertices[5].color    = (DWORD)g_FadeToColor2;
  
  if( FAILED( hr = pVB->Unlock() ) ) return false;
  if(g_FadeToColor2.a >= 1)
  {   
	  g_start=1;
	  g_time=0;	  
  }
  }
  
  
  return false;
}




HRESULT Render_Caption(LPCTSTR wFileName){

	HRESULT hr;
	
	// 필터그래프매니저 생성 (멤버 변수로 보관)
    // Get the interface for DirectShow's GraphBuilder

	int t;
	char buf[MAX_PATH] ;
	WCHAR temp[MAX_PATH];
	WideCharToMultiByte(CP_ACP, 0, 	
		wFileName,
		-1,
		buf,
		MAX_PATH,
		NULL, NULL);

	for(int i=0;i<MAX_PATH;i++)	
	{
		if(buf[i]=='.')	
			t=i;
	}
 
	buf[t+1]='s';
	buf[t+2]='m';
	buf[t+3]='i';
			
	MultiByteToWideChar( CP_ACP, 0, buf, -1,                          							
			temp, MAX_PATH );				

    CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **)&g_pGraphBuilder2) ;
	if(g_pGraphBuilder2->RenderFile(temp, NULL)!=S_OK)
	{
		g_bSMI_exist=false;
		return 0;
	}
	else g_bSMI_exist=true;

	// Setting SAMI (CC) Parser Filter
	CComPtr<IBaseFilter> pSP;
	hr = g_pGraphBuilder2->FindFilterByName(L"SAMI (CC) Parser", &pSP);
	if (SUCCEEDED(hr))
	{
		CComQIPtr<IAMStreamSelect> pSS(pSP);
		if (pSS != NULL) 
		{
			DWORD dwStreams = 0;
			pSS->Count(&dwStreams);
			TRACE("Stream count: %d\n", dwStreams);

			// Select what we want...
			// (하나의 언어/하나의 스타일 외에는 지정할 수 없다)
			// (나중에 지정된 것이 이전 설정된 내용을 덮어쓴다)
			hr = pSS->Enable(0, AMSTREAMSELECTENABLE_ENABLE);
			hr = pSS->Enable(2, AMSTREAMSELECTENABLE_ENABLE);


			// Print the name of each logical stream.
			for (DWORD index=0; index<dwStreams; index++)
			{
				DWORD dwFlags;
				WCHAR *wszName;

				hr = pSS->Info(index, NULL, &dwFlags, NULL, NULL, &wszName, NULL, NULL);
				if (SUCCEEDED(hr))
				{
					// wprintf(L"Stream %d: %s [%s]\n", index, wszName, (dwFlags ?  L"ENABLED" : L"DISABLED"));
				}

				CoTaskMemFree(wszName);
			}
		}
	
	}

	if(FAILED(hr = g_pGraphBuilder2->QueryInterface(IID_IMediaEventEx,
        reinterpret_cast<void **>(&g_pME2))))
        return hr;
	
	g_pME2->SetNotifyWindow((OAHWND)g_Hwnd, WM_GRAPHNOTIFY2, 0);

	if(FAILED(hr = g_pGraphBuilder2->QueryInterface(IID_IMediaControl,
        reinterpret_cast<void **>(&g_pMediaControl2))))
        return hr;

    // Get the IMediaControl Interface
    if(FAILED(hr = g_pGraphBuilder2->QueryInterface(IID_IMediaSeeking,
        reinterpret_cast<void **>(&g_pMediaSeeking2))))
        return hr;

	
	return true;
}

HRESULT DisplayFileDuration(void)
{
    HRESULT hr;

    if (!g_pMediaSeeking)
        return E_NOINTERFACE;

    // Initialize the display in case we can't read the duration
    //m_StrDuration.SetWindowText(TEXT("<00:00.000>"));

    // Is media time supported for this file?
    if (S_OK != g_pMediaSeeking->IsFormatSupported(&TIME_FORMAT_MEDIA_TIME))
        return E_NOINTERFACE;

    // Read the time format to restore later
    GUID guidOriginalFormat;
    hr = g_pMediaSeeking->GetTimeFormat(&guidOriginalFormat);
    if (FAILED(hr))
        return hr;

    // Ensure media time format for easy display
    hr = g_pMediaSeeking->SetTimeFormat(&TIME_FORMAT_MEDIA_TIME);
    if (FAILED(hr))
        return hr;

    // Read the file's duration
    LONGLONG llDuration=0;
    hr = g_pMediaSeeking->GetDuration(&llDuration);
    if (FAILED(hr))
        return hr;
	
    // Return to the original format
    if (guidOriginalFormat != TIME_FORMAT_MEDIA_TIME)
    {
        hr = g_pMediaSeeking->SetTimeFormat(&guidOriginalFormat);
        if (FAILED(hr))
            return hr;
    }

    // Convert the LONGLONG duration into human-readable format
    unsigned long nTotalMS = (unsigned long) ((float) llDuration / 10000.0); // 100ns -> ms
    int nSeconds = nTotalMS / (int) 1000;
    int nMinutes = nSeconds / (int) 60;
    nSeconds %= 60;

    // Update the display
    TCHAR szDuration[64];
    hr = StringCchPrintf(szDuration, NUMELMS(szDuration), _T("Duration: %d min %2d sec\0"), nMinutes, nSeconds);
   // m_StrDuration.SetWindowText(szDuration);

    return hr;
}




void ReadMediaPosition(float fPos, float ftype)
{
    HRESULT hr;
    
	
    // Convert position into a percentage value and update slider position
    if (gMedia_Len != 0)
    {
		
				long lTick = (long)((gMedia_Cur * 1400.0) / gMedia_Len);	
			
			long a=g_MP3_UI.GetSlider( IDC_SLIDER )->GetValue();					
			
		
		if(b_sliderbar_check==true && g_click == false &&ftype == -1 )
		{
			
			QWORD rtNew = (((QWORD)g_dwtime) * gMedia_Len) / 100;		
			REFERENCE_TIME rtNow = (REFERENCE_TIME)rtNew*28.33; //28.33를 알아내는데 4시간 걸렸습니다.		
	                                          //함부로 유출하는 걸 금지합니다.
			                                 //BASS와 다이렉트쇼의 시간 관계

			if(g_bSMI_exist==true){
			hr = g_pMediaSeeking2->SetPositions(&rtNow, AM_SEEKING_AbsolutePositioning,
                             NULL,   AM_SEEKING_NoPositioning);            
			}
			BASS_ChannelSetPosition(chan, rtNew);

			
			b_sliderbar_check=false;
		}
		if(b_sliderbar_check==true &&  g_click == false &&ftype == 0 )
		{
			

			QWORD rtNew = (((QWORD)fPos) * gMedia_Len) / 100;		
			REFERENCE_TIME rtNow = (REFERENCE_TIME)rtNew*28.33; //28.33를 알아내는데 4시간 걸렸습니다.		
	                                          //함부로 유출하는 걸 금지합니다.
			                                 //BASS와 다이렉트쇼의 시간 관계

			if(g_bSMI_exist==true){
			hr = g_pMediaSeeking2->SetPositions(&rtNow, AM_SEEKING_AbsolutePositioning,
                             NULL,   AM_SEEKING_NoPositioning);            
			}
			BASS_ChannelSetPosition(chan, rtNew);
			g_MP3_UI.GetSlider( IDC_SLIDER )->SetValue((int)fPos);					

			
			b_sliderbar_check=false;
		}		
		


    }

      
}


void SearchFileList(){
	HANDLE File_Hwnd;	
	WIN32_FIND_DATA wfd;
	
	BOOL bResult=TRUE;



	//GetCurrentDirectory(MAX_PATH,Path);
	//strcat(Path,"\\music3\\*.mp3");
	
	File_Hwnd=FindFirstFile(L"music/*.mp3",&wfd);
	if(((DWORD)(File_Hwnd))==0xffffffff)	
		bResult=false;
	else bResult=true;
	while(bResult)
	{
		
		g_MP3_UI.GetListBox( IDC_LISTBOXM )->AddItem( wfd.cFileName, (LPVOID)(size_t)g_Num );
		g_Num++;		
		bResult=FindNextFile(File_Hwnd,&wfd);
	}
	FindClose(File_Hwnd);
	File_Hwnd=FindFirstFile(L"music/*.wav",&wfd);
	if(((DWORD)(File_Hwnd))==0xffffffff)	
		bResult=false;
	else bResult=true;
	while(bResult)
	{
		
		g_MP3_UI.GetListBox( IDC_LISTBOXM )->AddItem( wfd.cFileName, (LPVOID)(size_t)g_Num );
		g_Num++;		
		bResult=FindNextFile(File_Hwnd,&wfd);
	}
	FindClose(File_Hwnd);
	File_Hwnd=FindFirstFile(L"music/*.ogg",&wfd);
	if(((DWORD)(File_Hwnd))==0xffffffff)	
		bResult=false;
	else bResult=true;
	while(bResult)
	{
		
		g_MP3_UI.GetListBox( IDC_LISTBOXM )->AddItem( wfd.cFileName, (LPVOID)(size_t)g_Num );
		g_Num++;		
		bResult=FindNextFile(File_Hwnd,&wfd);
	}
	FindClose(File_Hwnd);

	File_Hwnd=FindFirstFile(L"music/*.s3m",&wfd);
	if(((DWORD)(File_Hwnd))==0xffffffff)	
		bResult=false;
	else bResult=true;
	while(bResult)
	{
		
		g_MP3_UI.GetListBox( IDC_LISTBOXM )->AddItem( wfd.cFileName, (LPVOID)(size_t)g_Num );
		g_Num++;		
		bResult=FindNextFile(File_Hwnd,&wfd);
	}
	FindClose(File_Hwnd);

	File_Hwnd=FindFirstFile(L"music/*.umx",&wfd);
	if(((DWORD)(File_Hwnd))==0xffffffff)	
		bResult=false;
	else bResult=true;
	while(bResult)
	{
		
		g_MP3_UI.GetListBox( IDC_LISTBOXM )->AddItem( wfd.cFileName, (LPVOID)(size_t)g_Num );
		g_Num++;		
		bResult=FindNextFile(File_Hwnd,&wfd);
	}
	FindClose(File_Hwnd);

	File_Hwnd=FindFirstFile(L"music/*.xm",&wfd);
	if(((DWORD)(File_Hwnd))==0xffffffff)	
		bResult=false;
	else bResult=true;
	while(bResult)
	{
		
		g_MP3_UI.GetListBox( IDC_LISTBOXM )->AddItem( wfd.cFileName, (LPVOID)(size_t)g_Num );
		g_Num++;		
		bResult=FindNextFile(File_Hwnd,&wfd);
	}
	FindClose(File_Hwnd);

	File_Hwnd=FindFirstFile(L"music/*.xtm",&wfd);
	if(((DWORD)(File_Hwnd))==0xffffffff)	
		bResult=false;
	else bResult=true;
	while(bResult)
	{
		
		g_MP3_UI.GetListBox( IDC_LISTBOXM )->AddItem( wfd.cFileName, (LPVOID)(size_t)g_Num );
		g_Num++;		
		bResult=FindNextFile(File_Hwnd,&wfd);
	}
	FindClose(File_Hwnd);

	File_Hwnd=FindFirstFile(L"music/*.it",&wfd);
	if(((DWORD)(File_Hwnd))==0xffffffff)	
		bResult=false;
	else bResult=true;
	while(bResult)
	{
		
		g_MP3_UI.GetListBox( IDC_LISTBOXM )->AddItem( wfd.cFileName, (LPVOID)(size_t)g_Num );
		g_Num++;		
		bResult=FindNextFile(File_Hwnd,&wfd);
	}
	FindClose(File_Hwnd);

	File_Hwnd=FindFirstFile(L"music/*.mo3",&wfd);
		
	if(((DWORD)(File_Hwnd))==0xffffffff)	
		bResult=false;
	else bResult=true;
	while(bResult)
	{
		
		g_MP3_UI.GetListBox( IDC_LISTBOXM )->AddItem( wfd.cFileName, (LPVOID)(size_t)g_Num );
		g_Num++;		
		bResult=FindNextFile(File_Hwnd,&wfd);
	}
	FindClose(File_Hwnd);
}

void SearchFileList2(IDirect3DDevice9* pd3dDevice,float x, float y){
	HANDLE File_Hwnd;	
	WIN32_FIND_DATA wfd;
	
	BOOL bResult=TRUE;


    int get_count=0;
	int i=0,j=0;
	//GetCurrentDirectory(MAX_PATH,Path);
	//strcat(Path,"\\music3\\*.mp3");
	

	File_Hwnd=FindFirstFile(L"image/*.jpg",&wfd);
	while(bResult)
	{		
		get_count++;
		bResult=FindNextFile(File_Hwnd,&wfd);
	}
	FindClose(File_Hwnd);
	File_Hwnd=FindFirstFile(L"image/*.jpg",&wfd);	
	LPWSTR *pstrDynamic_Scene = new LPWSTR[get_count];	
	bool *temp_check = new bool[get_count];	
	for(i=0;i<get_count;i++)
	{		
		pstrDynamic_Scene[i]=new WCHAR[MAX_PATH];
		temp_check[i]=false;
	}
	i=0,j=0;
	bResult=true;
	File_Hwnd=FindFirstFile(L"image/*.jpg",&wfd);	
	WCHAR wFileName[MAX_PATH];	
	while(bResult)
	{			    
		StringCchPrintf( wFileName, MAX_PATH, L"image/%s",
                                 wfd.cFileName);
		lstrcpy(pstrDynamic_Scene[i],wFileName);
		bResult=FindNextFile(File_Hwnd,&wfd);
		i++;
	}		
	FindClose(File_Hwnd);

	i=0;
	while(i<MAX_IMAGE)
	{				
        j=rand()%get_count;
		if(temp_check[j]==false)		
			Create_3DSprite(pd3dDevice,g_pTexture_Fade[i],g_pVB_Fade[i],pstrDynamic_Scene[j],0,0,x,y,0);	
		else continue;
		i++;
	}
					
	for(i=0;i<get_count;i++)
	{		
		delete[] pstrDynamic_Scene[i];						
	}
	
	delete[] pstrDynamic_Scene;
	delete[] temp_check;
			
}
		






void Wait_Message()
{
	bool hr;  		
	D3DXMATRIX mat;  	   
	V( g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 45, 50, 170), 1.0f, 0) );

    // Render the scene
	if( SUCCEEDED( g_pd3dDevice->BeginScene() ) )
	{
		

		bool hr;  		
			D3DXMATRIX mat;  		
			D3DXMatrixOrthoLH(&mat, (float)g_width, (float)g_height,     			
				0.0, 1000.0);  		
			g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &mat );    		
			D3DXMATRIX matWorld,matTrans,matScale;  		
			D3DXMatrixScaling(&matScale, (float)g_width/2.0f, (float)g_height/2.0f, 1.0); 		
			D3DXMatrixMultiply(&matWorld, &matScale, D3DXMatrixTranslation(&matTrans, -0.5f, -0.5f, 0));  		
			g_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );    		
			g_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);		
 			g_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);		
			g_pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);		
			g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);     		
			g_pd3dDevice->SetTexture( 0, g_pTexture_Fade[g_Rand] );		   		
			g_pd3dDevice->SetStreamSource( 0, g_pVB_Fade[g_Rand],0, sizeof(CUSTOMVERTEX) );		
			g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );    		
			g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );    		
			g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );    		
			g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );												
			g_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );		
			g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 );		
		
  		V( g_pd3dDevice->EndScene() );   
		g_pd3dDevice->Present(NULL,NULL,NULL,NULL);//이게 이번 프레임워크에서는 꼭꼭 숨어 있군요....
		if(gScreen_Mode==CURTAIN_SPRITE)
			ResetSimulation();
		
	}
}


void NEXT_PLAY_MP3(DXUTListBoxItem *pItem,int chg){
                    
	
	g_Music_Start++;					
	g_pMediaControl2->Stop();
	b_sliderbar_check = false;
	BASS_MusicFree(chan);							
	BASS_StreamFree(chan);	
	if(g_Music_Start>=2&&g_bimage_change==true)					
	{						
		g_wait=0;			
		g_bgui_hide=true;
		Wait_Message();
		for(int i=0;i<MAX_IMAGE;i++)				
		{		
			SAFE_RELEASE(g_pTexture_Fade[i]);				
			SAFE_RELEASE(g_pVB_Fade[i]);			
		}
		SearchFileList2(g_pd3dDevice,g_width,g_height);
		g_wait=1;
		g_bgui_hide=false;
	}
	if(chg==1)	
		SwapSourceFilter(pItem->strText);
	else
	{
		int index=0,index2=0;
		
		index=g_MP3_UI.GetListBox( IDC_LISTBOXM )->GetSelectedIndex();
		index2=index;
		(g_MP3_UI.GetListBox( IDC_LISTBOXM )->GetItem(index2))->bSelected=false;
		index++;
		if(index>=g_Num)
			index=0;		
		g_MP3_UI.GetListBox( IDC_LISTBOXM )->SelectItem(index);				
		SwapSourceFilter((g_MP3_UI.GetListBox( IDC_LISTBOXM )->GetItem(index))->strText);

	}

	if(g_MP3_UI.GetCheckBox(IDC_FLANGER)->GetChecked()==true)			
	{             			
		memset(flabuf,0,sizeof(flabuf));								
		flapos=0;				
		flas=FLABUFLEN/2;
		flasinc=0.002f;
		fladsp=BASS_ChannelSetDSP(chan,&Flange,0,0);			
	}			
	else 	
		BASS_ChannelRemoveDSP(chan,fladsp);

	if(g_MP3_UI.GetCheckBox(IDC_ROTATE)->GetChecked()==true)									
	{             				
		rotpos=0.7853981f;								
		rotdsp=BASS_ChannelSetDSP(chan,&Rotate,0,2);							
	} else	
		BASS_ChannelRemoveDSP(chan,rotdsp);
			

	if(g_MP3_UI.GetCheckBox(IDC_ECHO)->GetChecked()==true)															
	{             				
		memset(echbuf,0,sizeof(echbuf));								
		echpos=0;						
		echdsp=BASS_ChannelSetDSP(chan,&Echo,0,1);					
	} else						
		BASS_ChannelRemoveDSP(chan,echdsp);					

	if(g_bimage_change==true)
	{	
		g_time=0;	
		g_FadeToColor=D3DCOLOR_ARGB(0,0,0,0);	
		g_FadeToColor2=D3DCOLOR_ARGB(255,0,0,0);
	}
	if(g_bCurtain==true)
	{	
		gScreen_Mode=CURTAIN_SPRITE;
	}
	else gScreen_Mode=IMAGE_PRESENT;
	if(g_bSMI_exist==true)			
	{								
		g_pMediaSeeking2->GetDuration(&g_rtTotalTime);        
	}
}

HRESULT Create_Book_VertexBuffer( IDirect3DDevice9* pd3dDevice,float x, float y, float width, float height,int alpha)
{


	HRESULT hr;
	SAFE_RELEASE(g_pVB_Book);				
	SAFE_RELEASE(g_pVB_Book2);				
	if(g_pVB_Book==NULL)
	{ 
		pd3dDevice->CreateVertexBuffer(
		6 * sizeof(CUSTOMVERTEX), 
		D3DUSAGE_WRITEONLY,
		D3DFVF_CUSTOMVERTEX,
		D3DPOOL_MANAGED,
		&g_pVB_Book,
		0);
	}
	if(g_pVB_Book2==NULL)
	{ 
		pd3dDevice->CreateVertexBuffer(
		6 * sizeof(CUSTOMVERTEX), 
		D3DUSAGE_WRITEONLY,
		D3DFVF_CUSTOMVERTEX,
		D3DPOOL_MANAGED,
		&g_pVB_Book2,
		0);
	}

	if(g_pVB_Book3==NULL)
	{ 
		pd3dDevice->CreateVertexBuffer(
		6 * sizeof(CUSTOMVERTEX), 
		D3DUSAGE_WRITEONLY,
		D3DFVF_CUSTOMVERTEX,
		D3DPOOL_MANAGED,
		&g_pVB_Book3,
		0);
	}

	if(g_pVB_Book4==NULL)
	{ 
		pd3dDevice->CreateVertexBuffer(
		6 * sizeof(CUSTOMVERTEX), 
		D3DUSAGE_WRITEONLY,
		D3DFVF_CUSTOMVERTEX,
		D3DPOOL_MANAGED,
		&g_pVB_Book4,
		0);
	}

	CUSTOMVERTEX* v;	
	if( FAILED( hr = g_pVB_Book->Lock( 0, 0, (void **)&v, 0 ) ) )
		return hr;

    // first triangle
    v[0].position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
    v[0].color    = D3DCOLOR_ARGB(alpha,0,0,0);
    v[0].tu       = 0.0f;
    v[0].tv       = 0.0f;

    v[1].position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    v[1].color    = D3DCOLOR_ARGB(alpha,0,0,0);
    v[1].tu       = 0.5f;
    v[1].tv       = 0.0f;

    v[2].position = D3DXVECTOR3(0.0f, -1.0f+g_ratio, 0.0f);
    v[2].color    = D3DCOLOR_ARGB(alpha,0,0,0);
    v[2].tu       = 0.5f;
    v[2].tv       = 1.0f;

    // second triangle
    v[3].position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
    v[3].color    = D3DCOLOR_ARGB(alpha,0,0,0);
    v[3].tu       = 0.0f;
    v[3].tv       = 0.0f;

    v[4].position = D3DXVECTOR3(0.0f, -1.0f+g_ratio, 0.0f);
    v[4].color    = D3DCOLOR_ARGB(alpha,0,0,0);
    v[4].tu       = 0.5f;
    v[4].tv       = 1.0f;

    v[5].position = D3DXVECTOR3(-1.0f, -1.0f+g_ratio, 0.0f);
    v[5].color    = D3DCOLOR_ARGB(alpha,0,0,0);
    v[5].tu       = 0.0f;
    v[5].tv       = 1.0f;

	if( FAILED( hr = g_pVB_Book->Unlock() ) )	
		return hr;

	if( FAILED( hr = g_pVB_Book2->Lock( 0, 0, (void **)&v, 0 ) ) )
		return hr;

	v[0].position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
    v[0].color    = D3DCOLOR_ARGB(alpha,0,0,0);
    v[0].tu       = 1.0f;
    v[0].tv       = 0.0f;

    v[1].position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    v[1].color    = D3DCOLOR_ARGB(alpha,0,0,0);
    v[1].tu       = 0.5f;
    v[1].tv       = 0.0f;

    v[2].position = D3DXVECTOR3(0.0f, -1.0f+g_ratio, 0.0f);
    v[2].color    = D3DCOLOR_ARGB(alpha,0,0,0);
    v[2].tu       = 0.5f;
    v[2].tv       = 1.0f;

    // second triangle
    v[3].position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
    v[3].color    = D3DCOLOR_ARGB(alpha,0,0,0);
    v[3].tu       = 1.0f;
    v[3].tv       = 0.0f;

    v[4].position = D3DXVECTOR3(0.0f, -1.0f+g_ratio, 0.0f);
    v[4].color    = D3DCOLOR_ARGB(alpha,0,0,0);
    v[4].tu       = 0.5f;
    v[4].tv       = 1.0f;

    v[5].position = D3DXVECTOR3(-1.0f, -1.0f+g_ratio, 0.0f);
    v[5].color    = D3DCOLOR_ARGB(alpha,0,0,0);
    v[5].tu       = 1.0f;
    v[5].tv       = 1.0f;

    if( FAILED( hr = g_pVB_Book2->Unlock() ) )	
		return hr;


	if( FAILED( hr = g_pVB_Book3->Lock( 0, 0, (void **)&v, 0 ) ) )
		return hr;

	v[0].position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
    v[0].color    = D3DCOLOR_ARGB(alpha,0,0,0);
    v[0].tu       = 0.0f;
    v[0].tv       = 0.0f;

    v[1].position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    v[1].color    = D3DCOLOR_ARGB(alpha,0,0,0);
    v[1].tu       = 0.5f;
    v[1].tv       = 0.0f;

    v[2].position = D3DXVECTOR3(0.0f, -1.0f+g_ratio, 0.0f);
    v[2].color    = D3DCOLOR_ARGB(alpha,0,0,0);
    v[2].tu       = 0.5f;
    v[2].tv       = 1.0f;

    // second triangle
    v[3].position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
    v[3].color    = D3DCOLOR_ARGB(alpha,0,0,0);
    v[3].tu       = 0.0f;
    v[3].tv       = 0.0f;

    v[4].position = D3DXVECTOR3(0.0f, -1.0f+g_ratio, 0.0f);
    v[4].color    = D3DCOLOR_ARGB(alpha,0,0,0);
    v[4].tu       = 0.5f;
    v[4].tv       = 1.0f;

    v[5].position = D3DXVECTOR3(-1.0f, -1.0f+g_ratio, 0.0f);
    v[5].color    = D3DCOLOR_ARGB(alpha,0,0,0);
    v[5].tu       = 0.0f;
    v[5].tv       = 1.0f;

    if( FAILED( hr = g_pVB_Book3->Unlock() ) )	
		return hr;


		if( FAILED( hr = g_pVB_Book4->Lock( 0, 0, (void **)&v, 0 ) ) )
		return hr;

	v[0].position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    v[0].color    = D3DCOLOR_ARGB(alpha,0,0,0);
    v[0].tu       = 0.0f;
    v[0].tv       = 0.0f;

    v[1].position = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
    v[1].color    = D3DCOLOR_ARGB(alpha,0,0,0);
    v[1].tu       = 0.5f;
    v[1].tv       = 0.0f;

    v[2].position = D3DXVECTOR3(1.0f, -1.0f+g_ratio, 0.0f);
    v[2].color    = D3DCOLOR_ARGB(alpha,0,0,0);
    v[2].tu       = 0.5f;
    v[2].tv       = 1.0f;

    // second triangle
    v[3].position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    v[3].color    = D3DCOLOR_ARGB(alpha,0,0,0);
    v[3].tu       = 0.0f;
    v[3].tv       = 0.0f;

    v[4].position = D3DXVECTOR3(1.0f, -1.0f+g_ratio, 0.0f);
    v[4].color    = D3DCOLOR_ARGB(alpha,0,0,0);
    v[4].tu       = 0.5f;
    v[4].tv       = 1.0f;

    v[5].position = D3DXVECTOR3(0.0f, -1.0f+g_ratio, 0.0f);
    v[5].color    = D3DCOLOR_ARGB(alpha,0,0,0);
    v[5].tu       = 0.0f;
    v[5].tv       = 1.0f;

    if( FAILED( hr = g_pVB_Book4->Unlock() ) )	
		return hr;

	return hr;
}

HRESULT InitCurtainGeometry()
{
    
	HRESULT hr;
	if(g_pVB_Curtain==NULL)
	{ 
		g_pd3dDevice->CreateVertexBuffer(
		16*16 * sizeof(CUSTOMVERTEX), 
		D3DUSAGE_WRITEONLY,
		D3DFVF_CUSTOMVERTEX,
		D3DPOOL_MANAGED,
		&g_pVB_Curtain,
		0);
		
	}

	
			
    // Fill the vertex buffer. We are setting the tu and tv texture
    // coordinates, which range from 0.0 to 1.0
    CUSTOMVERTEX* pVertices;
    if( FAILED( g_pVB_Curtain->Lock( 0, 0, (void**)&pVertices, 0 ) ) )
        return E_FAIL;
    for( DWORD i=0; i<16; i++ )
    {		
		for( DWORD j=0; j<16; j++ )    
		{		
			
			pVertices[16*i+j].position = D3DXVECTOR3(pa[16*i+j].x,pa[16*i+j].y,pa[16*i+j].z);
			pVertices[16*i+j].color    = 0xffffffff;

			pVertices[16*i+j].tv       = ((float)i)/15;
			pVertices[16*i+j].tu       = ((float)j)/(16-1);
			

	
		}
	}
	g_pVB_Curtain->Unlock();

	if(g_pIB_Curtain==NULL)
	{ 
	
		hr = g_pd3dDevice->CreateIndexBuffer( 6*16*16*sizeof(WORD),
                                        D3DUSAGE_WRITEONLY,
                                        D3DFMT_INDEX16, D3DPOOL_MANAGED,
                                        &g_pIB_Curtain,0 );
  
		if( FAILED(hr) )      
			return E_FAIL;
		WORD* pIndices;

  // Fill the index buffer
  g_pIB_Curtain->Lock( 0, 6*16*16*sizeof(WORD), (void**)&pIndices, 0 );
  if( FAILED(hr) ) return E_FAIL;

  // Fill in indices
  for(int i=0;i<16;i++)
  {  
	  for(DWORD j=0;j<16;j++)
	  {
	  *pIndices++ = (WORD)(i*16+j );
      *pIndices++ = (WORD)( i*16+1+j );
      *pIndices++ = (WORD)( (i+1)*16+1+j);
	  *pIndices++ = (WORD)(i*16+j );
      *pIndices++ = (WORD)( (i+1)*16+1+j );
      *pIndices++ = (WORD)( (i+1)*16+j);
	  }
  }
  
  g_pIB_Curtain->Unlock();
	}

  

    return S_OK;
}