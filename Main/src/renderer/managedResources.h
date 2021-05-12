# pragma once

#include <string>
#include <unordered_map>

// TODO: make existing resources private and make it useable with iterator (expose iterator for pairs in the map) (only affects the use case where you want to iterate over the currently loaded resources)
// NOTE: requires T to have "GetDefault()" method, and support move semantics
template<typename T>
class ManagedResources {
   public:
    // NOTE: This static variable would be on every type of managed resources object, which is not really ideal
    inline static std::string DEFAULT_RES_NAME = "default";
    std::unordered_map<std::string, T> existingResources;

    ManagedResources() {}

    ManagedResources(const ManagedResources& s) = delete;

    // moves the input resource into this structure (res manager takes ownership of the object)
    bool OwnRes(const std::string& name, T&& res) {
        if (existingResources.find(name) != existingResources.end() || name == DEFAULT_RES_NAME) return false;
        existingResources.insert({std::move(name), std::move(res)});
        return true;
    }

    bool DeleteRes(const std::string& name) {
        // cannot remove default res
        if (existingResources.find(name) == existingResources.end() || name == DEFAULT_RES_NAME) return false;
        existingResources.erase(name);
        return true;
    }
    
    const T* GetRes(const std::string& name) {
        if (existingResources.find(name) == existingResources.end() || name == DEFAULT_RES_NAME) return T::GetDefault();
        return &existingResources[name];
    }
};