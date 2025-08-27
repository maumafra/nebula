#pragma once

#include "Definitions.hpp"

#include <unordered_map>
#include <typeinfo>
#include <string>
#include <set>

namespace nebula {
    namespace ecs {

struct EntityIdIndex {
    EntityId nextId;
    std::set<EntityId> unalivedIds;
};

inline EntityId getNewId(EntityIdIndex *index) {
    if (index->unalivedIds.size() != 0) {
        /* Use unalived id */
        EntityId id = *index->unalivedIds.begin();
        index->unalivedIds.erase(index->unalivedIds.begin());
        return id;
    }

    EntityId id = index->nextId ++;
    return id;
};

inline void setIdUnalived(EntityIdIndex *index, EntityId id) {
    index->unalivedIds.insert(id);
};

inline bool isIdUnalived(EntityIdIndex *index, EntityId id) {
    if (*index->unalivedIds.end() == id) {
        return true;
    }
    return *index->unalivedIds.find(id) == id;
};

struct ComponentIdIndex {
    ComponentId nextId = 1; //lazy way to check if a component exists, since no component will be registered with an id of 0
    std::unordered_map<std::string, ComponentId> typeIdIndex;
};

template <typename T>
inline ComponentId getNewId(ComponentIdIndex *index) {
    ComponentId id = index->nextId ++;
    index->typeIdIndex.insert({std::string(typeid(T).name()), id});
    return id;
};

template <typename T>
inline ComponentId getComponentId(ComponentIdIndex *index) {
    if (index->typeIdIndex.count(std::string(typeid(T).name())) == 0) {
        return 0;
    }
    return index->typeIdIndex[std::string(typeid(T).name())];
};

struct TableIdIndex {
    TableId nextId;
    std::unordered_map<TypeHash, TableId> typeHashIndex;
};

inline TableId getNewId(TableIdIndex *index, TypeHash th) {
    TableId id = index->nextId ++;
    index->typeHashIndex.insert({th, id});
    return id;
};

inline TableId getId(TableIdIndex *index, TypeHash th) {
    return index->typeHashIndex.at(th);
};

inline bool hasId(TableIdIndex *index, TypeHash th) {
    return index->typeHashIndex.count(th) != 0;
};

}// ecs
}// nebula