
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include<stdio.h>

int echo(lua_State *L) {
    printf("world");
};

int main(int argc, char *argv[])

{
    lua_State *L = lua_open();
    lua_register(L,"echo", echo);
    luaL_openlibs(L);
    luaL_dofile(L, "hello.lua");
    lua_close(L);
    return 1;
}       