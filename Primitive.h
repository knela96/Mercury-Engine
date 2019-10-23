#ifndef PRIMITIVE_H
#define PRIMITIVE_H
#include "glmath.h"
#include "Color.h"

enum PrimitiveTypes
{
	Primitive_Point,
	Primitive_Line,
	Primitive_Plane,
	Primitive_Cube,
	Primitive_Sphere,
	Primitive_Cylinder,
	Primitive_Cone
};

class Primitive
{
public:

	Primitive();

	virtual void	Render() const;
	virtual void	InnerRender() const;
	void			SetPos(float x, float y, float z);
	void			SetRotation(float angle, const vec3 &u);
	void			Scale(float x, float y, float z);
	void DrawObj(PrimitiveTypes type);
	PrimitiveTypes	GetType() const;

public:

	Color color;
	mat4x4 transform;
	bool axis, wire;

private:

	float x_rotation[3] = { 1, 0, 0 };
	float y_rotation[3] = { 0, 1, 0 };
	float z_rotation[3] = { 0, 0, 1 };
	float normal[3] = { 0, 0, 1 };
	float center[3] = { 0, 0, 0 };

protected:
	PrimitiveTypes type;
};

// ============================================
class Cube : public Primitive
{
public:
	Cube();
	Cube(float sizeX, float sizeY, float sizeZ);
	void InnerRender() const;
public:
	vec3 size;
};

// ============================================
class Sphere : public Primitive
{
public:
	Sphere();
	Sphere(float radius);
public:
	float radius;
};

// ============================================
class Cylinder : public Primitive
{
public:
	Cylinder();
	Cylinder(float radius, float height);
	void InnerRender() const;
public:
	float radius;
	float height;
};

// ============================================
class Line : public Primitive
{
public:
	Line();
	Line(float x, float y, float z);
	void InnerRender() const;
public:
	vec3 origin;
	vec3 destination;
};

// ============================================
class Plane : public Primitive
{
public:
	Plane();
	Plane(float x, float y, float z, float d);
	void InnerRender() const;
public:
	vec3 normal;
	float constant;
};
#endif PRIMITIVE_H