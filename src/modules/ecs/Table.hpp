#pragma once

#include "Definitions.hpp"

#include <typeinfo>
#include <memory>

namespace nebula {
    namespace ecs {

struct ColumnBase { // Type erasure struct
    virtual ~ColumnBase() = default;
    virtual void moveData(ColumnBase *colToMoveTo, uint32_t entRow) = 0;
    virtual ColumnBase* createClone() const = 0;
    virtual void debug() const = 0;
};

template<typename T>
struct Column : ColumnBase {
    std::vector<T> data;                /* Array with component data */

    size_t size() const {               /* Size of the component data */
        return data.size();
    }

    void moveData(ColumnBase *colToMoveTo, uint32_t entRow) {
        Column<T> *col = (Column<T>*)colToMoveTo;
        col->data.emplace_back(this->data.at(entRow));
        this->data.erase(this->data.begin() + entRow);
    }

    ColumnBase* createClone() const {   /* Function to create an empty clone (used when creating another table) */
        return new Column<T>();
    }

    void debug() const {
        std::cout << typeid(T).name();
    }
};

// Table === Archetype
struct Table {
    TableId id;                             /* Table id */
    Type type;                              /* Components ids */
    std::vector<EntityId> entities;         /* Entity ids */
    std::vector<ColumnBase*> columns;       /* Component data */

    //unordered_map<ComponentId, Table&> edges;

    ~Table() {
        for (ColumnBase* c : columns) {
            delete c;
        }
    };
};

//struct TableEdge {
//  Table& add;
//  Table& remove;
//};

struct Record {
    Table* table;               /* Pointer for Entity table */
    uint32_t row;               /* Entity row in the table */
};

struct TableRecord {
    uint32_t column;            /* Component column in the table */
};

}// ecs
}// nebula