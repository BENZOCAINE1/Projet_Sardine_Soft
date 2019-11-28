#ifndef ERTMS
#define ERTMS

#include "Data.hpp"
#include "Symbol.hpp"
#include "Button.hpp"
#include "Default.hpp"
#include "define.hpp"

class ETCS : public Fenetre
{
	private :
		vector <Symbol> symbol;
		vector <Button> button;
		Event event;
		Default def;
		//Special special;
		//Settings settings;
		//SRspeed srSpeed;
		//DataView dataView;
		//SystemVersion systemVersion;
		VertexArray fond{Quads,4};
		void action();
	public :
		ETCS(RenderWindow &fenetre, Data &data);
		void update();
};

#endif