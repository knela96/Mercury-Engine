#include "Globals.h"
#include "Application.h"
#include "glew/include/GL/glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "Primitive.h"
#include "Assimp/include/mesh.h"
#include "ModuleImporter.h"
#define PAR_SHAPES_IMPLEMENTATION
#include "par-master/par_shapes.h"

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

	par_shapes_mesh* obj;

	aiMesh* mesh;

	switch (type)
	{
	case PrimitiveTypes::Primitive_Sphere:
		obj = par_shapes_create_subdivided_sphere(20);
		break;
	case PrimitiveTypes::Primitive_Plane:
		obj = par_shapes_create_plane(10, 10);
		break;
	case PrimitiveTypes::Primitive_Cube:
		obj = par_shapes_create_cube();
		break;
	case PrimitiveTypes::Primitive_Cone:
		obj = par_shapes_create_cone(4,2);
		break;
	case PrimitiveTypes::Primitive_Cylinder:
		obj = par_shapes_create_cylinder(5, 2);
		break;
	}

	//translate from par_shape_mesh to aiMesh
	if (obj->normals != nullptr) {
		for (int i = 0; i< obj->npoints; i + 3) { //vertices and normals are in the same amount
			//VERTICES
			mesh->mVertices[i].x = obj->points[i];
			mesh->mVertices[i].y = obj->points[i + 1];
			mesh->mVertices[i].z = obj->points[i + 2];
			//NORMALS
			mesh->mNormals[i].x = obj->normals[i];
			mesh->mNormals[i].y = obj->normals[i + 1];
			mesh->mNormals[i].z = obj->normals[i + 2];

		}
	}
	if (obj->tcoords != nullptr) {
		for (int i = 0; obj->npoints; i + 2) {
			mesh->mTextureCoords[0][i].x = obj->tcoords[i];
			mesh->mTextureCoords[0][i].y = obj->tcoords[i + 1];
			//mesh->mTextureCoords[0][i].z = obj->tcoords[i + 2]; the third chanel is not used bc the info is given in pairs from obj array
		}
	}
	if (obj->triangles != nullptr) {
		for (int i = 0; i < mesh->mFaces->mNumIndices; i++)//ok
			mesh->mFaces->mIndices[i] = obj->triangles[i];
	}

	//if(mesh->mVertices !=nullptr && mesh->mFaces!=nullptr && mesh->mTextureCoords != nullptr)
	App->importer->PushObj(mesh);
	












	//	glGenBuffers(1, &vboId);
	//	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(normals) + 288, 0, GL_STATIC_DRAW);
	//	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);                             // copy vertices starting from 0 offest
	//	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(normals), normals);                // copy normals after vertices
	//	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(normals), 288, colors);  // copy colours after normals
	//	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//	glGenBuffers(1, &iboId);
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
	//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//	// bind VBOs with IDs and set the buffer offsets of the bound VBOs
	//		// When buffer object is bound with its ID, all pointers in gl*Pointer()
	//		// are treated as offset instead of real pointer.
	//	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);


	//	// enable vertex arrays
	//	glEnableClientState(GL_NORMAL_ARRAY);
	//	glEnableClientState(GL_COLOR_ARRAY);
	//	glEnableClientState(GL_VERTEX_ARRAY);

	//	// before draw, specify vertex and index arrays with their offsets
	//	glNormalPointer(GL_FLOAT, 0, (void*)sizeof(vertices));
	//	glColorPointer(3, GL_FLOAT, 0, (void*)(sizeof(vertices) + sizeof(normals)));
	//	glVertexPointer(3, GL_FLOAT, 0, 0);
	//	//glTranslatef(X, Y, Z);

	//	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);

	//	glPopMatrix();

	//	glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
	//	glDisableClientState(GL_COLOR_ARRAY);
	//	glDisableClientState(GL_NORMAL_ARRAY);

	//	// it is good idea to release VBOs with ID 0 after use.
	//	// Once bound with 0, all pointers in gl*Pointer() behave as real
	//	// pointer, so, normal vertex array operations are re-activated
	//	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


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