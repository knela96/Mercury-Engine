#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "Color.h"
#include "PhysVehicle3D.h"
#include "ModulePlayer.h"
#include "Color.h"
#include "PhysBody3D.h"
#include "ModulePhysics3D.h"
#include "cmath"
#include "ModuleRenderer3D.h"

//#include <iostream>
#include "glut/glut.h"
#include <string>


ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->audio->PlayMusic("Music/song.ogg");
	App->audio->ChangeMusicVolume(0.6f);
	App->audio->LoadFx("FX/car2.wav");
	App->audio->LoadFx("FX/car3.wav");
	App->audio->LoadFx("FX/car_idle1.wav");
	App->audio->LoadFx("FX/car_idle2.wav");
	App->audio->LoadFx("FX/car_back1.wav");
	App->audio->LoadFx("FX/car_back2.wav");
	App->audio->LoadFx("FX/powerup.wav");
	App->audio->LoadFx("FX/jump.wav");
	App->audio->LoadFx("FX/boost.wav");
	App->audio->LoadFx("FX/tick.wav");
	App->audio->LoadFx("FX/go.wav");
	App->audio->LoadFx("FX/whistle.wav");
	App->audio->LoadFx("FX/goal.wav");

	player1 = new ModulePlayer(App, true);
	player2 = new ModulePlayer(App, true);

	player1->Start(0, 0, 95, 3.14, PLAYER1, Orange);

	player2->Start(0, 0, -95, 0, PLAYER2, Blue);

	player1->vehicle->SetVelocityZero();
	player2->vehicle->SetVelocityZero();

	createMap();
	PutSensors();
	Score();
	TimeLeft();

	ball.radius = 2;
	ball.color = White;

	pb_ball = App->physics->AddBody(ball, 0.01f);
	pb_ball->GetTransform(&ball.transform);
	pb_ball->SetPos(0, 2, 0);

	game.Start();
	endGame = false;
	start = true;

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	pb_ball = nullptr;
	goal_player1 = nullptr;
	goal_player2 = nullptr;

	for (p2List_item<Boost*>* item = boosts.getFirst(); item; item = item->next)
		delete item->data;
	boosts.clear();

	for (p2List_item<Primitive*>* item = map.getFirst(); item; item = item->next)
		delete item->data;
	map.clear();

	for (p2List_item<Cylinder*>* item = cylinders_list1.getFirst(); item; item = item->next)
		delete item->data;
	cylinders_list1.clear();

	for (p2List_item<Cylinder*>* item = cylinders_list2.getFirst(); item; item = item->next)
		delete item->data;
	cylinders_list2.clear();

	for (p2List_item<Cylinder*>* item = time_list.getFirst(); item; item = item->next)
		delete item->data;
	time_list.clear();

	for (p2List_item<Primitive*>* item = sensors.getFirst(); item; item = item->next)
		delete item->data;
	sensors.clear();

	player1->CleanUp();
	player2->CleanUp();

	delete player1;
	delete player2;

	


	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	if (!endGame) {
		time_remaining = game.Read();
		LOG("%i", time_remaining);
		if (time_remaining >= interval && time_remaining != 0)
		{
			p2List_item<Cylinder*>* item = time_list.getFirst();
			for (int i = 0; i < time_list.count() && item->next != nullptr; item = item->next) {
				if (item->data->color.IsBlack())
				{
					item->data->color = Red;
					break;
				}
				else if (item->next->next == nullptr)
				{
					item->next->data->color = Red;
					endGame = true;
					interval = counter;
					App->audio->PlayFx(WHISTLE);
					time_left.Start();
				}

			}
			interval += counter;
		}
	}

	player1->Update(dt);
	player2->Update(dt);

	pb_ball->GetTransform(&ball.transform);

	App->camera->Position.x = player1->vehicle->vehicle->getChassisWorldTransform().getOrigin().getX() - 10 * player1->vehicle->vehicle->getForwardVector().getX();
	App->camera->Position.y = player1->vehicle->vehicle->getChassisWorldTransform().getOrigin().getY() + 5 * player1->vehicle->vehicle->getUpAxis();
	App->camera->Position.z = player1->vehicle->vehicle->getChassisWorldTransform().getOrigin().getZ() - 10 * player1->vehicle->vehicle->getForwardVector().getZ();

	float player1_x = player1->vehicle->vehicle->getChassisWorldTransform().getOrigin().getX() + 10 * player1->vehicle->vehicle->getForwardVector().getX();
	float player1_z = player1->vehicle->vehicle->getChassisWorldTransform().getOrigin().getZ() + 10 * player1->vehicle->vehicle->getForwardVector().getZ();

	App->camera->LookAt(vec3(player1_x, 1, player1_z));
	
	App->camera2->Position.x = player2->vehicle->vehicle->getChassisWorldTransform().getOrigin().getX() - 10 * player2->vehicle->vehicle->getForwardVector().getX();
	App->camera2->Position.y = player2->vehicle->vehicle->getChassisWorldTransform().getOrigin().getY() + 5 * player2->vehicle->vehicle->getUpAxis();
	App->camera2->Position.z = player2->vehicle->vehicle->getChassisWorldTransform().getOrigin().getZ() - 10 * player2->vehicle->vehicle->getForwardVector().getZ();

	float player2_x = player2->vehicle->vehicle->getChassisWorldTransform().getOrigin().getX() + 10 * player2->vehicle->vehicle->getForwardVector().getX();
	float player2_z = player2->vehicle->vehicle->getChassisWorldTransform().getOrigin().getZ() + 10 * player2->vehicle->vehicle->getForwardVector().getZ();

	App->camera2->LookAt(vec3(player2_x, 1, player2_z));

	p2List_item<Boost*>* item;
	for (item = boosts.getFirst(); item != nullptr; item = item->next) {
		if (!item->data->active) {
			if (item->data->time.Read() > 3000) {
				item->data->active = true;
				item->data->cylinder->color = Orange;
				item->data->time.Stop();
			}
		}
	}
	
	if (endGame && !start)
		resetLevel();
	else if(!endGame && start)
		startRound();
	
	char title[80];
	sprintf_s(title, "Radio Control League");
	App->window->SetTitle(title);

	return UPDATE_CONTINUE;
}

bool ModuleSceneIntro::Draw() {
	bool ret = true;
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	ret = player1->Draw();
	ret = player2->Draw();
	//player1->arrow.Render();//If Drawn inside player on the second player duplicates the arrow
	//player2->arrow.Render();

	ball.Render();

	p2List_item<Primitive*>* item = map.getFirst();
	for (int i = 0; i < map.count() && item != nullptr; item = item->next) {
		item->data->Render();
	}

	p2List_item<Cylinder*>* item2 = cylinders_list1.getFirst();
	for (int i = 0; i < cylinders_list1.count() && item2 != nullptr; item2 = item2->next) {
		item2->data->Render();
	}

	p2List_item<Cylinder*>* item3 = cylinders_list2.getFirst();
	for (int i = 0; i < cylinders_list2.count() && item3 != nullptr; item3 = item3->next) {
		item3->data->Render();
	}

	p2List_item<Cylinder*>* item4 = time_list.getFirst();
	for (int i = 0; i < time_list.count() && item4 != nullptr; item4 = item4->next) {
		item4->data->Render();
	}

	p2List_item<Boost*>* boost;
	for (boost = boosts.getFirst(); boost != nullptr; boost = boost->next) {
		boost->data->cylinder->Render();
	}

	return ret;
}


void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	time = timer.Read();
	if (time >= 1000 && !endGame)
	{
		if (body1 == goal_player1)
		{
			if (body2 == pb_ball)
			{
				if (score2 < 4) {
					App->audio->PlayFx(GOAL);
					RestartPositions();
					start = true;
				}

				p2List_item<Cylinder*>* item = cylinders_list1.getLast();
				for (int i = cylinders_list1.count() - 1; i >= 0 && item->prev != nullptr; item = item->prev) {
					if (item->data->color.IsWhite())
					{
						item->data->color = Green;
						timer.Start();
						break;
					}
					else if(item->prev->prev == nullptr)
					{
						App->audio->PlayFx(WHISTLE);
						App->audio->PlayFx(GOAL);
						item->prev->data->color = Green;
						endGame = true;
						time_left.Start();
					}
					score2++;
				}
			}
		}
		else if (body1 == goal_player2)
		{
			if (body2 == pb_ball)
			{
				if (score1 < 4) {
					App->audio->PlayFx(GOAL);
					RestartPositions();
					start = true;
				}

				p2List_item<Cylinder*>* item = cylinders_list2.getFirst();
				for (int i = 0; i < cylinders_list2.count() && item != nullptr; item = item->next) {
					if (item->data->color.IsWhite())
					{
						item->data->color = Green;
						timer.Start();
						break;
					}
					else if (item->next->next == nullptr)
					{
						App->audio->PlayFx(WHISTLE);
						App->audio->PlayFx(GOAL);
						item->data->color = Green;
						endGame = true;
						time_left.Start();
					}
					score1++;
				}
			}
		}
	}

	p2List_item<Boost*>* item;
	for (item = boosts.getFirst(); item != nullptr; item = item->next) {
		if (item->data->active) {
			if (body1 == item->data->pbody || body2 == item->data->pbody) {
				item->data->cylinder->color = White;
				App->audio->PlayFx(POWERUP);
				item->data->active = false;
				item->data->time.Start();
				if (body1 == player1->vehicle || body2 == player1->vehicle) {
					player1->boost = true;
					player1->timer.Stop();
					player1->timer.Start();
				}
				if (body1 == player2->vehicle || body2 == player2->vehicle) {
					player2->boost = true;
					player2->timer.Stop();
					player2->timer.Start();
				}
			}
		}
	}
	
}



void ModuleSceneIntro::resetLevel() {
	if (time_left.Read() < 8000) {
		for (p2List_item<Primitive*>* item = map.getFirst(); item != nullptr; item = item->next) {
			if(score1 > score2)
				item->data->color = Orange;
			else if(score2 > score1)
				item->data->color = Blue;
			else if(score1 == score2)
				item->data->color = White;
		}
	}
	else {
		
		for (p2List_item<Cylinder*>* item = time_list.getFirst(); item != nullptr; item = item->next) {
			item->data->color = Black;
		}

		
		for (p2List_item<Cylinder*>* item2 = cylinders_list1.getFirst(); item2 != nullptr; item2 = item2->next) {
			item2->data->color = White;
		}

		
		for (p2List_item<Cylinder*>* item3 = cylinders_list2.getFirst(); item3 != nullptr; item3 = item3->next) {
			item3->data->color = White;
		}

		p2List_item<Primitive*>* item4 = map.getFirst();
		for (int i = 0; i < map.count() && item4 != nullptr; item4 = item4->next, ++i) {
			if(i < map.count()/2)
				item4->data->color = Orange;
			else
				item4->data->color = Blue;
		}
		score1 = 0;
		score2 = 0;
		interval = counter;
		RestartPositions();
		time_left.Start();
		game.Start();
		endGame = false;
		start = true;
	}
}

void ModuleSceneIntro::startRound() {
	uint res = time_left.Read();
	LOG("%i", res);
	if (res >= 1000) {
		second++;
		time_left.Start();
	}

	if ((second == 1 || second == 2) && res >= 1000) {
		App->audio->PlayFx(TICK);
	}
	else if (second == 3 && res >= 1000) {
		App->audio->PlayFx(GO);
		endGame = false;
		start = false;
		second = 0;
	}
}

void ModuleSceneIntro::createMap()
{
	//BLUE FIELD

	//Left
	Cube* p = new Cube(30,15,0.1);
	p->color = Orange;
	p->SetPos(-30, 7.5f, 100);
	map.add(p);
	App->physics->AddBody(*p, 0.0f);

	//Right
	p = new Cube(30, 15, 0.1);
	p->color = Orange;
	p->SetPos(30, 7.5f, 100);
	map.add(p);
	App->physics->AddBody(*p, 0.0f);

	//Center
	p = new Cube(30, 6, 0.1);
	p->color = Orange;
	p->SetPos(0, 12, 100);
	map.add(p);
	App->physics->AddBody(*p,0.0f);

	//GoalCenter
	p = new Cube(30, 9, 0.1);
	p->color = Orange;
	p->SetPos(0, 4.5, 112);
	map.add(p);
	App->physics->AddBody(*p, 0.0f);

	//GoalLeft
	p = new Cube(0.1, 9, 12);
	p->color = Orange;
	p->SetPos(15, 4.5, 106);
	map.add(p);
	App->physics->AddBody(*p, 0.0f);

	//GoalRight
	p = new Cube(0.1, 9, 12);
	p->color = Orange;
	p->SetPos(-15, 4.5, 106);
	map.add(p);
	App->physics->AddBody(*p, 0.0f);

	//GoalTop
	p = new Cube(30, 0.1, 12);
	p->color = Orange;
	p->SetPos(0, 9, 106);
	map.add(p);
	App->physics->AddBody(*p, 0.0f);
	
	//Side
	p = new Cube(0.1, 15, 100);
	p->color = Orange;
	p->SetPos(-45, 7.5f, 50);
	map.add(p);
	App->physics->AddBody(*p,0.0f);

	//Side
	p = new Cube(0.1, 15, 100);
	p->color = Orange;
	p->SetPos(45, 7.5f, 50);
	map.add(p);
	App->physics->AddBody(*p, 0.0f);

	//Top
	p = new Cube(90, 0.1, 100);
	p->color = Orange;
	p->SetPos(0, 15, 50);
	map.add(p);
	App->physics->AddBody(*p, 0.0f);


	//ORANGE FIEDLD

	//Side
	p = new Cube(0.1, 15, 100);
	p->color = Blue;
	p->SetPos(-45, 7.5f, -50);
	map.add(p);
	App->physics->AddBody(*p, 0.0f);

	//Side
	p = new Cube(0.1, 15, 100);
	p->color = Blue;
	p->SetPos(45, 7.5f, -50);
	map.add(p);
	App->physics->AddBody(*p, 0.0f);

	//Left
	p = new Cube(30, 15, 0.1);
	p->color = Blue;
	p->SetPos(-30, 7.5f, -100);
	map.add(p);
	App->physics->AddBody(*p, 0.0f);

	//Right
	p = new Cube(30, 15, 0.1);
	p->color = Blue;
	p->SetPos(30, 7.5f, -100);
	map.add(p);
	App->physics->AddBody(*p, 0.0f);

	//Center
	p = new Cube(30, 6, 0.1);
	p->color = Blue;
	p->SetPos(0, 12, -100);
	map.add(p);
	App->physics->AddBody(*p, 0.0f);

	//GoalCenter
	p = new Cube(30, 9, 0.1);
	p->color = Blue;
	p->SetPos(0, 4.5, -112);
	map.add(p);
	App->physics->AddBody(*p, 0.0f);

	//GoalLeft
	p = new Cube(0.1, 9, 12);
	p->color = Blue;
	p->SetPos(15, 4.5, -106);
	map.add(p);
	App->physics->AddBody(*p, 0.0f);

	//GoalRight
	p = new Cube(0.1, 9, 12);
	p->color = Blue;
	p->SetPos(-15, 4.5, -106);
	map.add(p);
	App->physics->AddBody(*p, 0.0f);

	//GoalTop
	p = new Cube(30, 0.1, -12);
	p->color = Blue;
	p->SetPos(0, 9, -106);
	map.add(p);
	App->physics->AddBody(*p, 0.0f);

	//Top
	p = new Cube(90, 0.1, 100);
	p->color = Blue;
	p->SetPos(0, 15, -50);
	map.add(p);
	App->physics->AddBody(*p, 0.0f);
}

void ModuleSceneIntro::PutSensors()
{
	Cube* sensor1 = new Cube(30, 9, 0.1);
	sensor1->SetPos(0, 4.5, 103);
	sensor1->color = White;
	sensors.add(sensor1);
	goal_player1 = App->physics->AddBody(*sensor1, 0.0f);
	goal_player1->SetSensors();
	goal_player1->collision_listeners.add(this);
	
	Cube* sensor2 = new Cube(30, 9, 0.1);
	sensor2->SetPos(0, 4.5, -103);
	sensor2->color = White;
	sensors.add(sensor2);
	goal_player2 = App->physics->AddBody(*sensor2, 0.0f);
	goal_player2->SetSensors();
	goal_player2->collision_listeners.add(this);

	createBoost({ 35,0.5,0 }, 0.5, 0.5);
	createBoost({ -35,0.5,0 }, 0.5, 0.5);

}

void ModuleSceneIntro::createBoost(vec3 pos, float radius, float height) {
	Cylinder* cylinder = new Cylinder(radius, height);
	cylinder->SetPos(pos.x, pos.y, pos.z);
	cylinder->SetRotation(90, vec3(0, 0, 1));
	cylinder->color = Orange;

	PhysBody3D* body = App->physics->AddBody(*cylinder, 0.0f);
	body->SetSensors();
	body->collision_listeners.add(this);

	Boost* boost = new Boost();
	boost->active = true;
	boost->cylinder = cylinder;
	boost->pbody = body;

	boosts.add(boost);
}

void ModuleSceneIntro::RotateBody(PhysVehicle3D * vehicle)
{
	float x = vehicle->GetPos().x;
	float z = vehicle->GetPos().z;
	float h = sqrt(x*x + z * z);

	float b = x / h;
	float angle = acos(b);
	if (vehicle == player1->vehicle)
		App->physics->rotateVehicle(angle + M_PI/2, vehicle);
	else if (vehicle == player2->vehicle)
		App->physics->rotateVehicle(angle - M_PI / 2, vehicle);
}

void ModuleSceneIntro::Score()
{
	float x = -12.75;
	int y = 11;
	int z = 98.5;

	for (int i = 0; i < 5; ++i)
	{
		Cylinder* score = new Cylinder(2, 0.75);
		score->SetPos(x, y, z);
		score->SetRotation(90, { 0,1,0 });
		score->color = White;
		cylinders_list1.add(score);
		x += 6.5;
	}

	for (int i = 0; i < 5; ++i)
	{
		x -= 6.5;
		Cylinder* score = new Cylinder(2, 0.75);
		score->SetPos(-x, y, -z);
		score->SetRotation(90, { 0,1,0 });
		score->color = White;
		cylinders_list2.add(score);	
	}
}

void ModuleSceneIntro::TimeLeft()
{
	Cylinder* time1 = new Cylinder(1, 0.75);
	time1->SetPos(4, -0.5, 0);
	time1->SetRotation(90, { 0,0,1 });
	time1->color = Black;
	time_list.add(time1);

	Cylinder* time2 = new Cylinder(1, 0.75);
	time2->SetPos(-4, -0.5, 0);
	time2->SetRotation(90, { 0,0,1 });
	time2->color = Black;
	time_list.add(time2);

	Cylinder* time3 = new Cylinder(1, 0.75);
	time3->SetPos(0, -0.5, 4);
	time3->SetRotation(90, { 0,0,1 });
	time3->color = Black;
	time_list.add(time3);

	Cylinder* time4 = new Cylinder(1, 0.75);
	time4->SetPos(0, -0.5, -4);
	time4->SetRotation(90, { 0,0,1 });
	time4->color = Black;
	time_list.add(time4);

	Cylinder* time = new Cylinder(1, 0.75);
	time->SetPos(0, -0.5, 0);
	time->SetRotation(90, { 0,0,1 });
	time->color = Black;
	time_list.add(time);
}

void ModuleSceneIntro::RestartPositions()
{
	player1->vehicle->SetPos(player1->initialPosition.x, player1->initialPosition.y, player1->initialPosition.z);
	player2->vehicle->SetPos(player2->initialPosition.x, player2->initialPosition.y, player2->initialPosition.z);
	RotateBody(player1->vehicle);
	RotateBody(player2->vehicle);
	pb_ball->SetPos(0, 2, 0);
	pb_ball->SetVelocityZero();
	player1->vehicle->SetVelocityZero();
	player2->vehicle->SetVelocityZero();
	player1->boost = false;
	player2->boost = false;
	player1->cable->SetPos(player1->initialPosition.x, player1->initialPosition.y + 2, player1->initialPosition.z);
	player2->cable->SetPos(player2->initialPosition.x, player2->initialPosition.y + 2, player2->initialPosition.z);
	player1->ball->SetPos(player1->initialPosition.x, player1->initialPosition.y + 2, player1->initialPosition.z);
	player2->ball->SetPos(player2->initialPosition.x, player2->initialPosition.y + 2, player2->initialPosition.z);
}

Boost::~Boost()
{
	pbody = nullptr;
	delete cylinder;
}
