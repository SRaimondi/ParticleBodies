#pragma once

class EulerSolver {
public:
	// Solve onestep of ODE using Euler method
	static inline void odeStep(float const y0[], float const ydot[], float yend[],
							   size_t const len, float const t0, float const t1) {
		for (size_t i = 0; i < len; i++) {
			yend[i] = y0[i] + (t1 - t0) * ydot[i];
		}
	}
};