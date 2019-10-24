#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "WindowInspector.h"
#include "GameObject.h"


ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(0.0f, 2.0f, 5.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);
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
		vec3 newPos(0, 0, 0);
		float speed = 20.0f * dt;

		if (!App->input->writting) {
			if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
				speed = 50.0f * dt;

		/*	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) newPos.y += speed;
			if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) newPos.y -= speed;*/

			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed;
			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * speed;


			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed;
			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed;

			Position += newPos;
			Reference += newPos;


			if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN) {
				GameObject* gameObject = App->gui->inspector->active_gameObject;
				if (gameObject != nullptr) {
					//Move camera to Point Reference of the Object(Pivot)
					vec3 distance = { gameObject->box.CenterPoint().x - Reference.x,
									gameObject->box.CenterPoint().y - Reference.y,
									gameObject->box.CenterPoint().z - Reference.z };
					
					Reference += distance;

					//Focus Camera distance
					float3 points [8];
					gameObject->box.GetCornerPoints(points);

					vec3 max_ = { 0,0,0 };
					for (int i = 0; i < 8; ++i) {
						max_.x = max(points[i].At(0), max_.x);
						max_.y = max(points[i].At(1), max_.y);
						max_.x = max(points[i].At(2), max_.z);
					}

					double boundSphereRadius = length(max_) / 2;

					double fov = 60 * DEGTORAD;

					double camDistance = (boundSphereRadius * 2.0) / tan(fov / 2.0);
					
					Position = Reference + Z * camDistance;

				}
			}
		}

		// Mouse motion ----------------
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP || App->input->GetKey(SDL_SCANCODE_LALT) == KEY_UP || App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_UP)
			moving = false;

		if (!moving)
			moving = App->gui->game->mouseHover();

		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
		{
			if (moving) {
				int dx = -App->input->GetMouseXMotion();
				int dy = -App->input->GetMouseYMotion();

				float Sensitivity = 0.25;

				Position -= Reference;

				if (dx != 0)
				{
					float DeltaX = (float)dx * Sensitivity;

					X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
					Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
					Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
				}

				if (dy != 0)
				{
					float DeltaY = (float)dy * Sensitivity;

					Y = rotate(Y, DeltaY, X);
					Z = rotate(Z, DeltaY, X);

					if (Y.y < 0.0f)
					{
						Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
						Y = cross(Z, X);
					}
				}

				Position = Reference + Z * length(Position);
			}
		}
			   
		if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT) {
			if (moving) {
				int dx = -App->input->GetMouseXMotion();
				int dy = -App->input->GetMouseYMotion();
				vec3 distance = (Position - Reference);
				float Sensitivity = 0.25;

				if (dx != 0)
				{
					newPos += X * speed * dx * length(distance) * Sensitivity;
				}

				if (dy != 0)
				{
					newPos -= Y * speed * dy * length(distance) * Sensitivity;
				}

				Position += newPos;
				Reference += newPos;
			}
		}

		//Mouse Wheel
		if (App->gui->game->mouseHover() && App->input->GetMouseZ() != 0) {
			vec3 distance = (Position - Reference);
			vec3 nPos = { 0,0,0 };
			if ((App->input->GetMouseZ() > 0 && length(distance) > 0.5) || App->input->GetMouseZ() < 0) {
					nPos -= Z * App->input->GetMouseZ() * length(distance) / 10;
				Position += nPos;
			}
		}

		// Recalculate matrix -------------
		CalculateViewMatrix();
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

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt(const vec3 &Spot)
{
	Reference = Spot;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const vec3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return &ViewMatrix;
}

mat4x4 ModuleCamera3D::GetViewMatrix4x4() {
	return  ViewMatrix;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}