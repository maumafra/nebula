#include "NebulaSetup.hpp"

extern "C" {
    extern int nlua_graphics(lua_State* L);
}

static const luaL_Reg modules[] = {
    {"nebula.graphics", nlua_graphics}
};

int nlua_nebula(lua_State *L) {
    // setup Modules
}