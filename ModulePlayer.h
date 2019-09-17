#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

struct PhysVehicle3D;

enum PLAYER {
	PLAYER1 = 0,
	PLAYER2
};

#define MAX_ACCELERATION 1000.0f
#define TURN_DEGREES 15.0f * DEGTORAD
#define BRAKE_POWER 1000.0f

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start(int x, int y, int z, float angle, PLAYER player, Color color);
	update_status Update(float dt);
	bool Draw();
	bool CleanUp();

public:

	vec3 initialPosition;

	Sphere sphere;
	PhysBody3D* ball;

	Cylinder cylinder;
	PhysBody3D* cable;

	Triangle arrow;

	vec3 position;
	PhysVehicle3D* vehicle;
	float turn;
	float acceleration;
	float brake;
	PLAYER player;
	bool boost = false;
	bool boosting = false;

	Timer timer;
	Timer timer2;
};