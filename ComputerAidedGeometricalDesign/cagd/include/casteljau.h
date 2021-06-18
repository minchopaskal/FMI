#pragma once

#include <cassert>
#include <vector>
#include <glm/vec4.hpp>

using Vec4 = glm::vec<4, float>;

// TODO: calculate with Hornet for linear speed
Vec4 casteljau(const std::vector<Vec4> &points, unsigned int start, unsigned int end, float t) {
	assert(points.size() != 0 && t >= 0 && t <= 1);

	std::vector<Vec4> intermediate(points.cbegin() + start, points.cbegin() + end);

	for (unsigned int i = start; i < end - 1; ++i) {
		std::vector<Vec4> current(intermediate.size() - 1);

		// find b_j^(i)
#pragma omp parallel for
		for (unsigned int j = 0; j < current.size(); ++j) {
			current[j] = (1 - t) * intermediate[j] + t * intermediate[j + 1];
		}

		intermediate = std::move(current);
	}

	return intermediate[0];
}