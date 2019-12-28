#ifndef GIZMO_H_
#define GIZMO_H_
#include "Globals.h"
#include "Color.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "ImGuizmo/ImGuizmo.h"
#include "glmath.h"
class Gizmo
{
public:
	template<typename Box>
	static void DrawBox(const Box& box, Color color) {
		float3 points[8];
		box.GetCornerPoints(points);
		DrawCubeBox(points, color);
	}
	static void DrawCubeBox(float3 * points, Color color);

	Gizmo();
	~Gizmo();

	void Update();
	void Draw();


	//Variables for gizmo handling
	ImGuizmo::OPERATION gizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
	ImGuizmo::MODE gizmoMode = ImGuizmo::MODE::WORLD;

	void HandleGuizmo();


	vec2 win_size;
	vec2 img_offset;

	vec2 img_corner;
	vec2 img_size;
	vec2 cornerPos;

};
#endif
