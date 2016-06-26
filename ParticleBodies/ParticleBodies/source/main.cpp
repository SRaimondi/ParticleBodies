#include "quaternion.h"
#include <iostream>

int main(void) {

	pb::math::Quaternion<float> q = pb::math::quaternionFromAngleAxis(90.f, pb::math::vec3f({ 0.f, 1.f, 0.f }));

	// Transform point
	pb::math::vec3f p_prim = pb::math::transformVectorByQuaternion(q, pb::math::vec3f({ 3.f, 1.f, 0.f }));

	std::cout << p_prim(0) << " " << p_prim(1) << " " << p_prim(2) << std::endl;

	getchar();

	return 0;
}