// This is a replacement loader for the original loader_x.cpp which requires
// DX SDK components.
//
// https://learn.microsoft.com/en-us/windows/win32/direct3d9/dx9-graphics-reference-x-file-format

#include <bb/blitz/app.h>
#include <bb/filesystem/filesystem.h>
#include "loader_x2.h"
#include "meshmodel.h"
#include "animation.h"
#include "pivot.h"

#include <map>
#include <fstream>

static std::map<std::string,MeshModel*> frames_map;
static int anim_len;

static bool conv,flip_tris;
static Transform conv_tform;
static bool collapse,animonly;

#define XOFFILE_FORMAT_MAGIC \
  ((long)'x' + ((long)'o' << 8) + ((long)'f' << 16) + ((long)' ' << 24))
#define XOFFILE_FORMAT_MAJOR_VERSION \
  ((long)'0' + ((long)'3' << 8))
#define XOFFILE_FORMAT_BINARY \
  ((long)'b' + ((long)'i' << 8) + ((long)'n' << 16) + ((long)' ' << 24))
#define XOFFILE_FORMAT_TEXT   \
  ((long)'t' + ((long)'x' << 8) + ((long)'t' << 16) + ((long)' ' << 24))
#define XOFFILE_FORMAT_COMPRESSED \
  ((long)'c' + ((long)'m' << 8) + ((long)'p' << 16) + ((long)' ' << 24))
#define XOFFILE_FORMAT_FLOAT_BITS_32 \
  ((long)'0' + ((long)'0' << 8) + ((long)'3' << 16) + ((long)'2' << 24))
#define XOFFILE_FORMAT_FLOAT_BITS_64 \
  ((long)'0' + ((long)'0' << 8) + ((long)'6' << 16) + ((long)'4' << 24))

#define TOKEN_NAME         1
#define TOKEN_STRING       2
#define TOKEN_INTEGER      3
#define TOKEN_GUID         5
#define TOKEN_INTEGER_LIST 6
#define TOKEN_FLOAT_LIST   7

#define TOKEN_OBRACE      10
#define TOKEN_CBRACE      11
#define TOKEN_OPAREN      12
#define TOKEN_CPAREN      13
#define TOKEN_OBRACKET    14
#define TOKEN_CBRACKET    15
#define TOKEN_OANGLE      16
#define TOKEN_CANGLE      17
#define TOKEN_DOT         18
#define TOKEN_COMMA       19
#define TOKEN_SEMICOLON   20
#define TOKEN_TEMPLATE    31
#define TOKEN_WORD        40
#define TOKEN_DWORD       41
#define TOKEN_FLOAT       42
#define TOKEN_DOUBLE      43
#define TOKEN_CHAR        44
#define TOKEN_UCHAR       45
#define TOKEN_SWORD       46
#define TOKEN_SDWORD      47
#define TOKEN_VOID        48
#define TOKEN_LPSTR       49
#define TOKEN_UNICODE     50
#define TOKEN_CSTRING     51
#define TOKEN_ARRAY       52

#define TOKEN_COMMENT 98
#define TOKEN_EOF     99

class XParser:public std::istream{
protected:
	int format,float_bits;

public:
	XParser( std::streambuf *in ):std::istream(in),format(0),float_bits(0){}

	enum{
		BINARY,
		TEXT
	};

	typedef unsigned int dword;
	typedef std::vector<dword> int_list;
	typedef std::vector<float> float32_list;

	class Object{
	public:
		std::string tmpl,id;
		std::vector<char> blob;
		std::vector<Object*> children;

		Object( const std::string &t,const std::string &i ):tmpl(t),id(i){}
		Object():Object( "","" ){}

		char *data(){
			return blob.data();
		}

		size_t size(){
			return blob.size();
		}

		char *expand( size_t len ){
			blob.resize( blob.size()+len );
			return blob.data()+blob.size()-len;
		}

		void add( void *mem,size_t len ){
			dword *data=(dword*)expand( len );
			memcpy( data,mem,len );
		}

		void add( const std::string &s ){
			dword len=s.size();
			add( &len,sizeof(len) );
			add( (void*)s.data(),s.size() );
		}

		void add( int_list list ){
			size_t len=list.size()*sizeof(dword);
			dword *data=(dword*)expand( len );
			for( auto &i:list ){
				*data++=i;
			}
		}
	};

	class Token{
	public:
		dword type;
		std::string text;
		Token( dword t,const std::string &s ):type(t),text(s){}
		Token():Token( 0,"" ){}

		bool operator==( const int i ){
			return type==i;
		}
		bool operator!=( const int i ){
			return type!=i;
		}
		bool operator==( const std::string &t ){
			return text==t;
		}
		bool operator!=( const std::string &t ){
			return text!=t;
		}
	};

	std::map<std::string,std::map<std::string,Object*>> objects;
	std::map<std::string,Brush> mats;
	Object tmp;

	void ex( const std::string &msg ){
		throw "["+itoa(tellg())+"] "+msg;
	}

	void log( const std::string &msg ){
		std::cerr<<"["<<tellg()<<"] "<<msg<<std::endl;
	}

	dword readInt(){
		dword v;
		read( (char*)&v,sizeof(v) );
		return v;
	}

	dword readShort(){
		unsigned short v;
		read( (char*)&v,sizeof(v) );
		return v;
	}

	void setFormat( int f ){
		format=f;
	}

	void setFloatBits( int bits ){
		float_bits=bits;
	}

	Token expect( int e ){
		Token t=readToken();
		if( t.type!=e ){
			ex( "Expected "+itoa(e)+", but found "+itoa(t.type) );
		}
		return t;
	}

	Token expect( const std::string &s ){
		Token t=readToken();
		if( t!=s ){
			ex( "Expected "+s+", but found "+t.text );
		}
		return t;
	}

	std::string expectName( const std::string &n="" ){
		Token t=expect( TOKEN_NAME );
		if( n!=""&&t.text!=n ){
			ex( "Expected "+n+", but got "+t.text );
		}
		return t.text;
	}

	Brush parseMaterial( Object *obj ){
		Brush brush;

		float *color=(float*)obj->data();
		brush.setColor( Vector( color[0],color[1],color[2] ) );
		if( color[3] ) brush.setAlpha( color[3] );

		std::string tex;
		for( auto& child:obj->children ){
			if( child->tmpl=="TextureFileName"||child->tmpl=="TextureFilename" ){
				dword cnt=*(dword*)child->data();
				// std::string tex( child->data()+4,cnt );
				tex=std::string( child->data()+4,cnt );

				brush.setTexture( 0,Texture( tex,0 ),0 );
				brush.setColor( Vector( 1,1,1 ) );
			}
		}

		return brush;
	}

	struct FaceX{
		dword *data,mat_index;
		FaceX( dword *d ):data(d),mat_index(0){}
	};

	void parseMesh( Object *obj,MeshModel *mesh, std::map<std::string,Brush> &mats_map ){
		//stuff...
		std::vector<FaceX> faces;
		std::vector<Brush> mats;

		MeshLoader::beginMesh();

		// log("parseMesh-start "+obj->id);

		dword *data=(dword*)obj->data();

		//setup vertices
		int num_verts=*data++;
		// log("parseMesh-num_verts "+itoa(num_verts));

		for( int k=0;k<num_verts;++k ){
			Surface::Vertex v;
			v.coords=*(Vector*)data;
			if( conv ) v.coords=conv_tform * v.coords;
			v.color=0xffffffff;//Vector(1,1,1);
			MeshLoader::addVertex( v );
			data+=3;
		}

		// setup faces
		dword num_faces=*data++;
		for( int k=0;k<num_faces;++k ){
			faces.push_back( FaceX( data ) );
			data+=*data+1;
		}

		bool normals=false;

		for( auto &child:obj->children ){
			std::string name=child->tmpl;
			if( name=="MeshNormals" ){
				dword *data=(dword*)child->data();
				dword num_normals=*data++;
				if( num_normals==num_verts ){
					Matrix co=conv_tform.m.cofactor();
					for( int k=0;k<num_normals;++k ){
						Surface::Vertex &v=MeshLoader::refVertex(k);
						v.normal=(co * *(Vector*)data).normalized();
						data+=3;
					}
					normals=true;
				}
			}else if( name=="MeshTextureCoords" ){
				dword num_coords=*(dword*)child->data();

				if( num_coords==num_verts ){
					float *coords=(float*)(child->data()+4);
					for( int k=0;k<num_coords;++k ){
						Surface::Vertex &v=MeshLoader::refVertex(k);
						float tu=*coords++;float tv=*coords++;
						v.tex_coords[0][0]=v.tex_coords[1][0]=tu;
						v.tex_coords[0][1]=v.tex_coords[1][1]=tv;
					}
				}
			} else if( name=="MeshMaterialList" ){
				dword *data=(dword*)child->data();
				dword num_mats=*data++;
				dword num_faces=*data++;
				for( dword k=0;k<num_faces;++k ){
					faces[k].mat_index=*data++;
				}
				for( auto &child:child->children ){
					Object *mat=0;
					if( child->tmpl=="Material" ){
						mat=child;
					}else if( child->tmpl=="DataReference" ){
						mat=objects["Material"][child->id];
						if( !mat ) ex("wat");
					}

					if( mat ){
						mats.push_back( parseMaterial( mat ) );
						if( --num_mats==0 ) break;
					}
				}

				for( dword i=0;i<num_mats;i++ ) mats.push_back( Brush() );
			}
		}

		// log("parseMesh-end ");

		if( !mats.size() ) mats.push_back( Brush() );

		for( dword k=0;k<faces.size();++k ){
			const FaceX &f=faces[k];
			dword *data=f.data;
			dword cnt=*data++;if( cnt<3 ) continue;
			int tri[3];
			tri[0]=data[0];
			for( dword j=2;j<cnt;++j ){
				tri[1]=data[j-1+flip_tris];
				tri[2]=data[j-flip_tris];
				MeshLoader::addTriangle( tri,mats[f.mat_index] );
			}
		}

		MeshLoader::endMesh( mesh );

		if( !normals ) mesh->updateNormals();
	}

	void parseFrameTransformMatrix( Object *c,MeshModel *e ){
		if( c->size()!=16*sizeof(float) ) return;

		float *els=(float*)c->data();
		Transform tform=Transform( Matrix(
			Vector( els[ 0],els[ 1],els[ 2] ),
			Vector( els[ 4],els[ 5],els[ 6] ),
			Vector( els[ 8],els[ 9],els[10] ) ),
			Vector( els[12],els[13],els[14] ) );
		if( conv ) tform=conv_tform * tform * -conv_tform;
		e->setLocalTform( tform );
	}

	MeshModel *parseFrame( Object *obj ){
		MeshModel *e=d_new MeshModel();
		e->setName( obj->id );
		frames_map[obj->id]=e;

		for( auto &child:obj->children ){
			std::string &tmpl=child->tmpl;
			if( tmpl=="FrameTransformMatrix" ){
				parseFrameTransformMatrix( child,e );
			}else if( tmpl=="Frame" ){
				MeshModel *t=parseFrame( child );
				t->setParent( e );
			}else if( tmpl=="Mesh" ){
				if( !animonly ) parseMesh( child,e,mats );
			}else if( tmpl=="Material" ){
				// nothing to do here...
			}else{
				log( "Unhandled object "+tmpl );
			}
		}

		// log("parseFrame-end");
		return e;
	}

	void parseAnimKey( Object *obj,MeshModel *e ){
		dword *data=(dword*)obj->data();
		dword type=*data++;
		dword cnt=*data++;
		Animation anim=e->getAnimation();
		for( int k=0;k<cnt;++k ){
			dword time=*data++;
			dword n=*data++;
			if( time>anim_len ) anim_len=time;
			switch( type ){
			case 0:
				if( n==4 ){
					Quat rot=*(Quat*)data;
					if( conv ){
						if( fabs(rot.w)<1-EPSILON ){
							rot.normalize();
							//quat-to-axis/angle
							float half=acosf( rot.w );
							if( flip_tris ) half=-half;
							rot=Quat( cosf( half ),(conv_tform.m*rot.v).normalized()*sinf( half ) );
						}else rot=Quat();
					}
					anim.setRotationKey( time,rot );
				}
				break;
			case 1:
				if( n==3 ){
					Vector scl=*(Vector*)data;
					if( conv ) scl=conv_tform.m * scl;
					scl.x=fabs(scl.x);scl.y=fabs(scl.y);scl.z=fabs(scl.z);
					anim.setScaleKey( time,scl );
				}
				break;
			case 2:
				if( n==3 ){
					Vector pos=*(Vector*)data;
					if( conv ) pos=conv_tform*pos;
					anim.setPositionKey( time,pos );
				}
				break;
			}
			data+=n;
		}
		e->setAnimation( anim );

		// log("parseAnimKey-end");
	}

	void parseAnim( Object *obj ){
		MeshModel *frame=0;

		// log("parseAnim-start");

		for( auto &child:obj->children ){
			if( child->tmpl=="DataReference" ){
				frame=frames_map[child->id];
				if( frame ) break;
			}
		}

		if( frame ){
			for( auto &child:obj->children ){
				if( child->tmpl=="AnimationKey" ){
					parseAnimKey( child,frame );
				}
			}
		}

		// log("parseAnim-end");
	}

	void parseAnimSet( Object *obj ){
		for( auto &child:obj->children ){
			if( child->tmpl=="Animation" ){
				parseAnim( child );
			}
		}
	}

	void eat( size_t sz ){
		char c=get();
		while( !eof()&&sz>0 ){
			c=get();
			if( !isspace(c) ) sz--;
		}
	}

	Token readToken(){
		Token t;
		if( format==BINARY ){
			dword c=readShort();
			if( eof() ) return Token( TOKEN_EOF,"<end-of-file>" );

			t.type=c;
			if( c==TOKEN_NAME ){
				dword count=readInt();
				t.text.resize( count );
				read( t.text.data(),count );
			}else if( c==TOKEN_GUID ){
				dword data1=readInt();
				dword data2=readShort();
				dword data3=readShort();
				readInt();readInt();
			}else if( c==TOKEN_STRING||c==TOKEN_INTEGER_LIST||c==TOKEN_FLOAT_LIST ){
			}else if( c==TOKEN_OBRACE ){
				t.text="{";
			}else if( c==TOKEN_CPAREN ){
				t.text="(";
			}else if( c==TOKEN_OPAREN ){
				t.text=")";
			}else if( c==TOKEN_CBRACE ){
				t.text="}";
			}else if( c==TOKEN_OBRACKET ){
				t.text="[";
			}else if( c==TOKEN_CBRACKET ){
				t.text="]";
			}else if( c==TOKEN_SEMICOLON ){
				t.text=";";
			}else if( c==TOKEN_TEMPLATE ){
				t.text="template";
			}else if( c==TOKEN_WORD ){
				t.text="WORD";
			}else if( c==TOKEN_DWORD ){
				t.text="DWORD";
			}else if( c==TOKEN_FLOAT ){
				t.text="FLOAT";
			}else if( c==TOKEN_DOUBLE ){
				t.text="DOUBLE";
			}else if( c==TOKEN_CHAR ){
				t.text="CHAR";
			}else if( c==TOKEN_UCHAR ){
				t.text="UCHAR";
			}else if( c==TOKEN_SWORD ){
				t.text="SWORD";
			}else if( c==TOKEN_SDWORD ){
				t.text="SDWORD";
			}else if( c==TOKEN_VOID ){
				t.text="VOID";
			}else if( c==TOKEN_LPSTR ){
				t.text="LPSTR";
			}else if( c==TOKEN_ARRAY ){
				readShort();
				expectName();
				expect( TOKEN_OBRACKET );
				readToken();
				expect( TOKEN_CBRACKET );
				t.text="";
			}else{
				ex( "Unmapped char "+itoa(c) );
			}
		}else if( format==TEXT ){
			char c=get();
			while( isspace(c) ) c=get();

			if( eof() ) return Token( TOKEN_EOF,"<end-of-file>" );

			std::string s( 1,c );
			if( isalpha(c)||peek()=='_' ){
				char p;
				while( isalnum(p=peek())||p=='_'||p=='-' ) s+=get();

				if( s=="template" ){
					t.type=TOKEN_TEMPLATE;
				}else{
					t.type=TOKEN_NAME;
				}
			}else if( isdigit(c)||c=='-' ){
				while( isdigit(peek()) ) s+=get();
				if( peek()=='.' ){
					s+=get();
					while( isdigit(peek()) ) s+=get();
					if( peek()=='E' ){
						s+=get();
						s+=get();
						while( isdigit(peek()) ) s+=get();
					}
					t.type=TOKEN_FLOAT;
				}else{
					t.type=TOKEN_INTEGER;
				}
			}else if( c=='"' ){
				s="";
				while( peek()!='"' ) s+=get();
				get();
				t.type=TOKEN_STRING;
			}else if( c=='{' ){
				t.type=TOKEN_OBRACE;
			}else if( c=='}' ){
				t.type=TOKEN_CBRACE;
			}else if( c=='[' ){
				t.type=TOKEN_OBRACKET;
			}else if( c==']' ){
				t.type=TOKEN_CBRACKET;
			}else if( c=='<' ){
				t.type=TOKEN_OANGLE;
			}else if( c=='>' ){
				t.type=TOKEN_CANGLE;
			}else if( c=='.' ){
				t.type=TOKEN_DOT;
			}else if( c==',' ){
				t.type=TOKEN_COMMA;
			}else if( c==';' ){
				t.type=TOKEN_SEMICOLON;
			}else if( c=='#'||c=='/'&&peek()=='/' ){
				t.type=TOKEN_COMMENT;
				while( peek()!='\n' ) s+=get();
			}
			t.text=s;
		}
		if( !eof()&&!t.type ) ex( "missing type for "+t.text );
		return t;
	}

	Object *parseObject( Token t ){
		std::string tmpl,id;
		tmpl=t.text;

		t=readToken();
		if( t!="{" ){
			if( t!=TOKEN_NAME ){
				ex( "Expected name, but got "+t.text );
			}
			id=t.text;
			expect( "{" );
		}

		t=readToken();
		if( t=="<" ){
			eat(36);
			expect( ">" );
		}

		tmp.blob.clear();
		tmp.blob.reserve( 1024 );

		Object *obj=d_new Object( tmpl,id );
		if( id!="" ) objects[tmpl][id]=obj;

		bool data=true;
		do {
			if( t=="}" ) break;

			if( t==TOKEN_NAME||t==TOKEN_OBRACE ){
				obj->blob=tmp.blob;

				do{
					if( t==TOKEN_NAME ){
						obj->children.push_back( parseObject( t ) );
					}else if( t==TOKEN_OBRACE ){
						Object *ref=d_new Object( "DataReference",expectName() );
						obj->children.push_back( ref );
						expect( "}" );
					}
				} while( !eof() && (t=readToken())!="}" );

				return obj;
			}

			if( format==BINARY ){
				switch( t.type ){
				case TOKEN_STRING:
					{
						dword count=readInt();
						char *data=tmp.expand( sizeof(dword)+count );
						*data=count;
						if( count>0 ) read( data+sizeof(dword),count );
					}
					readShort();
					break;
				// case TOKEN_INTEGER:
				// case TOKEN_GUID:
				case TOKEN_INTEGER_LIST:
					{
						dword count=readInt();
						int_list list;list.resize( count );
						read( (char*)list.data(),count*sizeof(dword) );
						tmp.add( list );
					}
					break;
				case TOKEN_FLOAT_LIST:
					{
						dword count=readInt();
						size_t len=count*sizeof(float);
						float *data=(float*)tmp.expand( len );
						if( float_bits==32 ){
							read( (char*)data,len );
						}else if( float_bits==64 ){
							double v;
							for( dword i=0;i<count;i++ ){
								read( (char*)&v,sizeof(double) );
								*data++=v;
							}
						}
					}
					break;
				default:
					ex( "Unexpected token "+t.text );
				}
			}else if( format==TEXT ){
				Token next=t;

				switch( next.type ){
				case TOKEN_COMMA:case TOKEN_SEMICOLON:
					break;
				case TOKEN_INTEGER:
					{
						dword i=atof( next.text );
						tmp.add( &i,sizeof(i) );
					}
					break;
				case TOKEN_FLOAT:
					{
						float f=atof( next.text );
						tmp.add( &f,sizeof(f) );
					}
					break;
				case TOKEN_STRING:
					tmp.add( next.text );
					break;
				default:
					ex( "unhandled "+next.text );
				}
			}
			t=readToken();
		} while( !eof() );

		obj->blob=tmp.blob;
		// expect( "}" );
		return obj;
	}

	MeshModel *parseObjects(){
		MeshModel *e=d_new MeshModel();

		std::vector<Object*> objs;

		Token t;
		while( (t=readToken())!=TOKEN_EOF ){
			if( t==TOKEN_NAME ){
				objs.push_back( parseObject( t ) );
			}else if( t==TOKEN_TEMPLATE ){
				expectName();
				expect( "{" );
				while( readToken().text!="}" ){}
			}
		}

		for( auto &obj:objs ){
			if( obj->tmpl=="Mesh" ){
				if( !animonly ) parseMesh( obj,e,mats );
			}else if( obj->tmpl=="Frame" ){
				MeshModel *t=parseFrame( obj );
				t->setParent( e );
			}else if( obj->tmpl=="AnimationSet" ){
				if( !collapse ) parseAnimSet( obj );
			}else if( obj->tmpl=="Header"||obj->tmpl=="Material" ){
				// nothing to do...
			}else{
				log( "Unhandled object "+obj->tmpl );
			}
		}

		return e;
	}

	MeshModel *parseFile(){
		MeshModel *e;

		try{
			e=parseObjects();
		}catch( const std::string &e ){
			LOGE( "[loader.x] %s",e.c_str() );
			return 0;
		}

		if( !collapse ){
			e->setAnimator( d_new Animator( e,anim_len ) );
		}

		return e;
	}
};

MeshModel *Loader_X2::load( const std::string &filename,const Transform &t,int hint ){
	conv_tform=t;
	conv=flip_tris=false;
	if( conv_tform!=Transform() ){
		conv=true;
		if( conv_tform.m.i.cross(conv_tform.m.j).dot(conv_tform.m.k)<0 ) flip_tris=true;
	}
	collapse=!!(hint&MeshLoader::HINT_COLLAPSE);
	animonly=!!(hint&MeshLoader::HINT_ANIMONLY);

	std::streambuf *in;
	if( !(in=gx_filesys->openFile( filename.c_str(),std::ios_base::in )) ){
		return 0;
	}

	XParser x( in );
	if( x.readInt()!=XOFFILE_FORMAT_MAGIC ) return 0;
	if( x.readShort()!=XOFFILE_FORMAT_MAJOR_VERSION ) return 0;
	x.readShort(); // minor version

	switch( x.readInt() ){
	case XOFFILE_FORMAT_BINARY:
		x.setFormat( XParser::BINARY );
		break;
	case XOFFILE_FORMAT_TEXT:
		x.setFormat( XParser::TEXT );
		break;
	default:
		return 0;
	}

	switch( x.readInt() ){
	case XOFFILE_FORMAT_FLOAT_BITS_32:
		x.setFloatBits( 32 );
		break;
	case XOFFILE_FORMAT_FLOAT_BITS_64:
		x.setFloatBits( 64 );
		break;
	default:
		return 0;
	}

	MeshModel *e=x.parseFile();
	frames_map.clear();

	delete in;

	return e;
}
