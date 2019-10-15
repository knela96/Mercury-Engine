#include "MeshObject.h"
#include "Application.h"
#include "glmath.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#pragma comment (lib, "lib/Assimp/libx86/assimp.lib")

MeshObject::MeshObject(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	SetupBuffers();
}

MeshObject::~MeshObject()
{
}


bool MeshObject::SetupBuffers() {
	bool ret = true;

	// create buffers/arrays
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	// load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	// load data into indices buffers
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// set the vertex attribute pointers
	// vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	// vertex colours
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Colors));


	glBindVertexArray(0);

	return ret;
}

void MeshObject::Draw() {

	// bind appropriate textures
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	LOGC("Loaded Textures:");

	glBindVertexArray(VAO);

	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
		// retrieve texture number (the N in diffuse_textureN)
		string number;
		aiTextureType type = textures[i].type;
		if (type == aiTextureType_DIFFUSE)
			number = std::to_string(diffuseNr++);
		else if (type == aiTextureType_SPECULAR)
			number = std::to_string(specularNr++); // transfer unsigned int to stream
		else if (type == aiTextureType_NORMALS)
			number = std::to_string(normalNr++); // transfer unsigned int to stream
		else if (type == aiTextureType_HEIGHT)
			number = std::to_string(heightNr++); // transfer unsigned int to stream

												 // now set the sampler to the correct texture unit
		//glUniform1i(glGetUniformLocation(shader.ID, (getType(type) + number).c_str()), i);
		// and finally bind the texture

		//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);		
	}	
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D,0);
	glActiveTexture(GL_TEXTURE0);

	//if (App->gui->vertex_normals) {
	//	//NORMAL VERTEX
	//	for (int i = 0; i < indices.size(); i++)
	//	{
	//		vec3 vertex = vec3(vertices[indices[i]].Position.x, vertices[indices[i]].Position.y, vertices[indices[i]].Position.z);
	//		vec3 normal = vec3(vertices[indices[i]].Normal.x, vertices[indices[i]].Normal.y, vertices[indices[i]].Normal.z);

	//		glBegin(GL_LINES);
	//		glColor3f(1.0, 0.0, 0.0);
	//		glVertex3f(vertex.x, vertex.y, vertex.z);
	//		glVertex3f((vertex.x + normal.x * 1.0f), (vertex.y + normal.y * 1.0f), (vertex.z + normal.z * 1.0f));
	//		glEnd();
	//	}
	//}
	//
	//if (App->gui->face_normals) {
	//	//NORMAL FACES
	//	for (int i = 0; i < indices.size(); i += 3)
	//	{
	//		vec3 p1 = vec3(vertices[indices[i]].Position.x, vertices[indices[i]].Position.y, vertices[indices[i]].Position.z);
	//		vec3 p2 = vec3(vertices[indices[i + 1]].Position.x, vertices[indices[i + 1]].Position.y, vertices[indices[i + 1]].Position.z);
	//		vec3 p3 = vec3(vertices[indices[i + 2]].Position.x, vertices[indices[i + 2]].Position.y, vertices[indices[i + 2]].Position.z);

	//		vec3 normal = getNormal(p1, p2, p3);

	//		vec3 face_center = vec3(
	//			(p1.x + p2.x + p3.x) / 3,
	//			(p1.y + p2.y + p3.y) / 3,
	//			(p1.z + p2.z + p3.z) / 3
	//		);

	//		glBegin(GL_LINES);
	//		glColor3f(0.0, 1.0, 1.0);
	//		glVertex3f(face_center.x, face_center.y, face_center.z);
	//		glVertex3f((face_center.x + normal.x * 1.0f), (face_center.y + normal.y * 1.0f), (face_center.z + normal.z * 1.0f));
	//		glEnd();
	//	}
	//}
}

char* MeshObject::getType(aiTextureType type) {
	switch (type) {
	case aiTextureType_DIFFUSE:
		return "Diffuse Map";
		break;
	case aiTextureType_SPECULAR:
		return "Specular Map";
		break;
	case aiTextureType_NORMALS:
		return "Normal Map";
		break;
	case aiTextureType_HEIGHT:
		return "Height Map";
		break;
	}
}

vec3 MeshObject::getNormal(vec3 p1, vec3 p2, vec3 p3) {

	//Create normal vector we are going to output.
	vec3 output;

	//Calculate vectors used for creating normal (these are the edges of the triangle).
	vec3 calU = vec3(p2.x - p1.x, p2.y - p1.y, p2.z - p1.z);
	vec3 calV = vec3(p3.x - p1.x, p3.y - p1.y, p3.z - p1.z);

	//The output vector is equal to the cross products of the two edges of the triangle
	output.x = calU.y * calV.z - calU.z * calV.y;
	output.y = calU.z * calV.x - calU.x * calV.z;
	output.z = calU.x * calV.y - calU.y * calV.x;

	//Return the resulting vector.
	return normalize(output);
}

void MeshObject::CleanUp() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

