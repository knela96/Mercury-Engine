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
	par_shapes_mesh* new_mesh = nullptr; 
	par_shapes_mesh* disk1 = nullptr;
	par_shapes_mesh* disk2 = nullptr;
	par_shapes_mesh* disk_aux = nullptr;

	aiMesh* mesh;
	char* name;

	switch (type)
	{
	case PrimitiveTypes::Primitive_Sphere:
		new_mesh = par_shapes_create_parametric_sphere(30,30);
		par_shapes_rotate(new_mesh, -M_PI / 2, x_rotation);
		name = "Sphere";
		LOGC("Sphere Primitive created");
		break;
	case PrimitiveTypes::Primitive_Plane:
		new_mesh = par_shapes_create_plane(2, 2);
		par_shapes_rotate(new_mesh, -M_PI / 2, x_rotation);
		par_shapes_translate(new_mesh, -0.5, 0, 0.5);
		name = "Plane";
		LOGC("Plane Primitive created");
		break;
	case PrimitiveTypes::Primitive_Cube:
		new_mesh = par_shapes_create_cube();
		par_shapes_rotate(new_mesh, -M_PI / 2, x_rotation);
		par_shapes_translate(new_mesh, -0.5, -0.5, 0.5);
		name = "Cube";
		LOGC("Cube Primitive created");
		break;
	case PrimitiveTypes::Primitive_Cone:
		new_mesh = par_shapes_create_cone(20, 5);

		disk1 = par_shapes_create_disk(1, 20, center, normal);
		par_shapes_rotate(disk1, -M_PI, x_rotation);
		par_shapes_merge(new_mesh, disk1);
		par_shapes_free_mesh(disk1);
		par_shapes_rotate(new_mesh, -M_PI / 2, x_rotation);
		name = "Cone";
		LOGC("Cone Primitive created");
		break;
	case PrimitiveTypes::Primitive_Cylinder:
		new_mesh = par_shapes_create_cylinder(30, 3);

		disk1 = par_shapes_create_disk(1, 30, center, normal);
		par_shapes_rotate(disk1, 18 * DEGTORAD, z_rotation);
		par_shapes_translate(disk1, 0, 0, 1);
		par_shapes_merge(new_mesh, disk1);
		par_shapes_free_mesh(disk1);

		disk2 = par_shapes_create_empty(); //Created Empty to align disks to Cylinder
		disk_aux = par_shapes_create_disk(1, 30, center, normal);
		par_shapes_rotate(disk_aux, -M_PI, x_rotation);
		par_shapes_merge(disk2, disk_aux);
		par_shapes_free_mesh(disk_aux);
		par_shapes_rotate(disk2, 18 * DEGTORAD, z_rotation);
		par_shapes_merge(new_mesh, disk2);
		par_shapes_free_mesh(disk2);
		par_shapes_rotate(new_mesh, -M_PI / 2, x_rotation);
		par_shapes_translate(new_mesh, 0, -0.5, 0);

		name = "Cylinder";

		LOGC("Cylinder Primitive created");
		break;
	}

	vector<Vertex> vertices;
	vector<uint> indices;
	vector<Texture*> textures;
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
				new_mesh->normals[3 * i],
				new_mesh->normals[(3 * i) + 1],
				new_mesh->normals[(3 * i) + 2]
			};
		}

		vertex.Colors = { 1.0f,1.0f,1.0f,1.0f };

		if (new_mesh->tcoords != nullptr)
		{
			vertex.TexCoords = {
				new_mesh->tcoords[2 * i],
				new_mesh->tcoords[(2 * i) + 1]
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

	if (new_mesh != nullptr)
		par_shapes_free_mesh(new_mesh);

	LOGC("Loaded Vertices: %u", vertices.size());
	LOGC("Loaded Indices: %u", indices.size());
	LOGC("Loaded Textures: %u", textures.size());

	App->importer->gameObjects.push_back((GameObject*) new MeshObject(vertices, indices, textures, name));
}

// LINE ==================================================
Line_::Line_() : Primitive(), origin(0, 0, 0), destination(1, 1, 1)
{
	type = PrimitiveTypes::Primitive_Line;
}

Line_::Line_(float x, float y, float z) : Primitive(), origin(0, 0, 0), destination(x, y, z)
{
	type = PrimitiveTypes::Primitive_Line;
}

void Line_::InnerRender() const
{
	glLineWidth(2.0f);

	glBegin(GL_LINES);

	glVertex3f(origin.x, origin.y, origin.z);
	glVertex3f(destination.x, destination.y, destination.z);

	glEnd();

	glLineWidth(1.0f);
}

// PLANE ==================================================
Plane_::Plane_() : Primitive(), normal(0, 1, 0), constant(1)
{
	type = PrimitiveTypes::Primitive_Plane;
}

Plane_::Plane_(float x, float y, float z, float d) : Primitive(), normal(x, y, z), constant(d)
{
	type = PrimitiveTypes::Primitive_Plane;
}

void Plane_::InnerRender() const
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