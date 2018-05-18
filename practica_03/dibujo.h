#ifndef DIBUJO_H
#define DIBUJO_H

#include "juego.h"
#include "checkML.h"

void dibujarTablero(const tJuego & juego);
//Muestra por pantalla el tablero de juego en un instante de la partida

void dibujarJugadores(const tJuego & juego);
//Muestra por pantalla los nombres de los jugadores, sus respectivas manos y el turno actual
//Ademas, a medida que van ganando los jugadores muestra su clasificacion

tTecla leerTecla();
//Lee alguna de las teclas especiales (flechas, enter, space) pulsada por el jugador

void dibujarTitulo();
//Muestra por pantalla el titulo del juego

void reglas();
//Muestra por pantalla las reglas del juego
#endif