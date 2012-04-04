
#pragma once

class CSoundManager
{
public:
	CSoundManager(void);
	~CSoundManager(void);
	BOOL Initialize(void);
	bool Play(void);
	
private:
// BASS!!
	
	DWORD m_volume;
	DWORD floatable; // floating-point channel support?
	DWORD chan;	// the channel... HMUSIC or HSTREAM	
	int specmode;
	int specpos; // spectrum mode (and marker pos for 2nd mode)	
	BYTE specbuf[SPECWIDTH*SPECHEIGHT];
	BYTE specbuf2[SPECWIDTH];
	float specbuf2timeset[SPECWIDTH*2];
		
public:
	void SetVolumeDown(void);
	void SetVolumeUp(void);
	void SetEffect(int nEffectType);	


	
	BOOL FileLoad(char *);
	void Delete(void);
	void GetVolume(void);
	void GetCurrentTimea(void);

	//void CALLBACK Echo(HDSP handle, DWORD channel, void *buffer, DWORD length, DWORD user);
	//void CALLBACK Rotate(HDSP handle, DWORD channel, void *buffer, DWORD length, DWORD user);
	//void CALLBACK Flange(HDSP handle, DWORD channel, void *buffer, DWORD length, DWORD user);
};



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