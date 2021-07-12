#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

#include "shader.h"
#include "texture.h"

namespace Renderer {

// TODO: make existing resources private and make it useable with iterator (expose iterator for pairs in the map) (only affects the use case where you want to iterate over the currently loaded resources)
// TODO: implement move constructor and assignment

// NOTE: requires T to have "GetDefault()" method, and support move semantics
template <typename T>
class ManagedResources {
   private:
    // NOTE: All resources used with the resource manager need to have a map for default resources, and a getter method for it
    const T* ResolveName(const std::string& name) {
        auto defaults = T::GetDefaultMap().find(name);
        if (defaults != T::GetDefaultMap().end()) return &defaults->second;

        auto existing = existingResources.find(name);
        if (existing != existingResources.end()) return &existing->second;

        return nullptr;
    }

   public:
    std::unordered_map<std::string, T> existingResources;

    ManagedResources() {}

    ManagedResources(const ManagedResources& s) = delete;

    // moves the input resource into this structure (res manager takes ownership of the object)
    bool OwnRes(const std::string& name, T&& res) {
        if (ResolveName(name) != nullptr) return false;
        existingResources.insert({name, std::move(res)});
        return true;
    }

    bool DeleteRes(const std::string& name) {
        // cannot remove default res
        if (ResolveName != nullptr) return false;
        existingResources.erase(name);
        return true;
    }

    const T* GetRes(const std::string& name) {
        auto res = ResolveName(name);
        if (res == nullptr) return &T::GetDefaultMap().begin()->second;
        return res;
    }
};

}  // namespace Renderer

template <typename T, typename U>
std::ostream& operator<<(std::ostream& os, const std::pair<T, U>& p) {
    os << "{ " << p.first << " , " << p.second << " }";
    return os;
}

template <typename T, typename U>
std::ostream& operator<<(std::ostream& os, const std::unordered_map<T, U>& umap) {
    os << "===== unordered map =======";

    for (auto& x : umap) {
        os << "\n{ " << x.first << " , " << x.second << " }";
    }

    os << "\n==========================";
    return os;
}