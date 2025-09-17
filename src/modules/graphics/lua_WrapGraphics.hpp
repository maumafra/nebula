#pragma once

#include "Graphics.h"

extern "C" {
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
}

namespace nebula {
    namespace graphics {

extern "C" int nlua_graphics(lua_State *L);

} // graphics    
} // nebula
