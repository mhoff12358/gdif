#include "di_context.h"

#include <godot_cpp/variant/utility_functions.hpp>

#include "exported_registry.h"

const char* DIContext::k_className = "DIContext";

template <typename T, typename... Rest>
inline std::size_t hash_combine(std::size_t& seed, std::size_t v, Rest... rest)
{
    seed ^= v + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    hash_combine(seed, rest...);
    return seed;
}

std::size_t hasha::operator()(const godot::String& value) const
{
    return value.hash();
}

std::size_t hashb::operator()(const std::tuple<String, String>& value) const
{
    hasha value_hasher;
    auto hash1 = value_hasher(std::get<0>(value));
    return hash1 ^ (value_hasher(std::get<1>(value)) + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2));
}

DIContext::DIContext()
{
}

DIContext::~DIContext()
{
}

Node* DIContext::get_registered_node(String type)
{
    return get_registered_node_with_id(type, "");
}

Node* DIContext::get_registered_node_with_id(String type, String id)
{
    auto registeredNode = _registered_nodes.find(std::make_tuple(type, id));
    if (registeredNode == _registered_nodes.end())
    {
        if (_parent_context.has_value())
        {
            auto parent_result = _parent_context->get_strong_handle()->get_registered_node_with_id(type, id);
            if (parent_result == nullptr)
            {
                UtilityFunctions::print("Getting a non-existent node from parent with type ", type);
            }
            return parent_result;
        }
        UtilityFunctions::print("Getting a non-existent node with type ", type);
        return nullptr;
    }
    return registeredNode->second.get_strong_handle().get();
}

void DIContext::register_node_of_type(Object* node, String type, String id)
{
    auto registration_tuple = std::make_tuple(type, id);
    auto existingRegistration = _registered_nodes.find(registration_tuple);
    if (existingRegistration != _registered_nodes.end())
    {
        UtilityFunctions::print("Double registering a node with type ", type);
    }
    _registered_nodes.insert(existingRegistration, std::make_pair(registration_tuple, node));

    if (_parent_context.has_value())
    {
        StrongObjectHandle<DIContext> parent = _parent_context->get_strong_handle();

        for (int i = 0; i < _exports.size(); i++)
        {
            Variant uncast_export = _exports[i];
            if (uncast_export.get_type() == Variant::OBJECT)
            {
                ExportedRegistry* possible_export = dynamic_cast<ExportedRegistry*>(static_cast<Object*>(uncast_export));
                if (possible_export == nullptr)
                {
                    UtilityFunctions::print("Registry export that isn't an ExportedRegistry");
                    continue;
                }
                if ((possible_export->get_type() == type) && (possible_export->get_id() == id))
                {
                    String mapped_type = type;
                    if (!possible_export->get_mapped_type().is_empty())
                    {
                        mapped_type = possible_export->get_mapped_type();
                    }
                    String mapped_id = possible_export->get_mapped_id();
                    parent->register_node_of_type(node, mapped_type, mapped_id);
                    return;
                }
            }
        }
    }
}

void DIContext::register_node(Object* node, String id)
{
    String type;
    if (node->has_method("_di_name"))
    {
        Variant di_name = node->call("_di_name");
        if (di_name.get_type() == Variant::STRING)
        {
            type = di_name;
        }
    }
    else
    {
        type = node->get_class();
    }
    register_node_of_type(node, type, id);
}

void DIContext::set_exported_registries(Array exports)
{
    _exports = exports;
}

Array DIContext::get_exported_registries() const
{
    return _exports;
}

DIContext* DIContext::get_nearest(Object* node)
{
    Node* current = cast_to<Node>(node);
    Node* parent = current->get_parent();
    while (parent != nullptr)
    {
        // DIContext* parentAsContext = cast_to<DIContext>(parent);
        DIContext* parentAsContext = dynamic_cast<DIContext*>(parent);
        if (parentAsContext != nullptr)
        {
            return parentAsContext;
        }
        parent = parent->get_parent();
    }
    return nullptr;
}

void DIContext::_enter_tree()
{
    DIContext* parent = get_nearest(this);
    if (parent != nullptr)
    {
        _parent_context.emplace(parent);
    }
}

void DIContext::_bind_methods()
{
    ClassDB::bind_static_method(k_className, D_METHOD("get_nearest", "node"), &DIContext::get_nearest);

    ClassDB::bind_method(D_METHOD("get_registered_node", "type"), &DIContext::get_registered_node);
    ClassDB::bind_method(D_METHOD("get_registered_node_with_id", "type", "id"), &DIContext::get_registered_node_with_id);
    ClassDB::bind_method(D_METHOD("register_node", "node"), &DIContext::register_node);
    ClassDB::bind_method(D_METHOD("register_node_of_type", "node"), &DIContext::register_node_of_type);

    ClassDB::bind_method(D_METHOD("set_exported_registries", "exports"), &DIContext::set_exported_registries);
    ClassDB::bind_method(D_METHOD("get_exported_registries"), &DIContext::get_exported_registries);
    auto array_hint = std::to_string(Variant::OBJECT) + "/" + std::to_string(PROPERTY_HINT_RESOURCE_TYPE) + ":" + "ExportedRegistry";
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "exported_registries", PROPERTY_HINT_ARRAY_TYPE, array_hint.c_str()), "set_exported_registries", "get_exported_registries");
}
