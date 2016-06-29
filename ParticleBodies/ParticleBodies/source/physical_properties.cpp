#include "physical_properties.h"

namespace pb {

	void stateToArray(PhysicalProperties const * const rb, float * y) {
		// Copy position
		*y++ = rb->x(0);
		*y++ = rb->x(1);
		*y++ = rb->x(2);

		// Copy orientation
		*y++ = rb->q.getReal();
		*y++ = rb->q.getImmaginary()(0);
		*y++ = rb->q.getImmaginary()(1);
		*y++ = rb->q.getImmaginary()(2);

		// Copy linear momentum
		*y++ = rb->P(0);
		*y++ = rb->P(1);
		*y++ = rb->P(2);

		// Copy angular momentum
		*y++ = rb->L(0);
		*y++ = rb->L(1);
		*y = rb->L(2);
	}

	void arrayToState(PhysicalProperties * const rb, float const * y) {
		// Copy position
		rb->x(0) = *y++;
		rb->x(1) = *y++;
		rb->x(2) = *y++;

		// Copy orientation
		rb->q.getReal() = *y++;
		rb->q.getImmaginary()(0) = *y++;
		rb->q.getImmaginary()(1) = *y++;
		rb->q.getImmaginary()(2) = *y++;

		// Copy linear momentum
		rb->P(0) = *y++;
		rb->P(1) = *y++;
		rb->P(2) = *y++;

		// Copy angular momentum
		rb->L(0) = *y++;
		rb->L(1) = *y++;
		rb->L(2) = *y++;

		//
		// Compute auxiliary variables
		//
		// Compute velocity
		rb->v = rb->P / rb->mass;

		// Compute inverse of inertia tensor
		rb->inv_inertia_tensor = rb->R * rb->inv_inertia_tensor_body * math::transpose(rb->R);

		// Compute angular velocity
		rb->omega = rb->inv_inertia_tensor * rb->L;
	}

	void ddtStateToArray(PhysicalProperties const * const rb, float * y_dot) {
		// Copy velocity
		*y_dot++ = rb->v(0);
		*y_dot++ = rb->v(1);
		*y_dot++ = rb->v(2);

		// Copy quaternion derivative
		math::Quaternion<float> dq_dt = (math::Quaternion<float>(0.f, rb->omega) * rb->q) * 0.5f;
		*y_dot++ = dq_dt.getReal();
		*y_dot++ = dq_dt.getImmaginary()(0);
		*y_dot++ = dq_dt.getImmaginary()(1);
		*y_dot++ = dq_dt.getImmaginary()(2);

		// Copy force
		*y_dot++ = rb->force(0);
		*y_dot++ = rb->force(1);
		*y_dot++ = rb->force(2);

		// Copy torque
		*y_dot++ = rb->torque(0);
		*y_dot++ = rb->torque(1);
		*y_dot++ = rb->torque(2);
	}

} // pb namespace