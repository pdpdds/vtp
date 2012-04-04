#include "Ch18p2_ParticleEmitter.h"


const int CParticleEmitter::NUMBLENDINGMODES = 11;
const BLENDINGMODE CParticleEmitter::m_BlendingModes[11] = {
  { "D3DBLEND_ZERO", D3DBLEND_ZERO },
  { "D3DBLEND_ONE", D3DBLEND_ONE },
  { "D3DBLEND_SRCCOLOR", D3DBLEND_SRCCOLOR },
  { "D3DBLEND_INVSRCCOLOR", D3DBLEND_INVSRCCOLOR },
  { "D3DBLEND_SRCALPHA", D3DBLEND_SRCALPHA },
  { "D3DBLEND_INVSRCALPHA", D3DBLEND_INVSRCALPHA },
  { "D3DBLEND_DESTALPHA", D3DBLEND_DESTALPHA },
  { "D3DBLEND_INVDESTALPHA", D3DBLEND_INVDESTALPHA },
  { "D3DBLEND_DESTCOLOR", D3DBLEND_DESTCOLOR },
  { "D3DBLEND_INVDESTCOLOR", D3DBLEND_INVDESTCOLOR },
  { "D3DBLEND_SRCALPHASAT", D3DBLEND_SRCALPHASAT }
};

CParticleEmitter::CParticleEmitter()
{
  // default particle system configuration - boring!
  SetGravity(D3DXVECTOR3(0.0f,0.0f,0.0f));
  SetPos(D3DXVECTOR3(0.0f,0.0f,0.0f));
  SetEmitRate(CMinMax<float>(1.0f, 10.0f));
  SetSize(CMinMax<float>(20.0f, 100.0f));
  
  SetStartColor1(D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));
  SetStartColor2(D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));
  SetEndColor1(D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));
  SetEndColor2(D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));
  
  SetSpawnDir1(D3DXVECTOR3(-1.0f, -1.0f, -1.0f));
  SetSpawnDir2(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
  SetEmitRadius(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
  
  SetSrcBlendMode(D3DBLEND_ONE);
  SetDestBlendMode(D3DBLEND_ONE);
  
  // initialize misc. other things
  m_pd3dDevice = NULL;
  m_texParticle = NULL;
  SetVBSize(NUMPARTICLES / 10);

  m_fNumNewPartsExcess = 0.0f;
  m_iMaxParticles = NUMPARTICLES;

}

CParticleEmitter::~CParticleEmitter()
{
}

HRESULT CParticleEmitter::RestoreDeviceObjects(LPDIRECT3DDEVICE8 pDev)
{
  HRESULT hr;
  m_pd3dDevice = pDev;
  m_Particles.DeleteAll();

  // load up the particle system texture
  if (FAILED(hr = D3DXCreateTextureFromFile(m_pd3dDevice, 
    m_strTexFilename.c_str(), &m_texParticle))) {
    return(hr);
  }
  
  // create vertex buffer
  if(FAILED(hr = m_pd3dDevice->CreateVertexBuffer(m_iVBSize * sizeof(VERTEX_PARTICLE), 
    D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY | D3DUSAGE_POINTS, 
    D3DFVF_PARTICLE, D3DPOOL_DEFAULT, &m_vbParticles))) {
    return(hr);
  }
  return S_OK;	
}

void CParticleEmitter::InvalidateDeviceObjects()
{
  SAFE_RELEASE(m_texParticle);
  SAFE_RELEASE(m_vbParticles);
}

void CParticleEmitter::Update(float fElapsedTime, float fTimeDelta)
{
  // update existing particles
  {
    for (int q=0; q < m_Particles.GetTotalElements(); q++) {
      if (m_Particles.IsAlive(q)) {
        CParticle &part = m_Particles.GetAt(q);

        if (!part.Update(fTimeDelta)) {
          m_Particles.Delete(&part);
        }
        else {
          // apply gravity to this particle.
          part.m_vDir += fTimeDelta * m_vGravity;
        }
      } // is alive
    } // next particle
  }

  // create new particles
  {
    // determine the number of particles we need to create
    
    float fEmitRateThisFrame = m_fEmitRate.GetRandomNumInRange();
    int iNumNewParts = fEmitRateThisFrame * fTimeDelta;
    m_fNumNewPartsExcess += (float)(fEmitRateThisFrame * fTimeDelta)-iNumNewParts;
    
    if (m_fNumNewPartsExcess > 1.0f) {
      iNumNewParts += (int)m_fNumNewPartsExcess;
      m_fNumNewPartsExcess -= (int)m_fNumNewPartsExcess;
    }

    for (int q=0; q < iNumNewParts && m_Particles.GetNumUsedElements() < m_iMaxParticles; q++) {
      try {
        CParticle *part = m_Particles.New();

        part->m_fLifetime = m_fLifetime.GetRandomNumInRange();
        part->m_fSize = m_fSize.GetRandomNumInRange();
        
        // determine a random vector between dir1 and dir2
        float fRandX = RandomNumber(m_vSpawnDir1.x, m_vSpawnDir2.x);
        float fRandY = RandomNumber(m_vSpawnDir1.y, m_vSpawnDir2.y);
        float fRandZ = RandomNumber(m_vSpawnDir1.z, m_vSpawnDir2.z);

        part->m_vDir = D3DXVECTOR3(fRandX, fRandY, fRandZ);
        part->m_vPos = m_vPos;

        // pick a random vector between +/- emitradius
        fRandX = RandomNumber(-m_vEmitRadius.x, m_vEmitRadius.x);
        fRandY = RandomNumber(-m_vEmitRadius.y, m_vEmitRadius.y);
        fRandZ = RandomNumber(-m_vEmitRadius.z, m_vEmitRadius.z);
        part->m_vPos += D3DXVECTOR3(fRandX, fRandY, fRandZ);
        
        float fRandR = RandomNumber(m_StartColor1.r, m_StartColor2.r);
        float fRandG = RandomNumber(m_StartColor1.g, m_StartColor2.g);
        float fRandB = RandomNumber(m_StartColor1.b, m_StartColor2.b);
        float fRandA = RandomNumber(m_StartColor1.a, m_StartColor2.a);
        D3DXCOLOR startcolor(fRandR, fRandG, fRandB, fRandA);

        fRandR = RandomNumber(m_EndColor1.r, m_EndColor2.r);
        fRandG = RandomNumber(m_EndColor1.g, m_EndColor2.g);
        fRandB = RandomNumber(m_EndColor1.b, m_EndColor2.b);
        fRandA = RandomNumber(m_EndColor1.a, m_EndColor2.a);
        D3DXCOLOR endcolor(fRandR, fRandG, fRandB, fRandA);

        // figure out startcolor and colorstep based on start and end color.
        part->m_Color = startcolor;
        part->m_ColorStep = (endcolor-startcolor)/part->m_fLifetime;
        

      } catch(...) { q = iNumNewParts; }
    }
  }
}

HRESULT CParticleEmitter::Render()
{
  HRESULT hr;

  // Set the render states for using point sprites
  m_pd3dDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
  m_pd3dDevice->SetRenderState(D3DRS_POINTSCALEENABLE,  true);
  m_pd3dDevice->SetRenderState(D3DRS_POINTSIZE,     FtoDW(0.08f));
  m_pd3dDevice->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDW(m_fSize.m_Min));
  m_pd3dDevice->SetRenderState(D3DRS_POINTSIZE_MAX, FtoDW(m_fSize.m_Max));
  m_pd3dDevice->SetRenderState(D3DRS_POINTSCALE_A,  FtoDW(0.00f));
  m_pd3dDevice->SetRenderState(D3DRS_POINTSCALE_B,  FtoDW(0.00f));
  m_pd3dDevice->SetRenderState(D3DRS_POINTSCALE_C,  FtoDW(1.00f));
  
  m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, GetSrcBlendMode());
  m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, GetDestBlendMode());

  // Set up the vertex buffer to be rendered
  m_pd3dDevice->SetStreamSource( 0, m_vbParticles, sizeof(VERTEX_PARTICLE) );
  m_pd3dDevice->SetVertexShader( D3DFVF_PARTICLE );

  m_pd3dDevice->SetTexture(0, m_texParticle);
  
  VERTEX_PARTICLE *pVertices;
  DWORD dwNumParticlesToRender = 0;

  // Lock the vertex buffer.  We fill the vertex buffer in small
  // chunks, using D3DLOCK_NOOVERWRITE.  When we are done filling
  // each chunk, we call DrawPrim, and lock the next chunk.  When
  // we run out of space in the vertex buffer, we start over at
  // the beginning, using D3DLOCK_DISCARD.

  int iCurStart = 0;

  if(FAILED(hr = m_vbParticles->Lock(iCurStart * sizeof(VERTEX_PARTICLE), m_iVBSize * sizeof(VERTEX_PARTICLE),
    (BYTE **) &pVertices, iCurStart ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD)))
  {
	  return hr;
  }

  // Render each particle
  for (int q=0; q < NUMPARTICLES; q++) {
    
    // Render each particle a bunch of times to get a blurring effect
    if (m_Particles.IsAlive(q)) {
      CParticle &part = m_Particles.GetAt(q);
    
      pVertices->position = part.m_vPos;
      pVertices->pointsize = part.m_fSize;
      pVertices->color = (DWORD)part.m_Color;
      pVertices++;

      if( ++dwNumParticlesToRender == m_iVBSize ) {
        // Done filling this chunk of the vertex buffer.  Lets unlock and
        // draw this portion so we can begin filling the next chunk.

        m_vbParticles->Unlock();

        if(FAILED(hr = m_pd3dDevice->DrawPrimitive( 
          D3DPT_POINTLIST, iCurStart, dwNumParticlesToRender)))
	        return hr;

        // Lock the next chunk of the vertex buffer.  If we are at the 
        // end of the vertex buffer, DISCARD the vertex buffer and start
        // at the beginning.  Otherwise, specify NOOVERWRITE, so we can
        // continue filling the VB while the previous chunk is drawing.
        iCurStart = 0;

        if(FAILED(hr = m_vbParticles->Lock(0, m_iVBSize * sizeof(VERTEX_PARTICLE),
		      (BYTE **) &pVertices, D3DLOCK_DISCARD)))
          return hr;

        dwNumParticlesToRender = 0;
      }
    }
  }

  // Unlock the vertex buffer
  m_vbParticles->Unlock();

  // Render any remaining particles
  if( dwNumParticlesToRender )
  {
    if(FAILED(hr = m_pd3dDevice->DrawPrimitive( D3DPT_POINTLIST, iCurStart, dwNumParticlesToRender )))
		return hr;
  }

  // Reset render states
  m_pd3dDevice->SetRenderState( D3DRS_POINTSPRITEENABLE, FALSE );
  m_pd3dDevice->SetRenderState( D3DRS_POINTSCALEENABLE,  FALSE );

  return S_OK;


}

void CParticleEmitter::SetTexture(const char *strTexFilename)
{
  m_strTexFilename = strTexFilename;
  if (m_pd3dDevice) {
    SAFE_RELEASE(m_texParticle);
    if (FAILED(D3DXCreateTextureFromFile(m_pd3dDevice, 
    m_strTexFilename.c_str(), &m_texParticle))) {
    m_texParticle = NULL;
    }
  }
}

void CParticleEmitter::WriteProperty(FILE *file, const char *strName, D3DXCOLOR &value)
{
  fprintf(file, "%s=(%.4f, %.4f, %.4f, %.4f)\n", strName, value.r, value.g, value.b, value.a);
}

void CParticleEmitter::WriteProperty(FILE *file, const char *strName, D3DXVECTOR3 &value)
{
  fprintf(file, "%s=(%.4f, %.4f, %.4f)\n", strName, value.x, value.y, value.z);
}

void CParticleEmitter::WriteProperty(FILE *file, const char *strName, CMinMax<float> &value)
{
  fprintf(file, "%s=(%.4f, %.4f)\n", strName, value.m_Min, value.m_Max);
}

void CParticleEmitter::WriteProperty(FILE *file, const char *strName, int value)
{
  fprintf(file, "%s=%d\n", strName, value);
}

void CParticleEmitter::WriteProperty(FILE *file, const char *strName, const char *value)
{
  fprintf(file, "%s=%s\n", strName, value);
}

void CParticleEmitter::WriteBlendModeProperty(FILE *file, const char *strName, int value)
{
  for (int q=0; q < NUMBLENDINGMODES; q++) {
    if (m_BlendingModes[q].mode == value) {
      fprintf(file, "%s=%s\n", strName, m_BlendingModes[q].name);
      return;
    }
  }
  throw("Invalid Blending Mode.");
}

void CParticleEmitter::GetRightSideFromValueMap(std::map<std::string, std::string> &valuemap, 
                                   const char *strName, char *dest, int destsize)
{
  std::string str;
  if (valuemap.find(strName) == valuemap.end()) { 
    std::string errorstr;
    errorstr = std::string("Missing property \"") + std::string(strName) + std::string("\"!");
    throw(errorstr);
  }
  strncpy(dest, valuemap[strName].c_str(), destsize);
}

void CParticleEmitter::ReadProperty(std::map<std::string, std::string> &valuemap, 
                                   const char *strName, D3DXCOLOR &value)
{
  char buf[256]; char token[256]; float r,g,b,a;

  GetRightSideFromValueMap(valuemap, strName, buf, sizeof(buf));

  // color is in the form (R,G,B,A)
  PluckFirstField(buf, token, sizeof(token), "("); // skip first (
  PluckFirstField(buf, token, sizeof(token), ","); r = atof(token);
  PluckFirstField(buf, token, sizeof(token), ","); g = atof(token);
  PluckFirstField(buf, token, sizeof(token), ","); b = atof(token);
  PluckFirstField(buf, token, sizeof(token), ")"); a = atof(token);

  value = D3DXCOLOR(r,g,b,a);
}

void CParticleEmitter::ReadProperty(std::map<std::string, std::string> &valuemap, 
                                   const char *strName, D3DXVECTOR3 &value)
{
  char buf[256]; char token[256]; float x,y,z;

  GetRightSideFromValueMap(valuemap, strName, buf, sizeof(buf));

  // vector is in the form (x,y,z)
  PluckFirstField(buf, token, sizeof(token), "("); // skip first (
  PluckFirstField(buf, token, sizeof(token), ","); x = atof(token);
  PluckFirstField(buf, token, sizeof(token), ","); y = atof(token);
  PluckFirstField(buf, token, sizeof(token), ")"); z = atof(token);

  value = D3DXVECTOR3(x,y,z);
}

void CParticleEmitter::ReadProperty(std::map<std::string, std::string> &valuemap, 
                                   const char *strName, CMinMax<float> &value)
{
  char buf[256]; char token[256]; float minvalue,maxvalue;

  GetRightSideFromValueMap(valuemap, strName, buf, sizeof(buf));

  // minmax is in the form (min,max)
  PluckFirstField(buf, token, sizeof(token), "("); // skip first (
  PluckFirstField(buf, token, sizeof(token), ","); minvalue = atof(token);
  PluckFirstField(buf, token, sizeof(token), ")"); maxvalue = atof(token);

  value = CMinMax<float>(minvalue,maxvalue);
}

void CParticleEmitter::ReadProperty(std::map<std::string, std::string> &valuemap, 
                                   const char *strName, int &value)
{
  char buf[256];
  GetRightSideFromValueMap(valuemap, strName, buf, sizeof(buf));
  value = atoi(buf);
}

void CParticleEmitter::ReadProperty(std::map<std::string, std::string> &valuemap, 
                                   const char *strName, std::string &value)
{
  char buf[256];
  GetRightSideFromValueMap(valuemap, strName, buf, sizeof(buf));
  value = buf;
}

void CParticleEmitter::ReadBlendModeProperty(std::map<std::string, std::string> &valuemap, 
                                            const char *strName, int &value)
{
  char buf[256];
  GetRightSideFromValueMap(valuemap, strName, buf, sizeof(buf));
  
  for (int q=0; q < NUMBLENDINGMODES; q++) {
    if (!stricmp(m_BlendingModes[q].name, buf)) {
      value = m_BlendingModes[q].mode;
      return;
    }
  }
  throw("Invalid Blending Mode.");
}

bool CParticleEmitter::Save(const char *strFilename)
{
  try {
    FILE *file = fopen(strFilename, "wt");

    fprintf(file, "[ParticleSystem 1.00]\n");

    WriteProperty(file, "Position", m_vPos);
    WriteProperty(file, "Gravity", m_vGravity);
    WriteProperty(file, "SpawnDir1", m_vSpawnDir1);
    WriteProperty(file, "SpawnDir2", m_vSpawnDir2);

    WriteProperty(file, "StartColor1", m_StartColor1);
    WriteProperty(file, "StartColor2", m_StartColor2);
    WriteProperty(file, "EndColor1", m_EndColor1);
    WriteProperty(file, "EndColor2", m_EndColor2);

    WriteProperty(file, "EmitRate", m_fEmitRate);
    WriteProperty(file, "EmitRadius", m_vEmitRadius);
    WriteProperty(file, "Lifetime", m_fLifetime);
    WriteProperty(file, "Size", m_fSize);

    WriteBlendModeProperty(file, "SrcBlend", m_iSrcBlendMode);
    WriteBlendModeProperty(file, "DestBlend", m_iDestBlendMode);

    WriteProperty(file, "MaxParticles", m_iMaxParticles);
    WriteProperty(file, "Texture", m_strTexFilename.c_str());


    fclose(file);
  }
  catch(std::string e) {
    m_strLastError = e;
    return(false);
  }
  catch(...) {
    m_strLastError = "Unknown error.";
    return(false);
  }

  return(true);
}

bool CParticleEmitter::Load(const char *strFilename)
{
  FILE *file = fopen(strFilename, "rt");
  if (file == NULL) { throw("Can't open file! (invalid filename?)"); }
  try {
    char buf[256];
    char leftside[256];
    char rightside[256];
    std::map<std::string, std::string> valuemap;

    // read first line - should be [ParticleSystem 1.00]
    fgets(buf, sizeof(buf), file);
    if (strstr(buf, "[ParticleSystem 1.00]") == NULL) {
      throw("Invalid Header (missing \"[ParticleSystem 1.00]\")");
    }
    
    // read the entire contents of file into a map.  If we read
    // more than 100 lines, we assume something's wrong and we
    // bail out.

    int count=0;
    while (!feof(file) && count < 1000) {
      // read a line
      fgets(buf, sizeof(buf), file);
      
      // if there's something on that line,
      if (strlen(buf) >= 2) {

        // seperate that line into left and right side of = sign
        memset(leftside, 0, sizeof(leftside));
        memset(rightside, 0, sizeof(rightside));

        PluckFirstField(buf, leftside, sizeof(leftside), "=");
        PluckFirstField(buf, rightside, sizeof(rightside), "\n");
      
        // make sure the stuff to the left of the = sign isn't currently
        // in the map (if it is, throw an error).
        if (valuemap.find(leftside) != valuemap.end()) {
          throw("Duplicate value encountered.");
        }

        // insert left and right sides into map.
        valuemap.insert(std::make_pair(std::string(leftside), std::string(rightside)));
      }

      // increment safety count
      count++;
    }

    if (count == 1000) {
      throw("File too big (most likely not a particle system file)");
    }

    ReadProperty(valuemap, "Position", m_vPos);
    ReadProperty(valuemap, "Gravity", m_vGravity);
    ReadProperty(valuemap, "SpawnDir1", m_vSpawnDir1);
    ReadProperty(valuemap, "SpawnDir2", m_vSpawnDir2);

    ReadProperty(valuemap, "StartColor1", m_StartColor1);
    ReadProperty(valuemap, "StartColor2", m_StartColor2);
    ReadProperty(valuemap, "EndColor1", m_EndColor1);
    ReadProperty(valuemap, "EndColor2", m_EndColor2);

    ReadProperty(valuemap, "EmitRate", m_fEmitRate);
    ReadProperty(valuemap, "EmitRadius", m_vEmitRadius);
    ReadProperty(valuemap, "Lifetime", m_fLifetime);
    ReadProperty(valuemap, "Size", m_fSize);

    ReadBlendModeProperty(valuemap, "SrcBlend", m_iSrcBlendMode);
    ReadBlendModeProperty(valuemap, "DestBlend", m_iDestBlendMode);

    ReadProperty(valuemap, "MaxParticles", m_iMaxParticles);
    ReadProperty(valuemap, "Texture", m_strTexFilename);

    fclose(file);
  }
  catch(const char *e) {
    fclose(file);
    m_strLastError = e;
    return(false);
  }
  catch(...) {
    fclose(file);
    m_strLastError = "Unknown error.";
    return(false);
  }

  return(true);
}
  