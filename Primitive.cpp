#include "Globals.h"
#include "Application.h"
#include "glew/include/GL/glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "Primitive.h"
#include "Assimp/include/mesh.h"
#include "ModuleImporter.h"

#include "par-master/par_shapes.h"
#include "MeshObject.h"

// ------------------------------------------------------------
Primitive::Primitive() : transform(IdentityMatrix), color(White), wire(false), axis(false), type(PrimitiveTypes::Primitive_Point)
{}

// ------------------------------------------------------------
PrimitiveTypes Primitive::GetType() const
{
	return type;
}

// ------------------------------------------------------------
void Primitive::Render() const
{
	glPushMatrix();
	glMultMatrixf(transform.M);

	if (axis == true)
	{
		// Draw Axis Grid
		glLineWidth(2.0f);

		glBegin(GL_LINES);

		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

		glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.1f, 0.0f); glVertex3f(1.1f, -0.1f, 0.0f);
		glVertex3f(1.1f, 0.1f, 0.0f); glVertex3f(1.0f, -0.1f, 0.0f);

		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);

		glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
		glVertex3f(0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
		glVertex3f(0.0f, 1.15f, 0.0f); glVertex3f(0.0f, 1.05f, 0.0f);

		glColor4f(0.0f, 0.0f, 1.0f, 1.0f);

		glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
		glVertex3f(-0.05f, 0.1f, 1.05f); glVertex3f(0.05f, 0.1f, 1.05f);
		glVertex3f(0.05f, 0.1f, 1.05f); glVertex3f(-0.05f, -0.1f, 1.05f);
		glVertex3f(-0.05f, -0.1f, 1.05f); glVertex3f(0.05f, -0.1f, 1.05f);

		glEnd();

		glLineWidth(1.0f);
	}

	glColor3f(color.r, color.g, color.b);

	if (wire)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	InnerRender();

	glPopMatrix();
}

// ------------------------------------------------------------
void Primitive::InnerRender() const
{
	glPointSize(5.0f);

	glBegin(GL_POINTS);

	glVertex3f(0.0f, 0.0f, 0.0f);

	glEnd();

	glPointSize(1.0f);
}

// ------------------------------------------------------------
void Primitive::SetPos(float x, float y, float z)
{
	transform.translate(x, y, z);
}

// ------------------------------------------------------------
void Primitive::SetRotation(float angle, const vec3 &u)
{
	transform.rotate(angle, u);
}

// ------------------------------------------------------------
void Primitive::Scale(float x, float y, float z)
{
	transform.scale(x, y, z);
}

// DRAW PAR SHAPES ============================================

void Primitive::DrawObj(PrimitiveTypes type) {

	par_shapes_mesh* new_mesh;

	aiMesh* mesh;

	switch (type)
	{
	case PrimitiveTypes::Primitive_Sphere:
		new_mesh = par_shapes_create_subdivided_sphere(2);
		break;
	case PrimitiveTypes::Primitive_Plane:
		new_mesh = par_shapes_create_plane(1, 1);
		break;
	case PrimitiveTypes::Primitive_Cube:
		new_mesh = par_shapes_create_cube();
		break;
	case PrimitiveTypes::Primitive_Cone:
		new_mesh = par_shapes_create_cone(20, 5);
		break;
	case PrimitiveTypes::Primitive_Cylinder:
		new_mesh = par_shapes_create_cylinder(20, 4);
		break;
	}


	vector<Vertex> vertices;
	vector<uint> indices;
	vector<Texture> textures;

	Vertex vertex;
	for (uint i = 0; i < new_mesh->npoints; i++)
	{
		if (new_mesh->triangles != nullptr)
		{
			vertex.Position = {
				new_mesh->points[3 * i],
				new_mesh->points[(3 * i) + 1],
				new_mesh->points[(3 * i) + 2]
			};
		}
		if (new_mesh->normals != nullptr)
		{
			vertex.Normal = {
				new_mesh->normals[i],
				new_mesh->normals[(3 * i) + 1],
				new_mesh->normals[(3 * i) + 2]
			};
		}

		vertex.Colors = { 1.0f,1.0f,1.0f,1.0f };

		if (new_mesh->tcoords != nullptr)
		{
			vertex.TexCoords = {
				new_mesh->tcoords[i],
				new_mesh->tcoords[i + 1]
			};
		}
		else
			vertex.TexCoords = { 0.0f,0.0f };

		vertices.push_back(vertex);
	}


	for (uint i = 0; i < new_mesh->ntriangles * 3; i++)
	{

		indices.push_back(new_mesh->triangles[i]);

	}
	App->importer->meshes.push_back(MeshObject(vertices, indices, textures));
}

// LINE ==================================================
Line::Line() : Primitive(), origin(0, 0, 0), destination(1, 1, 1)
{
	type = PrimitiveTypes::Primitive_Line;
}

Line::Line(float x, float y, float z) : Primitive(), origin(0, 0, 0), destination(x, y, z)
{
	type = PrimitiveTypes::Primitive_Line;
}

void Line::InnerRender() const
{
	glLineWidth(2.0f);

	glBegin(GL_LINES);

	glVertex3f(origin.x, origin.y, origin.z);
	glVertex3f(destination.x, destination.y, destination.z);

	glEnd();

	glLineWidth(1.0f);
}

// PLANE ==================================================
Plane::Plane() : Primitive(), normal(0, 1, 0), constant(1)
{
	type = PrimitiveTypes::Primitive_Plane;
}

Plane::Plane(float x, float y, float z, float d) : Primitive(), normal(x, y, z), constant(d)
{
	type = PrimitiveTypes::Primitive_Plane;
}

void Plane::InnerRender() const
{
	glLineWidth(1.0f);

	glBegin(GL_LINES);

	float d = 200.0f;

	for (float i = -d; i <= d; i += 1.0f)
	{
		glVertex3f(i, 0.0f, -d);
		glVertex3f(i, 0.0f, d);
		glVertex3f(-d, 0.0f, i);
		glVertex3f(d, 0.0f, i);
	}

	glEnd();
}