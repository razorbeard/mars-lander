#ifndef RESOURCE_HOLDER_HPP
#define RESOURCE_HOLDER_HPP

#include <unordered_map>
#include <string>
#include <memory>
#include <stdexcept>
#include <cassert>

template <typename Resource, typename Identifier>
class ResourceHolder
{
public:

    void load(Identifier id, const std::string& filename)
    {
        std::unique_ptr<Resource> resource(new Resource());
        if (!resource->loadFromFile(filename))
            throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);

        insertResource(id, std::move(resource));
    }
    
    template <typename Parameter>
    void load(Identifier id, const std::string& filename, const Parameter& secondParam)
    {
        std::unique_ptr<Resource> resource(new Resource());
        if (!resource->loadFromFile(filename, secondParam))
            throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);

        insertResource(id, std::move(resource));
    }
    
    Resource& get(Identifier id)
    {
        auto found = m_resourceMap.find(id);
        assert(found != m_resourceMap.end());

        return *found->second;
    }
    
    const Resource& get(Identifier id) const
    {
        auto found = m_resourceMap.find(id);
        assert(found != m_resourceMap.end());

        return *found->second;
    }

private:

    void insertResource(Identifier id, std::unique_ptr<Resource> resource)
    {
        auto inserted = m_resourceMap.insert(std::make_pair(id, std::move(resource)));
        assert(inserted.second);
    }

private:
    std::unordered_map<Identifier, std::unique_ptr<Resource>> m_resourceMap;
};

#endif
