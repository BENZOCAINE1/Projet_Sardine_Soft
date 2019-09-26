#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <math.h>
#include <string>
#include <ctime>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <algorithm>
#include "planning.hpp"

using namespace sf;
using namespace std;


float Gradient::getDistance_debut(){return distance_debut;}
void Gradient::setDistance_debut(float D){distance_debut = D;}
float Gradient::getTaille(){return taille;}
void Gradient::setTaille(float T){taille = T;}
int Gradient::getValeur(){return valeur;}
void Gradient::setValeur(int V){valeur = V;}

int PASP::getVitesse_but(){return vitesse_but;}
void PASP::setVitesse_but(int V){vitesse_but = V;}
int PASP::getDistance_but(){return distance_but;}
void PASP::setDistance_but(int D){distance_but = D;}

int Planning_Announcements::getDistance(){return distance;}
void Planning_Announcements::setDistance(int D){distance = D;}
int Planning_Announcements::getnumero(){return numero;}
void Planning_Announcements::setnumero(int N){numero = N;}
int Planning_Announcements::getPosition(){return position;}
void Planning_Announcements::setPosition(int P){position = P;}
int Planning_Announcements::getVitesse(){return vitesse;}
void Planning_Announcements::setVitesse(int V){vitesse = V;}


void pasp(int scale, double RE, RenderWindow & fenetre, int * ecart, vector<PASP> &tab_pasp, donnees &train, float delta_distance)
{

	int distance = 0;
	int target = 0;
	int vitesse = 0;
	int quart = 0;
	int plus_petite_V = train.getVligne();
	int distance_plus_petite_V = 40000;
	int n;

	for (size_t i = 0; i < tab_pasp.size(); i++)
	{
		if(i!=0)
		{
			tab_pasp[i].setDistance_but(tab_pasp[i].getDistance_but() - delta_distance);
			if(tab_pasp[i].getDistance_but()<0)
			{
				//train.setVligne(tab_pasp[i].getVitesse_but());
				tab_pasp.erase(tab_pasp.begin() + i);
			}

		}
		if(tab_pasp[i].getVitesse_but() < plus_petite_V)
		{
			plus_petite_V = tab_pasp[i].getVitesse_but();
			distance_plus_petite_V = tab_pasp[i].getDistance_but();
		}
	}

	plus_petite_V = train.getVligne();
	for(size_t i=0; i < tab_pasp.size(); i++)
	{
			n=0;
		if(tab_pasp[i].getVitesse_but() <= plus_petite_V && tab_pasp[i].getDistance_but() > 0 && i > 0)
		{
			if(tab_pasp[i].getDistance_but() == distance_plus_petite_V && tab_pasp[i].getVitesse_but() != 0)
				distance = 40000;
			else
				distance = tab_pasp[i].getDistance_but();
			if(i==1)
				vitesse = train.getVligne();
			else
			{
				do
				{
					n++;
				}while(tab_pasp[i-n].getVitesse_but() != plus_petite_V);
				vitesse = tab_pasp[i-n].getVitesse_but();
			}
			if(vitesse > 0 && vitesse <= int(train.getVligne()) / 4.0)
				quart = 1;
			else if(vitesse <= int(train.getVligne()) / 2.0)
				quart = 2;
			else if(vitesse <= 3 * int(train.getVligne()) / 4.0)
				quart = 3;
			else if (vitesse >= 3 * int(train.getVligne()) / 4.0)
				quart = 4;
			if(distance <= (scale / 40.0) && distance >= 0)
				target = 283 - (distance * (283 - 250) / (scale / 40.0));
			else if(distance <= scale && distance >= 0)
				target =  283 - (283 - 250) - log10(distance / (scale / 40.0)) * (250 - 21) / log10(scale / (scale / 40.0));
			else
				target = 15;
			rectangle(V2f(54 + 280 + 40 + 25 * 3 + 18 + 14, target), V2f(quart * 93 / 4.0, 283 - target), PASP_LIGHT, RE, fenetre, ecart);
		}
		else if (tab_pasp.size() == 1)
		{
			quart = 4;
			distance = 40000;
			if(distance <= (scale / 40.0) && distance >= 0)
				target = 283 - (distance * (283 - 250) / (scale / 40.0));
			else if(distance <= scale && distance >= 0)
				target =  283 - (283 - 250) - log10(distance / (scale / 40.0)) * (250 - 21) / log10(scale / (scale / 40.0));
			else
				target = 15;
		}
		plus_petite_V = tab_pasp[i].getVitesse_but();
		if(i==tab_pasp.size()-1)
		{
			plus_petite_V = train.getVligne();
		}
	}
}

void planningInformation(RenderWindow & fenetre, double RE, Font & arial, vector<Symbol> & symbol, donnees &train, int * ecart, float temps_ecoule, vector<Gradient> &tab_grad, vector<PASP> &tab_pasp, vector<Planning_Announcements> &tab_pa, vector<Planning_Announcements> &tab_paf)
{
	float delta_distance;
	if(train.getPlanningScale() == 1000)
	{
		NA_05.afficher(V2f(54 + 280 + 40 / 2.0, 15 + 270 + 15 / 2.0), RE, ecart, fenetre);	//D9
		NA_09.afficher(V2f(64 * 9 + 64 / 2.0, 54 + 30 + 191 + 5 * 25 + 30 + 50 / 2.0), RE, ecart, fenetre);	//F10
	}
	else
	{
		NA_03.afficher(V2f(54 + 280 + 40 / 2.0, 15 + 270 + 15 / 2.0), RE, ecart, fenetre);	//D9
		NA_07.afficher(V2f(64 * 9 + 64 / 2.0, 54 + 30 + 191 + 5 * 25 + 30 + 50 / 2.0), RE, ecart, fenetre);	//F10
	}
	if(train.getPlanningScale() == 32000)
	{
		NA_06.afficher(V2f(54 + 280 + 40 / 2.0, 15 / 2.0), RE, ecart, fenetre);	//D9
		NA_10.afficher(V2f(64 * 8 + 64 / 2.0, 54 + 30 + 191 + 5 * 25 + 30 + 50 / 2.0), RE, ecart, fenetre);	//F9
	}
	else
	{
		NA_04.afficher(V2f(54 + 280 + 40 / 2.0, 15 / 2.0), RE, ecart, fenetre);	//D9
		NA_08.afficher(V2f(64 * 8 + 64 / 2.0, 54 + 30 + 191 + 5 * 25 + 30 + 50 / 2.0), RE, ecart, fenetre);	//F9
	}
	//TEXTE GRADUATION
	creation_texte(RE, to_string(int(train.getPlanningScale())), arial, MEDIUM_GREY, 10, 0, V2f(54 + 280 + 40 - 3, 21 + 1), fenetre, 2, ecart);
	creation_texte(RE, to_string(int(train.getPlanningScale() / 2)), arial, MEDIUM_GREY, 10, 0, V2f(54 + 280 + 40 - 3, 64), fenetre, 2, ecart);
	creation_texte(RE, to_string(int(train.getPlanningScale() / 4)), arial, MEDIUM_GREY, 10, 0, V2f(54 + 280 + 40 - 3, 107), fenetre, 2, ecart);
	creation_texte(RE, to_string(int(train.getPlanningScale() / 8)), arial, MEDIUM_GREY, 10, 0, V2f(54 + 280 + 40 - 3, 150 + 1), fenetre, 2, ecart);
	creation_texte(RE, "0", arial, MEDIUM_GREY, 10, 0, V2f(54 + 280 + 40 - 3, 283 + 1), fenetre, 2, ecart);
	//FOND
	rectangle(V2f(54 + 280 + 40 + 25 * 3 + 18 + 14, 15), V2f(93 + 8, 270), PASP_DARK, RE, fenetre, ecart);
	//MOUVEMENT
	delta_distance = (train.getVtrain()/3.6)*temps_ecoule;
	//LES PASP

	pasp(train.getPlanningScale(), RE, fenetre, ecart, tab_pasp, train, delta_distance);
	Orders_and_announcements(train.getPlanningScale(), RE, ecart, delta_distance, tab_pa, symbol, fenetre);
	SpeedProfileDiscontinuityInformation(train.getPlanningScale(), RE, ecart, delta_distance, tab_paf, symbol, fenetre, arial);
	//LES GRADUATIONS
	rectangle(V2f(54 + 280 + 40, 283), V2f(200, 2), MEDIUM_GREY, RE, fenetre, ecart);
	rectangle(V2f(54 + 280 + 40, 250), V2f(200, 1), DARK_GREY, RE, fenetre, ecart);
	rectangle(V2f(54 + 280 + 40, 206), V2f(200, 1), DARK_GREY, RE, fenetre, ecart);
	rectangle(V2f(54 + 280 + 40, 182), V2f(200, 1), DARK_GREY, RE, fenetre, ecart);
	rectangle(V2f(54 + 280 + 40, 164), V2f(200, 1), DARK_GREY, RE, fenetre, ecart);
	rectangle(V2f(54 + 280 + 40, 150), V2f(200, 2), MEDIUM_GREY, RE, fenetre, ecart);
	rectangle(V2f(54 + 280 + 40, 107), V2f(200, 1), DARK_GREY, RE, fenetre, ecart);
	rectangle(V2f(54 + 280 + 40, 64), V2f(200, 1), DARK_GREY, RE, fenetre, ecart);
	rectangle(V2f(54 + 280 + 40, 21), V2f(200, 2), MEDIUM_GREY, RE, fenetre, ecart);

	//?
	/*if(TSMstart <= (scale / 40.0) && TSMstart >= 0) //Indication Marker
		rectangle(V2f(54 + 280 + 40 + 25 * 3 + 18 + 14, 283 - TSMstart * (283 - 250) / (scale / 40.0)), V2f(93, 2), YELLOW, RE, fenetre, ecart);
	else if(TSMstart <= scale && TSMstart >= 0)
		rectangle(V2f(54 + 280 + 40 + 25 * 3 + 18 + 14, 283 - (283 - 250) - log10(TSMstart / (scale / 40.0)) * (250 - 21) / log10(scale / (scale / 40.0))), V2f(93, 2), YELLOW, RE,
			fenetre, ecart); //Indication Marker*/
	//LES GRADIENT
	gradientProfile(fenetre, RE, arial, train.getPlanningScale(), ecart, delta_distance, tab_grad);
}

void Orders_and_announcements(int scale,double RE, int * ecart, float delta_distance, vector<Planning_Announcements> &tab_pa, vector<Symbol> & symbol, RenderWindow & fenetre)
{
	int target = 0;
	int distance;
	int position = 0;


	for(size_t i = 0; i<tab_pa.size(); i++)
	{
		if (i == 0)
		{
			for(size_t k = 1; k < tab_pa.size(); k++)
			{
				for(size_t j = 1; j <= k; j++)
				{
					if(tab_pa[k].getDistance() < tab_pa[k - j].getDistance())
					{	position = k - j;
						tab_pa.insert(tab_pa.begin() + position, tab_pa[k]);
						tab_pa.erase(tab_pa.begin() + k + 1);
					}
				}


				position = 0;

			}
		}


		distance = tab_pa[i].getDistance();
		position = 0;
		if(distance <= (scale / 40.0) && distance >= 0)
			target = 283 - (distance * (283 - 250) / (scale / 40.0));
		else if(distance <= scale && distance >= 0)
			target =  283 - (283 - 250) - log10(distance / (scale / 40.0)) * (250 - 21) / log10(scale / (scale / 40.0));
		else
			target = 15;
		target = target - 5;

		if(i == 0 || tab_pa[i-1].getPosition() == 3)
		{
			position = 54+280+40;
			tab_pa[i].setPosition(1);
		}
		else if(tab_pa[i-1].getPosition() == 1)
		{
			position = 54+280+40+25;
			tab_pa[i].setPosition(2);
		}
		else if(tab_pa[i-1].getPosition() == 2)
		{
			position = 54+280+40+2*25;
			tab_pa[i].setPosition(3);
		}
		if(tab_pa[i].getDistance() <= scale)
			symbol[tab_pa[i].getnumero()].afficher(V2f(position + 12, target), RE, ecart, fenetre);

		tab_pa[i].setDistance(tab_pa[i].getDistance() - delta_distance);
		if(tab_pa[i].getDistance() <= 0)
		{
			tab_pa.erase(tab_pa.begin()+i);
		}
	}

}

void gradientProfile(RenderWindow & fenetre, double RE, Font & arial, int scale, int * ecart, float delta_distance, vector<Gradient> &tab_grad)
{
	Color couleur;
	string sens;
	float taille;
	float hauteur;
	int valeur;
	for(size_t i=0; i<tab_grad.size(); i++)
		{
			tab_grad[i].setDistance_debut(tab_grad[i].getDistance_debut() - delta_distance);
			if(tab_grad[i].getDistance_debut() > 0)
			{
				hauteur = tab_grad[i].getDistance_debut();
				taille = tab_grad[i].getTaille();
			}
		else
		{
			hauteur = 0;
			tab_grad[i].setTaille(tab_grad[i].getTaille() - delta_distance);
			taille = tab_grad[i].getTaille();
		}
		if(tab_grad[i].getTaille() < 0)
		taille = 0;
		valeur = tab_grad[i].getValeur();

		if(hauteur < scale)
		{
			taille = hauteur + taille;
			if(hauteur <= (scale / 40.0) && hauteur >= 0)
				hauteur = hauteur * (283 - 250) / (scale / 40.0);
			else if(hauteur <= scale && hauteur >= 0)
				hauteur =  (283 - 250) + log10(hauteur / (scale / 40.0)) * (250 - 21) / log10(scale / (scale / 40.0));
			if(taille <= (scale / 40.0) && taille >= 0)
				taille = taille * (283 - 250) / (scale / 40.0);
			else if(taille <= scale && taille >= 0)
				taille =  (283 - 250) + log10(taille / (scale / 40.0)) * (250 - 21) / log10(scale / (scale / 40.0));
			else
				taille = 283 - 15;
			taille = taille - hauteur - 2;
			if(valeur >= 0)
			{
				couleur = GREY;
				sens = "+";
			}
			else
			{
				couleur = DARK_GREY;
				sens = "-";
				valeur = abs(valeur);
			}
			rectangle(V2f(54 + 280 + 40 + 25 * 3, 283 - 2 - hauteur - taille), V2f(18, 1), WHITE, RE, fenetre, ecart);
			rectangle(V2f(54 + 280 + 40 + 25 * 3, 283 - 1 - hauteur - taille), V2f(1, 1 + taille), WHITE, RE, fenetre, ecart);
			rectangle(V2f(54 + 280 + 40 + 25 * 3 + 1, 283 - 1 - hauteur - taille), V2f(17, taille), couleur, RE, fenetre, ecart);
			rectangle(V2f(54 + 280 + 40 + 25 * 3, 283 - 1 - hauteur), V2f(18, 1), BLACK, RE, fenetre, ecart);
			if(couleur == GREY)
				couleur = BLACK;
			else if(couleur == DARK_GREY)
				couleur = WHITE;
			if(taille > 30)
			{
				creation_texte(RE, sens, arial, couleur, 12, 0, V2f(54 + 280 + 40 + 25 * 3 + 18 / 2.0, 283 - hauteur - taille + 12), fenetre, 1, ecart); //400km/h
				creation_texte(RE, sens, arial, couleur, 12, 0, V2f(54 + 280 + 40 + 25 * 3 + 18 / 2.0, 283 - hauteur - 12), fenetre, 1, ecart); //400km/h
			}
			if(taille > 50)
				creation_texte(RE, to_string(valeur), arial, couleur, 12, 0, V2f(54 + 280 + 40 + 25 * 3 + 18 / 2.0, 283 - hauteur - taille / 2.0), fenetre, 1, ecart); //400km/h
		}
	}
}
void SpeedProfileDiscontinuityInformation(int scale,double RE, int * ecart, float delta_distance, vector<Planning_Announcements> &tab_paf, vector<Symbol> & symbol, RenderWindow & fenetre, Font arial)
{
	int distance;
	int target;
	int position;

	for (size_t i = 0; i < tab_paf.size(); i++)
	{
		distance = tab_paf[i].getDistance();
		if(distance <= (scale / 40.0) && distance >= 0)
			target = 283 - (distance * (283 - 250) / (scale / 40.0));
		else if(distance <= scale && distance >= 0)
			target =  283 - (283 - 250) - log10(distance / (scale / 40.0)) * (250 - 21) / log10(scale / (scale / 40.0));
		else
			target = 15;
		target = target - 5;
		tab_paf[i].setnumero(92);

		//Dans le futur faire comparer la première case avec la vitesse ligne puis comparer les cases entre elles afin de voir si il y aura une augmentation ou une diminution

		if(tab_paf[i].getDistance() <= scale)
		{
			symbol[tab_paf[i].getnumero()].afficher(V2f(54 + 280 + 40 + 3 * 25 + 18 + 13, target + 10), RE, ecart, fenetre);
			if(tab_paf[i].getVitesse() > 99)
				position = 19;
			else if(tab_paf[i].getVitesse() > 9)
				position = 20 - 4;
			else
				position = 20 - 7;
			if(tab_paf[i].getnumero() == 92)
				creation_texte(RE, to_string(tab_paf[i].getVitesse()), arial, GREY, 11, 0, V2f(54 + 280 + 40 + 3 * 25 + 18 + 13 + position, target + 9), fenetre, 1, ecart);
			if(tab_paf[i].getnumero() == 91)
				creation_texte(RE, to_string(tab_paf[i].getVitesse()), arial, GREY, 11, 0, V2f(54 + 280 + 40 + 3 * 25 + 18 + 13 + position, target + 14), fenetre, 1, ecart);
		}
		tab_paf[i].setDistance(tab_paf[i].getDistance() - delta_distance);
		if(tab_paf[i].getDistance() <= 0)
		{
			tab_paf.erase(tab_paf.begin()+i);
		}
	}
}