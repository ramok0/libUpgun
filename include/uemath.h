#pragma once

#include "../lib/glm/vec3.hpp"
#include "../lib/glm/gtc/quaternion.hpp"

typedef glm::vec2 FVector2D;
typedef glm::vec3 FVector;
typedef glm::quat FQuat;

struct FTransform {
	FQuat Rotation; // 0x00(0x10)
	FVector Translation; // 0x10(0x0c)
	char pad_1C[0x4]; // 0x1c(0x04)
	FVector Scale3D; // 0x20(0x0c)
	char pad_2C[0x4]; // 0x2c(0x04)
};

