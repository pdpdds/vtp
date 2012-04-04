#include ".\soundmanager.h"

CSoundManager::CSoundManager(void)
{
	specmode=0;
	specpos=0;
}

CSoundManager::~CSoundManager(void)
{
}

BOOL CSoundManager::Initialize(void)
{
	if (BASS_GetVersion()!=MAKELONG(2,2)) 
	{
		MessageBox(0,L"BASS version 2.2 was not loaded",L"Incorrect BASS.DLL",0);
		return false;   
	}

	BASS_SetConfig(BASS_CONFIG_FLOATDSP,TRUE);
			
	// initialize - default device, 44100hz, stereo, 16 bits			
	if (!BASS_Init(1,44100,0,NULL,NULL)) {				
		return false;
			
	}
			// check for floating-point capability			
	floatable=BASS_StreamCreate(44100,2,BASS_SAMPLE_FLOAT,NULL,0);		
	if (floatable) 
	{ // woohoo!				
		BASS_StreamFree(floatable);				
		floatable=BASS_SAMPLE_FLOAT;			
	}

	return true;
}



bool CSoundManager::Play(void)
{
	BASS_ChannelPlay(chan,FALSE);
	return true;
}
BOOL CSoundManager::FileLoad(char *str)
{	
	BASS_CHANNELINFO info;
 	if(!(chan=BASS_StreamCreateFile(FALSE,str,0,0,floatable))											
 		&& !(chan=BASS_MusicLoad(FALSE,str,0,0,BASS_MUSIC_RAMPS|floatable|BASS_MUSIC_PRESCAN,0))) //BASS_MUSIC_LOOP|
	{								
			// whatever it is, it ain't playable											
			return false;													
	}

	BASS_ChannelGetInfo(chan,&info);					
	if (info.chans!=2) { // only stereo is allowed																			
		BASS_MusicFree(chan);										
		BASS_StreamFree(chan);			
		return false;
	}									
	return true;
}

void CSoundManager::Delete(void)
{
	BASS_MusicFree(chan);							
	BASS_StreamFree(chan);	
}

void CSoundManager::GetVolume(void)
{
	m_volume = BASS_GetVolume();
}

void CSoundManager::SetVolumeDown(void)
{
	m_volume--;		
	if(m_volume<=0)			
		m_volume=0;		
	BASS_SetVolume(m_volume);  
}

void CSoundManager::SetVolumeUp(void)
{
	m_volume++;	
	if(m_volume>100)	
		m_volume = 99;				
	BASS_SetVolume(m_volume);    
}

void CSoundManager::GetCurrentTimea(void)
{
	QWORD len=BASS_ChannelGetPosition(chan);									
	DWORD time=BASS_ChannelBytes2Seconds(chan,len);		
	char text[10];	
	sprintf(text,"%u:%02u",time/60,time%60);
}

void CSoundManager::SetEffect(int nEffectType)
{	
	switch (nEffectType)
	{	
	case EFFECT_FLANGER:                        												
		memset(flabuf,0,sizeof(flabuf));										
		flapos=0;								
		flas=FLABUFLEN/2;				
		flasinc=0.002f;				
		fladsp=BASS_ChannelSetDSP(chan,&Flange,0,0);			
		break;		

	case UNEFFECT_FLANGER:                        																			
		BASS_ChannelRemoveDSP(chan,fladsp);            
		break;
					
	case EFFECT_ROTATE:                        					
		rotpos=0.7853981f;						
		rotdsp=BASS_ChannelSetDSP(chan,&Rotate,0,2);				
		break;

		
	case UNEFFECT_ROTATE:                        												
		BASS_ChannelRemoveDSP(chan,rotdsp);			
		break;        
					
	case EFFECT_ECHO:                        							
		memset(echbuf,0,sizeof(echbuf));						
		echpos=0;						
		echdsp=BASS_ChannelSetDSP(chan,&Echo,0,1);					
		break;        
		
	case UNEFFECT_ECHO:                        									
		BASS_ChannelRemoveDSP(chan,echdsp);								
		break;
	}				
}


/*void UpdateSpectrum()
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
		
		for (x=0;x<BANDS;x++) {
			float sum=0;
			int sc,jk;
			jk=x*10.0/(BANDS-1);
			int b1=pow(2,jk);
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

}*/
