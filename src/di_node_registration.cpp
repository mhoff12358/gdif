#include "di_node_registration.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "di_context.h"

DINodeRegistration::DINodeRegistration()
{
}

DINodeRegistration::~DINodeRegistration()
{
}

void DINodeRegistration::_enter_tree()
{
    if (Engine::get_singleton()->is_editor_hint())
    {
        return;
    }

    NodePath to_register = _registered_node;
    if (to_register == NodePath())
    {
        to_register = NodePath("..");
    }
    auto nearest_context = DIContext::get_nearest(this);
    if (nearest_context == nullptr)
    {
        UtilityFunctions::print("DINodeRegistration with no nearest context");
        return;
    }
    Node* node_to_register = get_node<Node>(to_register);
    if (node_to_register == nullptr)
    {
        UtilityFunctions::print("DINodeRegistration for non-existent node");
        return;
    }
    if (_type == "")
    {
        nearest_context->register_node(node_to_register, _id);
    }
    else
    {
        nearest_context->register_node_of_type(node_to_register, _type, _id);
    }
}

void DINodeRegistration::set_registered_node(NodePath path)
{
    _registered_node = path;
}

NodePath DINodeRegistration::get_registered_node() const
{
    return _registered_node;
}

void DINodeRegistration::set_registration_id(String id)
{
    _id = id;
}

String DINodeRegistration::get_registration_id() const
{
    return _id;
}

void DINodeRegistration::set_registration_type(String type)
{
    _type = type;
}

String DINodeRegistration::get_registration_type() const
{
    return _type;
}

void DINodeRegistration::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("set_registered_node", "path"), &DINodeRegistration::set_registered_node);
    ClassDB::bind_method(D_METHOD("get_registered_node"), &DINodeRegistration::get_registered_node);
    ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "registered_node"), "set_registered_node", "get_registered_node");

    ClassDB::bind_method(D_METHOD("set_registration_id", "id"), &DINodeRegistration::set_registration_id);
    ClassDB::bind_method(D_METHOD("get_registration_id"), &DINodeRegistration::get_registration_id);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "registration_id"), "set_registration_id", "get_registration_id");

    ClassDB::bind_method(D_METHOD("set_registration_type", "type"), &DINodeRegistration::set_registration_type);
    ClassDB::bind_method(D_METHOD("get_registration_type"), &DINodeRegistration::get_registration_type);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "registration_type"), "set_registration_type", "get_registration_type");
}