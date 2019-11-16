#ifndef GIZMO_H_
#define GIZMO_H_
#include "Globals.h"
#include "Color.h"
#include "MathGeoLib/include/MathGeoLib.h"

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
};
#endif
