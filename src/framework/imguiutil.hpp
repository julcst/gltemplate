#pragma once

#include <glm/glm.hpp>

#include <string>
#include <vector>
#include <filesystem>
#include <imgui.h>

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
    bool Combo(const char* label, size_t* curr, const std::vector<std::string>& items);

    /**
     * @brief Checkbox for a single bit in a bitfield.
     */
    bool FlagCheckbox(const char* label, unsigned int* flags, unsigned int flag);

    /**
     * @brief File combo box for selecting a file from a vector of paths.
     */
    bool FileCombo(const char* label, int* curr, const std::vector<std::filesystem::path>& items);

    /**
     * @brief Combo box for selecting an item from an enum type.
     */
    template <typename T, size_t N>
    inline bool EnumCombo(const char* label, T* curr, const std::array<std::string_view, N>& labels) {
        return ImGui::Combo(
            label, reinterpret_cast<int*>(curr),
            [](void* data, int idx, const char** out_text) {
                auto labels = reinterpret_cast<const std::array<std::string_view, N>*>(data);
                *out_text = labels->at(idx).data();
                return true;
            },
            const_cast<void*>(reinterpret_cast<const void*>(&labels)),
            N
        );
    }

}