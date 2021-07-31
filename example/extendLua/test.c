#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

static int test(lua_State* L)
{
    const char *a = luaL_checkstring(L, 1); //取栈第一个参数
    lua_pushstring(L, (const char *)"hi");//返回值入栈
    return 1;
}

static const struct luaL_Reg reg_test[] = {
    {"test", test},
    {NULL, NULL}
};

int luaopen_test(lua_State *L) {
    const char* libName = "test";
    luaL_register(L, libName, reg_test);
    return 1;
}