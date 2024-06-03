#include "imguiutil.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>

#include "config.hpp"
#include "framework/series.hpp"

using namespace glm;

void ImGui::StatisticsWindow(float frametime, const vec2& resolution) {
    static Series<float, Config::FRAMETIME_SMOOTHING> measurements;
    measurements.push(frametime);
    float avg = measurements.avg;

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.5f));
    ImGui::Begin("Statistics", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings);
    ImGui::Text("%2.1ffps (%2.1ffps) | %2.1fms (%2.1fms) | %.0fx%.0f", 1.0f / avg, 1.0f / frametime, avg * 1000.0f, frametime * 1000.0f, resolution.x, resolution.y);
    ImGui::PopStyleColor();
    ImGui::End();
}

bool ImGui::SphericalSlider(const char* label, vec3& cart) {
    vec2 sph = vec2(asin(cart.y), atan(cart.x, cart.z));
    ImGui::PushID(label);
    bool changed = false;
    ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());
    ImGui::PushID(0);
    changed |= ImGui::SliderAngle("", &sph.x, -89.0f, 89.0f);
    ImGui::PopItemWidth();
    ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
    ImGui::PopID();
    ImGui::PushID(1);
    changed |= ImGui::SliderAngle("", &sph.y);
    ImGui::PopItemWidth();
    ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
    ImGui::PopID();
    ImGui::TextUnformatted(label);
    ImGui::PopID();
    if (changed) cart = vec3(cos(sph.x) * sin(sph.y), sin(sph.x), cos(sph.x) * cos(sph.y));
    return changed;
}

bool ImGui::AngleSlider3(const char* label, vec3& angles) {
    vec3 anglesDeg = degrees(angles);
    bool changed = ImGui::SliderFloat3(label, value_ptr(anglesDeg), -360.0f, 360.0f, "%.0f deg");
    if (changed) angles = radians(anglesDeg);
    return changed;
}

bool ImGui::Combo(const char* label, int* curr, const std::vector<std::string>& items) {
    return ImGui::Combo(
        label, curr,
        [](void* data, int idx, const char** out_text) {
            auto items = reinterpret_cast<const std::vector<std::string>*>(data);
            // if (idx < 0 || idx >= items->size()) return false; // unnecessary
            *out_text = items->at(idx).c_str();
            return true;
        },
        const_cast<void*>(reinterpret_cast<const void*>(&items)), items.size());
}

bool ImGui::Combo(const char* label, unsigned int* curr, const std::vector<std::string>& items) {
    return ImGui::Combo(label, reinterpret_cast<int*>(curr), items);
}

bool ImGui::Combo(const char* label, size_t* curr, const std::vector<std::string>& items) {
    return ImGui::Combo(label, reinterpret_cast<int*>(curr), items);
}

bool ImGui::FlagCheckbox(const char* label, unsigned int* flags, unsigned int flagBitIndex) {
    bool flag = (*flags & (1 << flagBitIndex)) != 0;
    bool changed = ImGui::Checkbox(label, &flag);
    if (changed) {
        if (flag) *flags |= (1 << flagBitIndex);
        else *flags &= ~(1 << flagBitIndex);
    }
    return changed;
}