#pragma once

#ifdef WIN32
#include <windows.h>
#endif

#include <optional>

#include <godot_cpp/core/object.hpp>

using namespace godot;

template <typename ObjectType>
class StrongObjectHandle;

template <typename ObjectType>
class ObjectHandle
{
public:
    ObjectHandle(Object* node)
        : _instance_id(node->get_instance_id()) {}

    StrongObjectHandle<ObjectType> get_strong_handle() const;

    static std::optional<ObjectHandle> create(Object* node)
    {
        std::optional<ObjectHandle> result;
        if (node != nullptr)
        {
            result.emplace(node);
        }
        return result;
    }

    bool operator==(const ObjectHandle<ObjectType>& other) const
    {
        return _instance_id == other._instance_id;
    }

    uint64_t get_id() const
    {
        return _instance_id;
    }

private:
    uint64_t _instance_id;
};

template <typename ObjectType>
class StrongObjectHandle
{
public:
    StrongObjectHandle(ObjectType* object) : _object(object) {}

    ObjectType* operator->()
    {
        return _object;
    }

    ObjectType* get()
    {
        return _object;
    }

    ObjectHandle<ObjectType> get_weak_handle() const;

    bool operator==(const StrongObjectHandle<ObjectType>& other) const
    {
        return _object == other._object;
    }

    bool operator==(const nullptr_t null) const
    {
        return _object == nullptr;
    }

private:
    ObjectType* _object;
};

template <typename ObjectType>
StrongObjectHandle<ObjectType> ObjectHandle<ObjectType>::get_strong_handle() const
{
    return StrongObjectHandle<ObjectType>(static_cast<ObjectType*>(ObjectDB::get_instance(_instance_id)));
}

template <typename ObjectType>
ObjectHandle<ObjectType> StrongObjectHandle<ObjectType>::get_weak_handle() const
{
    return ObjectHandle<ObjectType>(_object);
}