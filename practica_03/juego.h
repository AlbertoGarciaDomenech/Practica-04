#ifndef JUEGO_H
#define JUEGO_H

#include "cartas.h"
#include "puntuaciones.h"
#include <string>
using namespace std;

const int MAX_JUGADORES = 4;
const int MAX_FILAS = 8;
const int MAX_COLUMNAS = 8;
const int MAX_HIELOS = 12; //numero maximo de hielos en el tablero

typedef enum { _AVANZAR, _DERECHA, _IZQUIERDA, _LASER, _SALIR, _BICHO, _INCORRECTO } tTecla;
typedef enum { NORTE, ESTE, SUR, OESTE } tDir;
typedef enum { VACIA, HIELO, MURO, CAJA, JOYA, FUEGO, TORTUGA, NUM_ESTADOS } tEstadoCasilla;
typedef enum { ACCION_ROBAR, ACCION_SECUENCIA, ACCION_BICHO, ACCION_INCORRECTA } tAccion;

typedef struct
{
	int fila, columna;
}tCoordenadas;

typedef struct
{
	tCoordenadas bloques[MAX_HIELOS];
	int num_derretidos;
}tBloquesDerretidos;

typedef struct
{
	int num_avanzar = 0,
		num_izquierda = 0,
		num_derecha = 0,
		num_laser = 0,
		num_bicho = 0;
}tMano;

typedef struct
{
	int numero;
	tDir direccion;
}tTortuga;

typedef struct
{
	tEstadoCasilla estado;
	tTortuga tortuga;
}tCasilla;

typedef tCasilla tTablero[MAX_FILAS][MAX_COLUMNAS];

typedef struct
{
	string nombre;
	tMazo mazo;
	tMano mano;
	tCoordenadas coordenadas;
	tCoordenadas pos_inicial; //Coordenadas al empezar
	tCoordenadas pos_anterior; // Coordenadas al inciar anterior jugada
	tAccion ultima_accion;
	tCarta carta_robada;
	int cartas_movidas; // cartas retiradas o devueltas al mazo
	tBloquesDerretidos bloques_derretidos; // bloques derretidos en la accion anterior
	bool sigue_jugando = true;
}tJugador;

typedef struct
{
	int num_ganadores = 0;
	tJugador puesto[MAX_JUGADORES];
}tGanadores;

typedef struct
{
	int num_jugadores;
	int turno;
	tJugador jugadores[MAX_JUGADORES];
	tTablero tablero;
	tGanadores ganadores;
	bool fin_partida = false;;
}tJuego;

bool cargarJuego(tJuego & juego);
//Pide nombre del fichero de tableros y numero de jugadores, y carga el tablero correspondiente
//Inicializa las manos y los mazos de cada jugador
//Establece el turno del juego a 0 para empezar a jugar
//Devuelve true si el juego se ha cargado con exito

void ejecutarJuego(tJuego & juego);
//Tras haberlo cargado, realiza todo lo necesario para mostrar y jugar al juego
//Muestra el juego completo y ejecuta un turno y comprueba que no ha acabado la partida
//Al final pasa al siguiente turno y se ejecuta hasta que acabe el juego

void reiniciarPartida(tJuego & juego);
//Reinicia las manos de los jugadores para poder iniciar una nueva partida
//Devuelve a true el campo seguir_jugando de cada jugador 

void calcularPuntuaciones(tJuego & juego, tPuntuaciones & puntuaciones);
//Calcula la puntuacion correspondiente a cada jugador dependiendo de su posicion al ganar
//Llama a actualizar puntuacion cada vez que calcula una puntuacion

#endif