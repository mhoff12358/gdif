#pragma once

#ifdef WIN32
#include <windows.h>
#endif

#include <godot_cpp/classes/resource.hpp>

using namespace godot;

class ExportedRegistry : public Resource
{
    GDCLASS(ExportedRegistry, Resource);

public:
    ExportedRegistry();
    virtual ~ExportedRegistry();

    void set_type(String type);
    String get_type() const;
    void set_id(String id);
    String get_id() const;
    void set_mapped_type(String type);
    String get_mapped_type() const;
    void set_mapped_id(String id);
    String get_mapped_id() const;

protected:
    static void _bind_methods();

private:
    String _type;
    String _id;
    String _mapped_type;
    String _mapped_id;
};
