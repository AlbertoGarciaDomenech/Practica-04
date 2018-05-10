#include "dibujo.h"
#include "colores.h"
#include <iostream>
#include <conio.h>
#include <fstream>
using namespace std;

void dibujarTablero(const tJuego & juego)
{
	system("cls");

	dibujarTitulo();

	for (int fila = 0; fila < MAX_FILAS; fila++)
	{
		for (int columna = 0; columna < MAX_COLUMNAS; columna++)
		{
			switch (juego.tablero[fila][columna].estado)
			{
			case VACIA: setColor(PALETA[VACIA]); cout << "  ";  break;
			case TORTUGA: 
				switch (juego.tablero[fila][columna].tortuga.numero)
				{
				case 0: setColor(PALETA[TORTUGA]); break;
				case 1: setColor(PALETA[TORTUGA + 1]); break;
				case 2: setColor(PALETA[TORTUGA + 2]); break;
				case 3: setColor(PALETA[TORTUGA + 3]); break;
				}
				switch (juego.tablero[fila][columna].tortuga.direccion)
				{
				case NORTE: cout << "^^"; break;
				case SUR: cout << "vv"; break;
				case ESTE: cout << ">>"; break;
				case OESTE: cout << "<<"; break;
				}
				setColor(0);
				break;
			case CAJA: setColor(PALETA[CAJA]); cout << "[]"; setColor(0); break;
			case JOYA: setColor(PALETA[JOYA]); cout << "OO"; setColor(0); break;
			case MURO: setColor(PALETA[MURO]); cout << "||"; setColor(0); break;
			case HIELO: setColor(PALETA[HIELO]); cout << "**"; setColor(0); break;
			case FUEGO: setColor(PALETA[FUEGO]); cout << "[]"; setColor(0); break;
			}
		}
		cout << endl;
	}

}

void dibujarJugadores(const tJuego & juego)
{
	bool algun_ganador = false;
	cout << "JUGADORES: \n";
	for (int i = 0; i < juego.num_jugadores; i++)
	{
		if (juego.jugadores[i].sigue_jugando)
		{
			setColor(PALETA[TORTUGA + i]);

			if (juego.turno == i) { cout << "> "; }

			cout << i + 1 << ". " << juego.jugadores[i].nombre << ": ";
			setColor(0);

			cout << juego.jugadores[i].mano.num_avanzar;
			setColor(PALETA[10]);
			cout << "^ ";
			setColor(0);

			cout << juego.jugadores[i].mano.num_derecha;
			setColor(PALETA[10]);
			cout << "> ";
			setColor(0);

			cout << juego.jugadores[i].mano.num_izquierda;
			setColor(PALETA[10]);
			cout << "< ";
			setColor(0);

			cout << juego.jugadores[i].mano.num_laser;
			setColor(PALETA[10]);
			cout << "~";
			setColor(0);

			cout << juego.jugadores[i].mano.num_bicho;
			setColor(PALETA[10]);
			cout << "&" << endl;
			setColor(0);
		}
		else
		{
			algun_ganador = true;
		}
	}
	if (algun_ganador)
	{
		setColor(0);
		cout << "PODIO:" << endl;
		for (int i = 0; i < juego.ganadores.num_ganadores; i++)
		{
			cout << i + 1 << "-" << juego.ganadores.puesto[i].nombre << endl;
		}
	}
}

tTecla leerTecla()
{
	tTecla tecla;
	int num_T = _getch();
	switch (num_T)
	{
	case 13: tecla = _SALIR; break;
	case 32: tecla = _LASER; break;
	default: tecla = _INCORRECTO; break;
	case 0xe0:
		int num_T = _getch();
		switch (num_T)
		{
		case 72: tecla = _AVANZAR; break;
		case 77: tecla = _DERECHA; break;
		case 75: tecla = _IZQUIERDA; break;
		default: tecla = _INCORRECTO; break;
		}
		break;
	}
	return tecla;
}

void dibujarTitulo()
{
	system("color 0A");
	cout << " _____     _       _      _____         _   _            ___   ___ " << endl
		<< "| __  |___| |_ ___| |_   |_   _|_ _ ___| |_| |___ ___   |_  | |   |" << endl
		<< "|    -| . | . | . |  _|    | | | | |  _|  _| | -_|_ -|  |  _|_| | |" << endl
		<< "|__|__|___|___|___|_|      |_| |___|_| |_| |_|___|___|  |___|_|___|" << endl;
	cout << "\t\tBy Alberto Garcia & Pablo Daurell" << endl << endl;
	setColor(0);
}

void reglas()
{
	ifstream archivo;
	string linea;

	archivo.open("reglas.txt");

	if (archivo.is_open())
	{
		getline(archivo, linea);
		cout << linea << endl;

		while (!archivo.eof())
		{
			getline(archivo, linea);
			cout << linea << endl;
		}
		cout << "----------------------------------------CASILLAS----------------------------------------" << endl;
		cout << "HIELO: "; setColor(PALETA[HIELO]);cout << "**"; setColor(0);
		cout << "  JOYA: "; setColor(PALETA[JOYA]); cout << "00"; setColor(0);
		cout << "  CAJA: "; setColor(PALETA[CAJA]); cout << "[]"; setColor(0);
		cout << "  CAJA ARDIENDO: "; setColor(PALETA[FUEGO]); cout << "[]"; setColor(0);
		cout << "  MURO: "; setColor(PALETA[MURO]); cout << "||";  setColor(0);
		cout << endl;
		cout << "TORTUGAS: "; setColor(PALETA[TORTUGA]); cout << "vv"; setColor(0); cout << "  ";
		setColor(PALETA[TORTUGA + 1]); cout << "<<"; setColor(0); cout << "  ";
		setColor(PALETA[TORTUGA + 2]); cout << ">>"; setColor(0); cout << "  ";
		setColor(PALETA[TORTUGA + 3]); cout << "^^"; setColor(0);
	}
	else
	{
		cout << "No se ha podido abrirr el archivo de las reglas." << endl;
	}

	archivo.close();
}