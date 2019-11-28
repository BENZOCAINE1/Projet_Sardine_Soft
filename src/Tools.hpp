#ifndef TOOLS
#define TOOLS

#include "Data.hpp"
#include "define.hpp"

class Tools
{
	protected :
		RenderWindow *fenetre;
		Data *data;
		void creation_texte(string message, Color couleur, int taille, double OutlineThickness, V2f pos, int mode);
		void rectangle(V2f pos, V2f taille, Color col);
		void couleurForme(VertexArray & bande,Color col, int n);
};

#endif