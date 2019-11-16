#ifndef GIZMO_H_
#define GIZMO_H_
#include "Globals.h"
#include "Color.h"
#include "MathGeoLib/include/MathGeoLib.h"

#ifdef _DEBUG
#pragma comment (lib, "lib/MathGeoLib/libx86/Debug/MathGeoLib.lib")
#else
#pragma comment (lib, "lib/MathGeoLib/libx86/Release/MathGeoLib.lib")
#endif
class Gizmo
{
public:
	template<typename Box>
	static void DrawBox(Box& box, Color color) {
		float3 points[8];
		box.GetCornerPoints(points);
		DrawCubeBox(points, color);
	}
	static void DrawCubeBox(float3 * points, Color color);
};
#endif

