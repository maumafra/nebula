#pragma once

#include <unordered_map>
#include <iostream>

#include "Table.hpp"
#include "Type.hpp"
#include "IdStorage.hpp"

using namespace std;

namespace nebula {
    namespace ecs {

using TableMap = unordered_map<TableId, TableRecord*>;

struct World {
public:
    World() 
        : entity_index(NULL)
        , component_index(NULL) 
        , entityIdIndex(nullptr) {
        initWorld();
    }

    EntityId spawn() {
        EntityId id = getNewId(this->entityIdIndex);
        Table *t = this->table_index[0];
        Record *r = this->createRecord(id, t);
        this->entity_index[id] = r;
        return id;
    }

    template <typename T>
    void addComponent(EntityId id, T t) {
        if(this->entity_index.count(id) == 0) { // Entity does not exist
            return;
        }
        Record *r = this->entity_index[id];
        ComponentId c_id = getComponentId<T>(this->componentIdIndex);
        if(!c_id) { // Component not yet registered
            c_id = getNewId<T>(this->componentIdIndex); // So register it here
            this->component_index.insert({c_id, new TableMap()}); // Saving it on component_index
        }
        if(this->hasComponent(id, c_id)) { // Entity already has this component, so we override it
            uint32_t compIndex = this->getComponentColumn(c_id, r->table);
            Column<T>* col = (Column<T>*)r->table->columns.at(compIndex);
            col->data.at(r->row) = t;
            return;
        }
        // Entity does not have this component, which means we need to:
        // -> check if there is a table with those components
        //    -> if so, move the entity to this table
        //    -> if not, create a new table and then move the entity
        // -> delete the entity data on the old table
        // -> update entity data in entity_index
        // -> create component data in component_index for the new table
        //TODO if not check/create edges

        // v This could be the move_entity func
        uint32_t ent_index = r->row;
        Table *oldTable = r->table;
        Type newType = getNewTypeAdd(oldTable->type, c_id);
        Table* newTable = ensureTable(newType);
        this->moveTableComponentsAdd<T>(id, ent_index, oldTable, newTable, t);
        oldTable->entities.erase(oldTable->entities.begin() + ent_index); // Check
        this->rearrangeTableEntities(oldTable, ent_index);
        ent_index = newTable->entities.size();
        r->row = ent_index;
        r->table = newTable;
        newTable->entities.emplace_back(id);
    }

    template <typename T>
    void removeComponent(EntityId id) {
        if(this->entity_index.count(id) == 0) { // Entity does not exist
            return;
        }
        ComponentId c_id = getComponentId<T>(this->componentIdIndex);
        if(!c_id) { // Component not yet registered
            return;
        }
        if(!this->hasComponent(id, c_id)) { // Entity doesn't have this component
            return;
        }
        Record *r = this->entity_index[id];
        Table *oldTable = r->table;
        uint32_t componentIdx = getComponentColumn(c_id, oldTable);
        Type newType = getNewTypeRemove(oldTable->type, componentIdx);
        Table* newTable = ensureTable(newType);
        uint32_t ent_index = r->row;
        this->moveTableComponentsRemove<T>(id, ent_index, oldTable, newTable, componentIdx);
        oldTable->entities.erase(oldTable->entities.begin() + ent_index);
        this->rearrangeTableEntities(oldTable, ent_index);
        ent_index = newTable->entities.size();
        r->row = ent_index;
        r->table = newTable;
        newTable->entities.emplace_back(id);
    }

    template <typename T>
    T* getComponent(EntityId id) {
        if(this->entity_index.count(id) == 0) { // Entity does not exist
            return nullptr;
        }
        Record *r = this->entity_index[id];
        ComponentId c_id = getComponentId<T>(this->componentIdIndex);
        if (!c_id) { // Component does not exist
            return nullptr; 
        }
        Table* table = r->table;
        TableMap* tabMap = this->component_index[c_id];
        if (tabMap->count(table->id) == 0) { // Entity does not have this component
            return nullptr;
        }
        TableRecord* tabRec = tabMap->at(table->id);
        Column<T>* col = (Column<T>*)table->columns.at(tabRec->column);
        return &col->data.at(r->row);
    }

    template <typename T>
    bool hasComponent(EntityId id) {
        if(this->entity_index.count(id) == 0) { // Entity does not exist
            return false;
        }
        ComponentId c_id = getComponentId<T>(this->componentIdIndex);
        if(!c_id) { // Component not yet registered
            return false;
        }
        return this->hasComponent(id, c_id);
    }

    //template <typename T>
    //void removeComponent(EntityId id);

    template <typename T, typename... Ts>
    const std::vector<EntityId> getEntitiesWith() const {
        if constexpr (sizeof...(Ts) == 0) {
            return getEntitiesWithOne<T>();
        }
        return getEntitiesWithMultiple<T, Ts...>();
    }

    void print() {
        std::cout << "---- ENTITY INDEX ----" << std::endl;
        for (auto const &entPair : this->entity_index) {
            Record* r = entPair.second;
            std::cout << "Entity: " << entPair.first << " Table: " << r->table->id << " at " << r->row << std::endl; 
        }
        std::cout << "---- TABLE INDEX ----" << std::endl;
        for (auto const &tabPair : this->table_index) {
            std::cout << "Table: " << tabPair.second->id << "\n";
            std::cout << "      > Entities (" << tabPair.second->entities.size() << "): ";
            for (EntityId entId : tabPair.second->entities) {
                std::cout << entId << " ";
            }
            std::cout << "\n";
            std::cout << "      > Components : ";
            for (ComponentId compId : tabPair.second->type) {
                std::cout << compId << " ";
            }
            std::cout << "\n";
            std::cout << "      > Columns : ";
            for (ColumnBase* column : tabPair.second->columns) {
                column->debug();
                std::cout << ", ";
            }
            std::cout << "\n";
        }
    }

private:
    void initWorld() {
        entityIdIndex = new EntityIdIndex();
        componentIdIndex = new ComponentIdIndex();
        tableIdIndex = new TableIdIndex();
        ensureTable({}); // Create first (empty) table
    }

    Record* createRecord(EntityId entity, Table* table) {
        uint32_t recRow = table->entities.size();
        table->entities.push_back(entity);
        Record* r = new Record();
        r->row = recRow;
        r->table = table;
        return r;
    }

    //Table* getTable(const Type type) {
    //    TypeHash th = 0;
    //    if (type.size() != 0) {
    //        th = typeHash(type);
    //    }
    //    if (hasId(tableIdIndex, th)) {
    //        return table_index[getId(tableIdIndex, th)];
    //    }
    //    return nullptr;
    //}

    Table* ensureTable(const Type type) {
        TypeHash th = 0;
        if (type.size() != 0) {
            th = typeHash(type);
        }
        if (hasId(tableIdIndex, th)) {
            return table_index[getId(tableIdIndex, th)];
        }
        // there is no table for this type, so we create it
        Table *t = new Table();
        t->id = getNewId(tableIdIndex, th);
        t->type = type;
        t->columns = {};
        // save the table on table_index
        table_index.insert({t->id, t});
        return t;
    }

    bool hasComponent(EntityId entity, ComponentId component) {
        if (component_index.count(component) == 0) {
            return false;
        }
        Record *record = entity_index[entity];
        Table *table = record->table;
        TableMap *tableMap = component_index[component];
        return tableMap->count(table->id) != 0;
    }

    // This can be moved to Table.h file, dont forget to add the World* parameter
    template<typename T>
    void moveTableComponentsAdd(
        const EntityId eId, 
        const uint32_t entRow, 
        Table* oldTable, 
        Table* newTable,
        T newComponentData
    ) {
        for (ComponentId cId : newTable->type) {
            if (tableHasComponent(cId, oldTable)) { // components from previous table
                // get component index on the table
                uint32_t compIndex = this->getComponentColumn(cId, oldTable);

                // get component column
                ColumnBase* oldCol = oldTable->columns.at(compIndex);
                ColumnBase* newCol;

                if (this->tableHasComponent(cId, newTable)) { // this table has been created previously
                    uint32_t newCompIndex  = this->getComponentColumn(cId, newTable);
                    newCol = newTable->columns.at(newCompIndex);
                } else { // in case this is a new table
                    uint32_t index = newTable->columns.size();
                    newCol = oldCol->createClone();
                    newTable->columns.emplace_back(newCol);
                    this->saveComponentColumn(cId, newTable, index);
                }
                oldCol->moveData(newCol, entRow);
            } else { // new component
                if (this->tableHasComponent(cId, newTable)) { // this table has been created previously
                    uint32_t compIndex = this->getComponentColumn(cId, newTable);
                    Column<T>* col = (Column<T>*)newTable->columns.at(compIndex);
                    col->data.emplace_back(newComponentData);
                } else { // in case this is a new table
                    uint32_t index = newTable->columns.size();
                    Column<T>* col = new Column<T>();
                    col->data.emplace_back(newComponentData);
                    newTable->columns.emplace_back(col);
                    this->saveComponentColumn(cId, newTable, index);
                }
            }
        }
    }

    template <typename T>
    void moveTableComponentsRemove(
        const EntityId eId, 
        const uint32_t entRow, 
        Table* oldTable, 
        Table* newTable,
        uint32_t componentIdx
    ) {
        for (ComponentId cId : newTable->type) {
            // get component index on the table
            uint32_t compIndex = this->getComponentColumn(cId, oldTable);

            // get component column
            ColumnBase* oldCol = oldTable->columns.at(compIndex);
            ColumnBase* newCol;

            if (this->tableHasComponent(cId, newTable)) { // this table has been created previously
                uint32_t newCompIndex  = this->getComponentColumn(cId, newTable);
                newCol = newTable->columns.at(newCompIndex);
            } else { // in case this is a new table
                uint32_t index = newTable->columns.size();
                newCol = oldCol->createClone();
                newTable->columns.emplace_back(newCol);
                this->saveComponentColumn(cId, newTable, index);
            }
            oldCol->moveData(newCol, entRow);
        }
        Column<T>* col = (Column<T>*)oldTable->columns.at(componentIdx);
        col->data.erase(col->data.begin() + entRow);
    }

    bool tableHasComponent(ComponentId id, Table* table) {
        TableMap *tabMap = this->component_index[id];
        return tabMap->count(table->id) != 0;
    }

    uint32_t getComponentColumn(ComponentId id, Table* table) {
        TableMap *tabMap = this->component_index[id];
        TableRecord *tabRec = tabMap->at(table->id);
        return tabRec->column;
    }

    void saveComponentColumn(ComponentId id, Table* table, uint32_t index) {
        TableMap *tabMap = this->component_index[id];
        TableRecord *tabRec = new TableRecord();
        tabRec->column = index;
        tabMap->insert({table->id, tabRec});
    }

    void rearrangeTableEntities(Table* table, const uint32_t idxStart) { // This can be moved to Table.h file
        for (uint32_t i = idxStart; i < table->entities.size(); i++) { // Check if there is a better way to do this
            EntityId id = table->entities.at(i);
            Record* entRec = this->entity_index.at(id);
            entRec->row = i;
        }
    }

    template <typename T>
    const std::vector<EntityId> getEntitiesWithOne() const {
        std::vector<EntityId> returnVec {};
        ComponentId c_id = getComponentId<T>(this->componentIdIndex);
        if(!c_id) { // Component not yet registered
            return returnVec;
        }
        TableMap tabMap = *this->component_index.at(c_id);
        for (auto tableInfo : tabMap) {
            Table* table = this->table_index.at(tableInfo.first);
            returnVec.insert(returnVec.end(), table->entities.begin(), table->entities.end());
        }
        return returnVec;
    }

    template <typename T>
    TableMap* getComponentTableMap() const {
        ComponentId c_id = getComponentId<T>(this->componentIdIndex);
        if(!c_id) { // Component not yet registered
            return nullptr;
        }
        return this->component_index.at(c_id);
    }

    template <typename T>
    void tableHasComponent(TableId id, bool* check) const {
        TableMap* tabMap = this->getComponentTableMap<T>();
        if (!tabMap) {
            *check = *check && false;
            return;
        }
        *check = *check && tabMap->count(id) != 0;
    }

    template <typename T, typename... Ts>
    const std::vector<EntityId> getEntitiesWithMultiple() const {
        TableMap* tabMap = this->getComponentTableMap<T>();
        if (!tabMap) { // In case the first component is not registered, we return an empty vec
            return {};
        }
        // If the first component is registered, continue
        std::vector<EntityId> returnVec {};
        for (const auto& tabMapPair : *tabMap) {
            bool checkTables = true;
            // tabMapPair.first = TableId
            // "looping" through a parameter pack
            (this->tableHasComponent<Ts>(tabMapPair.first, &checkTables), ...);
            if (!checkTables) {
                continue;
            }
            Table* table = this->table_index.at(tabMapPair.first);
            for (EntityId id : table->entities) {
                returnVec.emplace_back(id);
            }
        }
        return returnVec;
    }

    //void addEdges(Table& Table, ComponentId component);

    // finds the Table of an entity
    unordered_map<EntityId, Record*> entity_index;

    // finds the Table by its list of components
    unordered_map<TableId, Table*> table_index;

    // finds the archetypes that has the component
    unordered_map<ComponentId, TableMap*> component_index;

    EntityIdIndex *entityIdIndex;

    ComponentIdIndex *componentIdIndex;

    TableIdIndex *tableIdIndex;
};

}// ecs
}// nebula