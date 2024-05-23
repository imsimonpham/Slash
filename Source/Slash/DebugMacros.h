#pragma once
#include "DrawDebugHelpers.h"

#define DRAW_SPHERE(Location, Color) if(GetWorld()) DrawDebugSphere(GetWorld(), Location, 25.f, 24, Color, true);
#define DRAW_SPHERE_SingleFrame(Location, Color) if(GetWorld()) DrawDebugSphere(GetWorld(), Location, 25.f, 24, Color, false, -1.f);
#define DRAW_VECTOR(StartLocation, EndLocation, Color) if(GetWorld()) \
		{\
			DrawDebugLine(GetWorld(), StartLocation, EndLocation, Color, true, -1.f, 0, 1.f); \
			DrawDebugPoint(GetWorld(), EndLocation, 15, Color, true); \
		}

#define DRAW_VECTOR_SingleFrame(StartLocation, EndLocation, Color) if(GetWorld()) \
		{\
			DrawDebugLine(GetWorld(), StartLocation, EndLocation, Color, false, -1.f, 0, 1.f); \
			DrawDebugPoint(GetWorld(), EndLocation, 15, Color, false, -1.f); \
		}
