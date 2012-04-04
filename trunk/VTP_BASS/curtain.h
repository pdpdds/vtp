// '바람에 날리는 커텐' 데모 프로그램
// Copyright (c) 1999 Kim Seong Wan
// all rights reserved
// 이 프로그램의 소스는 개인적인 프로그래밍 공부를 목적으로만 사용하실 수 있고
// 이외의 다른 목적으로 사용할 수 없습니다. 다른 목적으로 사용하고자 할 경우는
// 반드시 저작권자인 김성완 (kaswan@hitel.net)에게 연락을 하셔서 허락을 받으셔야 합니다.  
#ifndef PARTICLES_H
#define PARTICLES_H

#define MAXPARTICLE	300

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

extern PARTICLE pa[];
extern int harray[];

void SetPhysicsWorld(void);
void ResetSimulation(void);
void SetCollisionBound(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax);
void CreateParticle(float mass, float x, float y, float z, float vx, float vy, float vz, int life);
void MoveParticle(PARTICLE * p,float fElapsedTime);
void UpdateLocalForce(void);
void DetectCollision(void);

#endif