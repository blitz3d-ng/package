// AUTOGENERATED. DO NOT EDIT.
// RUN `ruby bindings/luajit/generate.rb` TO UPDATE.

#include <bb/math/commands.h>

#if defined(BB_WIN32) || defined(BB_WIN64) || defined(BB_MINGW32) || defined(BB_MACOS) || defined(BB_LINUX) || defined(BB_IOS) || defined(BB_ANDROID) || defined(BB_EMSCRIPTEN)
#define BB_MATH_ENABLED

static int l_math_ACos( lua_State *L ){
  bb_float_t __degrees = luaL_checknumber( L,1 );
  lua_pushnumber( L,(bb_float_t)bbACos( __degrees ) );
  return 1;
}

static int l_math_ASin( lua_State *L ){
  bb_float_t __degrees = luaL_checknumber( L,1 );
  lua_pushnumber( L,(bb_float_t)bbASin( __degrees ) );
  return 1;
}

static int l_math_ATan( lua_State *L ){
  bb_float_t __degrees = luaL_checknumber( L,1 );
  lua_pushnumber( L,(bb_float_t)bbATan( __degrees ) );
  return 1;
}

static int l_math_ATan2( lua_State *L ){
  bb_float_t __y = luaL_checknumber( L,1 );
  bb_float_t __x = luaL_checknumber( L,2 );
  lua_pushnumber( L,(bb_float_t)bbATan2( __y,__x ) );
  return 1;
}

static int l_math_Ceil( lua_State *L ){
  bb_float_t __n = luaL_checknumber( L,1 );
  lua_pushnumber( L,(bb_float_t)bbCeil( __n ) );
  return 1;
}

static int l_math_Cos( lua_State *L ){
  bb_float_t __degrees = luaL_checknumber( L,1 );
  lua_pushnumber( L,(bb_float_t)bbCos( __degrees ) );
  return 1;
}

static int l_math_Exp( lua_State *L ){
  bb_float_t __n = luaL_checknumber( L,1 );
  lua_pushnumber( L,(bb_float_t)bbExp( __n ) );
  return 1;
}

static int l_math_Floor( lua_State *L ){
  bb_float_t __n = luaL_checknumber( L,1 );
  lua_pushnumber( L,(bb_float_t)bbFloor( __n ) );
  return 1;
}

static int l_math_Log( lua_State *L ){
  bb_float_t __x = luaL_checknumber( L,1 );
  lua_pushnumber( L,(bb_float_t)bbLog( __x ) );
  return 1;
}

static int l_math_Log10( lua_State *L ){
  bb_float_t __x = luaL_checknumber( L,1 );
  lua_pushnumber( L,(bb_float_t)bbLog10( __x ) );
  return 1;
}

static int l_math_Rand( lua_State *L ){
  bb_int_t __from = luaL_checknumber( L,1 );
  bb_int_t __to;
  if( lua_gettop( L ) > 1 ){
    __to = luaL_checknumber( L,2 );
  }else{
   __to = 1;
  }
  lua_pushnumber( L,(bb_int_t)bbRand( __from,__to ) );
  return 1;
}

static int l_math_Rnd( lua_State *L ){
  bb_float_t __from = luaL_checknumber( L,1 );
  bb_float_t __to;
  if( lua_gettop( L ) > 1 ){
    __to = luaL_checknumber( L,2 );
  }else{
   __to = 0;
  }
  lua_pushnumber( L,(bb_float_t)bbRnd( __from,__to ) );
  return 1;
}

static int l_math_RndSeed( lua_State *L ){
  lua_pushnumber( L,(bb_int_t)bbRndSeed(  ) );
  return 1;
}

static int l_math_SeedRnd( lua_State *L ){
  bb_int_t __seed = luaL_checknumber( L,1 );
  bbSeedRnd( __seed );
  return 0;
}

static int l_math_Sin( lua_State *L ){
  bb_float_t __degrees = luaL_checknumber( L,1 );
  lua_pushnumber( L,(bb_float_t)bbSin( __degrees ) );
  return 1;
}

static int l_math_Sqr( lua_State *L ){
  bb_float_t __n = luaL_checknumber( L,1 );
  lua_pushnumber( L,(bb_float_t)bbSqr( __n ) );
  return 1;
}

static int l_math_Tan( lua_State *L ){
  bb_float_t __degrees = luaL_checknumber( L,1 );
  lua_pushnumber( L,(bb_float_t)bbTan( __degrees ) );
  return 1;
}

#endif
