// AUTOGENERATED. DO NOT EDIT.
// RUN `ruby bindings/luajit/generate.rb` TO UPDATE.

#include <bb/sockets/commands.h>

#if defined(BB_WIN32) || defined(BB_WIN64) || defined(BB_MINGW32)
#define BB_SOCKETS_ENABLED

static int l_sockets_AcceptTCPStream( lua_State *L ){
  bb_int_t __tcp_server = luaL_checknumber( L,1 );
  lua_pushnumber( L,(bb_int_t)bbAcceptTCPStream( __tcp_server ) );
  return 1;
}

static int l_sockets_CloseTCPServer( lua_State *L ){
  bb_int_t __tcp_server = luaL_checknumber( L,1 );
  bbCloseTCPServer( __tcp_server );
  return 0;
}

static int l_sockets_CloseTCPStream( lua_State *L ){
  bb_int_t __tcp_stream = luaL_checknumber( L,1 );
  bbCloseTCPStream( __tcp_stream );
  return 0;
}

static int l_sockets_CloseUDPStream( lua_State *L ){
  bb_int_t __udp_stream = luaL_checknumber( L,1 );
  bbCloseUDPStream( __udp_stream );
  return 0;
}

static int l_sockets_CountHostIPs( lua_State *L ){
  BBStr * __host_name = _bbStrConst( luaL_checkstring( L,1 ) );
  lua_pushnumber( L,(bb_int_t)bbCountHostIPs( __host_name ) );
  return 1;
}

static int l_sockets_CreateTCPServer( lua_State *L ){
  bb_int_t __port = luaL_checknumber( L,1 );
  lua_pushnumber( L,(bb_int_t)bbCreateTCPServer( __port ) );
  return 1;
}

static int l_sockets_CreateUDPStream( lua_State *L ){
  bb_int_t __port;
  if( lua_gettop( L ) > 0 ){
    __port = luaL_checknumber( L,1 );
  }else{
   __port = 0;
  }
  lua_pushnumber( L,(bb_int_t)bbCreateUDPStream( __port ) );
  return 1;
}

static int l_sockets_DottedIP( lua_State *L ){
  bb_int_t __IP = luaL_checknumber( L,1 );
  lua_pushstring( L,bbDottedIP( __IP ) );
  return 1;
}

static int l_sockets_HostIP( lua_State *L ){
  bb_int_t __host_index = luaL_checknumber( L,1 );
  lua_pushnumber( L,(bb_int_t)bbHostIP( __host_index ) );
  return 1;
}

static int l_sockets_OpenTCPStream( lua_State *L ){
  BBStr * __server = _bbStrConst( luaL_checkstring( L,1 ) );
  bb_int_t __server_port = luaL_checknumber( L,2 );
  bb_int_t __local_port;
  if( lua_gettop( L ) > 2 ){
    __local_port = luaL_checknumber( L,3 );
  }else{
   __local_port = 0;
  }
  lua_pushnumber( L,(bb_int_t)bbOpenTCPStream( __server,__server_port,__local_port ) );
  return 1;
}

static int l_sockets_RecvUDPMsg( lua_State *L ){
  bb_int_t __udp_stream = luaL_checknumber( L,1 );
  lua_pushnumber( L,(bb_int_t)bbRecvUDPMsg( __udp_stream ) );
  return 1;
}

static int l_sockets_SendUDPMsg( lua_State *L ){
  bb_int_t __udp_stream = luaL_checknumber( L,1 );
  bb_int_t __dest_ip = luaL_checknumber( L,2 );
  bb_int_t __dest_port;
  if( lua_gettop( L ) > 2 ){
    __dest_port = luaL_checknumber( L,3 );
  }else{
   __dest_port = 0;
  }
  bbSendUDPMsg( __udp_stream,__dest_ip,__dest_port );
  return 0;
}

static int l_sockets_TCPStreamIP( lua_State *L ){
  bb_int_t __tcp_stream = luaL_checknumber( L,1 );
  lua_pushnumber( L,(bb_int_t)bbTCPStreamIP( __tcp_stream ) );
  return 1;
}

static int l_sockets_TCPStreamPort( lua_State *L ){
  bb_int_t __tcp_stream = luaL_checknumber( L,1 );
  lua_pushnumber( L,(bb_int_t)bbTCPStreamPort( __tcp_stream ) );
  return 1;
}

static int l_sockets_TCPTimeouts( lua_State *L ){
  bb_int_t __read_millis = luaL_checknumber( L,1 );
  bb_int_t __accept_millis = luaL_checknumber( L,2 );
  bbTCPTimeouts( __read_millis,__accept_millis );
  return 0;
}

static int l_sockets_UDPMsgIP( lua_State *L ){
  bb_int_t __udp_stream = luaL_checknumber( L,1 );
  lua_pushnumber( L,(bb_int_t)bbUDPMsgIP( __udp_stream ) );
  return 1;
}

static int l_sockets_UDPMsgPort( lua_State *L ){
  bb_int_t __udp_stream = luaL_checknumber( L,1 );
  lua_pushnumber( L,(bb_int_t)bbUDPMsgPort( __udp_stream ) );
  return 1;
}

static int l_sockets_UDPStreamIP( lua_State *L ){
  bb_int_t __udp_stream = luaL_checknumber( L,1 );
  lua_pushnumber( L,(bb_int_t)bbUDPStreamIP( __udp_stream ) );
  return 1;
}

static int l_sockets_UDPStreamPort( lua_State *L ){
  bb_int_t __udp_stream = luaL_checknumber( L,1 );
  lua_pushnumber( L,(bb_int_t)bbUDPStreamPort( __udp_stream ) );
  return 1;
}

static int l_sockets_UDPTimeouts( lua_State *L ){
  bb_int_t __recv_timeout = luaL_checknumber( L,1 );
  bbUDPTimeouts( __recv_timeout );
  return 0;
}

#endif
