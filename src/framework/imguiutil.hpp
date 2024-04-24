#pragma once

#include <glm/glm.hpp>

#include <string>
#include <vector>

namespace ImGui {

    void StatisticsWindow(float frametime, const glm::vec2& resolution);
    bool SphericalSlider(const char* label, glm::vec3& cartesian);
    bool AngleSlider3(const char* label, glm::vec3& angles);
    bool Combo(const char* label, int* curr, const std::vector<std::string>& items);
    bool Combo(const char* label, unsigned int* curr, const std::vector<std::string>& items);
    bool FlagCheckbox(const char* label, unsigned int* flags, unsigned int flagBitIndex);

}