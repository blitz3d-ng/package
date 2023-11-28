
#include "multiplay.enet.h"
#include <bb/blitz/blitz.h>
#include <enet/enet.h>

#include <guid.h>

#include <cstring>
#include <iostream>
#include <map>

// - StartNetGame%():"bbStartNetGame"
// - HostNetGame%( game_name$ ):"bbHostNetGame"
// - JoinNetGame%( game_name$,ip_address$ ):"bbJoinNetGame"
// - StopNetGame():"bbStopNetGame"
//
// - CreateNetPlayer%( name$ ):"bbCreateNetPlayer"
// - DeleteNetPlayer( player% ):"bbDeleteNetPlayer"
// - NetPlayerName$( player% ):"bbNetPlayerName"
// - NetPlayerLocal%( player% ):"bbNetPlayerLocal"
//
// - SendNetMsg%( type%,msg$,from_player%,to_player%=0,reliable%=1 ):"bbSendNetMsg"
//
// - RecvNetMsg%():"bbRecvNetMsg"
// - NetMsgType%():"bbNetMsgType"
// - NetMsgFrom%():"bbNetMsgFrom"
// - NetMsgTo%():"bbNetMsgTo"
// - NetMsgData$():"bbNetMsgData"

static GuidGenerator generator;

BBPlayer::BBPlayer(){
#ifndef BB_NX
	id=generator.newGuid();
#else
	// FIXME
	id=Guid( "46a38d6b-fd83-44bb-b17a-2fdea913a4cc" );
#endif
}

const std::string BBPlayer::getId(){
	std::stringstream stream;
	stream<<id;
	return stream.str();
}

static ENetAddress address;
static ENetHost *host;
static ENetPeer *peer;

static std::map<std::string,BBPlayer> bbPlayers;

struct BBNetMsg{
	union Peer{
		char id[16];
		BBPlayer *ptr;
	};

	unsigned char type;
	Peer from,to;

	unsigned short length;
	unsigned char *data;
};

BBNetMsg bbLastNetMsg;

static int sendNetMsg( unsigned char type,std::string from,std::string to,std::string data,bool reliable ){
	static BBNetMsg *msg=0;
	static int msg_size=0;

	int new_size=sizeof(BBNetMsg)+data.length()+1;
	if( !msg || msg_size<new_size ){
		msg_size=new_size;
		msg=(BBNetMsg*)(msg?realloc( msg,msg_size ):malloc( msg_size ));
	}
	memset( msg,0,msg_size );

	msg->type=type;
	if( from.length()>0 ) strncpy( msg->from.id,from.c_str(),16 );
	if( to.length()>0 ) strncpy( msg->to.id,to.c_str(),16 );
	msg->length=data.length();
	// strncpy( (char*)(msg+sizeof(BBNetMsg)-sizeof(msg->data)),data.c_str(),data.length() );

	ENetPacket *pk=enet_packet_create( msg,msg_size,reliable?ENET_PACKET_FLAG_RELIABLE:0 );

	if( peer ){
		enet_peer_send( peer,0,pk );
	}else{
		for( int i=0;i<host->peerCount;i++ ){
			enet_peer_send( &host->peers[i],0,pk );
		}
	}
	enet_host_flush( host );
	return 1;
}

bb_int_t BBCALL bbSendNetMsg( bb_int_t type,BBStr *data,BBPlayer *from,BBPlayer *to,bb_int_t reliable ){
	if( bb_env.debug && type<1||type>99 ) RTEX( "Message type must be between 1 and 99." );

	std::string d=*data;delete data;
	std::string from_id;//=string( from->id,16 );
	std::string to_id;//=to?string( to->id,16 ):"";
	return sendNetMsg( type,from_id,to_id,d,reliable );
}

int recvServerMsg( const ENetEvent &e ){
	BBNetMsg msg;
	switch( e.type ){
	case ENET_EVENT_TYPE_CONNECT:
		// printf( "A new client connected from %x:%u.\n",
		//         event.peer->address.host,
		//         event.peer->address.port);
		// /* Store any relevant client information here. */
		// event.peer->data=(void*)"Client information";
		break;
	case ENET_EVENT_TYPE_RECEIVE:
		memcpy( &msg,e.packet->data,sizeof(BBNetMsg) );
		std::cout<<"new message: type="<<msg.type<<std::endl;

		switch( msg.type ){
		case 100:
			std::cout<<"new player!!!"<<std::endl;
			break;
		}

		// printf ("A packet of length %u containing %s was received from %s on channel %u.\n",
		//         e.packet -> dataLength,
		//         e.packet -> data,
		//         e.peer -> data,
		//         e.channelID);
		enet_packet_destroy( e.packet );
		break;

	case ENET_EVENT_TYPE_DISCONNECT:
		// LOGD( "%s disconnected.\n",(char*)e.peer->data );
		e.peer->data=NULL;
	case ENET_EVENT_TYPE_NONE:
		break;
	}

	return 0;
}

int recvClientMsg( const ENetEvent &e ){
	return 0;
}

bb_int_t BBCALL bbHostNetGame( BBStr *game_name ){
	delete game_name;

	address.host=ENET_HOST_ANY;
	address.port=1234;
	host=enet_host_create( &address,32,2,0,0 );
	if( host==0 ) return 0;

	return 2;
}

bb_int_t BBCALL bbJoinNetGame( BBStr *game_name,BBStr *ip_address ){
	delete game_name;delete ip_address;

	if( !(host=enet_host_create( NULL,1,2,0,0 )) ){
		return 0;
	}

	enet_address_set_host( &address,"127.0.0.1" );
	address.port=1234;

	if( !(peer=enet_host_connect( host,&address,2,0 )) ){
		return 0;
	}

	ENetEvent event;
	if( enet_host_service( host,&event,5000 )>0 && event.type == ENET_EVENT_TYPE_CONNECT ){
		return 1;
	}else{
		enet_peer_reset (peer);
		return 0;
	}
}

BBPlayer * BBCALL bbCreateNetPlayer( BBStr *name ){
	std::string n=*name;delete name;

	BBPlayer p;
	p.name=n;
	p.local=true;

	bbPlayers[p.getId()]=p;
	sendNetMsg( 100,"","","",true );

	return &bbPlayers[p.getId()];
}

bb_int_t BBCALL bbRecvNetMsg(){
	ENetEvent e;
	if( enet_host_service( host,&e,0 )<=0 ) return 0;
	return peer?recvClientMsg(e):recvServerMsg(e);
}

bb_int_t	BBCALL bbNetMsgType(){
	return bbLastNetMsg.type;
}

BBStr *	BBCALL bbNetMsgData(){
	return new BBStr( (char*)bbLastNetMsg.data,bbLastNetMsg.length );
}

BBPlayer * BBCALL bbNetMsgFrom(){
	return bbLastNetMsg.from.ptr;
}

BBPlayer * BBCALL bbNetMsgTo(){
	return bbLastNetMsg.to.ptr;
}

BBMODULE_CREATE( multiplay_enet ){
	memset( &bbLastNetMsg,0,sizeof(BBNetMsg) );
	return true;
}

BBMODULE_DESTROY( multiplay_enet ){
	return true;
}
