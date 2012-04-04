// '바람에 날리는 커텐' 데모 프로그램
// Copyright (c) 1999 Kim Seong Wan
// all rights reserved
// 이 프로그램의 소스는 개인적인 프로그래밍 공부를 목적으로만 사용하실 수 있고
// 이외의 다른 목적으로 사용할 수 없습니다. 다른 목적으로 사용하고자 할 경우는
// 반드시 저작권자인 김성완 (kaswan@hitel.net)에게 연락을 하셔서 허락을 받으셔야 합니다.  

#include "stdafx.h"
#include <math.h>

#ifndef MAXPARTICLE
#define MAXPARTICLE	1000
#endif
#define DELTA_TIME	0.2f
#define AFC			0.003f

#ifndef PI
#define PI 3.1415926535
#endif

typedef struct tagFORCE {
	float fx, fy, fz;
} FORCE;

typedef struct tagPARTICLE {
	int life;
	float x, y, z;
	float vx, vy, vz;
	float ax, ay, az;
	float mass;
	FORCE localforce;
	int cf;
} PARTICLE;

struct {
	float xmin, xmax;
	float ymin, ymax;
	float zmin, zmax;
} Bounding;

float delta_time = DELTA_TIME;
float AirFriction = AFC;

PARTICLE pa[MAXPARTICLE];
FORCE GlobalForce;
FORCE springforce[16*16];
int harray[100];


void SetPhysicsWorld(void);
void ResetSimulation(void);
void SetCollisionBound(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax);
void CreateParticle(float mass, float x, float y, float z, float vx, float vy, float vz, int life);
void MoveParticle(PARTICLE * p);
void UpdateLocalForce(void);
void DetectCollision(void);

void SetPhysicsWorld(void)
{
	GlobalForce.fy = -0.2f;

	for ( int i = 0; i <= 50; i += 1)
	{
		harray[i] = (int)(-cos((float)i * 10.0 * PI / 180.0) * 100 + 350);
	}

	SetCollisionBound(0, 640, -113, 112, -225, 0);
}

void ResetSimulation(void)
{
	int i, j;

	for ( i = 0; i < 16*16; i++)
		pa[i].life = 0;

	for ( j = 0; j < 16; j++)
	{
		for ( i = 0; i < 16; i++)
			if ( i == 0 )
				CreateParticle(0.5, i*15 - 112, 112-(j*15)  , 0, 10, 0, 0, 10000);
			else
				CreateParticle(0.5, i*15 - 112, 112-(j*15), 0, 0, 0, 0, 10000);
	}
}	

void SetCollisionBound(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax)
{
	Bounding.xmin = xmin;
	Bounding.xmax = xmax;
	Bounding.ymin = ymin;
	Bounding.ymax = ymax;
	Bounding.zmin = zmin;
	Bounding.zmax = zmax;
}

int GetHeight(int x)
{
	int i = x / 15;
	int r = x % 15;

	if (i >= 0)
	{ 
		return (int)((float)(15 - r)/15.0f * (float)harray[i] + (float)r / 15.0f * (float)harray[i + 1]);
	}
	else return 480;
}

void CreateParticle(float mass, float x, float y, float z, float vx, float vy, float vz, int life)
{
	for ( int i = 0; i < MAXPARTICLE; i ++ )
	{
		if ( pa[i].life < 1 )
		{
			pa[i].life = life;
			pa[i].mass = mass;
			
			pa[i].x = x;
			pa[i].y = y;
			pa[i].z = z;

			pa[i].vx = vx;
			pa[i].vy = vy;
			pa[i].vz = vz;
			
			return;
		}
	}
}

void MoveParticle(PARTICLE * p,float fElapsedTime)
{
	static int count = 100;
	//delta_time = DELTA_TIME*fElapsedTime*;
	if ( p->life > 0 ) 
	{	
		//p->life--;

		p->ax = (GlobalForce.fx + p->localforce.fx) / p->mass;
		p->ay = (GlobalForce.fy + p->localforce.fy) / p->mass;
		p->az = (GlobalForce.fz + p->localforce.fz) / p->mass;

		p->vx += p->ax * delta_time;
		p->vy += p->ay * delta_time;
		p->vz += p->az * delta_time;

		p->x += p->vx * delta_time;
		p->y += p->vy * delta_time;
		p->z += p->vz * delta_time;
	}
	
	// simulatae wind
	if ( count == 0 )
	{
		GlobalForce.fz = -3-rand()%3;
		count = 12;
	}
	else GlobalForce.fz = 0;
	count--;
}

void UpdateLocalForce()
{
	float length;
    
	for ( int i = 0; i < 16; i ++ )
	{
		for ( int j = 0; j < 16; j ++ )
		if ( pa[i*16+j].life > 0 )
		{
			int index = i * 16 + j;

			FORCE up, down, left, right;
			
			up.fx = down.fx = left.fx = right.fx = 0; 
			up.fy = down.fy = left.fy = right.fy = 0; 
			up.fz = down.fz = left.fz = right.fz = 0; 

			if ( j-1 >= 0 )
			{
				up.fx = pa[i * 16 + (j-1)].x - pa[i * 16 + j].x; 
				up.fy = pa[i * 16 + (j-1)].y - pa[i * 16 + j].y; 
				up.fz = pa[i * 16 + (j-1)].z - pa[i * 16 + j].z;
				
				length = sqrt(up.fx * up.fx + up.fy * up.fy + up.fz * up.fz);

				up.fx = (length - 15) * up.fx / length;
				up.fy = (length - 15) * up.fy / length;
				up.fz = (length - 15) * up.fz / length;
			}
			
			if ( j+1 < 16 )
			{
				down.fx = pa[i * 16 + j+1].x - pa[i * 16 + j].x; 
				down.fy = pa[i * 16 + j+1].y - pa[i * 16 + j].y; 
				down.fz = pa[i * 16 + j+1].z - pa[i * 16 + j].z; 

				length = sqrt(down.fx * down.fx + down.fy * down.fy + down.fz * down.fz);
				
				down.fx = (length - 15) * down.fx / length;
				down.fy = (length - 15) * down.fy / length;
				down.fz = (length - 15) * down.fz / length;
				
			}			
			
			if ( i-1 >= 0 )
			{
				left.fx = pa[(i-1) * 16 + j].x - pa[i * 16 + j].x; 
				left.fy = pa[(i-1) * 16 + j].y - pa[i * 16 + j].y; 
				left.fz = pa[(i-1) * 16 + j].z - pa[i * 16 + j].z; 

				length = sqrt(left.fx * left.fx + left.fy * left.fy + left.fz * left.fz);
				
				left.fx = (length - 15) * left.fx / length;
				left.fy = (length - 15) * left.fy / length;
				left.fz = (length - 15) * left.fz / length;
				
			}

			if ( i+1 < 16 )
			{
				right.fx = pa[(i+1) * 16 + j].x - pa[i * 16 + j].x; 
				right.fy = pa[(i+1) * 16 + j].y - pa[i * 16 + j].y; 
				right.fz = pa[(i+1) * 16 + j].z - pa[i * 16 + j].z; 

				length = sqrt(right.fx * right.fx + right.fy * right.fy + right.fz * right.fz);

				right.fx = (length - 15) * right.fx / length;
				right.fy = (length - 15) * right.fy / length;
				right.fz = (length - 15) * right.fz / length;
				
			}
			
			springforce[index].fx = up.fx + down.fx + left.fx + right.fx; 
			springforce[index].fy = up.fy + down.fy + left.fy + right.fy; 
			springforce[index].fz = up.fz + down.fz + left.fz + right.fz; 

			pa[index].localforce.fx = -pa[index].vx * AirFriction + springforce[index].fx;
			pa[index].localforce.fy = -pa[index].vy * AirFriction + springforce[index].fy;
			pa[index].localforce.fz = -pa[index].vz * AirFriction + springforce[index].fz;
		}
	}
}

void DetectCollision(void)
{
	for ( int i = 0; i < MAXPARTICLE; i ++ )
	{
		if ( pa[i].life > 0)
		{
			if ( pa[i].x <= Bounding.xmin || pa[i].x >= Bounding.xmax )
			{	
				pa[i].vx = -pa[i].vx * 0.7f;
				//continue;
				//pa[i].cf = 2;
			}

			if ( pa[i].y <= Bounding.ymin || pa[i].y >= Bounding.ymax )
			{
				float oldz = pa[i].z - pa[i].vz * delta_time;
				float oldy = pa[i].y - pa[i].vy * delta_time;
				
				float dt_before = delta_time * (Bounding.ymax - oldy)/(pa[i].y - oldy);
				float dt_after = delta_time * (pa[i].y - Bounding.ymax)/(pa[i].y - oldy);
				
				pa[i].z = oldz - pa[i].vz * dt_before;
				pa[i].y = oldy -pa[i].vy * dt_before;
				
				pa[i].vy = -pa[i].vy * 0.6f;
				
				pa[i].z += pa[i].vz * dt_after;
				pa[i].y += pa[i].vy * dt_after;
				
				//continue;
				//pa[i].cf = 2;
			}

			if ( pa[i].z <= Bounding.zmin || pa[i].z >= Bounding.zmax )
			{
				pa[i].vz = -pa[i].vz * 0.6f;
				//continue;
				//pa[i].cf = 2;
			}
/*
			if ( GetHeight((int)pa[i].x) <= (int)pa[i].y )
			{
				float nx = (float)GetHeight((int)pa[i].x) - (float)GetHeight((int)pa[i].x + 5);
				float ny = 5.0f;
				
				float px = 5.0f;
				float py = (float)GetHeight((int)pa[i].x + 5) - (float)GetHeight((int)pa[i].x);
				
				float d = (float)sqrt(nx * nx + ny * ny);
				
				float sine = -py / d;
				float cosine = px / d;
				
				nx = nx / d * 0.7f;
				ny = ny / d * 0.7f;
				
				px = px / d;
				py = py / d;
				
				float vn = -(nx * pa[i].vx + ny * pa[i].vy);
				float vp = (px * pa[i].vx + py * pa[i].vy);
				
				//-----------------------------------
				float oldx = pa[i].x - pa[i].vx * delta_time;
				float oldy = pa[i].y - pa[i].vy * delta_time;
				
				float dh_before = abs(oldy - (float)GetHeight((int)oldx));
				float dh_after = abs(pa[i].y - (float)GetHeight((int)pa[i].x));

				float dt_before = delta_time * dh_before / (dh_before + dh_after);
				float dt_after = delta_time * dh_after / (dh_before + dh_after);

				pa[i].x = oldx + pa[i].vx * dt_before;
				pa[i].y = oldy + pa[i].vy * dt_before;

				pa[i].vx = (vp * cosine + vn * sine);
				pa[i].vy = (-vp * sine + vn * cosine);

				pa[i].x += pa[i].vx * dt_after;
				pa[i].y += pa[i].vy * dt_after;

				//pa[i].cf = 5;
			}
*/
		}
		

		pa[i].cf--;
		if ( pa[i].cf < 0 ) pa[i].cf = 0;
	}
}