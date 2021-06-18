#pragma once

#include <string>
#include <vector>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

template <class T>
using Vec = std::vector<T>;
using Vec4 = glm::vec<4, float>;

using String = std::string;

#define max(x, y) (x) > (y) ? (x) : (y)
#define min(x, y) (x) < (y) ? (x) : (y)