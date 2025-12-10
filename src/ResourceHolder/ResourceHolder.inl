#include <cassert>
#include <exception>

template <typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::load(
    Identifier id, const std::string& filename
) {
    std::unique_ptr<Resource> resource(new Resource());

    if (!resource->loadFromFile(filename)) {
        throw std::runtime_error("Failed to load " + filename);
    }
    mResourceMap[id] = std::move(resource);
}

template <typename Resource, typename Identifier>
inline void ResourceHolder<Resource, Identifier>::load(
    Identifier id1, Identifier id2
) {
    mResourceMap[id1] = mResourceMap[id2];
}

template <typename Resource, typename Identifier>
Resource& ResourceHolder<Resource, Identifier>::get(Identifier id) {
    auto found = mResourceMap.find(id);
    assert(found != mResourceMap.end());
    return *found->second;
}

template <typename Resource, typename Identifier>
const Resource& ResourceHolder<Resource, Identifier>::get(Identifier id) const {
    auto found = mResourceMap.find(id);
    assert(found != mResourceMap.end());
    return *found->second;
}

template <typename Resource, typename Identifier>
Identifier ResourceHolder<Resource, Identifier>::getSharedID(
    Identifier id, const std::vector<Identifier>& possibleIDs
) const {
    auto found = mResourceMap.find(id);

    if (found == mResourceMap.end()) {
        return Identifier::None;
    }

    for (auto itr = mResourceMap.begin(); itr != mResourceMap.end(); ++itr) {
        if (itr->second == found->second) {
            for (auto possibleID : possibleIDs) {
                if (itr->first == possibleID) {
                    return possibleID;
                }
            }
        }
    }

    return Identifier::None;
}
