#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"
#include "ModulePlayer.h"
#include "PhysBody3D.h"
#include "Timer.h"

#define MAX_SNAKE 2

struct PhysBody3D;
struct PhysMotor3D;

struct Boost {
	~Boost();

	PhysBody3D* pbody;
	Cylinder* cylinder;
	bool active;
	Timer time;
};

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool Draw();
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

	void resetLevel();

	void startRound();

	void createMap();
	void PutSensors();

	void createBoost(vec3 pos, float radius, float height);

	void RotateBody(PhysVehicle3D * vehicle);

	void Score();

	void TimeLeft();

	void RestartPositions();

public:
	
	Sphere ball;
	PhysBody3D* pb_ball;
	PhysBody3D* goal_player1;
	PhysBody3D* goal_player2;

	p2List<Boost*> boosts;

	ModulePlayer* player1;
	ModulePlayer* player2;

	p2List<Primitive*> map;
	p2List<Primitive*> sensors;

	p2List<Cylinder*> cylinders_list1;
	p2List<Cylinder*> cylinders_list2;
	p2List<Cylinder*> time_list;

	Timer timer;
	Timer time_left;
	Timer game;
	Uint32 time = 0;
	Uint32 time_remaining;
	Uint32 time_spend = 0;

	uint interval = 60000;
	uint counter = interval;

	bool goal = false;
	bool endGame = false;
	bool start = false;

private:
	uint score1 = 0;
	uint score2 = 0;
	int second = 0;

};
