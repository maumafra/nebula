#include "WrapGraphics.hpp"

//#define graphics() (Module)

namespace nebula {
    namespace graphics {

int w_draw(lua_State *L) {
    ecs::EntityId id = (ecs::EntityId) lua_touserdata(L, 1);
    //graphics()->draw(id);
    return 0;
}

static const luaL_Reg functions[] = {
    {"draw", }
};

extern "C" int nlua_graphics(lua_State *L) {
    //Graphics *instance = Graphics::create;
    //if (instance == nullptr) {
    //    return luaL_error(L, "Error creating Graphics module.");
    //}

}

} // graphics    
} // nebula
