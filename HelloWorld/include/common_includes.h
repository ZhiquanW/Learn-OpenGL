#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/string_cast.hpp>
#include <stdexcept>
#include <iostream>
#include <filesystem>
#include <algorithm>
#include <memory>
#include <numeric>
#include <unordered_map>
#include <cmath>
#include <vector>
#include <deque>
#include <set>
#include <map>
#include <typeinfo>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include "stb_image.h"
#include <fmt/core.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "deps/nlohmann_json/json.hpp"

#include "deps/cista.h"
#define quote(x) #x
