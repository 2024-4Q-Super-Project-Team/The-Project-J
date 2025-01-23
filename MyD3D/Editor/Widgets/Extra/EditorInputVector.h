#pragma once
#include "Editor/Interface/IEditorObject.h"

#define _EDITOR_WIDGET_INPUT_CLASS_HEADER_DEFINE(name, type)\
class name : public IWidget\
{\
public:\
    explicit name(const char* _label, void* _pValue = nullptr)\
        : mLabel(_label), mValue(reinterpret_cast<type*>(_pValue)) {} \
    virtual ~name() = default;\
public:\
    inline  void Set##type(type* _pVector) { mValue = _pVector; }\
    virtual void Render() override;\
private:\
    const char* mLabel;\
    type* mValue;\
};\

#define _EDITOR_WIDGET_INPUT_CLASS_CPP_DEFINE_NORMAL(name, func)\
void name::Render(){\
std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));\
if (mValue) func((mLabel + uid).c_str(), mValue);\
}\

#define _EDITOR_WIDGET_INPUT_CLASS_CPP_DEFINE_VECTOR(name, func)\
void name::Render(){\
std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));\
if (mValue) func((mLabel + uid).c_str(), &mValue->x);\
}\

#define _EDITOR_WIDGET_DRAG_CLASS_CPP_DEFINE_NORMAL(name, func)\
void name::Render(){\
std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));\
if (mValue) func((mLabel + uid).c_str(), mValue, 0.05f);\
}\

#define _EDITOR_WIDGET_DRAG_CLASS_CPP_DEFINE_VECTOR(name, func)\
void name::Render(){\
std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));\
if (mValue) func((mLabel + uid).c_str(), &mValue->x, 0.05f);\
}\

namespace Editor
{
    _EDITOR_WIDGET_INPUT_CLASS_HEADER_DEFINE(InputInt,      INT);
    _EDITOR_WIDGET_INPUT_CLASS_HEADER_DEFINE(InputFloat,    FLOAT);
    _EDITOR_WIDGET_INPUT_CLASS_HEADER_DEFINE(InputVector2,  Vector2);
    _EDITOR_WIDGET_INPUT_CLASS_HEADER_DEFINE(InputVector3,  Vector3);
    _EDITOR_WIDGET_INPUT_CLASS_HEADER_DEFINE(InputVector4,  Vector4);

    _EDITOR_WIDGET_INPUT_CLASS_HEADER_DEFINE(DragInt, INT);
    _EDITOR_WIDGET_INPUT_CLASS_HEADER_DEFINE(DragFloat, FLOAT);
    _EDITOR_WIDGET_INPUT_CLASS_HEADER_DEFINE(DragVector2, Vector2);
    _EDITOR_WIDGET_INPUT_CLASS_HEADER_DEFINE(DragVector3, Vector3);
    _EDITOR_WIDGET_INPUT_CLASS_HEADER_DEFINE(DragVector4, Vector4);
}