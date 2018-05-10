#include <iostream>
#include <string>
#include <fstream>
#include "puntuaciones.h"
#include "juego.h"
#include <cctype>
using namespace std;

void inicializar(tPuntuaciones & puntuaciones); //Inicializa el arary dinamico de puntuaciones
void liberar(tPuntuaciones & puntuaciones); //Libera la memoria dinamica de las puntuaciones
void redimensionar(tPuntuaciones & puntuaciones); //Amplia *2 el tamaño dal array dinamico
bool operator<(const tInfoJugador & p1, const tInfoJugador & p2); //Sobrecarga del operador < para comparar structs tInfoJugador
string toMayus(const string & nombre); //Recibe un string y lo devuelve todo en mayusculas
void ordenarRanking(tPuntuaciones & puntuaciones, int pos); //Ordena el ranking de puntuaciones

void inicializar(tPuntuaciones & puntuaciones)
{
	puntuaciones.ranking = new tInfoJugador[MAX_RANKING];
	puntuaciones.contador = 0;
	puntuaciones.tam = MAX_RANKING;
}

void liberar(tPuntuaciones & puntuaciones)
{
	delete[] puntuaciones.ranking;
	puntuaciones.ranking = nullptr;
	puntuaciones.contador = 0;
	puntuaciones.tam = 0;
}

void redimensionar(tPuntuaciones & puntuaciones)
{
	tInfoJugador * aux;

	aux = puntuaciones.ranking;
	puntuaciones.tam *= 2;
	puntuaciones.ranking = new tInfoJugador[puntuaciones.tam];

	for (int i = 0; i < puntuaciones.contador; i++)
	{
		puntuaciones.ranking[i] = aux[i];
	}

	delete[] aux;
	//aux = nullptr; No es necesario ya que al acabar la funcion se destruye el puntero
}

void actualizarPuntuacion(tPuntuaciones & puntuaciones, const string & nombre, int nuevos)
{
	int i = 0;

	while (i < puntuaciones.contador && puntuaciones.ranking[i].nombre != nombre)
	{
		i++;
	}

	if (i < puntuaciones.contador) //Jugador ya existente
	{
		puntuaciones.ranking[i].puntos += nuevos;

		ordenarRanking(puntuaciones, i);
	}
	else //Nuevo jugador
	{
		if (puntuaciones.contador == puntuaciones.tam) //Array lleno
		{
			redimensionar(puntuaciones);
		}

		puntuaciones.contador++;

		puntuaciones.ranking[i].nombre = nombre;
		puntuaciones.ranking[i].puntos = nuevos;

		ordenarRanking(puntuaciones, i);
	}
}

void ordenarRanking(tPuntuaciones & puntuaciones, int pos)
{
	tInfoJugador temp;

	while ((pos > 0) && puntuaciones.ranking[pos - 1] < puntuaciones.ranking[pos])
	{
		temp = puntuaciones.ranking[pos];

		puntuaciones.ranking[pos] = puntuaciones.ranking[pos - 1];

		puntuaciones.ranking[pos - 1] = temp;

		pos--;
	}
}

bool operator<(const tInfoJugador & p1, const tInfoJugador & p2)
{
	bool ordenar;

	if (p1.puntos < p2.puntos) ordenar = true;
	else if (p1.puntos > p2.puntos) ordenar = false;
	else ordenar = (toMayus(p1.nombre) > toMayus(p2.nombre));

	return ordenar;
}

string toMayus(const string & nombre)
{
	string nombre_mayus = nombre;

	for (int i = 0; i < nombre.length(); i++)
	{
		nombre_mayus[i] = toupper(nombre[i]);
	}

	return nombre_mayus;
}

bool cargarPuntuaciones(tPuntuaciones & puntuaciones)
{
	ifstream archivo;
	int i = 0;
	bool abierto;

	inicializar(puntuaciones);
	archivo.open("puntuaciones.txt");

	if (archivo.is_open())
	{
		abierto = true;

		archivo >> puntuaciones.ranking[i].nombre;
		archivo >> puntuaciones.ranking[i].puntos;

		while (!archivo.eof())
		{
			i++;

			if (i == puntuaciones.tam)
			{
				puntuaciones.contador = i;
				redimensionar(puntuaciones);
			}

			archivo >> puntuaciones.ranking[i].nombre;
			archivo >> puntuaciones.ranking[i].puntos;
		}
		puntuaciones.contador = i;
	}
	else
	{
		abierto = false;
	}

	archivo.close();

	return abierto;
}

void guardarPuntuaciones(const tPuntuaciones & puntuaciones)
{
	ofstream archivo;
	int i = 0;

	archivo.open("puntuaciones.txt");

	if (archivo.is_open())
	{
		for (int i = 0; i < puntuaciones.contador; i++)
		{
			archivo << puntuaciones.ranking[i].nombre << " ";
			archivo << puntuaciones.ranking[i].puntos << endl;
		}
	}
	else
	{
		cout << "Error al guardar las puntuaciones" << endl;
	}

	archivo.close();
}

void mostrarPuntuaciones(const tPuntuaciones & puntuaciones)
{
	for (int i = 0; i < puntuaciones.contador; i++)
	{
		cout << puntuaciones.ranking[i].nombre << " ";
		cout << puntuaciones.ranking[i].puntos << endl;
	}
}

void mostrarAlfabeticamente(const tPuntuaciones & puntuaciones)
{
	tInfoJugador** alfabeto = new tInfoJugador*[puntuaciones.contador];
	tInfoJugador* temp;

	for (int i = 0; i < puntuaciones.contador; i++) //Cada puntero de alfabeto apunta a su correspodiente casilla de puntuaciones
	{
		alfabeto[i] = &puntuaciones.ranking[i];
	}
	
	for (int i = 0; i < puntuaciones.contador; i++) //Se ordenan los punteros de alfabeto en funcion del orden alfabetico del nombre al que apuntan
	{
		int  j = i;

		while ((j > 0) && toMayus(alfabeto[j - 1]->nombre) > toMayus(alfabeto[j]->nombre))
		{
			temp = alfabeto[j];

			alfabeto[j] = alfabeto[j - 1];

			alfabeto[j - 1] = temp;

			j--;
		}
	}
	for (int i = 0; i < puntuaciones.contador; i++) //Muestra las variables apuntadas por alfabeto en orden
	{
		cout << alfabeto[i]->nombre << " ";
		cout << alfabeto[i]->puntos << endl;
	}

	delete[] alfabeto;
}