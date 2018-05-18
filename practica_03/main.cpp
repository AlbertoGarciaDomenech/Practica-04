/*
Practica 04 : ROBOT-TURTLES 2.0
Grupo 1ºB FP
Alumnos: PABLO DAURELL MARINA y ALBERTO GARCIA DOMENECH
*/

#include "puntuaciones.h"
#include "dibujo.h"
#include "juego.h"
#include "checkML.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

int menu(); //Muestra el menu inicial para jugar, ver puntuaciones, ver reglas o salir
int subMenuPuntuaciones(); //Menu para elegir como mostrar las puntuaciones

int main()
{
	srand(time(NULL)); //Semilla aleatoria para no crear siempre el mismo mazo

	int opcion_menu;
	tPuntuaciones puntuaciones;
	bool punt_cargadas;
	tJuego juego;

	punt_cargadas = cargarPuntuaciones(puntuaciones);
	if (!punt_cargadas) { cout << "ERROR: No se han cargado las puntuaciones correctamente" << endl << endl; }

	do
	{
		opcion_menu = menu();

		if (opcion_menu == 1)
		{
			if (cargarJuego(juego))
			{
				ejecutarJuego(juego);
				calcularPuntuaciones(juego, puntuaciones);
				reiniciarPartida(juego);
			}
			else
			{
				cout << "No se ha podido iniciar el juego, por favor comprueba que todo esta en orden.\n";
			}
			system("pause");
			system("cls");
		}
		else if (opcion_menu == 2)
		{
			opcion_menu = subMenuPuntuaciones();

			cout << "-------" << endl;
			cout << "RANKING: " << endl;
			if (opcion_menu == 1) { mostrarPuntuaciones(puntuaciones); }
			else { mostrarAlfabeticamente(puntuaciones); }
			cout << "-------" << endl;

			system("pause");
			system("cls");
		}
		else if (opcion_menu == 3)
		{
			system("cls");
			reglas();
			system("pause");
			system("cls");
		}

	} while (opcion_menu != 0);


	cout << "-------" << endl;
	cout << "RANKING FINAL: " << endl;
	mostrarPuntuaciones(puntuaciones);
	cout << "-------" << endl;

	guardarPuntuaciones(puntuaciones);

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); //DEBUG: VER MEMORIA DINAMICA

	system("pause");
	system("cls");
	return 0;
}

int menu()
{
	int opcion;

	dibujarTitulo();

	cout << "1. Jugar" << endl << "2. Mostrar puntuaciones" << endl << "3. Mostrar reglas" << endl << endl << "0. Salir"
		<< endl << endl << "Elige una opcion: ";
	cin >> opcion;
	cout << endl;

	while (opcion != 0 && opcion != 1 && opcion != 2 && opcion != 3)
	{
		cout << "OPCION NO VALIDA" << endl << endl;
		cout << "Elige una opcion: ";
		cin >> opcion;
		cout << endl;
	}

	return opcion;
}

int subMenuPuntuaciones()
{
	int opcion;

	system("cls");

	dibujarTitulo();

	cout << "Mostrar puntuaciones: " << endl << endl << "1. Orden por puntuaciones" << endl << "2. Orden alfabetico" << endl << endl << "Elige una opcion: ";
	cin >> opcion;
	cout << endl;

	while (opcion != 1 && opcion != 2)
	{
		cout << "OPCION NO VALIDA" << endl << endl;
		cout << "Elige una opcion: ";
		cin >> opcion;
		cout << endl;
	}

	return opcion;
}