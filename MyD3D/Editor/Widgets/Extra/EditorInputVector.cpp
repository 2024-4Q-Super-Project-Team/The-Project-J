#include "pch.h"
#include "EditorInputVector.h"

namespace Editor
{
    _EDITOR_WIDGET_INPUT_CLASS_CPP_DEFINE_NORMAL(InputInt,  ImGui::InputInt);
    _EDITOR_WIDGET_INPUT_CLASS_CPP_DEFINE_NORMAL(InputFloat, ImGui::InputFloat);
    _EDITOR_WIDGET_INPUT_CLASS_CPP_DEFINE_VECTOR(InputVector2, ImGui::InputFloat2);
    _EDITOR_WIDGET_INPUT_CLASS_CPP_DEFINE_VECTOR(InputVector3, ImGui::InputFloat3);
    _EDITOR_WIDGET_INPUT_CLASS_CPP_DEFINE_VECTOR(InputVector4, ImGui::InputFloat4);
    
}