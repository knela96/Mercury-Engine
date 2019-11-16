#include "Application.h"
#include "Gizmo.h"

void Gizmo::DrawCubeBox(float3* points, Color color)
{
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES); 
	glLineWidth(25.0f);
	glColor3f(color.r, color.g, color.b);
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

	glLineWidth(1);
	glEnd();
	glEnable(GL_LIGHTING);

}