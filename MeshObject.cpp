#include "MeshObject.h"
#include "Application.h"
#include "glmath.h"
#include "glmath.h"
#include "ModuleGUI.h"
#include "C_Normals.h"

MeshObject::MeshObject(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture*> textures, string name) : GameObject(this,textures,name)
{
	this->vertices = vertices;
	this->indices = indices;

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);
	// vertex normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	glEnableVertexAttribArray(1);
	// vertex texture coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	glEnableVertexAttribArray(2);
	// vertex colours
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Colors));
	glEnableVertexAttribArray(3);

	glBindVertexArray(0);
	App->importer->shader->stop();

	return ret;
}

void MeshObject::Draw()
{
	// bind appropriate textures
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;

	if (App->renderer3D->texture_active && getComponent(Material)->isActive()) {
		if(!debug_tex){
			for (unsigned int i = 0; i < textures.size(); i++)
			{
				glActiveTexture(GL_TEXTURE0 + i); // active texture unit
				string number;
				aiTextureType type = textures[i]->type;
				if (type == aiTextureType_DIFFUSE)
					number = std::to_string(diffuseNr++);
				else if (type == aiTextureType_SPECULAR)
					number = std::to_string(specularNr++);
				else if (type == aiTextureType_NORMALS)
					number = std::to_string(normalNr++);
				else if (type == aiTextureType_HEIGHT)
					number = std::to_string(heightNr++);

				//set the sampler to the correct texture unit
				App->importer->shader->setInt((getType(type) + number).c_str(), i);

				glBindTexture(GL_TEXTURE_2D, textures[i]->id);
			}
			//If mesh has no textures, don't draw any texture
			if (textures.size() > 0) {
				mat4x4 model = mat4x4();
				App->importer->shader->use();
				App->importer->shader->setMat4("model", model);
				App->importer->shader->setMat4("view", App->camera->GetViewMatrix4x4());
				App->importer->shader->setMat4("projection", App->renderer3D->ProjectionMatrix);
			}
		}
		else {
			glActiveTexture(GL_TEXTURE0);
			App->importer->shader->setInt("Diffuse_Map1", 1);
			glBindTexture(GL_TEXTURE_2D, App->importer->checkImage_id);

			mat4x4 model = mat4x4();
			App->importer->shader->use();
			App->importer->shader->setMat4("model", model);
			App->importer->shader->setMat4("view", App->camera->GetViewMatrix4x4());
			App->importer->shader->setMat4("projection", App->renderer3D->ProjectionMatrix);
		}
	}

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	App->importer->shader->stop();
	glBindTexture(GL_TEXTURE_2D, 0);

	if(getComponent(Normals)->isActive())
		DebugNormals();

	DrawBox();
}

const vec3 MeshObject::getNormal(vec3 p1, vec3 p2, vec3 p3) const
{
	vec3 output;

	//Calculate vectors to create the normal
	vec3 calU = vec3(p2.x - p1.x, p2.y - p1.y, p2.z - p1.z);
	vec3 calV = vec3(p3.x - p1.x, p3.y - p1.y, p3.z - p1.z);

	//Normal Vector
	output.x = calU.y * calV.z - calU.z * calV.y;
	output.y = calU.z * calV.x - calU.x * calV.z;
	output.z = calU.x * calV.y - calU.y * calV.x;

	//Return normal normalized
	return normalize(output);
}

void MeshObject::CleanUp() {
	LOGC("Cleaned MeshObject Buffer");
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDeleteBuffers(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glBindVertexArray(0);
}

void MeshObject::DebugNormals() const
{
	if (vertex_normals) {
		//NORMAL VERTEX
		for (int i = 0; i < vertices.size(); i++)
		{
			vec3 vertex = vec3(vertices[i].Position.x, vertices[i].Position.y, vertices[i].Position.z);
			vec3 normal = vec3(vertices[i].Normal.x, vertices[i].Normal.y, vertices[i].Normal.z);

			glBegin(GL_LINES);
			glColor3f(normals->vertex_color.r, normals->vertex_color.g, normals->vertex_color.b);
			glVertex3f(vertex.x, vertex.y, vertex.z);
			glVertex3f((vertex.x + normal.x * normals->vertex_lenght), (vertex.y + normal.y * normals->vertex_lenght), (vertex.z + normal.z * normals->vertex_lenght));
			glEnd();
		}
	}

	if (face_normals) {
		//NORMAL FACES
		for (int i = 0; i < indices.size(); i += 3)
		{
			vec3 p1 = vec3(vertices[indices[i]].Position.x, vertices[indices[i]].Position.y, vertices[indices[i]].Position.z);
			vec3 p2 = vec3(vertices[indices[i + 1]].Position.x, vertices[indices[i + 1]].Position.y, vertices[indices[i + 1]].Position.z);
			vec3 p3 = vec3(vertices[indices[i + 2]].Position.x, vertices[indices[i + 2]].Position.y, vertices[indices[i + 2]].Position.z);

			const vec3 normal = getNormal(p1, p2, p3);

			vec3 face_center = vec3(
				(p1.x + p2.x + p3.x) / 3,
				(p1.y + p2.y + p3.y) / 3,
				(p1.z + p2.z + p3.z) / 3
			);

			glBegin(GL_LINES);
			glColor3f(normals->face_color.r, normals->face_color.g, normals->face_color.b);
			glVertex3f(face_center.x, face_center.y, face_center.z);
			glVertex3f((face_center.x + normal.x * normals->face_lenght), (face_center.y + normal.y * normals->face_lenght), (face_center.z + normal.z * normals->face_lenght));
			glEnd();
		}
	}
}
void MeshObject::DrawBox() const
{
	if (boundary_box) {
		float3 points[8];
		box.GetCornerPoints(points);

		glBegin(GL_LINES);
		glColor3f(1, 0.84, 0);
		glVertex3f(points[0].At(0), points[0].At(1), points[0].At(2));
		glVertex3f(points[1].At(0), points[1].At(1), points[1].At(2));
		glVertex3f(points[2].At(0), points[2].At(1), points[2].At(2));
		glVertex3f(points[3].At(0), points[3].At(1), points[3].At(2));
		glVertex3f(points[4].At(0), points[4].At(1), points[4].At(2));
		glVertex3f(points[5].At(0), points[5].At(1), points[5].At(2));
		glVertex3f(points[6].At(0), points[6].At(1), points[6].At(2));
		glVertex3f(points[7].At(0), points[7].At(1), points[7].At(2));


		glVertex3f(points[0].At(0), points[0].At(1), points[0].At(2));
		glVertex3f(points[4].At(0), points[4].At(1), points[4].At(2));
		glVertex3f(points[1].At(0), points[1].At(1), points[1].At(2));
		glVertex3f(points[5].At(0), points[5].At(1), points[5].At(2));
		glVertex3f(points[2].At(0), points[2].At(1), points[2].At(2));
		glVertex3f(points[6].At(0), points[6].At(1), points[6].At(2));
		glVertex3f(points[3].At(0), points[3].At(1), points[3].At(2));
		glVertex3f(points[7].At(0), points[7].At(1), points[7].At(2));

		glVertex3f(points[0].At(0), points[0].At(1), points[0].At(2));
		glVertex3f(points[2].At(0), points[2].At(1), points[2].At(2));
		glVertex3f(points[1].At(0), points[1].At(1), points[1].At(2));
		glVertex3f(points[3].At(0), points[3].At(1), points[3].At(2));
		glVertex3f(points[4].At(0), points[4].At(1), points[4].At(2));
		glVertex3f(points[6].At(0), points[6].At(1), points[6].At(2));
		glVertex3f(points[5].At(0), points[5].At(1), points[5].At(2));
		glVertex3f(points[7].At(0), points[7].At(1), points[7].At(2));

		glEnd();
	}

}
