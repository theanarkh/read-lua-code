/*
** $Id: lzio.c,v 1.31.1.1 2007/12/27 13:02:25 roberto Exp $
** a generic input stream interface
** See Copyright Notice in lua.h
*/


#include <string.h>

#define lzio_c
#define LUA_CORE

#include "lua.h"

#include "llimits.h"
#include "lmem.h"
#include "lstate.h"
#include "lzio.h"

// 读取数据
int luaZ_fill (ZIO *z) {
  size_t size;
  lua_State *L = z->L;
  const char *buff;
  lua_unlock(L);
  // 读取size个字节
  buff = z->reader(L, z->data, &size);
  lua_lock(L);
  if (buff == NULL || size == 0) return EOZ;
  z->n = size - 1;
  z->p = buff;
  // 返回读取的第一个字符的值
  return char2int(*(z->p++));
}

// 前瞻
int luaZ_lookahead (ZIO *z) {
  // 没有数据
  if (z->n == 0) {
    // 读取新数据
    if (luaZ_fill(z) == EOZ)
      return EOZ;
    else {
      // luaZ_fill消耗了一个，这里补回去，个数加一，指针往回走
      z->n++;  /* luaZ_fill removed first byte; put back it */
      z->p--;
    }
  }
  // 返回当前的字符，但是指针不往前走
  return char2int(*z->p);
}

// 初始化zio
void luaZ_init (lua_State *L, ZIO *z, lua_Reader reader, void *data) {
  z->L = L;
  // 读取函数
  z->reader = reader;
  z->data = data;
  // 数据大小
  z->n = 0;
  // 数据
  z->p = NULL;
}


/* --------------------------------------------------------------- read --- */
// 从zio中读取n个字节到b中
size_t luaZ_read (ZIO *z, void *b, size_t n) {
  // 循环读取
  while (n) {
    size_t m;
    // 前瞻判断是否没有数据了，是则返回已读取的字节数
    if (luaZ_lookahead(z) == EOZ)
      return n;  /* return number of missing bytes */
    // 判断应该读取多少个字节，取小者
    m = (n <= z->n) ? n : z->n;  /* min. between n and z->n */
    // 从zio复制到b
    memcpy(b, z->p, m);
    // 减去被消费的
    z->n -= m;
    // 指针往前走m字节
    z->p += m;
    // 更新下一个读入的位置
    b = (char *)b + m;
    // 更新还需要读取的字节数
    n -= m;
  }
  return 0;
}

/* ------------------------------------------------------------------------ */
char *luaZ_openspace (lua_State *L, Mbuffer *buff, size_t n) {
  if (n > buff->buffsize) {
    if (n < LUA_MINBUFFER) n = LUA_MINBUFFER;
    luaZ_resizebuffer(L, buff, n);
  }
  return buff->buffer;
}


