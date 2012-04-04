/*
#############################################################################

  Ch18p2_ParticleProps.cpp: a program that demonstrates how to create a
  particle system editor.
  
#############################################################################
*/

// include files ////////////////////////////////////////////////////////////
#define STRICT
#include <stdio.h>
#include <math.h>
#include <windows.h>
#include <D3DX8.h>
#include "D3DApp.h"
#include "D3DFile.h"
#include "D3DFont.h"
#include "D3DUtil.h"
#include "DXUtil.h"
#include "D3DHelperFuncs.h"
#include "Ch18p2_resource.h"
#include "GroundPlane.h"
#include "Camera.h"
#include "InputManager.h"
#include "Ch18p2_ParticleEmitter.h"

//-----------------------------------------------------------------------------
// Name: class CMyD3DApplication
// Desc: Application class. The base class (CD3DApplication) provides the 
//       generic functionality needed in all Direct3D samples. CMyD3DApplication 
//       adds functionality specific to this sample program.
//-----------------------------------------------------------------------------
class CMyD3DApplication : public CD3DApplication
{
  // Font for drawing text
  CD3DFont* m_pFont;
  CD3DFont* m_pFontSmall;

  // Scene
  CGroundPlane m_Ground;
  CUserControlledCamera m_Camera;

  // Mouse Input
  CInputManager m_InputManager;

  // The Particle System
  CParticleEmitter m_PartSys;

  // dialog box window handle
  HWND m_hWndPropDlg;

protected:
  HRESULT OneTimeSceneInit();
  HRESULT InitDeviceObjects();
  HRESULT RestoreDeviceObjects();
  HRESULT InvalidateDeviceObjects();
  HRESULT DeleteDeviceObjects();
  HRESULT FinalCleanup();
  HRESULT Render();
  HRESULT FrameMove();
  HRESULT ConfirmDevice( D3DCAPS8* pCaps, DWORD dwBehavior, D3DFORMAT Format );
  LRESULT MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

  void ProcessInput();

public:
  CMyD3DApplication();

  // is the ground shown?
  bool m_bShowGround;
};

CMyD3DApplication* g_pD3DApp = NULL;

CParticleEmitter *g_theSystem = NULL;

void PopulateAlphaBlendingComboBox(HWND hWnd, UINT idCombo)
{
  SendMessage(GetDlgItem(hWnd, idCombo), CB_RESETCONTENT, 0, 0);
  for (int q=0; q < CParticleEmitter::NUMBLENDINGMODES; q++) {
    SendMessage(GetDlgItem(hWnd, idCombo), CB_ADDSTRING, 0, (long)(g_theSystem->m_BlendingModes[q].name));
    SendMessage(GetDlgItem(hWnd, idCombo), CB_SETITEMDATA, q, g_theSystem->m_BlendingModes[q].mode);
  }
}

void SelectBlendModeInComboBox(HWND hWnd, UINT idCombo, int blendmode)
{
  for (int q=0; q < CParticleEmitter::NUMBLENDINGMODES; q++) {
    int data = SendMessage(GetDlgItem(hWnd, idCombo), CB_GETITEMDATA, q, 0);
    if (data == blendmode) {
      SendMessage(GetDlgItem(hWnd, idCombo), CB_SETCURSEL, q, 0);
    }
  }
}

int GetCurSelDataFromComboBox(HWND hWnd, UINT idCombo)
{
  int ndx = SendMessage(GetDlgItem(hWnd, idCombo), CB_GETCURSEL, 0, 0);
  int data = 0;
  if (ndx >= 0) {
    data = SendMessage(GetDlgItem(hWnd, idCombo), CB_GETITEMDATA, ndx, 0);
  }
  return(data);
}
 

D3DXVECTOR3 Stuff3EditsIntoVector(HWND hWnd, UINT idX, UINT idY, UINT idZ)
{
  char buf[256];
  float fX, fY, fZ;
  GetWindowText(GetDlgItem(hWnd, idX), buf, sizeof(buf)); fX = atof(buf);
  GetWindowText(GetDlgItem(hWnd, idY), buf, sizeof(buf)); fY = atof(buf);
  GetWindowText(GetDlgItem(hWnd, idZ), buf, sizeof(buf)); fZ = atof(buf);
  return(D3DXVECTOR3(fX,fY,fZ));
}

void StuffVectorInto3Edits(D3DXVECTOR3 vector, HWND hWnd, UINT idX, UINT idY, UINT idZ)
{
  char buf[256];
  _snprintf(buf, sizeof(buf), "%.1f", vector.x); SetWindowText(GetDlgItem(hWnd, idX), buf);
  _snprintf(buf, sizeof(buf), "%.1f", vector.y); SetWindowText(GetDlgItem(hWnd, idY), buf);
  _snprintf(buf, sizeof(buf), "%.1f", vector.z); SetWindowText(GetDlgItem(hWnd, idZ), buf);
}

D3DXCOLOR Stuff4EditsIntoColor(HWND hWnd, UINT idR, UINT idG, UINT idB, UINT idA)
{
  char buf[256];
  float fR, fG, fB, fA;
  GetWindowText(GetDlgItem(hWnd, idR), buf, sizeof(buf)); fR = atof(buf);
  GetWindowText(GetDlgItem(hWnd, idG), buf, sizeof(buf)); fG = atof(buf);
  GetWindowText(GetDlgItem(hWnd, idB), buf, sizeof(buf)); fB = atof(buf);
  GetWindowText(GetDlgItem(hWnd, idA), buf, sizeof(buf)); fA = atof(buf);
  return(D3DXCOLOR(fR,fG,fB,fA));
}

void StuffColorInto4Edits(D3DXCOLOR color, HWND hWnd, UINT idR, UINT idG, UINT idB, UINT idA)
{
  char buf[256];
  _snprintf(buf, sizeof(buf), "%.1f", color.r); SetWindowText(GetDlgItem(hWnd, idR), buf);
  _snprintf(buf, sizeof(buf), "%.1f", color.g); SetWindowText(GetDlgItem(hWnd, idG), buf);
  _snprintf(buf, sizeof(buf), "%.1f", color.b); SetWindowText(GetDlgItem(hWnd, idB), buf);
  _snprintf(buf, sizeof(buf), "%.1f", color.a); SetWindowText(GetDlgItem(hWnd, idA), buf);
}

CMinMax<float> Stuff2EditsIntoMinMax(HWND hWnd, UINT idMin, UINT idMax)
{
  char buf[256];
  float fMin, fMax;
  GetWindowText(GetDlgItem(hWnd, idMin), buf, sizeof(buf)); fMin = atof(buf);
  GetWindowText(GetDlgItem(hWnd, idMax), buf, sizeof(buf)); fMax = atof(buf);
  return(CMinMax<float>(fMin,fMax));
}

void StuffMinMaxInto2Edits(CMinMax<float> minmax, HWND hWnd, UINT idMin, UINT idMax)
{
  char buf[256];
  _snprintf(buf, sizeof(buf), "%.1f", minmax.m_Min); SetWindowText(GetDlgItem(hWnd, idMin), buf);
  _snprintf(buf, sizeof(buf), "%.1f", minmax.m_Max); SetWindowText(GetDlgItem(hWnd, idMax), buf);
}

void BrowseForColor(HWND hWnd, UINT idR, UINT idG, UINT idB, UINT idA)
{
  static COLORREF custcolors[16];

  CHOOSECOLOR cc;
  memset(&cc, 0, sizeof(cc));
  cc.lStructSize = sizeof(cc);
  cc.hwndOwner = hWnd;
  cc.Flags = CC_RGBINIT | CC_FULLOPEN;
  cc.lpCustColors = custcolors;

  // get current values, convert to 0-255 range
  D3DXCOLOR color = Stuff4EditsIntoColor(hWnd, idR, idG, idB, idA);
  cc.rgbResult = D3DXColorTo255RGB(color);

  if (ChooseColor(&cc)) {
    color.r = (float)GetRValue(cc.rgbResult) / 255.0f;
    color.g = (float)GetGValue(cc.rgbResult) / 255.0f;
    color.b = (float)GetBValue(cc.rgbResult) / 255.0f;
    StuffColorInto4Edits(color, hWnd, idR, idG, idB, idA);
  }
}

LRESULT CALLBACK PropDlgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
  CMyD3DApplication *app = (CMyD3DApplication *)g_pD3DApp;
  static bool bTurnOffUpdates = false;
  switch (uMsg) {
    case WM_INITDIALOG: 
      {
        bTurnOffUpdates = true; // so as we call ourselves we don't change things
        
        // put current properties into window

        StuffVectorInto3Edits(g_theSystem->GetSpawnDir1(), hWnd, IDC_SPAWNDIR1X, IDC_SPAWNDIR1Y, IDC_SPAWNDIR1Z);
        StuffVectorInto3Edits(g_theSystem->GetSpawnDir2(), hWnd, IDC_SPAWNDIR2X, IDC_SPAWNDIR2Y, IDC_SPAWNDIR2Z);
        StuffVectorInto3Edits(g_theSystem->GetGravity(),   hWnd, IDC_GRAVITYX,   IDC_GRAVITYY,   IDC_GRAVITYZ);
        StuffVectorInto3Edits(g_theSystem->GetEmitRadius(),hWnd, IDC_EMITRADIUSX,IDC_EMITRADIUSY,IDC_EMITRADIUSZ);
        StuffVectorInto3Edits(g_theSystem->GetPos(),       hWnd, IDC_POSX,       IDC_POSY,       IDC_POSZ);

        char buf[256];
        _snprintf(buf, sizeof(buf), "%d", g_theSystem->GetMaxParticles()); SetWindowText(GetDlgItem(hWnd, IDC_MAXPARTS), buf);
  
        StuffColorInto4Edits(g_theSystem->GetStartColor1(),hWnd, IDC_STARTCOLOR1R, IDC_STARTCOLOR1G, IDC_STARTCOLOR1B, IDC_STARTCOLOR1A);
        StuffColorInto4Edits(g_theSystem->GetStartColor2(),hWnd, IDC_STARTCOLOR2R, IDC_STARTCOLOR2G, IDC_STARTCOLOR2B, IDC_STARTCOLOR2A);
        StuffColorInto4Edits(g_theSystem->GetEndColor1(),  hWnd, IDC_ENDCOLOR1R,   IDC_ENDCOLOR1G,   IDC_ENDCOLOR1B,   IDC_ENDCOLOR1A);
        StuffColorInto4Edits(g_theSystem->GetEndColor2(),  hWnd, IDC_ENDCOLOR2R,   IDC_ENDCOLOR2G,   IDC_ENDCOLOR2B,   IDC_ENDCOLOR2A);

        StuffMinMaxInto2Edits(g_theSystem->GetLifetime(), hWnd, IDC_LIFETIMEMIN, IDC_LIFETIMEMAX);
        StuffMinMaxInto2Edits(g_theSystem->GetSize(),     hWnd, IDC_SIZEMIN,     IDC_SIZEMAX);
        StuffMinMaxInto2Edits(g_theSystem->GetEmitRate(), hWnd, IDC_EMITRATEMIN, IDC_EMITRATEMAX);

        SendMessage(GetDlgItem(hWnd, IDC_SHOWGROUND), BM_SETCHECK, app->m_bShowGround, 0);

        SetWindowText(GetDlgItem(hWnd, IDC_TEXTURE), g_theSystem->GetTextureFilename().c_str());

        PopulateAlphaBlendingComboBox(hWnd, IDC_SRCBLEND);
        PopulateAlphaBlendingComboBox(hWnd, IDC_DESTBLEND);

        SelectBlendModeInComboBox(hWnd, IDC_SRCBLEND, g_theSystem->GetSrcBlendMode());
        SelectBlendModeInComboBox(hWnd, IDC_DESTBLEND, g_theSystem->GetDestBlendMode());

        bTurnOffUpdates = false; // now allow updates to go to part sys

        return(true); // focus not set
      }

    case WM_COMMAND:
      {
        if (bTurnOffUpdates) return(false);
        UINT wID = LOWORD(wParam);         // item, control, or accelerator identifier 
        HWND hwndCtl = (HWND) lParam;      // handle of control 
        
        switch(wID) {
          case IDC_SPAWNDIR1X: case IDC_SPAWNDIR1Y: case IDC_SPAWNDIR1Z: 
            g_theSystem->SetSpawnDir1(Stuff3EditsIntoVector(hWnd, IDC_SPAWNDIR1X, IDC_SPAWNDIR1Y, IDC_SPAWNDIR1Z));
            break;
            
          case IDC_SPAWNDIR2X: case IDC_SPAWNDIR2Y: case IDC_SPAWNDIR2Z: 
            g_theSystem->SetSpawnDir2(Stuff3EditsIntoVector(hWnd, IDC_SPAWNDIR2X, IDC_SPAWNDIR2Y, IDC_SPAWNDIR2Z));
            break;

          case IDC_GRAVITYX: case IDC_GRAVITYY: case IDC_GRAVITYZ: 
            g_theSystem->SetGravity(Stuff3EditsIntoVector(hWnd, IDC_GRAVITYX, IDC_GRAVITYY, IDC_GRAVITYZ));
            break;

          case IDC_EMITRADIUSX: case IDC_EMITRADIUSY: case IDC_EMITRADIUSZ: 
            g_theSystem->SetEmitRadius(Stuff3EditsIntoVector(hWnd, IDC_EMITRADIUSX, IDC_EMITRADIUSY, IDC_EMITRADIUSZ));
            break;

          case IDC_POSX: case IDC_POSY: case IDC_POSZ: 
            g_theSystem->SetPos(Stuff3EditsIntoVector(hWnd, IDC_POSX, IDC_POSY, IDC_POSZ));
            break;

          case IDC_SRCBLEND:
            g_theSystem->SetSrcBlendMode(GetCurSelDataFromComboBox(hWnd, IDC_SRCBLEND));
            break;  
            
          case IDC_DESTBLEND:
            g_theSystem->SetDestBlendMode(GetCurSelDataFromComboBox(hWnd, IDC_DESTBLEND));
            break; 
            
          case IDC_MAXPARTS:
            {
              char buf[256];
              GetWindowText(GetDlgItem(hWnd, IDC_MAXPARTS), buf, sizeof(buf));
              g_theSystem->SetMaxParticles(atoi(buf));
            }
            break;

          case IDC_STARTCOLOR1R: case IDC_STARTCOLOR1G: case IDC_STARTCOLOR1B: case IDC_STARTCOLOR1A:
            g_theSystem->SetStartColor1(Stuff4EditsIntoColor(hWnd, IDC_STARTCOLOR1R, IDC_STARTCOLOR1G, IDC_STARTCOLOR1B, IDC_STARTCOLOR1A));
            break;

          case IDC_STARTCOLOR2R: case IDC_STARTCOLOR2G: case IDC_STARTCOLOR2B: case IDC_STARTCOLOR2A:
            g_theSystem->SetStartColor2(Stuff4EditsIntoColor(hWnd, IDC_STARTCOLOR2R, IDC_STARTCOLOR2G, IDC_STARTCOLOR2B, IDC_STARTCOLOR2A));
            break;

          case IDC_ENDCOLOR1R: case IDC_ENDCOLOR1G: case IDC_ENDCOLOR1B: case IDC_ENDCOLOR1A:
            g_theSystem->SetEndColor1(Stuff4EditsIntoColor(hWnd, IDC_ENDCOLOR1R, IDC_ENDCOLOR1G, IDC_ENDCOLOR1B, IDC_ENDCOLOR1A));
            break;

          case IDC_ENDCOLOR2R: case IDC_ENDCOLOR2G: case IDC_ENDCOLOR2B: case IDC_ENDCOLOR2A:
            g_theSystem->SetEndColor2(Stuff4EditsIntoColor(hWnd, IDC_ENDCOLOR2R, IDC_ENDCOLOR2G, IDC_ENDCOLOR2B, IDC_ENDCOLOR2A));
            break;

          case IDC_STARTCOLOR1BROWSE:
            BrowseForColor(hWnd, IDC_STARTCOLOR1R, IDC_STARTCOLOR1G, IDC_STARTCOLOR1B, IDC_STARTCOLOR1A);
            break;

          case IDC_STARTCOLOR2BROWSE:
            BrowseForColor(hWnd, IDC_STARTCOLOR2R, IDC_STARTCOLOR2G, IDC_STARTCOLOR2B, IDC_STARTCOLOR2A);
            break;

          case IDC_ENDCOLOR1BROWSE:
            BrowseForColor(hWnd, IDC_ENDCOLOR1R, IDC_ENDCOLOR1G, IDC_ENDCOLOR1B, IDC_ENDCOLOR1A);
            break;

          case IDC_ENDCOLOR2BROWSE:
            BrowseForColor(hWnd, IDC_ENDCOLOR2R, IDC_ENDCOLOR2G, IDC_ENDCOLOR2B, IDC_ENDCOLOR2A);
            break;

          case IDC_LIFETIMEMIN: case IDC_LIFETIMEMAX:
            g_theSystem->SetLifetime(Stuff2EditsIntoMinMax(hWnd, IDC_LIFETIMEMIN, IDC_LIFETIMEMAX));
            break;

          case IDC_SIZEMIN: case IDC_SIZEMAX:
            g_theSystem->SetSize(Stuff2EditsIntoMinMax(hWnd, IDC_SIZEMIN, IDC_SIZEMAX));
            break;

          case IDC_EMITRATEMIN: case IDC_EMITRATEMAX:
            g_theSystem->SetEmitRate(Stuff2EditsIntoMinMax(hWnd, IDC_EMITRATEMIN, IDC_EMITRATEMAX));
            break;

          case IDC_RESET:
            g_theSystem->Reset();
            break;

          case IDC_SHOWGROUND:
            app->m_bShowGround = app->m_bShowGround ? false : true;
            break;

          case IDC_TEXTURE_BROWSE:
            {
              OPENFILENAME ofn;       // common dialog box structure
              char szFile[260];       // buffer for filename
              
              // Initialize OPENFILENAME
              ZeroMemory(&ofn, sizeof(OPENFILENAME));
              ofn.lStructSize = sizeof(OPENFILENAME);
              ofn.hwndOwner = hWnd;
              strncpy(szFile, g_theSystem->GetTextureFilename().c_str(), sizeof(szFile));
              ofn.lpstrFile = szFile;
              ofn.nMaxFile = sizeof(szFile);
              ofn.lpstrFilter = "PNG Files (*.png)\0*.png\0Bitmap Files (*.bmp)\0*.bmp\0All Files (*.*)\0*.*\0\0";
              ofn.lpstrFileTitle = "Choose Texture File";
              ofn.nMaxFileTitle = 0;
              ofn.lpstrInitialDir = NULL;
              ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

              // Display the Open dialog box. 

              if (GetOpenFileName(&ofn)==TRUE) {
                g_theSystem->SetTexture(ofn.lpstrFile);
                SetWindowText(GetDlgItem(hWnd, IDC_TEXTURE), g_theSystem->GetTextureFilename().c_str());
              }
            }
            break;

          case IDC_SAVE:
            {
              OPENFILENAME ofn;       // common dialog box structure
              char szFile[260];       // buffer for filename
              
              // Initialize OPENFILENAME
              ZeroMemory(&ofn, sizeof(OPENFILENAME));
              ofn.lStructSize = sizeof(OPENFILENAME);
              ofn.hwndOwner = hWnd;
              strcpy(szFile, "ParticleSystem.txt");
              ofn.lpstrFile = szFile;
              ofn.nMaxFile = sizeof(szFile);
              ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0\0";
              ofn.lpstrFileTitle = "Save Particle System Properties";
              ofn.nMaxFileTitle = 0;
              ofn.lpstrInitialDir = NULL;
              ofn.Flags = OFN_OVERWRITEPROMPT;

              // Display the Save As dialog box. 

              if (GetSaveFileName(&ofn)==TRUE) {
                if (!g_theSystem->Save(ofn.lpstrFile)) {
                  ::MessageBox(hWnd, g_theSystem->m_strLastError.c_str(), "Error Saving File", MB_ICONSTOP);
                } 
                else SetWindowText(hWnd, ofn.lpstrFile);
              }
            }
            break;

          case IDC_LOAD:
            {
              OPENFILENAME ofn;       // common dialog box structure
              char szFile[260];       // buffer for filename
              
              // Initialize OPENFILENAME
              ZeroMemory(&ofn, sizeof(OPENFILENAME));
              ofn.lStructSize = sizeof(OPENFILENAME);
              ofn.hwndOwner = hWnd;
              strcpy(szFile, "ParticleSystem.txt");
              ofn.lpstrFile = szFile;
              ofn.nMaxFile = sizeof(szFile);
              ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0\0";
              ofn.lpstrFileTitle = "Save Particle System Properties";
              ofn.nMaxFileTitle = 0;
              ofn.lpstrInitialDir = NULL;
              ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

              // Display the Save As dialog box. 

              if (GetOpenFileName(&ofn)==TRUE) {
                if (!g_theSystem->Load(ofn.lpstrFile)) {
                  ::MessageBox(hWnd, g_theSystem->m_strLastError.c_str(), "Error Loading File", MB_ICONSTOP);
                } 
                else SetWindowText(hWnd, ofn.lpstrFile);
              }
            }
            break;



        } // switch wID
      } // WM_COMMAND block
      break;
  } // switch uMsg
  return(0);
}

//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: Entry point to the program. Initializes everything, and goes into a
//       message-processing loop. Idle time is used to render the scene.
//-----------------------------------------------------------------------------
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{
  CMyD3DApplication d3dApp;
  g_pD3DApp = &d3dApp;

  if( FAILED( d3dApp.Create( hInst ) ) )
    return 0;

  return d3dApp.Run();
}

//-----------------------------------------------------------------------------
// Name: CMyD3DApplication()
// Desc: Application constructor. Sets attributes for the app.
//-----------------------------------------------------------------------------
CMyD3DApplication::CMyD3DApplication()
{
  m_strWindowTitle    = _T("Ch18p2_ParticleProps");
  m_bUseDepthBuffer   = TRUE;

  m_pFont            = new CD3DFont( _T("Arial"), 12, D3DFONT_BOLD );
  m_pFontSmall       = new CD3DFont( _T("Arial"),  9, D3DFONT_BOLD );
}

//-----------------------------------------------------------------------------
// Name: OneTimeSceneInit()
// Desc: Called during initial app startup, this function performs all the
//       permanent initialization.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::OneTimeSceneInit()
{
  return S_OK;
}

//-----------------------------------------------------------------------------
// Name: FrameMove()
// Desc: Called once per frame, the call is the entry point for animating
//       the scene.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::FrameMove()
{
  if (m_fElapsedTime < 0.0001f) m_fElapsedTime = 0.0001f;
  m_PartSys.Update(m_fTime, m_fElapsedTime);
  return S_OK;
}

void CMyD3DApplication::ProcessInput()
{
  const float fSpeed = 0.5f;
  unsigned char m_bKey[256];
  ZeroMemory( m_bKey, 256 );
  GetKeyboardState(m_bKey);
  // Process keyboard input
  if(m_bKey['D'] & 128) m_Camera.AddToVelocity(D3DXVECTOR3(fSpeed, 0.0f, 0.0f)); // Slide Right
  if(m_bKey['A'] & 128) m_Camera.AddToVelocity(D3DXVECTOR3(-fSpeed, 0.0f, 0.0f));// Slide Left
  if(m_bKey['Q'] & 128) m_Camera.AddToVelocity(D3DXVECTOR3(0.0f, fSpeed, 0.0f)); // Slide Up
  if(m_bKey['Z'] & 128) m_Camera.AddToVelocity(D3DXVECTOR3(0.0f, -fSpeed, 0.0f));// Slide Down
  if(m_bKey['W'] & 128) m_Camera.AddToVelocity(D3DXVECTOR3(0.0f, 0.0f, fSpeed)); // Slide Foward
  if(m_bKey['S'] & 128) m_Camera.AddToVelocity(D3DXVECTOR3(0.0f, 0.0f, -fSpeed));// Slide Back
  if(m_bKey['L'] & 128) m_Camera.AddToYawPitchRoll(fSpeed, 0.0f, 0.0f);  // Turn Right
  if(m_bKey['J'] & 128) m_Camera.AddToYawPitchRoll(-fSpeed, 0.0f, 0.0f); // Turn Left
  if(m_bKey['K'] & 128) m_Camera.AddToYawPitchRoll(0.0f, fSpeed, 0.0f); // Turn Down
  if(m_bKey['I'] & 128) m_Camera.AddToYawPitchRoll(0.0f, -fSpeed, 0.0f);// Turn Up

  // mouse look
  DIMOUSESTATE2 dims2;
  m_InputManager.ReadMouse(dims2);

  // play with the divisor constants to change the mouselook sensitivity.
  // I've found that these values most accurately simulate my beloved Q3A setup. :)
  m_Camera.AddToYawPitchRoll((float)dims2.lX/0.8f, (float)dims2.lY/0.8f, 0.0f);
  
}

//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Called once per frame, the call is the entry point for 3d
//       rendering. This function sets up render states, clears the
//       viewport, and renders the scene.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::Render()
{
  
  // these are done here so that you can move the camera around during a freeze
  // frame, ala The Matrix

  //ProcessInput();
  m_Camera.Update(m_fElapsedTime);

  m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
  m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
  m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

  // Clear the backbuffer
  m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,
                       0x000000, 1.0f, 0L );
  
  if( SUCCEEDED( m_pd3dDevice->BeginScene() ) )
  {
    if (m_bShowGround) {
      // draw our quad
      m_pd3dDevice->SetTransform( D3DTS_VIEW, &m_Camera.GetViewMatrix());
      m_pd3dDevice->SetTexture( 0, m_Ground.GetTexture() );
      m_pd3dDevice->SetVertexShader( D3DFVF_XYZ_DIFFUSE_TEX1 );
      m_pd3dDevice->SetStreamSource( 0, m_Ground.GetVB(), sizeof(VERTEX_XYZ_DIFFUSE_TEX1) );
      m_pd3dDevice->SetIndices( m_Ground.GetIB(), 0L );
      m_pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 
        m_Ground.GetNumVerts(), 0, m_Ground.GetNumIndices()/3 );
    }

    // setup alpha blending states
    m_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
    m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
    
    // render the particle system
    m_PartSys.Render();

    char buf[256];
    _snprintf(buf, 256, "Particles: %d, FPS: %0.2f", 
      m_PartSys.GetNumActiveParticles(), m_fFPS);

    m_pFontSmall->DrawText( 2,  0, D3DCOLOR_ARGB(255,255,255,0), buf );

    // End the scene.
    m_pd3dDevice->EndScene();
  }

  return S_OK;
}

//-----------------------------------------------------------------------------
// Name: InitDeviceObjects()
// Desc: Initialize scene objects.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::InitDeviceObjects()
{
  m_pFont->InitDeviceObjects( m_pd3dDevice );
  m_pFontSmall->InitDeviceObjects( m_pd3dDevice );
  return S_OK;
}

//-----------------------------------------------------------------------------
// Name: RestoreDeviceObjects()
// Desc: Initialize scene objects.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::RestoreDeviceObjects()
{

  SetWindowLong( m_hWnd, GWL_STYLE, m_dwWindowStyle );

  static bool bBeenHere = false;
  m_InputManager.CreateDevices(m_hWnd, false, true);

  m_pFont->RestoreDeviceObjects();
  m_pFontSmall->RestoreDeviceObjects();
  m_Camera.SetPosition(D3DXVECTOR3(0.0f, 2.0f, -5.0f));

  // create the ground plane
  m_Ground.RestoreDeviceObjects(m_pd3dDevice, "Ch18p2_GroundTexture.png", 256.0f, 256.0f, 8);

  // initialize the particle system
  if (!bBeenHere) {
    // first time here, set initial system properties
    m_PartSys.SetSpawnDir1(D3DXVECTOR3(-2.0f, -2.0f, -2.0f));
    m_PartSys.SetSpawnDir2(D3DXVECTOR3( 2.0f, 2.0f,  2.0f));
    m_PartSys.SetStartColor1(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
    m_PartSys.SetStartColor2(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
    m_PartSys.SetEndColor1(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
    m_PartSys.SetEndColor2(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
    m_PartSys.SetPos(D3DXVECTOR3(0.0f, 2.0f, 0.0f));
    m_PartSys.SetLifetime(CMinMax<float>(0.0f, 10.0f));
    m_PartSys.SetEmitRate(CMinMax<float>(10.0f, 20.0f));
    m_PartSys.SetTexture("Ch18p2_ParticleTexture.png");
  }  

  bBeenHere = true;
  
  // initialize it!
  m_PartSys.RestoreDeviceObjects(m_pd3dDevice);

  // initialize the dialog
  {
    g_theSystem = &m_PartSys; // so dialog box can get at it.

    m_hWndPropDlg = CreateDialog(NULL, MAKEINTRESOURCE(IDD_PARTICLEPROPS), m_hWnd, 
      (DLGPROC) PropDlgProc); 

    ShowWindow(m_hWndPropDlg, SW_SHOW); 
  }

  // Set the world matrix
  D3DXMATRIX matWorld;
  D3DXMatrixIdentity( &matWorld );
  m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

  // Set projection matrix
  D3DXMATRIX matProj;
  FLOAT fAspect = ((FLOAT)m_d3dsdBackBuffer.Width) / m_d3dsdBackBuffer.Height;
  D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4, fAspect, 0.1f, 100.0f );
  m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );

  m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
  m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
  m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
  m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
  m_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE );
  m_pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );

  m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ONE );
  m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
  m_pd3dDevice->SetRenderState( D3DRS_LIGHTING,  FALSE );
  m_pd3dDevice->SetRenderState( D3DRS_CULLMODE,  D3DCULL_CCW );
  m_pd3dDevice->SetRenderState( D3DRS_SHADEMODE, D3DSHADE_FLAT );

  return S_OK;
}

//-----------------------------------------------------------------------------
// Name: InvalidateDeviceObjects()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::InvalidateDeviceObjects()
{
  DestroyWindow(m_hWndPropDlg);

  m_InputManager.DestroyDevices();

  m_pFont->InvalidateDeviceObjects();
  m_pFontSmall->InvalidateDeviceObjects();
  
  m_Ground.InvalidateDeviceObjects();
  m_PartSys.InvalidateDeviceObjects();
  return S_OK;
}

//-----------------------------------------------------------------------------
// Name: DeleteDeviceObjects()
// Desc: Called when the app is exiting, or the device is being changed,
//       this function deletes any device dependent objects.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::DeleteDeviceObjects()
{
  m_pFont->DeleteDeviceObjects();
  m_pFontSmall->DeleteDeviceObjects();
  
  m_Ground.DeleteDeviceObjects();
  return S_OK;
}

//-----------------------------------------------------------------------------
// Name: FinalCleanup()
// Desc: Called before the app exits, this function gives the app the chance
//       to cleanup after itself.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::FinalCleanup()
{
  SAFE_DELETE( m_pFont );
  SAFE_DELETE( m_pFontSmall );
  return S_OK;
}

//-----------------------------------------------------------------------------
// Name: ConfirmDevice()
// Desc: Called during device intialization, this code checks the device
//       for some minimum set of capabilities
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::ConfirmDevice( D3DCAPS8* pCaps, DWORD dwBehavior,
                                          D3DFORMAT Format )
{
  return S_OK;
}

//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: Message proc function to handle key and menu input
//-----------------------------------------------------------------------------
LRESULT CMyD3DApplication::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam,
                                    LPARAM lParam )
{
  // Pass remaining messages to default handler
  return CD3DApplication::MsgProc( hWnd, uMsg, wParam, lParam );
}