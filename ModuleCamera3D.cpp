#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "WindowInspector.h"
#include "GameObject.h"
#include "C_Camera.h"


ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	//CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(0.0f, 2.0f, 5.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);

	camera = new C_Camera(nullptr,Component_Type::Camera);
	camera->SetPos(Position);
	camera->SetFOV(60);
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOGC("Starting camera module");
	bool ret = true;

	return ret;
}

void ModuleCamera3D::MoveCamera(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 50.0f * dt;

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) 
		newPos -= Z * speed;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) 
		newPos += Z * speed;


	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) 
		newPos -= X * speed;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) 
		newPos += X * speed;

	Position += newPos;
	Reference += newPos;
	camera->SetPos(Position);
}

void ModuleCamera3D::FocusTarget()
{
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN) {
		GameObject* gameObject = App->gui->inspector->active_gameObject;
		if (gameObject != nullptr) {
			//Move camera to Point Reference of the Object(Pivot)
			vec3 distance = { gameObject->aabb.CenterPoint().x - Reference.x,
							gameObject->aabb.CenterPoint().y - Reference.y,
							gameObject->aabb.CenterPoint().z - Reference.z };

			Reference += distance;

			//Focus Camera distance
			float3 points[8];
			gameObject->aabb.GetCornerPoints(points);
			vec3 max_ = { points[0].At(0),points[0].At(1),points[0].At(2) }; //set first point
			for (int i = 0; i < 8 - 1; ++i) {
				vec3 point_ = { points[i].At(0), points[i].At(1), points[i].At(2) };
				if (length(max_) < length(point_))
					max_ = point_;
			}
			double radius = length(max_) / 2; //radius of sphere
			
			
			//double radius = gameObject->obb.MaximalContainedSphere().Diameter() / 2; //radius of sphere

			double fov = camera->GetFOV() * DEGTORAD;

			double camDistance = (radius * 2.0) / tan(fov / 2.0);

			Position = Reference + to_vec3(-camera->frustum.front) * camDistance;
			camera->SetPos(Position);
		}
	}
}

void ModuleCamera3D::Orbit()
{
	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
	{
		if (moving) {
			int dx = -App->input->GetMouseXMotion();
			int dy = -App->input->GetMouseYMotion();

			float3 vector = camera->frustum.pos - to_float3(Reference);

			Quat quat_y(camera->frustum.up, dx * 0.005);
			Quat quat_x(camera->frustum.WorldRight(), dy * 0.005);

			vector = quat_x.Transform(vector);
			vector = quat_y.Transform(vector);

			Position = to_vec3(vector) + Reference;
			camera->SetPos(Position);
			LookAt(Reference);
		}
	}
}

void ModuleCamera3D::Pan()
{
	if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT) {
		if (moving) {
			int dx = -App->input->GetMouseXMotion();
			int dy = -App->input->GetMouseYMotion();
			vec3 distance = (Position - Reference);
			float Sensitivity = 0.10;

			if (dx != 0)
			{
				newPos += to_vec3(camera->frustum.WorldRight()) * speed * dx * length(distance) * Sensitivity;
			}

			if (dy != 0)
			{
				newPos -= to_vec3(camera->frustum.up) * speed * dy * length(distance) * Sensitivity;
			}

			Position += newPos;
			Reference += newPos;
			camera->SetPos(Position);
		}
	}
}

void ModuleCamera3D::Zoom()
{
	if (App->gui->game->mouseHover() && App->input->GetMouseZ() != 0) {
		vec3 distance = (Position - Reference);
		vec3 nPos = { 0,0,0 };
		if ((App->input->GetMouseZ() > 0 && length(distance) > 0.5) || App->input->GetMouseZ() < 0) {
			nPos -= to_vec3(-camera->frustum.front) * App->input->GetMouseZ() * length(distance) / 10;
			Position += nPos;
			camera->SetPos(Position);
		}
	}
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	// Implement a debug camera with keys and mouse
	// Now we can make this movememnt frame rate independant!
		newPos = { 0,0,0 };
		speed = 20.0f * dt;

		if (!App->input->writting) {
			
			MoveCamera(dt);

			FocusTarget();
			
		}

		// Mouse motion ----------------
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP || App->input->GetKey(SDL_SCANCODE_LALT) == KEY_UP || App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_UP)
			moving = false;

		if (!moving)
			moving = App->gui->game->mouseHover();

		Pan();
			   
		Orbit();

		Zoom();

		// Recalculate matrix -------------
		//CalculateViewMatrix();
	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if (!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	//CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt(const vec3 &Spot)
{
	Reference = Spot;
	float3 position = { Spot.x,Spot.y,Spot.z };
	camera->Look(position);
}


// -----------------------------------------------------------------
//void ModuleCamera3D::Move(const vec3 &Movement)
//{
//	Position += Movement;
//	Reference += Movement;
//
//	CalculateViewMatrix();
//}

// -----------------------------------------------------------------
//float* ModuleCamera3D::GetViewMatrix()
//{
//	return &ViewMatrix;
//}
//
//mat4x4 ModuleCamera3D::GetViewMatrix4x4() {
//	return  ViewMatrix;
//}
//
//// -----------------------------------------------------------------
//void ModuleCamera3D::CalculateViewMatrix()
//{
//	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
//	ViewMatrixInverse = inverse(ViewMatrix);
//}


float3 ModuleCamera3D::to_float3(vec3 vec) {
	return float3(vec.x, vec.y, vec.z);
}
vec3 ModuleCamera3D::to_vec3(float3 vec) {
	return vec3(vec.x, vec.y, vec.z);
}