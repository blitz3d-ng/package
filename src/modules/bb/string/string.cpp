
#include "string.h"
#include "../stdutil/stdutil.h"
#include <utf8.h>

#define CHKPOS(x) if( (x)<0 ) RTEX( "parameter must be positive" );
#define CHKOFF(x) if( (x)<=0 ) RTEX( "parameter must be greater than 0" );

BBStr * BBCALL bbString( BBStr *s,bb_int_t n ){
	BBStr *t=d_new BBStr();
	while( n-->0 ) *t+=*s;
	delete s;return t;
}

BBStr * BBCALL bbLeft( BBStr *s,bb_int_t n ){
	CHKPOS( n );
	const char *last=s->data();
	const char *end=last+s->size();
	while( n-->0&&last<end ){
		utf8_int32_t chr;
		last=utf8codepoint( last,&chr );
	}
	n=last-s->data();
	*s=s->substr( 0,n );return s;
}

BBStr * BBCALL bbRight( BBStr *s,bb_int_t n ){
	CHKPOS( n );
	const char *begin=s->data();
	const char *end=begin+s->size()-1;
	while( --n>0&&end>begin ){
		utf8_int32_t chr;
		end=utf8rcodepoint( end,&chr );
	}
	n=end-begin;
	*s=s->substr( n );return s;
}

BBStr * BBCALL bbReplace( BBStr *s,BBStr *from,BBStr *to ){
	int n=0,from_sz=from->size(),to_sz=to->size();
	while( n<s->size() && (n=s->find( *from,n ))!=std::string::npos ){
		s->replace( n,from_sz,*to );
		n+=to_sz;
	}
	delete from;delete to;return s;
}

bb_int_t BBCALL bbInstr( BBStr *s,BBStr *t,bb_int_t from ){
	CHKOFF( from );
	utf8_int32_t chr;
	const char *l=s->c_str(),*r=s->c_str()+s->size()-1;
	const char *o=l;
	while( --from>0&&o<=r ) o=utf8codepoint( o,&chr );

	utf8_int8_t *m=utf8str( o,t->c_str() );
	size_t n;
	if( m ){
		n=0;
		const char *c=l;
		while( c<m ){
			c=utf8codepoint( c,&chr );
			++n;
		}
	}else{
		n=-1;
	}
	delete s;delete t;
	return n==-1 ? 0 : n+1;
}

BBStr * BBCALL bbMid( BBStr *s,bb_int_t o,bb_int_t n ){
	CHKOFF( o );
	if( n==-1 ) n=s->size();
	utf8_int32_t chr;
	const char *l=s->c_str(),*r=s->c_str()+s->size();
	const char *p=l;while( --o>0&&p<r ) p=utf8codepoint( p,&chr );
	const char *e=p;while( n-->0&&p<r ) e=utf8codepoint( e,&chr );
	*s=s->substr( p-l,e-p );return s;
}

BBStr * BBCALL bbUpper( BBStr *s ){
	utf8upr( s->data() );
	return s;
}

BBStr * BBCALL bbLower( BBStr *s ){
	utf8lwr( s->data() );
	return s;
}

BBStr * BBCALL bbTrim( BBStr *s ){
	int n=0,p=s->size();
	while( n<s->size() && !isgraph( (*s)[n] ) ) ++n;
	while( p>n && !isgraph( (*s)[p-1] ) ) --p;
	*s=s->substr( n,p-n );return s;
}

BBStr * BBCALL bbLSet( BBStr *s,bb_int_t n ){
	CHKPOS(n);
	if( utf8len( s->c_str() )>n ){
		utf8_int32_t chr;
		const char *l=s->c_str(),*r=s->c_str()+s->size()-1;
		const char *p=l;while( n-->0&&p<r ) p=utf8codepoint( p,&chr );
		*s=s->substr( 0,p-l );
	}else{
		while( utf8len( s->c_str() )<n ) *s+=' ';
	}
	return s;
}

BBStr * BBCALL bbRSet( BBStr *s,bb_int_t n ){
	CHKPOS(n);
	if( utf8len( s->c_str() )>n ){
		utf8_int32_t chr;
		const char *l=s->c_str(),*r=s->c_str()+s->size()-1;
		const char *p=r;while( --n>0&&p>l ) p=utf8rcodepoint( p,&chr );
		*s=s->substr( p-l );
	}else{
		while( utf8len( s->c_str() )<n ) *s=' '+*s;
	}
	return s;
}

BBStr * BBCALL bbChr( bb_int_t n ){
	utf8_int8_t b[4];
	const char *e=utf8catcodepoint( b,n,4 );
	return d_new BBStr( b,e-b );
}

BBStr * BBCALL bbHex( bb_int_t n ){
	char buff[12];
	for( int k=7;k>=0;n>>=4,--k ){
		int t=(n&15)+'0';
		buff[k]=t>'9' ? t+='A'-'9'-1 : t;
	}
	buff[8]=0;
	return d_new BBStr( buff );
}

BBStr * BBCALL bbBin( bb_int_t n ){
	char buff[36];
	for( int k=31;k>=0;n>>=1,--k ){
		buff[k]=n&1 ? '1' : '0';
	}
	buff[32]=0;
	return d_new BBStr( buff );
}

bb_int_t BBCALL bbAsc( BBStr *s ){
	int n=-1;
	if( s->size() ) utf8codepoint( s->data(),&n );
	delete s;return n;
}

bb_int_t BBCALL bbLen( BBStr *s ){
	int n=utf8len( s->c_str() );
	delete s;return n;
}

BBMODULE_CREATE( string ){
	return true;
}

BBMODULE_DESTROY( string ){
	return true;
}
