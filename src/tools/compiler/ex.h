
#ifndef EX_H
#define EX_H

struct Ex{
	std::string ex;		//what happened
	int pos;		//source offset
	std::string file;
	Ex( const std::string &ex ):ex(ex),pos(-1){}
	Ex( const std::string &ex,int pos,const std::string &t ):ex(ex),pos(pos),file(t){}
};

#endif
