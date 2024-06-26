#pragma once

#include <glm/glm.hpp>

#include <string>
#include <vector>

/**
 * @file imguiutil.hpp
 * @brief Defines common ImGui elements missing from the main library.
 */

namespace ImGui {

    /** Number of frames to average for frame time smoothing, used by `void StatisticsWindow()` */
    const unsigned int FRAMETIME_SMOOTHING = 60;

    /**
     * @brief Draws a window with average frame time, frames per second, and resolution.
     */
    void StatisticsWindow(float frametime, const glm::vec2& resolution);

    /**
     * @brief Slider to select a vector on the unit sphere using two angles.
     */
    bool SphericalSlider(const char* label, glm::vec3& cartesian);

    /**
     * @brief Slider for three angles in degrees.
     */
    bool AngleSlider3(const char* label, glm::vec3& angles);

    /**
     * @brief Combo box for selecting an item from a vector of strings.
     */
    bool Combo(const char* label, int* curr, const std::vector<std::string>& items);

    /**
     * @brief Combo box for selecting an item from a vector of strings.
     */
    bool Combo(const char* label, unsigned int* curr, const std::vector<std::string>& items);

    /**
     * @brief Combo box for selecting an item from a vector of strings.
     */
    bool Combo(const char* label, size_t* curr, const std::vector<std::string>& item);

    /**
     * @brief Checkbox for a single bit in a bitfield.
     */
    bool FlagCheckbox(const char* label, unsigned int* flags, unsigned int flagBitIndex);

}