#include "quaternion.h"

int main(void) {

	pb::math::Quaternion<float> q = pb::math::Quaternion<float>();

	float t = pb::math::magnitude(q);

	return 0;
}