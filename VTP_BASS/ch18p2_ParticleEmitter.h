#pragma warning(disable:4786)
#include <map>
#include <string>

#include "CommonFuncs.h"
#include "D3DHelperFuncs.h"
#include "Ch18p2_Particle.h"
#include "RecyclingArray.h"

#define NUMPARTICLES 2000

typedef struct 
{
    D3DXVECTOR3 position;
    float       pointsize;
    D3DCOLOR    color;
} VERTEX_PARTICLE;

#define D3DFVF_PARTICLE (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_PSIZE)

template <class T> class CMinMax
{
public:
  CMinMax() { m_Min = m_Max = 0; }
  CMinMax(T tMin, T tMax) { m_Min = tMin; m_Max = tMax; }
  ~CMinMax() { }

  T m_Min;
  T m_Max;

  T GetRandomNumInRange(void) { return(RandomNumber(m_Min, m_Max)); }
};

typedef class CMinMax<int> CMinMaxInt;
typedef class CMinMax<float> CMinMaxFloat;

typedef struct {
  char name[25];
  int mode;
} BLENDINGMODE;

class CParticleEmitter
{
public:
  static const int NUMBLENDINGMODES;
  static const BLENDINGMODE m_BlendingModes[11];

  std::string m_strLastError;

  CParticleEmitter();
  virtual ~CParticleEmitter();

  virtual void Update(float fElapsedTime, float fTimeDelta);

  virtual HRESULT Render();

  virtual HRESULT RestoreDeviceObjects(LPDIRECT3DDEVICE8 pDev);

  virtual void InvalidateDeviceObjects();

  // attributes
  D3DXVECTOR3 GetGravity(void) const { return(m_vGravity); }
  void SetGravity(const D3DXVECTOR3 &data) { m_vGravity = data; }

  D3DXVECTOR3 GetPos(void) const { return(m_vPos); }
  void SetPos(const D3DXVECTOR3 &data) { m_vPos = data; }

  CMinMax<float> GetEmitRate(void) const { return(m_fEmitRate); }
  void SetEmitRate(const CMinMax<float> data) { m_fEmitRate = data; }

  CMinMax<float> GetLifetime(void) const { return(m_fLifetime); }
  void SetLifetime(const CMinMax<float> data) { m_fLifetime = data; }

  CMinMax<float> GetSize(void) const { return(m_fSize); }
  void SetSize(const CMinMax<float> data) { m_fSize = data; }

  D3DXCOLOR GetStartColor1(void) const { return(m_StartColor1); }
  void SetStartColor1(const D3DXCOLOR &data) { m_StartColor1 = data; }

  D3DXCOLOR GetStartColor2(void) const { return(m_StartColor2); }
  void SetStartColor2(const D3DXCOLOR &data) { m_StartColor2 = data; }

  D3DXCOLOR GetEndColor1(void) const { return(m_EndColor1); }
  void SetEndColor1(const D3DXCOLOR &data) { m_EndColor1 = data; }

  D3DXCOLOR GetEndColor2(void) const { return(m_EndColor2); }
  void SetEndColor2(const D3DXCOLOR &data) { m_EndColor2 = data; }

  D3DXVECTOR3 GetSpawnDir1(void) const { return(m_vSpawnDir1); }
  void SetSpawnDir1(const D3DXVECTOR3 &data) { m_vSpawnDir1 = data; }

  D3DXVECTOR3 GetSpawnDir2(void) const { return(m_vSpawnDir2); }
  void SetSpawnDir2(const D3DXVECTOR3 &data) { m_vSpawnDir2 = data; }

  D3DXVECTOR3 GetEmitRadius(void) const { return(m_vEmitRadius); }
  void SetEmitRadius(const D3DXVECTOR3 &data) { m_vEmitRadius = data; }

  std::string GetTextureFilename(void) const { return(m_strTexFilename); }
  LPDIRECT3DTEXTURE8 GetTexture(void) const { return(m_texParticle); }
  void SetTexture(const char *strTexFilename);
 
  int GetVBSize(void) const { return(m_iVBSize); }
  void SetVBSize(const int data) { m_iVBSize = data; }

  int GetMaxParticles(void) const { return(m_iMaxParticles); }
  void SetMaxParticles(const int data) { m_iMaxParticles = data; }
  
  int GetSrcBlendMode(void) const { return(m_iSrcBlendMode); }
  void SetSrcBlendMode(const int data) { m_iSrcBlendMode = data; }

  int GetDestBlendMode(void) const { return(m_iDestBlendMode); }
  void SetDestBlendMode(const int data) { m_iDestBlendMode = data; }

  int GetNumActiveParticles(void) { return(m_Particles.GetNumUsedElements()); }

  void Reset(void) { m_Particles.DeleteAll(); }

  bool Save(const char *strFilename);
  bool Load(const char *strFilename);
  
private:

  void WriteProperty(FILE *file, const char *strName, D3DXCOLOR &value);
  void WriteProperty(FILE *file, const char *strName, D3DXVECTOR3 &value);
  void WriteProperty(FILE *file, const char *strName, CMinMax<float> &value);
  void WriteProperty(FILE *file, const char *strName, int value);
  void WriteProperty(FILE *file, const char *strName, const char *value);
  void WriteBlendModeProperty(FILE *file, const char *strName, int value);

  void GetRightSideFromValueMap(std::map<std::string, std::string> &valuemap, 
                                const char *strName, char *dest, int destsize);

  void ReadProperty(std::map<std::string, std::string> &valuemap, const char *strName, D3DXCOLOR &value);
  void ReadProperty(std::map<std::string, std::string> &valuemap, const char *strName, D3DXVECTOR3 &value);
  void ReadProperty(std::map<std::string, std::string> &valuemap, const char *strName, CMinMax<float> &value);
  void ReadProperty(std::map<std::string, std::string> &valuemap, const char *strName, int &value);
  void ReadProperty(std::map<std::string, std::string> &valuemap, const char *strName, std::string &value);
  void ReadBlendModeProperty(std::map<std::string, std::string> &valuemap, const char *strName, int &value);

  // particle system attributes
  D3DXVECTOR3 m_vGravity;
  D3DXVECTOR3 m_vPos; // position of emitter
  
  // these vars determine starting values for each particle
  CMinMax<float> m_fEmitRate; // in particles/sec
  CMinMax<float> m_fLifetime; // in particles/sec
  CMinMax<float> m_fSize; // in particles/sec
  
  D3DXCOLOR m_StartColor1;
  D3DXCOLOR m_StartColor2;
  D3DXCOLOR m_EndColor1;
  D3DXCOLOR m_EndColor2;
  
  D3DXVECTOR3 m_vSpawnDir1;
  D3DXVECTOR3 m_vSpawnDir2;

  D3DXVECTOR3 m_vEmitRadius;

  // other stuff
  int m_iDestBlendMode;
  int m_iSrcBlendMode;
  
  int m_iMaxParticles;

  int m_iVBSize; // size of the vertex buffer (can and should be less than NUMPARTICLES)
  LPDIRECT3DDEVICE8 m_pd3dDevice;
  LPDIRECT3DVERTEXBUFFER8 m_vbParticles;
  
  std::string m_strTexFilename;
  LPDIRECT3DTEXTURE8 m_texParticle;

  float m_fNumNewPartsExcess;

  CRecyclingArray<CParticle, NUMPARTICLES> m_Particles;
};

