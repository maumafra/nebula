#pragma once

#include <cstdint>
#include <vector>

namespace nebula {
    namespace ecs {

typedef uint32_t ecsId;
typedef uint64_t TypeHash;

typedef ecsId EntityId;
typedef ecsId ComponentId;
typedef ecsId TableId;

typedef std::vector<ComponentId> Type;

}// ecs
}// nebula