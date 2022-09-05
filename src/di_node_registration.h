#pragma once

#ifdef WIN32
#include <windows.h>
#endif

#include <godot_cpp/classes/node.hpp>

#include <optional>
#include <unordered_map>

#include "object_handle.h"

using namespace godot;

class DINodeRegistration : public Node
{
    GDCLASS(DINodeRegistration, Node);

public:
    DINodeRegistration();
    virtual ~DINodeRegistration();

public:
    void _enter_tree();

    void set_registered_node(NodePath path);
    NodePath get_registered_node() const;

    void set_registration_type(String type);
    String get_registration_type() const;

    void set_registration_id(String id);
    String get_registration_id() const;

protected:
    static void _bind_methods();

private:
    NodePath _registered_node;
    String _id;
    String _type;
};