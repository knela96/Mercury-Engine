#include "PhysBody3D.h"
#include "Bullet/include/btBulletDynamicsCommon.h"
#include "PhysVehicle3D.h"


// =================================================
PhysBody3D::PhysBody3D(btRigidBody* body) : body(body)
{
	body->setUserPointer(this);
}

// ---------------------------------------------------------
PhysBody3D::~PhysBody3D()
{
	for (p2List_item<Module*>* item = collision_listeners.getFirst(); item; item = item->next)
	{
		item->data = nullptr;
	}
	collision_listeners.clear();

	delete body;
}

// ---------------------------------------------------------
void PhysBody3D::Push(float x, float y, float z)
{
	body->applyCentralImpulse(btVector3(x, y, z));
}

// ---------------------------------------------------------
void PhysBody3D::GetTransform(float* matrix) const
{
	if(body != NULL && matrix != NULL)
	{
		body->getWorldTransform().getOpenGLMatrix(matrix);
	}
}

// ---------------------------------------------------------
void PhysBody3D::SetTransform(const float* matrix) const
{
	if(body != NULL && matrix != NULL)
	{
		btTransform t;
		t.setFromOpenGLMatrix(matrix);
		body->setWorldTransform(t);
	}
}

// ---------------------------------------------------------
void PhysBody3D::SetPos(float x, float y, float z)
{
	btTransform t = body->getWorldTransform();
	t.setOrigin(btVector3(x, y, z));
	body->setWorldTransform(t);
}

vec3 PhysBody3D::GetPos()
{
	btTransform t = body->getWorldTransform();
	vec3 vector;
	vector.x = t.getOrigin().getX();
	vector.y = t.getOrigin().getY();
	vector.z = t.getOrigin().getZ();
	return vector;
}

void PhysBody3D::SetSensors()
{
	body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
}

void PhysBody3D::SetVelocityZero()
{
	const btVector3 vec = { 0,0,0 };
	body->setLinearVelocity(vec);
	body->setAngularVelocity(vec);
}


