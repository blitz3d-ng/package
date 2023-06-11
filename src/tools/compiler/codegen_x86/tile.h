
#ifndef TILE_H
#define TILE_H

enum{ EAX=1,ECX,EDX,EDI,ESI,EBX };

struct Tile{

	int want_l,want_r,hits,argFrame;

	Tile( const std::string &a,Tile *l=0,Tile *r=0 );
	Tile( const std::string &a,const std::string &a2,Tile *l=0,Tile *r=0 );
	~Tile();

	void label();
	int  eval( int want );

private:
	int  need;
	Tile *l,*r;
	std::string assem,assem2;

};

#endif
