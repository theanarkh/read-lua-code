#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include<stdio.h>

const char * script = "print('hi');";

int main(int argc, char *argv[])

{
    lua_State *L = lua_open();
    luaL_openlibs(L);
    luaL_dostring(L, script);
    lua_close(L);

    return 1;

}