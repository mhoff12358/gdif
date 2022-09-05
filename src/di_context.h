#pragma once

#ifdef WIN32
#include <windows.h>
#endif

#include <godot_cpp/classes/node2d.hpp>

#include <optional>

#include "object_handle.h"

using namespace godot;

// template <>
// struct hash<String>
struct hasha
{
    std::size_t operator()(const godot::String& value) const;
};

// template <>
// struct hash<std::tuple<String, String>>
struct GDN_EXPORT hashb
{
    std::size_t operator()(const std::tuple<String, String>& value) const;
};

class GDN_EXPORT DIContext : public Node2D
{
    GDCLASS(DIContext, Node2D);

public:
    DIContext();
    virtual ~DIContext();

public:
    Node* get_registered_node(String type);
    Node* get_registered_node_with_id(String type, String id);
    void register_node_of_type(Object* node, String type, String id);
    void register_node(Object* node, String id);

    void set_exported_registries(Array exports);
    Array get_exported_registries() const;

    static DIContext* get_nearest(Object* node);
    static const char* k_className;

    void _enter_tree() override;

protected:
    static void _bind_methods();

private:
    std::unordered_map<std::tuple<String, String>, ObjectHandle<Node>, hashb> _registered_nodes;
    std::optional<ObjectHandle<DIContext>> _parent_context;
    Array _exports;
};
