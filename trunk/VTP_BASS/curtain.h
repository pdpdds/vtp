// '�ٶ��� ������ Ŀ��' ���� ���α׷�
// Copyright (c) 1999 Kim Seong Wan
// all rights reserved
// �� ���α׷��� �ҽ��� �������� ���α׷��� ���θ� �������θ� ����Ͻ� �� �ְ�
// �̿��� �ٸ� �������� ����� �� �����ϴ�. �ٸ� �������� ����ϰ��� �� ����
// �ݵ�� ���۱����� �輺�� (kaswan@hitel.net)���� ������ �ϼż� ����� �����ž� �մϴ�.  
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