#include "cartas.h"
#include <algorithm>
using namespace std;

void liberar(tMazo & mazo)
{
	delete[] mazo.cartas;

	for (int i = 0; i < mazo.num_cartas; i++)
	{
		//delete mazo.cartas[i];
		mazo.cartas[i] = nullptr;
	}
	mazo.num_cartas = 0;
}

void crearMazoAleatorio(tMazo & mazo)
{
	mazo.num_cartas = 0;
	mazo.inicio = 0;

	for (int i = 0; i < MAX_AVANZAR; i++, mazo.num_cartas++)
	{
		mazo.cartas[mazo.num_cartas] = new tCarta;
		*(mazo.cartas[mazo.num_cartas]) = AVANZAR;
	}

	for (int i = 0; i < MAX_GIRO; i++, mazo.num_cartas++)
	{
		mazo.cartas[mazo.num_cartas] = new tCarta;
		*(mazo.cartas[mazo.num_cartas]) = GIRODERECHA;
	}

	for (int i = 0; i < MAX_GIRO; i++, mazo.num_cartas++)
	{
		mazo.cartas[mazo.num_cartas] = new tCarta;
		*(mazo.cartas[mazo.num_cartas]) = GIROIZQUIERDA;
	}

	for (int i = 0; i < MAX_LASER; i++, mazo.num_cartas++)
	{
		mazo.cartas[mazo.num_cartas] = new tCarta;
		*(mazo.cartas[mazo.num_cartas]) = LASER;
	}
	for (int i = 0; i < MAX_BICHO; i++, mazo.num_cartas++)
	{
		mazo.cartas[mazo.num_cartas] = new tCarta;
		*(mazo.cartas[mazo.num_cartas]) = BICHO;
	}

	random_shuffle(begin(mazo.cartas), end(mazo.cartas));
}

void crearVacia(tMazo & mazo)
{
	mazo.inicio = 0;
	mazo.num_cartas = 0;
}

/////////////////////////////////////////////////////////////////////////////////

bool sacar(tMazo & mazo, tCarta & carta, bool arriba)
{
	bool sacada;

	if (arriba)
	{
		if (mazo.num_cartas > 0)
		{
			carta = *(mazo.cartas[mazo.inicio]);
			
			delete mazo.cartas[mazo.inicio];
			mazo.cartas[mazo.inicio] = nullptr;
			
			mazo.inicio = (mazo.inicio + 1) % MAX_CARTAS;

			mazo.num_cartas--;
 
			sacada = true;
		}
		else //mazo vacio
		{
			sacada = false;
		}
	}
	else //abajo
	{
		if (mazo.num_cartas > 0)
		{
			carta = *(mazo.cartas[(mazo.inicio + mazo.num_cartas - 1) % MAX_CARTAS]);
			
			delete mazo.cartas[(mazo.inicio + mazo.num_cartas - 1) % MAX_CARTAS];
			mazo.cartas[(mazo.inicio + mazo.num_cartas - 1) % MAX_CARTAS] = nullptr;
			
			mazo.num_cartas--;

			sacada = true;
		}
		else //mazo vacio
		{
			sacada = false;
		}
	}

	return sacada;
}

void insertar(tMazo & mazo, tCarta & carta, bool arriba)
{
	if (arriba)
	{
		if (mazo.num_cartas < MAX_CARTAS) //Hueco en el mazo (NO ES NECESARIO ?)
		{
			if (mazo.inicio - 1 < 0)
			{
				mazo.inicio = MAX_CARTAS - 1;
			}
			else
			{
				mazo.inicio--;
			}

			mazo.cartas[mazo.inicio] = new tCarta;
			*(mazo.cartas[mazo.inicio]) = carta;

			mazo.num_cartas++;
		}
	}
	else //abajo
	{
		if (mazo.num_cartas < MAX_CARTAS) //Hueco en el mazo
		{
			mazo.cartas[(mazo.inicio + mazo.num_cartas) % MAX_CARTAS] = new tCarta;
			*(mazo.cartas[(mazo.inicio + mazo.num_cartas) % MAX_CARTAS]) = carta;

			mazo.num_cartas++;
		}
	}
}

/*
void crearMazoAleatorio(tMazo & mazo)
{
	mazo.inicio = 0;
	mazo.num_cartas = 0;

	for (int i = 0; i < MAX_AVANZAR; i++, mazo.num_cartas++)
	{
		mazo.cartas[mazo.num_cartas] = AVANZAR;
	}

	for (int i = 0; i < MAX_GIRO; i++, mazo.num_cartas++)
	{
		mazo.cartas[mazo.num_cartas] = GIRODERECHA;
	}

	for (int i = 0; i < MAX_GIRO; i++, mazo.num_cartas++)
	{
		mazo.cartas[mazo.num_cartas] = GIROIZQUIERDA;
	}

	for (int i = 0; i < MAX_LASER; i++, mazo.num_cartas++)
	{
		mazo.cartas[mazo.num_cartas] = LASER;
	}
	for (int i = 0; i < MAX_BICHO; i++, mazo.num_cartas++)
	{
		mazo.cartas[mazo.num_cartas] = BICHO;
	}

	random_shuffle(begin(mazo.cartas), end(mazo.cartas));
}
*/