#include "exported_registry.h"

ExportedRegistry::ExportedRegistry() {}
ExportedRegistry::~ExportedRegistry() {}

void ExportedRegistry::set_type(String type)
{
    _type = type;
}
String ExportedRegistry::get_type() const
{
    return _type;
}
void ExportedRegistry::set_id(String id)
{
    _id = id;
}
String ExportedRegistry::get_id() const
{
    return _id;
}
void ExportedRegistry::set_mapped_type(String type)
{
    _mapped_type = type;
}
String ExportedRegistry::get_mapped_type() const
{
    return _mapped_type;
}
void ExportedRegistry::set_mapped_id(String id)
{
    _mapped_id = id;
}
String ExportedRegistry::get_mapped_id() const
{
    return _mapped_id;
}

void ExportedRegistry::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("set_type", "exports"), &ExportedRegistry::set_type);
    ClassDB::bind_method(D_METHOD("get_type"), &ExportedRegistry::get_type);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "type"), "set_type", "get_type");

    ClassDB::bind_method(D_METHOD("set_id", "exports"), &ExportedRegistry::set_id);
    ClassDB::bind_method(D_METHOD("get_id"), &ExportedRegistry::get_id);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "id"), "set_id", "get_id");

    ClassDB::bind_method(D_METHOD("set_mapped_type", "exports"), &ExportedRegistry::set_mapped_type);
    ClassDB::bind_method(D_METHOD("get_mapped_type"), &ExportedRegistry::get_mapped_type);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "mapped_type"), "set_mapped_type", "get_mapped_type");

    ClassDB::bind_method(D_METHOD("set_mapped_id", "exports"), &ExportedRegistry::set_mapped_id);
    ClassDB::bind_method(D_METHOD("get_mapped_id"), &ExportedRegistry::get_mapped_id);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "mapped_id"), "set_mapped_id", "get_mapped_id");
}
