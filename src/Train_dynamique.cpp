#include "Train_dynamique.hpp"

float Train_dynamique::getV_train(){return V_train;}
void Train_dynamique::setV_train(float V){V_train = V;}
float Train_dynamique::getDistance_Uptdate(){return distance_update;}
float Train_dynamique::getTraction(){return traction;}
void Train_dynamique::setTraction(float A){traction = A;}



void Train_dynamique::update()
{
	effortTraction();
	calculVitesse();
	effortFreinage();
}


void Train_dynamique::effortTraction()
{
	float coefManip=0; // coef multiplicateur de la puissance envoyé par le conducteur avec le manip
	float Rav;			// resistance avancement
	float Rp;  // resistance profil
	float Rd=0;
	float Fres;
	float F;
	float Rd0=0;
//	float Rp0;      // rp corrigé

	valeurManip = traction;

	if (valeurManip > 512 && valeurManip < 540)
	{
		coefManip = 0;
	}
	else if (valeurManip >= 540)
	{
		coefManip = (valeurManip - 540);
		coefManip = coefManip/483;
	}
	else if (valeurManip < 512)
	{
		coefManip = 0;
	}

	cout << valeurManip << endl;

	///////// calcul de RAV
	Rav = A + B*V_train + C*(V_train*V_train);
	Rp = masse*9.81*(i/1000);

	if (V_train == 0)
	{
		if (typeTrain == 0)
		{
			if (i <= 18)
			{
				Rd = (masse/100)*(i*0.981 + 6.8);
				Rd0 = (masse/100)*6.8;
			}
			else if (i > 17)
			{
				Rd = (masse/100)*(1.25*i*0.981 + 2.75);
				Rd0 = (masse/100)*2.75;
			}
		}
		else if (typeTrain == 1)									/// FAIRE ATTENTION UNITE DE i
		{
			if (i <= 7)
			{
				Rd = (masse/100)*(i*0.981 + 4.5);
				Rd0 = (masse/100)*4.5;
			}
			else if (i > 7)
			{
				Rd = (masse/100)*(1.25*i*0.981 + 2.75);
				Rd0 = (masse/100)*2.75;
			}
		}
	}
	else if (V_train != 0)
	{
		Rd = 0;
	}
	cout << Rd << endl;
	Fres = Rav + Rd + Rp;

	if (V_train == 0)
	{
		Rp =  Rd - Rd0;
		if (abs(Ftraction*coefManip - Rp) > Rd0)
		{
			Ft = (Ftraction*coefManip - Rd -Rp);
			gamma = Ft / (masse*k);
		}
		else if (abs(Ftraction*coefManip - Rp) <= Rd0)
		{
			V_train = 0;
		}
	}
	else if (V_train != 0)
	{
		if (coefManip == 0)
		{
			Ft = -Fres;
			gamma = Ft / (masse*k);
		}
		else if (Ptraction*coefManip / (abs(V_train/3.6)) >= Ftraction*coefManip)                     // le train travail à sa force de trction max pour des vitesses relativement faible.
		{
			Ft = (Ftraction*coefManip - Fres);
			gamma = Ft / (masse*k);
		}
		else if (Ptraction*coefManip / (V_train/3.6) < Ftraction*coefManip)          // le train travail au max de sa puissance.
		{
			F = Ptraction*coefManip / (V_train/3.6);
			Ft = (F - Fres);
			gamma = Ft / (masse*k);
		}
	}
	cout << "gamma = " << gamma << endl;
}

void Train_dynamique::effortFreinage()
{
	float coefManip=0;

//====================================================
	valeurManip = traction;
	if (valeurManip > 0 && valeurManip < 512)
	{
		coefManip = (511 - valeurManip);
		coefManip = coefManip/511;
		int passe = 123;
		cout << passe << endl;
	}
	cout << coefManip << endl;
//======================================================



}

void Train_dynamique::adherence()
{
	float Q;
	float Freac;     // force de réaction du rail
	float µ;
	float F;
	float gamma;

	Q = 9.81*masseEssieuMoteur*1000;

	if (Ft > µmax*Q)
	{
		Freac = µ*Q;
		F = Freac + Ft;
		gamma2 = F / (masse*k);
	}
}






void Train_dynamique::calculVitesse()
{
	float NVitesse;      // nouvelle vitesse tampon
	float NVitesse2;

	diftime = chrono.getElapsedTime();
	deltats = diftime.asSeconds();
	NVitesse2 = gamma2*deltats*3.6;		// pour calculer la vitesse de glissement de l'essieu
	NVitesse = gamma*deltats*3.6;

	V_train = (NVitesse + V_train) - NVitesse2;
	distance_update = deltats*V_train/3.6;
	chrono.restart();
}

