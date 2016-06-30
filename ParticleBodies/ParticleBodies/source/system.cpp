#include "system.h"
#include "body.h"
#include "euler.h"

namespace pb {

	System::System(float const t0, float const dt)
		: t(t0), delta_t(dt) {}

	void System::addBody(BodyParticlesDiscretisation * const body) {
		bodies.push_back(body);
	}

	void System::computeForceAndTorque(float const time) const {
		// Add gravity to all bodies
		for (auto body = bodies.begin(); body != bodies.end(); body++) {
			(*body)->getBody()->resetForce();
			(*body)->getBody()->resetTorque();

			// Add gravity
			(*body)->getBody()->addForce(math::vec3f({0.f, -0.1f, 0.f}));
		}
	}

	void System::computeStep() {
		// Allocate space
		float * y0 = new float[PhysicalProperties::STATE_SIZE * bodies.size()];
		float * y_end = new float[PhysicalProperties::STATE_SIZE * bodies.size()];
		float * y_dot = new float[PhysicalProperties::STATE_SIZE * bodies.size()];

		computeForceAndTorque(t);

		// Insert body state to array
		for (size_t i = 0; i < bodies.size(); i++) {
			bodies[i]->getBody()->bodyStateToArray(&y0[i * PhysicalProperties::STATE_SIZE]);
			bodies[i]->getBody()->ddtBodyStateToArray(&y_dot[i * PhysicalProperties::STATE_SIZE]);
		}

		// Compute step
		EulerSolver::odeStep(y0, y_dot, y_end, PhysicalProperties::STATE_SIZE * bodies.size(), t, t + delta_t);

		// Update body state
		for (size_t i = 0; i < bodies.size(); i++) {
			bodies[i]->getBody()->arrayToBodyState(&y_end[i * PhysicalProperties::STATE_SIZE]);
		}

		// Update time
		t += delta_t;
	}

	void System::draw(GLuint const v_p, GLuint const i_p, GLuint const n_p,
					  GLuint const v_s, GLuint const i_s, GLuint const n_s) const {
		for (auto it = bodies.begin(); it != bodies.end(); it++) {
			// DEBUG draw particles
			//(*it)->drawParticles(v_p, i_p, n_p);
			// Draw body
			(*it)->getBody()->drawBody(v_s, i_s, n_s);
		}
	}

} // pb namespace