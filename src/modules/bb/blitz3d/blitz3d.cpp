
#include "std.h"

#ifdef PRO
#include "blitz3d.h"
#include <bb/system/system.h>
#include <bb/graphics/graphics.h>
#include <bb/pixmap/pixmap.h>
#include "blitz3d.h"
#ifndef USE_ASSIMP
#include "loader_x.h"
#else
#include "loader_assimp.h"
#endif
#include "loader_3ds.h"
#include "loader_b3d.h"
#include "std.h"
#include "graphics.h"

B3DGraphics *bbSceneDriver;
BBScene *bbScene;

static int tri_count;
static World *world;

static set<Brush*> brush_set;
static set<Texture*> texture_set;
static set<Entity*> entity_set;

static Listener *listener;

static bool stats_mode;

//converts 0...255 color to 0...1
static const float ctof=1.0f/255.0f;

//degrees to radians and back
static const float dtor=0.0174532925199432957692369076848861f;
static const float rtod=1/dtor;

static Vector projected,tformed;

static ObjCollision picked;

extern float stats3d[10];

#ifdef WIN32
static Loader_X loader_x;
#else
static Loader_Assimp loader_x;
#endif
static Loader_3DS loader_3ds;
static Loader_B3D loader_b3d;

static map<string,Transform> loader_mat_map;

static inline void debug3d(){
	if( bb_env.debug && !bbScene ) RTEX( "3D Graphics mode not set" );
}
static inline void debugTexture( Texture *t ){
	if( bb_env.debug && !texture_set.count( t ) ) RTEX( "Texture does not exist" );
}
static inline void debugBrush( Brush *b ){
	if( bb_env.debug && !brush_set.count( b ) ) RTEX( "Brush does not exist" );
}
static inline void debugEntity( Entity *e ){
	if( bb_env.debug && !entity_set.count(e) ) RTEX( "Entity does not exist" );
}
static inline void debugParent( Entity *e ){
	if( bb_env.debug ){
		debug3d();
		if( e && !entity_set.count(e) ) RTEX( "Parent entity does not exist" );
	}
}
static inline void debugMesh( MeshModel *m ){
	if( bb_env.debug ){
		debugEntity(m);if( !m->getMeshModel() ) RTEX( "Entity is not a mesh" );
	}
}
static inline void debugObject( Object *o ){
	if( bb_env.debug ){
		debugEntity(o);if( !o->getObject() ) RTEX( "Entity is not an object" );
	}
}
static inline void debugColl( Object *o,int index ){
	if( bb_env.debug ){
		debugObject(o);
		if( index<1 || index>o->getCollisions().size() ) RTEX( "Collision index out of range" );
	}
}
static inline void debugCamera( Camera *c ){
	if( bb_env.debug ){
		debugEntity(c);if( !c->getCamera() ) RTEX( "Entity is not a camera" );
	}
}
static inline void debugLight( Light *l ){
	if( bb_env.debug ){
		debugEntity(l);if( !l->getLight() ) RTEX( "Entity is not a light" );
	}
}
static inline void debugModel( Model *m ){
	if( bb_env.debug ){
		debugEntity(m);if( !m->getModel() ) RTEX( "Entity is not a model" );
	}
}
static inline void debugSprite( Sprite *s ){
	if( bb_env.debug ){
		debugModel(s);if( !s->getSprite() ) RTEX( "Entity is not a sprite" );
	}
}
static inline void debugMD2( MD2Model *m ){
	if( bb_env.debug ){
		debugModel(m);if( !m->getMD2Model() ) RTEX( "Entity is not an MD2 Model" );
	}
}
static inline void debugBSP( Q3BSPModel *m ){
	if( bb_env.debug ){
		debugModel(m);if( !m->getBSPModel() ) RTEX( "Entity is not a BSP Model" );
	}
}
static inline void debugTerrain( Terrain *t ){
	if( bb_env.debug ){
		debugModel(t);if( !t->getTerrain() ) RTEX( "Entity is not a terrain" );
	}
}
static inline void debugSegs( int n ){
	if( bb_env.debug ){
		debug3d();
		if( n<3 || n>50 ) RTEX( "Illegal number of segments" );
	}
}
static inline void debugVertex( Surface *s,int n ){
	if( bb_env.debug ){
		debug3d();
		if( n<0 || n>=s->numVertices() ) RTEX( "Vertex index out of range" );
	}
}
static inline void debugVertex( Surface *s,int n,int t ){
	if( bb_env.debug ){
		debug3d();
		if( n<0 || n>=s->numVertices() ) RTEX( "Vertex index out of range" );
		if( t<0 || t>1 ) RTEX( "Texture coordinate set out of range" );
	}
}

static Entity *loadEntity( string t,int hint ){
	t=canonicalpath(t);
	int n=t.rfind( "." );if( n==string::npos ) return 0;
	string ext=t.substr( n+1 );
	MeshLoader *l;

	if( ext=="3ds" ) l=&loader_3ds;
	else if( ext=="b3d" ) l=&loader_b3d;
	else if( ext=="x" ) l=&loader_x;
	else return 0;

	const Transform &conv=loader_mat_map[ext];

	CachedTexture::setPath( filenamepath( t ) );
	Entity *e=l->load( t,conv,hint );
	CachedTexture::setPath( "" );
	return e;
}

static void collapseMesh( MeshModel *mesh,Entity *e ){
	while( e->children() ){
		collapseMesh( mesh,e->children() );
	}
	if( Model *p=e->getModel() ){
		if( MeshModel *t=p->getMeshModel() ){
			t->transform( e->getWorldTform() );
			mesh->add( *t );
		}
	}
	delete e;
}

static void insert( Entity *e ){
	if( bb_env.debug ) entity_set.insert( e );
	e->setVisible(true);
	e->setEnabled(true);
	e->getObject()->reset();
	for( Entity *p=e->children();p;p=p->successor() ){
		insert( p );
	}
}

static Entity *insertEntity( Entity *e,Entity *p ){
	e->setParent( p );
	insert( e );
	return e;
}

static void erase( Entity *e ){
	for( Entity *p=e->children();p;p=p->successor() ){
		erase( p );
	}
	if( e->getListener() ) listener=0;
	if( bb_env.debug ) entity_set.erase( e );
}

static Entity *findChild( Entity *e,const string &t ){
	if( e->getName()==t ) return e;
	for( Entity *p=e->children();p;p=p->successor() ){
		if( Entity *q=findChild(p,t) ) return q;
	}
	return 0;
}

///////////////////////////
// GLOBAL WORLD COMMANDS //
///////////////////////////
void BBCALL bbLoaderMatrix( BBStr *ext,float xx,float xy,float xz,float yx,float yy,float yz,float zx,float zy,float zz ){
	loader_mat_map.erase( *ext );
	loader_mat_map[*ext]=Transform(Matrix(Vector(xx,xy,xz),Vector(yx,yy,yz),Vector(zx,zy,zz)));
	delete ext;
}

int  BBCALL bbHWTexUnits(){
	debug3d();
	return bbScene->hwTexUnits();
}

int	 BBCALL bbGfxDriverCaps3D(){
	debug3d();
	return bbScene->gfxDriverCaps3D();
}

void BBCALL bbHWMultiTex( int enable ){
	debug3d();
	bbScene->setHWMultiTex( !!enable );
}

void BBCALL bbWBuffer( int enable ){
	debug3d();
	bbScene->setWBuffer( !!enable );
}

void BBCALL bbDither( int enable ){
	debug3d();
	bbScene->setDither( !!enable );
}

void BBCALL bbAntiAlias( int enable ){
	debug3d();
	bbScene->setAntialias( !!enable );
}

void BBCALL bbWireFrame( int enable ){
	debug3d();
	bbScene->setWireframe( !!enable );
}

void BBCALL bbAmbientLight( float r,float g,float b ){
	debug3d();
	Vector t( r*ctof,g*ctof,b*ctof );
	bbScene->setAmbient( &(t.x) );
}

void BBCALL bbClearCollisions(){
	debug3d();
	world->clearCollisions();
}

void BBCALL bbCollisions( int src_type,int dest_type,int method,int response ){
	debug3d();
	world->addCollision( src_type,dest_type,method,response );
}

static int update_ms;

void BBCALL bbUpdateWorld( float elapsed ){
	debug3d();

#ifndef BETA
	world->update( elapsed );
	return;
#else
	update_ms=bbMilliSecs();
	world->update( elapsed );
	update_ms=bbMilliSecs()-update_ms;
#endif
}

void BBCALL bbCaptureWorld(){
	debug3d();
	world->capture();
}

void BBCALL bbRenderWorld( float tween ){
	debug3d();

#ifndef BETA
	tri_count=bbScene->getTrianglesDrawn();
	world->render( tween );
	tri_count=bbScene->getTrianglesDrawn()-tri_count;
	return;
#else

	int tris=bbScene->getTrianglesDrawn();
	int render_ms=bbMilliSecs();
	world->render( tween );
	render_ms=bbMilliSecs()-render_ms;

	extern int BBCALL bbKeyHit(int);
	extern void BBCALL bbDelay(int);
	bbDelay(0);
	if( bbKeyHit( 0x57 ) ){
		stats_mode=!stats_mode;
	}
	if( bbKeyHit( 0x58 ) ){
		static int n;
		string t="screenshot"+itoa(++n)+".bmp";
		bbSaveBuffer( bbBackBuffer(),d_new BBStr(t) );
	}

	if( !stats_mode ) return;

	tris=bbScene->getTrianglesDrawn()-tris;

	static int time;
	int frame_ms=bbMilliSecs()-time;
	time+=frame_ms;

	int fps=frame_ms ? 1000/frame_ms : 1000;
	int ups=update_ms ? 1000/update_ms : 1000;
	int rps=render_ms ? 1000/render_ms : 1000;

	string t_fps="000"+itoa(fps);t_fps=t_fps.substr( t_fps.size()-4 );
	string t_ups="000"+itoa(ups);t_ups=t_ups.substr( t_ups.size()-4 );
	string t_rps="000"+itoa(rps);t_rps=t_rps.substr( t_rps.size()-4 );
	string t_tris="00000"+itoa(tris);t_tris=t_tris.substr( t_tris.size()-6 );

	string t="FPS:"+t_fps+" UPS:"+t_ups+" RPS:"+t_rps+" TRIS:"+t_tris;

	bbText( 0,bbGraphicsHeight()-bbFontHeight(),d_new BBStr(t),0,0 );
#endif
}

int BBCALL bbTrisRendered(){
	return tri_count;
}

float BBCALL bbStats3D( int n ){
	return stats3d[n];
}

//////////////////////
// TEXTURE COMMANDS //
//////////////////////

//Note: modify canvas->backup() to NOT release backup image!
//
Texture * BBCALL bbLoadTexture( BBStr *file,int flags ){
	debug3d();
	Texture *t=d_new Texture( *file,flags );delete file;
	if( !t->getCanvas(0) ){ delete t;return 0; }
	texture_set.insert( t );
	return t;
}

Texture * BBCALL bbLoadAnimTexture( BBStr *file,int flags,int w,int h,int first,int cnt ){
	debug3d();
	Texture *t=d_new Texture( *file,flags,w,h,first,cnt );
	delete file;
	if( !t->getCanvas(0) ){
		delete t;
		return 0;
	}
	texture_set.insert( t );
	return t;
}

Texture * BBCALL bbCreateTexture( int w,int h,int flags,int frames ){
	if( bb_env.debug ){
		debug3d();
		if( frames<=0 ){
			RTEX( "Illegal number of texture frames" );
		}
	}
	Texture *t=d_new Texture( w,h,flags,frames );
	texture_set.insert( t );
	return t;
}

void BBCALL bbFreeTexture( Texture *t ){
	if( !t ) return;
	debugTexture(t);
	if( texture_set.erase( t ) ) delete t;
}

void BBCALL bbTextureBlend( Texture *t,int blend ){
	debugTexture(t);
	t->setBlend( blend );
}

void BBCALL bbTextureCoords( Texture *t,int flags ){
	debugTexture(t);
	t->setFlags( flags );
}

void BBCALL bbScaleTexture( Texture *t,float u_scale,float v_scale ){
	debugTexture(t);
	t->setScale( 1/u_scale,1/v_scale );
}

void BBCALL bbRotateTexture( Texture *t,float angle ){
	debugTexture(t);
	t->setRotation( -angle*dtor );
}

void BBCALL bbPositionTexture( Texture *t,float u_pos,float v_pos ){
	debugTexture(t);
	t->setPosition( -u_pos,-v_pos );
}

int BBCALL bbTextureWidth( Texture *t ){
	debugTexture(t);
	return t->getCanvas(0)->getWidth();
}

int BBCALL bbTextureHeight( Texture *t ){
	debugTexture(t);
	return t->getCanvas(0)->getHeight();
}

BBStr * BBCALL bbTextureName( Texture *t ){
	debugTexture(t);
	CachedTexture *c=t->getCachedTexture();
	return c ? d_new BBStr( c->getName().c_str() ) : d_new BBStr("");
}

void BBCALL bbSetCubeFace( Texture *t,int face ){
	debugTexture(t);
	if( BBCanvas *c=t->getCanvas( 0 ) ){
		c->setCubeFace(face);
	}
}

void BBCALL bbSetCubeMode( Texture *t,int mode ){
	debugTexture(t);
	if( BBCanvas *c=t->getCanvas( 0 ) ){
		c->setCubeMode( mode );
	}
}

BBCanvas * BBCALL bbTextureBuffer( Texture *t,int frame ){
	//v1.04
	debugTexture(t);
	if( BBCanvas *c=t->getCanvas( frame ) ){
		if( c->getDepth() ) return c;
	}
	return 0;
	/*
	//v1.03  crashes if t->getCanvas returns null!
	debugTexture(t);
	gxCanvas *c=t->getCanvas( frame );
	if( c->getDepth() ) return c;
	return 0;
	*/
}

void BBCALL bbClearTextureFilters(){
	debug3d();
	Texture::clearFilters();
}

void BBCALL bbTextureFilter( BBStr *t,int flags ){
	debug3d();
	Texture::addFilter( *t,flags );
	delete t;
}

////////////////////
// BRUSH COMMANDS //
////////////////////
Brush * BBCALL bbCreateBrush( float r,float g,float b ){
	debug3d();
	Brush *br=d_new Brush();
	br->setColor( Vector( r*ctof,g*ctof,b*ctof ) );
	brush_set.insert( br );
	return br;
}

Brush * BBCALL bbLoadBrush( BBStr *file,int flags,float u_scale,float v_scale ){
	debug3d();
	Texture t( *file,flags );
	delete file;if( !t.getCanvas(0) ) return 0;
	if( u_scale!=1 || v_scale!=1 ) t.setScale( 1/u_scale,1/v_scale );
	Brush *br=bbCreateBrush( 255,255,255 );
	br->setTexture( 0,t,0 );
	delete file;
	return br;
}

void BBCALL bbFreeBrush( Brush *b ){
	if( !b ) return;
	debugBrush(b);
	if( brush_set.erase( b ) ) delete b;
}

void BBCALL bbBrushColor( Brush *br,float r,float g,float b ){
	debugBrush(br);
	br->setColor( Vector( r*ctof,g*ctof,b*ctof ) );
}

int BBCALL bbGetBrushRed( Brush *b ){
	debugBrush(b);
	Vector c=b->getColor( );
	return c.x*255.0f;
}

int BBCALL bbGetBrushGreen( Brush *b ){
	debugBrush(b);
	Vector c=b->getColor( );
	return c.y*255.0f;
}

int BBCALL bbGetBrushBlue( Brush *b ){
	debugBrush(b);
	Vector c=b->getColor( );
	return c.z*255.0f;
}

void BBCALL bbBrushAlpha( Brush *b,float alpha ){
	debugBrush(b);
	b->setAlpha( alpha );
}

float BBCALL bbGetBrushAlpha( Brush *b ){
	debugBrush(b);
	return b->getAlpha( );
}

void BBCALL bbBrushShininess( Brush *b,float n ){
	debugBrush(b);
	b->setShininess( n );
}

float BBCALL bbGetBrushShininess( Brush *b ){
	debugBrush(b);
	return b->getShininess( );
}

void BBCALL bbBrushTexture( Brush *b,Texture *t,int frame,int index ){
	debugBrush(b);
	debugTexture(t);
	b->setTexture( index,*t,frame );
}

Texture *bbGetBrushTexture( Brush *b,int index ){
	debugBrush(b);
	Texture *tex=d_new Texture(b->getTexture(index));
	texture_set.insert( tex );
	return tex;
}

void BBCALL bbBrushBlend( Brush *b,int blend ){
	debugBrush(b);
	b->setBlend( blend );
}

int BBCALL bbGetBrushBlend( Brush *b ){
	debugBrush(b);
	return b->getBlend( );
}

void BBCALL bbBrushFX( Brush *b,int fx ){
	debugBrush(b);
	b->setFX( fx );
}

int BBCALL bbGetBrushFX( Brush *b ){
	debugBrush(b);
	return b->getFX( );
}

///////////////////
// MESH COMMANDS //
///////////////////
Entity * BBCALL bbCreateMesh( Entity *p ){
	debugParent(p);
	MeshModel *m=d_new MeshModel();
	return insertEntity( m,p );
}

Entity * BBCALL bbLoadMesh( BBStr *f,Entity *p ){
	debugParent(p);
	Entity *e=loadEntity( f->c_str(),MeshLoader::HINT_COLLAPSE );
	delete f;

	if( !e ) return 0;
	MeshModel *m=d_new MeshModel();
	collapseMesh( m,e );
	return insertEntity( m,p );
}

Entity * BBCALL bbLoadAnimMesh( BBStr *f,Entity *p ){
	debugParent(p);
	Entity *e=loadEntity( f->c_str(),0 );
	delete f;

	if( !e ) return 0;
	if( Animator *anim=e->getObject()->getAnimator() ){
		anim->animate( 1,0,0,0 );
	}
	return insertEntity( e,p );
}

Entity * BBCALL bbCreateCube( Entity *p ){
	debugParent(p);
	Entity *e=MeshUtil::createCube( Brush() );
	return insertEntity( e,p );
}

Entity * BBCALL bbCreateSphere( int segs,Entity *p ){
	if( bb_env.debug ){ debugParent(p);if( segs<2 || segs>100 ) RTEX( "Illegal number of segments" ); }
	Entity *e=MeshUtil::createSphere( Brush(),segs );
	return insertEntity( e,p );
}

Entity * BBCALL bbCreateCylinder( int segs,int solid,Entity *p ){
	if( bb_env.debug ){ debugParent(p);if( segs<3 || segs>100 ) RTEX( "Illegal number of segments" ); }
	Entity *e=MeshUtil::createCylinder( Brush(),segs,!!solid );
	return insertEntity( e,p );
}

Entity * BBCALL bbCreateCone( int segs,int solid,Entity *p ){
	if( bb_env.debug ){ debugParent(p);if( segs<3 || segs>100 ) RTEX( "Illegal number of segments" ); }
	Entity *e=MeshUtil::createCone( Brush(),segs,!!solid );
	return insertEntity( e,p );
}

Entity * BBCALL bbCopyMesh( MeshModel *m,Entity *p ){
	debugMesh(m);
	debugParent(p);

	MeshModel *t=d_new MeshModel();
	t->add( *m );
	return insertEntity( t,p );
}

void BBCALL bbScaleMesh( MeshModel *m,float x,float y,float z ){
	debugMesh(m);
	m->transform( scaleMatrix(x,y,z) );
}

void BBCALL bbRotateMesh( MeshModel *m,float x,float y,float z ){
	debugMesh(m);
	m->transform( rotationMatrix(x*dtor,y*dtor,z*dtor) );
}

void BBCALL bbPositionMesh( MeshModel *m,float x,float y,float z ){
	debugMesh(m);
	m->transform( Vector(x,y,z) );
}

void BBCALL bbFitMesh( MeshModel *m,float x,float y,float z,float w,float h,float d,int uniform ){
	debugMesh(m);
	Box box( Vector(x,y,z) );
	box.update( Vector(x+w,y+h,z+d) );
	const Box &curr_box=m->getBox();
	float x_scale=box.width()/curr_box.width();
	float y_scale=box.height()/curr_box.height();
	float z_scale=box.depth()/curr_box.depth();
	Transform t;
	if( uniform ){
		if( x_scale<y_scale && x_scale<z_scale ){
			y_scale=z_scale=x_scale;
		}else if( y_scale<x_scale && y_scale<z_scale ){
			x_scale=z_scale=y_scale;
		}else{
			x_scale=y_scale=z_scale;
		}
	}
	t.m.i.x=x_scale;
	t.m.j.y=y_scale;
	t.m.k.z=z_scale;
	t.v=box.centre() - t.m * curr_box.centre();
	m->transform( t );
}

void BBCALL bbFlipMesh( MeshModel *m ){
	debugMesh(m);
	m->flipTriangles();
}

void BBCALL bbPaintMesh( MeshModel *m,Brush *b ){
	if( bb_env.debug ){ debugMesh(m);debugBrush(b); }
	m->paint( *b );
}

void BBCALL bbAddMesh( MeshModel *src,MeshModel *dest ){
	if( bb_env.debug ){
		debugMesh(src);debugMesh(dest);
		if( src==dest ) RTEX( "A mesh cannot be added to itself" );
	}

	dest->add( *src );
}

void BBCALL bbUpdateNormals( MeshModel *m ){
	debugMesh(m);
	m->updateNormals();
}

void BBCALL bbLightMesh( MeshModel *m,float r,float g,float b,float range,float x,float y,float z ){
	debugMesh(m);
	MeshUtil::lightMesh( m,Vector(x,y,z),Vector(r*ctof,g*ctof,b*ctof),range );
}

float BBCALL bbMeshWidth( MeshModel *m ){
	debugMesh(m);
	return m->getBox().width();
}

float BBCALL bbMeshHeight( MeshModel *m ){
	debugMesh(m);
	return m->getBox().height();
}

float BBCALL bbMeshDepth( MeshModel *m ){
	debugMesh(m);
	return m->getBox().depth();
}

int BBCALL bbMeshesIntersect( MeshModel *a,MeshModel *b ){
	if( bb_env.debug ){ debugMesh(a);debugMesh(b); }
	return a->intersects( *b );
}

int BBCALL bbCountSurfaces( MeshModel *m ){
	debugMesh(m);
	return m->getSurfaces().size();
}

Surface * BBCALL bbGetSurface( MeshModel *m,int index ){
	if( bb_env.debug ){
		debugMesh(m);
		if( index<1 || index>m->getSurfaces().size() ){
			RTEX( "Surface Index out of range" );
		}
	}
	return m->getSurfaces()[index-1];
}

void BBCALL bbMeshCullBox( MeshModel *m,float x,float y,float z,float width,float height,float depth ){
	if( bb_env.debug ){
		debugMesh( m );
	}
	m->setCullBox( Box( Vector(x,y,z),Vector(x+width,y+height,z+depth) ) );
}


//////////////////////
// SURFACE COMMANDS //
//////////////////////
Surface * BBCALL bbFindSurface( MeshModel *m,Brush *b ){
	if( bb_env.debug ){ debugMesh(m);debugBrush(b); }
	return m->findSurface(*b);
}

Surface * BBCALL bbCreateSurface( MeshModel *m,Brush *b ){
	if( bb_env.debug ){ debugMesh(m);if( b ) debugBrush(b); }
	Surface *s=b ? m->createSurface( *b ) : m->createSurface( Brush() );
	return s;
}

Brush * BBCALL bbGetSurfaceBrush( Surface *s ){
	Brush *br=d_new Brush( s->getBrush() );
	brush_set.insert( br );
	return br;
}

Brush * BBCALL bbGetEntityBrush( Model *m ){
	debugModel(m);
	Brush *br=d_new Brush( m->getBrush() );
	brush_set.insert( br );
	return br;
}

void BBCALL bbClearSurface( Surface *s,int verts,int tris ){
	s->clear( !!verts,!!tris );
}

void BBCALL bbPaintSurface( Surface *s,Brush *b ){
	debugBrush(b);
	s->setBrush(*b);
}

int BBCALL bbAddVertex( Surface *s,float x,float y,float z,float tu,float tv,float tw ){
	Surface::Vertex v;
	v.coords=Vector(x,y,z);
	v.color=0xffffffff;
	v.tex_coords[0][0]=v.tex_coords[1][0]=tu;
	v.tex_coords[0][1]=v.tex_coords[1][1]=tv;
	s->addVertex( v );
	return s->numVertices()-1;
}

int BBCALL bbAddTriangle( Surface *s,int v0,int v1,int v2 ){
	Surface::Triangle t;
	t.verts[0]=v0;t.verts[1]=v1;t.verts[2]=v2;
	s->addTriangle( t );
	return s->numTriangles()-1;
}

void BBCALL bbVertexCoords( Surface *s,int n,float x,float y,float z ){
	s->setCoords( n,Vector(x,y,z) );
}

void BBCALL bbVertexNormal( Surface *s,int n,float x,float y,float z ){
	s->setNormal( n,Vector(x,y,z) );
}

void BBCALL bbVertexColor( Surface *s,int n,float r,float g,float b,float a ){
	if(r<0)r=0;else if(r>255)r=255;
	if(g<0)g=0;else if(g>255)g=255;
	if(b<0)b=0;else if(b>255)b=255;
	a*=255;if(a<0)a=0;else if(a>255)a=255;
	s->setColor( n,(int(a)<<24)|(int(r)<<16)|(int(g)<<8)|int(b) );
}

void BBCALL bbVertexTexCoords( Surface *s,int n,float u,float v,float w,int set ){
	s->setTexCoords( n,Vector(u,v,w),set );
}

int BBCALL bbCountVertices( Surface *s ){
	return s->numVertices();
}

int BBCALL bbCountTriangles( Surface *s ){
	return s->numTriangles();
}

float BBCALL bbVertexX( Surface *s,int n ){
	debugVertex(s,n);
	return s->getVertex(n).coords.x;
}
float BBCALL bbVertexY( Surface *s,int n ){
	debugVertex(s,n);
	return s->getVertex(n).coords.y;
}
float BBCALL bbVertexZ( Surface *s,int n ){
	debugVertex(s,n);
	return s->getVertex(n).coords.z;
}
float BBCALL bbVertexNX( Surface *s,int n ){
	debugVertex(s,n);
	return s->getVertex(n).normal.x;
}
float BBCALL bbVertexNY( Surface *s,int n ){
	debugVertex(s,n);
	return s->getVertex(n).normal.y;
}
float BBCALL bbVertexNZ( Surface *s,int n ){
	debugVertex(s,n);
	return s->getVertex(n).normal.z;
}
float BBCALL bbVertexRed( Surface *s,int n ){
	debugVertex(s,n);
	return (s->getVertex(n).color&0xff0000)>>16;
}
float BBCALL bbVertexGreen( Surface *s,int n ){
	debugVertex(s,n);
	return (s->getVertex(n).color&0xff00)>>8;
}
float BBCALL bbVertexBlue( Surface *s,int n ){
	debugVertex(s,n);
	return s->getVertex(n).color&0xff;
}
float BBCALL bbVertexAlpha( Surface *s,int n ){
	debugVertex(s,n);
	return ((s->getVertex(n).color&0xff000000)>>24)/255.0f;
}
float BBCALL bbVertexU( Surface *s,int n,int t ){
	debugVertex(s,n,t);
	return s->getVertex(n).tex_coords[t][0];
}
float BBCALL bbVertexV( Surface *s,int n,int t ){
	debugVertex(s,n,t);
	return s->getVertex(n).tex_coords[t][1];
}
float BBCALL bbVertexW( Surface *s,int n,int t ){
	debugVertex(s,n,t);
	return 1;
}
int BBCALL bbTriangleVertex( Surface *s,int n,int v ){
	return s->getTriangle(n).verts[v];
}

/////////////////////
// CAMERA COMMANDS //
/////////////////////
Entity * BBCALL bbCreateCamera( Entity *p ){
	debugParent(p);
	int x,y,w,h;
	gx_canvas->getViewport( &x,&y,&w,&h );
	Camera *c=d_new Camera();
	c->setViewport( x,y,w,h );
	return insertEntity( c,p );
}

void BBCALL bbCameraZoom( Camera *c,float zoom ){
	debugCamera(c);
	c->setZoom( zoom );
}

void BBCALL bbCameraRange( Camera *c,float nr,float fr ){
	debugCamera(c);
	c->setRange( nr,fr );
}

void BBCALL bbCameraClsColor( Camera *c,float r,float g,float b ){
	debugCamera(c);
	c->setClsColor( Vector( r*ctof,g*ctof,b*ctof ) );
}

void BBCALL bbCameraClsMode( Camera *c,int cls_color,int cls_zbuffer ){
	debugCamera(c);
	c->setClsMode( cls_color ? true : false,cls_zbuffer ? true : false );
}

void BBCALL bbCameraProjMode( Camera *c,int mode ){
	debugCamera(c);
	c->setProjMode( mode );
}

void BBCALL bbCameraViewport( Camera *c,int x,int y,int w,int h ){
	debugCamera(c);
	c->setViewport( x,y,w,h );
}

void BBCALL bbCameraFogRange( Camera *c,float nr,float fr ){
	debugCamera(c);
	c->setFogRange( nr,fr );
}

void BBCALL bbCameraFogColor( Camera *c,float r,float g,float b ){
	debugCamera(c);
	c->setFogColor( Vector( r*ctof,g*ctof,b*ctof ) );
}

void BBCALL bbCameraFogMode( Camera *c,int mode ){
	debugCamera(c);
	c->setFogMode( mode );
}

int BBCALL bbCameraProject( Camera *c,float x,float y,float z ){
	debugCamera(c);
	Vector v=-c->getWorldTform()*Vector(x,y,z);
	const Frustum &f=c->getFrustum();
	if( c->getProjMode()==Camera::PROJ_ORTHO ){
		int vp_x,vp_y,vp_w,vp_h;
		c->getViewport( &vp_x,&vp_y,&vp_w,&vp_h );
		float nr=c->getFrustumNear();
		float fr=c->getFrustumFar();
		float nr_w=c->getFrustumWidth();
		float nr_h=c->getFrustumHeight();
		projected=Vector( (v.x/nr_w+.5f)*vp_w,(.5f-v.y/nr_h)*vp_h,nr );
		return 1;
	}
	if( v.z>0 ){
		float fr=+f.getPlane( Frustum::PLANE_FAR ).d;
		if( v.z<=fr ){
			int vp_x,vp_y,vp_w,vp_h;
			c->getViewport( &vp_x,&vp_y,&vp_w,&vp_h );
			float nr=c->getFrustumNear();
			float fr=c->getFrustumFar();
			float nr_w=c->getFrustumWidth();
			float nr_h=c->getFrustumHeight();
			projected=Vector(
				(v.x*nr/v.z/nr_w+.5f)*vp_w,
				(.5f-v.y*nr/v.z/nr_h)*vp_h,nr );
			return 1;
		}
	}
	projected=Vector();
	return 0;
}

float BBCALL bbProjectedX(){
	return projected.x;
}

float BBCALL bbProjectedY(){
	return projected.y;
}

float BBCALL bbProjectedZ(){
	return projected.z;
}

static Object *doPick( const Line &l,float radius ){
	picked.collision.time=1;
	return world->traceRay( l,radius,&picked );
}

Entity * BBCALL bbCameraPick( Camera *c,float x,float y ){
	debugCamera(c);

	int vp_x,vp_y,vp_w,vp_h;
	c->getViewport( &vp_x,&vp_y,&vp_w,&vp_h );
	float nr=c->getFrustumNear();
	float fr=c->getFrustumFar();
	float nr_w=c->getFrustumWidth();
	float nr_h=c->getFrustumHeight();

	x=((x/vp_w)-.5f)*nr_w;
	y=(.5f-(y/vp_h))*nr_h;

	Line l;
	if( c->getProjMode()==Camera::PROJ_ORTHO ){
		l=c->getWorldTform() * Line( Vector(x,y,0),Vector(0,0,fr) );	//x,y,fr) );
	}else{
		x/=nr;y/=nr;
		l=c->getWorldTform() * Line( Vector(),Vector( x*fr,y*fr,fr ) );
	}

	return doPick( l,0 );
}

Entity * BBCALL bbLinePick( float x,float y,float z,float dx,float dy,float dz,float radius ){
	debug3d();

	Line l( Vector( x,y,z ),Vector( dx,dy,dz ) );

	return doPick( l,radius );
}

Entity * BBCALL bbEntityPick( Object *src,float range ){
	debugEntity(src);

	Line l( src->getWorldPosition(),src->getWorldTform().m.k * range );

	return doPick( l,0 );
}

int BBCALL bbEntityVisible( Object *src,Object *dest ){
	if( bb_env.debug ){ debugObject(src);debugObject(dest); }

	return world->checkLOS( src,dest ) ? 1 : 0;
}

int BBCALL bbEntityInView( Entity *e,Camera *c ){
	if( bb_env.debug ){ debugEntity(e);debugCamera(c); }
	if( Model *p=e->getModel() ){
		if( MeshModel *m=p->getMeshModel() ){
			const Box &b=m->getBox();
			Transform t=-c->getWorldTform() * e->getWorldTform();
			Vector p[]={
				t*b.corner(0),t*b.corner(1),t*b.corner(2),t*b.corner(3),
				t*b.corner(4),t*b.corner(5),t*b.corner(6),t*b.corner(7)
			};
			return c->getFrustum().cull( p,8 );
		}
	}
	Vector p[]={ -c->getWorldTform() * e->getWorldPosition() };
	return c->getFrustum().cull( p,1 );
}

float BBCALL bbPickedX(){
	return picked.coords.x;
}

float BBCALL bbPickedY(){
	return picked.coords.y;
}

float BBCALL bbPickedZ(){
	return picked.coords.z;
}

float BBCALL bbPickedNX(){
	return picked.collision.normal.x;
}

float BBCALL bbPickedNY(){
	return picked.collision.normal.y;
}

float BBCALL bbPickedNZ(){
	return picked.collision.normal.z;
}

float BBCALL bbPickedTime(){
	return picked.collision.time;
}

Object * BBCALL bbPickedEntity(){
	return picked.with;
}

void * BBCALL bbPickedSurface(){
	return picked.collision.surface;
}

int BBCALL bbPickedTriangle(){
	return picked.collision.index;
}

////////////////////
// LIGHT COMMANDS //
////////////////////
Entity * BBCALL bbCreateLight( int type,Entity *p ){
	debugParent(p);
	Light *t=d_new Light( type );
	return insertEntity( t,p );
}

void BBCALL bbLightColor( Light *light,float r,float g,float b ){
	debugLight(light);
	light->setColor( Vector(r*ctof,g*ctof,b*ctof) );
}

void BBCALL bbLightRange( Light *light,float range ){
	debugLight(light);
	light->setRange( range );
}

void BBCALL bbLightConeAngles( Light *light,float inner,float outer ){
	debugLight(light);
	inner*=dtor;
	outer*=dtor;
	if( inner<0 ) inner=0;
	else if( inner>PI ) inner=PI;
	if( outer<inner ) outer=inner;
	else if( outer>PI ) outer=PI;
	light->setConeAngles( inner,outer );
}

////////////////////
// PIVOT COMMANDS //
////////////////////
Entity * BBCALL bbCreatePivot( Entity *p ){
	debugParent(p);
	Pivot *t=d_new Pivot();
	return insertEntity( t,p );
}

/////////////////////
// SPRITE COMMANDS //
/////////////////////
Entity * BBCALL bbCreateSprite( Entity *p ){
	debugParent(p);
	Sprite *s=d_new Sprite();
	s->setFX( BBScene::FX_FULLBRIGHT );
	return insertEntity( s,p );
}

Entity * BBCALL bbLoadSprite( BBStr *file,int flags,Entity *p ){
	debugParent(p);
	Texture t( *file,flags );
	delete file;if( !t.getCanvas(0) ) return 0;
	Sprite *s=d_new Sprite();
	s->setTexture( 0,t,0 );
	s->setFX( BBScene::FX_FULLBRIGHT );

	if( flags & BBCanvas::CANVAS_TEX_MASK ) s->setBlend( BBScene::BLEND_REPLACE );
	else if( flags & BBCanvas::CANVAS_TEX_ALPHA ) s->setBlend( BBScene::BLEND_ALPHA );
	else s->setBlend( BBScene::BLEND_ADD );

	return insertEntity( s,p );
}

void BBCALL bbRotateSprite( Sprite *s,float angle ){
	debugSprite(s);
	s->setRotation( angle*dtor );
}

void BBCALL bbScaleSprite( Sprite *s,float x,float y ){
	debugSprite(s);
	s->setScale( x,y );
}

void BBCALL bbHandleSprite( Sprite *s,float x,float y ){
	debugSprite(s);
	s->setHandle( x,y );
}

void BBCALL bbSpriteViewMode( Sprite *s,int mode ){
	debugSprite(s);
	s->setViewmode( mode );
}

/////////////////////
// MIRROR COMMANDS //
/////////////////////
Entity * BBCALL bbCreateMirror( Entity *p ){
	debugParent(p);
	Mirror *t=d_new Mirror();
	return insertEntity( t,p );
}

////////////////////
// PLANE COMMANDS //
////////////////////
Entity * BBCALL bbCreatePlane( int segs,Entity *p ){
	if( bb_env.debug ){
		debugParent(p);
		if( segs<1 || segs>20 ) RTEX( "Illegal number of segments" );
	}
	PlaneModel *t=d_new PlaneModel( segs );
	return insertEntity( t,p );
}

//////////////////
// MD2 COMMANDS //
//////////////////
Entity * BBCALL bbLoadMD2( BBStr *file,Entity *p ){
	debugParent(p);
	MD2Model *t=d_new MD2Model( *file );delete file;
	if( !t->getValid() ){ delete t;return 0; }
	return insertEntity( t,p );
}

void BBCALL bbAnimateMD2( MD2Model *m,int mode,float speed,int first,int last,float trans ){
	debugMD2(m);
	m->startMD2Anim( first,last,mode,speed,trans );
}

float BBCALL bbMD2AnimTime( MD2Model *m ){
	debugMD2(m);
	return m->getMD2AnimTime();
}

int BBCALL bbMD2AnimLength( MD2Model *m ){
	debugMD2(m);
	return m->getMD2AnimLength();
}

int BBCALL bbMD2Animating( MD2Model *m ){
	debugMD2(m);
	return m->getMD2Animating();
}

//////////////////
// BSP Commands //
//////////////////
Entity * BBCALL bbLoadBSP( BBStr *file,float gam,Entity *p ){
	debugParent(p);
	CachedTexture::setPath( filenamepath( *file ) );
	Q3BSPModel *t=d_new Q3BSPModel( *file,gam );delete file;
	CachedTexture::setPath( "" );

	if( !t->isValid() ){ delete t;return 0; }

	return insertEntity( t,p );
}

void BBCALL bbBSPAmbientLight( Q3BSPModel *t,float r,float g,float b ){
	debugBSP(t);
	t->setAmbient( Vector( r*ctof,g*ctof,b*ctof ) );
}

void BBCALL bbBSPLighting( Q3BSPModel *t,int lmap ){
	debugBSP(t);
	t->setLighting( !!lmap );
}

//////////////////////
// TERRAIN COMMANDS //
//////////////////////
static float terrainHeight( Terrain *t,float x,float z ){
	int ix=floor(x);
	int iz=floor(z);
	float tx=x-ix,tz=z-iz;
	float h0=t->getHeight(ix,iz);
	float h1=t->getHeight(ix+1,iz);
	float h2=t->getHeight(ix,iz+1);
	float h3=t->getHeight(ix+1,iz+1);
	float ha=(h1-h0)*tx+h0,hb=(h3-h2)*tx+h2;
	float h=(hb-ha)*tz+ha;
	return h;
}

static Vector terrainVector( Terrain *t,float x,float y,float z ){
	Vector v=-t->getWorldTform() * Vector( x,y,z );
	return t->getWorldTform() * Vector( v.x,terrainHeight( t,v.x,v.z ),v.z );
}

Entity * BBCALL bbCreateTerrain( int n,Entity *p ){
	debugParent(p);
	int shift=0;
	while( (1<<shift)<n ) ++shift;
	if( (1<<shift)!=n ) RTEX( "Illegal terrain size" );
	Terrain *t=d_new Terrain( shift );
	return insertEntity( t,p );
}

Entity * BBCALL bbLoadTerrain( BBStr *file,Entity *p ){
	debugParent(p);
	BBPixmap *m=bbLoadPixmap( *file );
	int w=m->getWidth(),h=m->getHeight();
	if( w!=h ) RTEX( "Terrain must be square" );
	int shift=0;
	while( (1<<shift)<w ) ++shift;
	if( (1<<shift)!=w ) RTEX( "Illegal terrain size" );
	Terrain *t=d_new Terrain( shift );
	for( int y=0;y<h;++y ){
		for( int x=0;x<w;++x ){
			int rgb=m->read( x,y );
			int r=(rgb>>16)&0xff,g=(rgb>>8)&0xff,b=rgb&0xff;
			float p=(r>g?(r>b?r:b):(g>b?g:b))/255.0f;
			t->setHeight( x,h-1-y,p,false );
		}
	}
	delete m;
	return insertEntity( t,p );
}

void BBCALL bbTerrainDetail( Terrain *t,int n,int m ){
	debugTerrain(t);
	t->setDetail( n,!!m );
}

void BBCALL bbTerrainShading( Terrain *t,int enable ){
	debugTerrain(t);
	t->setShading( !!enable );
}

float BBCALL bbTerrainX( Terrain *t,float x,float y,float z ){
	debugTerrain(t);
	return terrainVector( t,x,y,z ).x;
}

float BBCALL bbTerrainY( Terrain *t,float x,float y,float z ){
	debugTerrain(t);
	return terrainVector( t,x,y,z ).y;
}

float BBCALL bbTerrainZ( Terrain *t,float x,float y,float z ){
	debugTerrain(t);
	return terrainVector( t,x,y,z ).z;
}

int BBCALL bbTerrainSize( Terrain *t ){
	debugTerrain(t);
	return t->getSize();
}

float BBCALL bbTerrainHeight( Terrain *t,int x,int z ){
	debugTerrain(t);
	return t->getHeight( x,z );
}

void BBCALL bbModifyTerrain( Terrain *t,int x,int z,float h,int realtime ){
	debugTerrain(t);
	t->setHeight( x,z,h,!!realtime );
}

////////////////////
// AUDIO COMMANDS //
////////////////////
Entity * BBCALL bbCreateListener( Entity *p,float roll,float dopp,float dist ){
	if( bb_env.debug ){
		debugParent(p);
		if( listener ) RTEX( "Listener already created" );
	}
	listener=d_new Listener( roll,dopp,dist );
	return insertEntity( listener,p );
}

BBChannel * BBCALL bbEmitSound( BBSound *sound,Object *o ){
	if( bb_env.debug ){
		debugObject(o);
		if( !listener ) RTEX( "No Listener created" );
	}
	return o->emitSound( sound );
}

/////////////////////
// ENTITY COMMANDS //
/////////////////////
Entity * BBCALL bbCopyEntity( Entity *e,Entity *p ){
	if( bb_env.debug ){
		debugEntity(e);
		debugParent(p);
	}
	Entity *t=e->getObject()->copy();
	if( !t ) return 0;
	return insertEntity( t,p );
}

void BBCALL bbFreeEntity( Entity *e ){
	if( !e ) return;
	if( bb_env.debug ){
		debugEntity(e);
		erase(e);
	}
	delete e;
}

void BBCALL bbHideEntity( Entity *e ){
	debugEntity(e);
	e->setEnabled(false);
	e->setVisible(false);
}

void BBCALL bbShowEntity( Entity *e ){
	debugEntity(e);
	e->setVisible(true);
	e->setEnabled(true);
	e->getObject()->reset();
}

void BBCALL bbEntityParent( Entity *e,Entity *p,int global ){
	if( bb_env.debug ){
		debugEntity(e);
		debugParent(p);
		Entity *t=p;
		while( t ){
			if( t==e ){
				RTEX( "Entity cannot be parented to itself!" );
			}
			t=t->getParent();
		}
	}

	if( e->getParent()==p ) return;

	if( global ){
		Transform t=e->getWorldTform();
		e->setParent( p );
		e->setWorldTform( t );
	}else{
		e->setParent( p );
		e->getObject()->reset();
	}
}

int BBCALL bbCountChildren( Entity *e ){
	debugEntity(e);
	int n=0;
	for( Entity *p=e->children();p;p=p->successor() ) ++n;
	return n;
}

Entity * BBCALL bbGetChild( Entity *e,int index ){
	debugEntity(e);
	Entity *p=e->children();
	while( --index && p ) p=p->successor();
	return p;
}

Entity * BBCALL bbFindChild( Entity *e,BBStr *t ){
	debugEntity(e);
	e=findChild( e,*t );
	delete t;
	return e;
}

////////////////////////
// ANIMATION COMMANDS //
////////////////////////
int BBCALL bbLoadAnimSeq( Object *o,BBStr *f ){
	debugObject( o );
	if( Animator *anim=o->getAnimator() ){
		Entity *t=loadEntity( f->c_str(),MeshLoader::HINT_ANIMONLY );
		delete f;
		if( t ){
			if( Animator *p=t->getObject()->getAnimator() ){
				anim->addSeqs( p );
			}
			delete t;
		}
		return anim->numSeqs()-1;
	}else{
		delete f;
	}
	return -1;
}

void BBCALL bbSetAnimTime( Object *o,float time,int seq ){
	debugObject( o );
	if( Animator *anim=o->getAnimator() ){
		anim->setAnimTime( time,seq );
	}else{
		RTEX( "Entity has not animation" );
	}
}

void BBCALL bbAnimate( Object *o,int mode,float speed,int seq,float trans ){
	debugObject( o );
	if( Animator *anim=o->getAnimator() ){
		anim->animate( mode,speed,seq,trans );
	}else{
		RTEX( "Entity has no animation" );
	}
}

void BBCALL bbSetAnimKey( Object *o,int frame,int pos_key,int rot_key,int scl_key ){
	debugObject( o );
	Animation anim=o->getAnimation();
	if( pos_key ) anim.setPositionKey( frame,o->getLocalPosition() );
	if( rot_key ) anim.setRotationKey( frame,o->getLocalRotation() );
	if( scl_key ) anim.setScaleKey( frame,o->getLocalScale() );
	o->setAnimation( anim );
}

int BBCALL bbExtractAnimSeq( Object *o,int first,int last,int seq ){
	debugObject( o );
	if( Animator *anim=o->getAnimator() ){
		anim->extractSeq( first,last,seq );
		return anim->numSeqs()-1;
	}
	return -1;
}

int BBCALL bbAddAnimSeq( Object *o,int length ){
	debugObject( o );
	Animator *anim=o->getAnimator();
	if( anim ){
		anim->addSeq( length );
	}else{
		anim=d_new Animator( o,length );
		o->setAnimator( anim );
	}
	return anim->numSeqs()-1;
}

int BBCALL bbAnimSeq( Object *o ){
	debugObject(o);
	if( Animator *anim=o->getAnimator() ) return anim->animSeq();
	return -1;
}

float BBCALL bbAnimTime( Object *o ){
	debugObject(o);
	if( Animator *anim=o->getAnimator() ) return anim->animTime();
	return -1;
}

int BBCALL bbAnimLength( Object *o ){
	debugObject(o);
	if( Animator *anim=o->getAnimator() ) return anim->animLen();
	return -1;
}

int BBCALL bbAnimating( Object *o ){
	debugObject(o);
	if( Animator *anim=o->getAnimator() ) return anim->animating();
	return 0;
}

////////////////////////////////
// ENTITY SPECIAL FX COMMANDS //
////////////////////////////////
void BBCALL bbPaintEntity( Model *m,Brush *b ){
	if( bb_env.debug ){
		debugModel(m);
		debugBrush(b);
	}
	m->setBrush( *b );
}

void BBCALL bbEntityColor( Model *m,float r,float g,float b ){
	debugModel(m);
	m->setColor( Vector( r*ctof,g*ctof,b*ctof ) );
}

void BBCALL bbEntityAlpha( Model *m,float alpha ){
	debugModel(m);
	m->setAlpha( alpha );
}

void BBCALL bbEntityShininess( Model *m,float shininess ){
	debugModel(m);
	m->setShininess( shininess );
}

void BBCALL bbEntityTexture( Model *m,Texture *t,int frame,int index ){
	debugModel(m);
	debugTexture(t);
	m->setTexture( index,*t,frame );
}

void BBCALL bbEntityBlend( Model *m,int blend ){
	debugModel(m);
	m->setBlend( blend );
}

void BBCALL bbEntityFX( Model *m,int fx ){
	debugModel(m);
	m->setFX( fx );
}

void BBCALL bbEntityAutoFade( Model *m,float nr,float fr ){
	debugModel(m);
	m->setAutoFade( nr,fr );
}

void BBCALL bbEntityOrder( Object *o,int n ){
	if( bb_env.debug ){
		debugEntity(o);
		if( !o->getModel() && !o->getCamera() ){
			RTEX( "Entity is not a model or camera" );
		}
	}
	o->setOrder( n );
}

//////////////////////////////
// ENTITY PROPERTY COMMANDS //
//////////////////////////////
float BBCALL bbEntityX( Entity *e,int global ){
	debugEntity(e);
	return global ? e->getWorldPosition().x : e->getLocalPosition().x;
}

float BBCALL bbEntityY( Entity *e,int global ){
	debugEntity(e);
	return global ? e->getWorldPosition().y : e->getLocalPosition().y;
}

float BBCALL bbEntityZ( Entity *e,int global ){
	debugEntity(e);
	return global ? e->getWorldPosition().z : e->getLocalPosition().z;
}

float BBCALL bbEntityPitch( Entity *e,int global ){
	debugEntity(e);
	return quatPitch( global ? e->getWorldRotation() : e->getLocalRotation() ) * rtod;
}

float BBCALL bbEntityYaw( Entity *e,int global ){
	debugEntity(e);
	return quatYaw( global ? e->getWorldRotation() : e->getLocalRotation() ) * rtod;
}

float BBCALL bbEntityRoll( Entity *e,int global ){
	debugEntity(e);
	return quatRoll( global ? e->getWorldRotation() : e->getLocalRotation() ) * rtod;
}

float BBCALL bbGetMatElement( Entity *e,int row,int col ){
	debugEntity(e);
	return row<3 ? e->getWorldTform().m[row][col] : e->getWorldTform().v[col];
}

void BBCALL bbTFormPoint( float x,float y,float z,Entity *src,Entity *dest ){
	if( bb_env.debug ){
		if( src ) debugEntity(src);
		if( dest ) debugEntity(dest);
	}
	tformed=Vector( x,y,z );
	if( src ) tformed=src->getWorldTform() * tformed;
	if( dest ) tformed=-dest->getWorldTform() * tformed;
}

void BBCALL bbTFormVector( float x,float y,float z,Entity *src,Entity *dest ){
	if( bb_env.debug ){
		if( src ) debugEntity(src);
		if( dest ) debugEntity(dest);
	}
	tformed=Vector( x,y,z );
	if( src ) tformed=src->getWorldTform().m * tformed;
	if( dest ) tformed=-dest->getWorldTform().m * tformed;
}

void BBCALL bbTFormNormal( float x,float y,float z,Entity *src,Entity *dest ){
	if( bb_env.debug ){
		if( src ) debugEntity(src);
		if( dest ) debugEntity(dest);
	}
	tformed=Vector( x,y,z );
	if( src ) tformed=(src->getWorldTform().m).cofactor() * tformed;
	if( dest ) tformed=(-dest->getWorldTform().m).cofactor() * tformed;
	tformed.normalize();
}

float BBCALL bbTFormedX(){
	return tformed.x;
}

float BBCALL bbTFormedY(){
	return tformed.y;
}

float BBCALL bbTFormedZ(){
	return tformed.z;
}

float BBCALL bbVectorYaw( float x,float y,float z ){
	return Vector(x,y,z).yaw() * rtod;
}

float BBCALL bbVectorPitch( float x,float y,float z ){
	return Vector(x,y,z).pitch() * rtod;
}

float BBCALL bbDeltaYaw( Entity *src,Entity *dest ){
	float x=src->getWorldTform().m.k.yaw();
	float y=(dest->getWorldTform().v-src->getWorldTform().v).yaw();
	float d=y-x;
	if( d<-PI ) d+=TWOPI;
	else if( d>=PI ) d-=TWOPI;
	return d*rtod;
}

float BBCALL bbDeltaPitch( Entity *src,Entity *dest ){
	float x=src->getWorldTform().m.k.pitch();
	float y=(dest->getWorldTform().v-src->getWorldTform().v).pitch();
	float d=y-x;
	if( d<-PI ) d+=TWOPI;
	else if( d>=PI ) d-=TWOPI;
	return d*rtod;
}

///////////////////////////////
// ENTITY COLLISION COMMANDS //
///////////////////////////////
void BBCALL bbResetEntity( Object *o ){
	debugObject(o);
	o->reset();
}

static void entityType( Entity *e,int type ){
	e->getObject()->setCollisionType(type);
	e->getObject()->reset();
	for( Entity *p=e->children();p;p=p->successor() ){
		entityType( p,type );
	}
}

void BBCALL bbEntityType( Object *o,int type,int recurs ){
	if( bb_env.debug ){
		debugObject(o);
		if( type<0 || type>999 ) RTEX( "EntityType ID must be in the range 0...999" );
	}
	if( recurs ) entityType( o,type );
	else{
		o->setCollisionType(type);
		o->reset();
	}
}

void BBCALL bbEntityPickMode( Object *o,int mode,int obs ){
	debugObject(o);
	o->setPickGeometry( mode );
	o->setObscurer( !!obs );
}

Entity * BBCALL bbGetParent( Entity *e ){
	debugEntity(e);
	return e->getParent();
}

int BBCALL bbGetEntityType( Object *o ){
	debugObject(o);
	return o->getCollisionType();
}

void BBCALL bbEntityRadius( Object *o,float x_radius,float y_radius ){
	debugObject(o);
	Vector radii( x_radius,y_radius ? y_radius : x_radius,x_radius );
	o->setCollisionRadii( radii );
}

void BBCALL bbEntityBox( Object *o,float x,float y,float z,float w,float h,float d ){
	debugObject(o);
	Box b( Vector(x,y,z) );
	b.update( Vector( x+w,y+h,z+d ) );
	o->setCollisionBox( b );
}

Object * BBCALL bbEntityCollided( Object *o,int type ){
	debugObject(o);
	Object::Collisions::const_iterator it;
	const Object::Collisions &c=o->getCollisions();
	for( it=c.begin();it!=c.end();++it ){
		const ObjCollision *c=*it;
		if( c->with->getCollisionType()==type ) return c->with;
	}
	return 0;
}

int BBCALL bbCountCollisions( Object *o ){
	debugObject(o);
	return o->getCollisions().size();
}

float BBCALL bbCollisionX( Object *o,int index ){
	debugColl(o,index);
	return o->getCollisions()[index-1]->coords.x;
}

float BBCALL bbCollisionY( Object *o,int index ){
	debugColl(o,index);
	return o->getCollisions()[index-1]->coords.y;
}

float BBCALL bbCollisionZ( Object *o,int index ){
	debugColl(o,index);
	return o->getCollisions()[index-1]->coords.z;
}

float BBCALL bbCollisionNX( Object *o,int index ){
	debugColl(o,index);
	return o->getCollisions()[index-1]->collision.normal.x;
}

float BBCALL bbCollisionNY( Object *o,int index ){
	debugColl(o,index);
	return o->getCollisions()[index-1]->collision.normal.y;
}

float BBCALL bbCollisionNZ( Object *o,int index ){
	debugColl(o,index);
	return o->getCollisions()[index-1]->collision.normal.z;
}

float BBCALL bbCollisionTime( Object *o,int index ){
	debugColl(o,index);
	return o->getCollisions()[index-1]->collision.time;
}

Object * BBCALL bbCollisionEntity( Object *o,int index ){
	debugColl(o,index);
	return o->getCollisions()[index-1]->with;
}

void * BBCALL bbCollisionSurface( Object *o,int index ){
	debugColl(o,index);
	return o->getCollisions()[index-1]->collision.surface;
}

int BBCALL bbCollisionTriangle( Object *o,int index ){
	debugColl(o,index);
	return o->getCollisions()[index-1]->collision.index;
}

float BBCALL BBCALL bbEntityDistance( Entity *src,Entity *dest ){
	debugEntity(src);
	debugEntity(dest);
	return src->getWorldPosition().distance( dest->getWorldPosition() );
}

////////////////////////////////////
// ENTITY TRANSFORMATION COMMANDS //
////////////////////////////////////
void BBCALL bbMoveEntity( Entity *e,float x,float y,float z ){
	debugEntity(e);
	e->setLocalPosition( e->getLocalPosition()+e->getLocalRotation()*Vector(x,y,z) );
}

void BBCALL bbTurnEntity( Entity *e,float p,float y,float r,int global ){
	debugEntity(e);
	global?
	e->setWorldRotation( rotationQuat( p*dtor,y*dtor,r*dtor )*e->getWorldRotation() ):
	e->setLocalRotation( e->getLocalRotation()*rotationQuat( p*dtor,y*dtor,r*dtor ) );
}

void BBCALL bbTranslateEntity( Entity *e,float x,float y,float z,int global ){
	debugEntity(e);
	global?
	e->setWorldPosition( e->getWorldPosition()+Vector( x,y,z ) ):
	e->setLocalPosition( e->getLocalPosition()+Vector( x,y,z ) );
}

void BBCALL bbPositionEntity( Entity *e,float x,float y,float z,int global ){
	debugEntity(e);
	global?
	e->setWorldPosition(Vector(x,y,z)):
	e->setLocalPosition(Vector(x,y,z));
}

void BBCALL bbScaleEntity( Entity *e,float x,float y,float z,int global ){
	debugEntity(e);
	global?
	e->setWorldScale(Vector(x,y,z)):
	e->setLocalScale(Vector(x,y,z));
}

void BBCALL bbRotateEntity( Entity *e,float p,float y,float r,int global ){
	debugEntity(e);
	global?
	e->setWorldRotation( rotationQuat( p*dtor,y*dtor,r*dtor ) ):
	e->setLocalRotation( rotationQuat( p*dtor,y*dtor,r*dtor ) );
}

void BBCALL bbPointEntity( Entity *e,Entity *t,float roll ){
	if( bb_env.debug ){ debugEntity(e);debugEntity(t); }
	Vector v=t->getWorldTform().v-e->getWorldTform().v;
	e->setWorldRotation( rotationQuat( v.pitch(),v.yaw(),roll*dtor ) );
}

void BBCALL bbAlignToVector( Entity *e,float nx,float ny,float nz,int axis,float rate ){
	Vector ax( nx,ny,nz );
	float l=ax.length();
	if( l<=EPSILON ) return;
	ax/=l;

	Quat q=e->getWorldRotation();
	Vector tv=(axis==1) ? q.i() : (axis==2 ? q.j() : q.k());

	float dp=ax.dot( tv );

	if( dp>=1-EPSILON ) return;

	if( dp<=-1+EPSILON ){
		float an=PI*rate/2;
		Vector cp=(axis==1) ? q.j() : (axis==2 ? q.k() : q.i());
		e->setWorldRotation( Quat( cosf(an),cp*sinf(an) ) * q );
		return;
	}

	float an=acosf( dp )*rate/2;
	Vector cp=ax.cross( tv ).normalized();
	e->setWorldRotation( Quat( cosf(an),cp*sinf(an) ) * q );
}

//////////////////////////
// ENTITY MISC COMMANDS //
//////////////////////////
void BBCALL bbNameEntity( Entity *e,BBStr *t ){
	debugEntity(e);
	e->setName( *t );
	delete t;
}

BBStr * BBCALL bbEntityName( Entity *e ){
	debugEntity(e);
	return d_new BBStr( e->getName() );
}

BBStr * BBCALL bbEntityClass( Entity *e ){
	debugEntity(e);
	const char *p="Pivot";
	if( e->getLight() ) p="Light";
	else if( e->getCamera() ) p="Camera";
	else if( e->getMirror() ) p="Mirror";
	else if( e->getListener() ) p="Listener";
	else if( Model *t=e->getModel() ){
		if( t->getSprite() ) p="Sprite";
		else if( t->getTerrain() ) p="Terrain";
		else if( t->getPlaneModel() ) p="Plane";
		else if( t->getMeshModel() ) p="Mesh";
		else if( t->getMD2Model() ) p="MD2";
		else if( t->getBSPModel() ) p="BSP";
	}
	return new BBStr(p);
}

void BBCALL bbClearWorld( int e,int b,int t ){
	if( e ){
		while( Entity::orphans() ) bbFreeEntity( Entity::orphans() );
	}
	if( b ){
		while( brush_set.size() ) bbFreeBrush( *brush_set.begin() );
	}
	if( t ){
		while( texture_set.size() ) bbFreeTexture( *texture_set.begin() );
	}
}

extern int active_texs;

int BBCALL bbActiveTextures(){
	return active_texs;
}

void blitz3d_open( int w,int h ){
	bbScene=bbSceneDriver->createScene( w,h,0 );
	if( !bbScene ) RTEX( "Unable to create 3D Scene" );
	world=d_new World();
	projected=Vector();
	picked.collision=Collision();
	picked.with=0;picked.coords=Vector();
	Texture::clearFilters();
	Texture::addFilter( "",BBCanvas::CANVAS_TEX_RGB|BBCanvas::CANVAS_TEX_MIPMAP );
	loader_mat_map.clear();
	loader_mat_map["x"]=Transform();
	loader_mat_map["3ds"]=Transform(Matrix(Vector(1,0,0),Vector(0,0,1),Vector(0,1,0)));
	listener=0;
	stats_mode=false;
}

void blitz3d_close(){
	if( !bbScene ) return;
	bbClearWorld( 1,1,1 );
	Texture::clearFilters();
	loader_mat_map.clear();
	delete world;
	bbScene=0;
}

BBMODULE_CREATE( blitz3d ){
	tri_count=0;
	bbScene=0;world=0;
	return true;
}

BBMODULE_DESTROY( blitz3d ){
	blitz3d_close();
	return true;
}

#endif
