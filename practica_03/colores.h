#ifndef COLORES_H_
#define COLORES_H_

#include "juego.h"

const int PALETA[NUM_ESTADOS + MAX_JUGADORES] = { 1, 11, 7, 4, 12, 12, 5, 13, 9, 10, 3 };
// VACIA, HIELO, MURO, CAJA, JOYA, FUEGO, TORTUGA, NUM_ESTADOS

void setColor(int color);
/*
COLORES:
0 = Negro
1 = Azul oscuro
2 = Verde
3 = Azul celeste
4 = Granate
5 = Fucsia
6 = Caqui
7 = Gris claro
8 = Gris oscuro
9 = Azul electrico
10 = Verde lima
11 = Azul hielo
12 = Naranja
13 = Rosa
14 = Amarillo
15 = Blanco
*/

#endif 

/*
* Autor: manuel.freire@fdi.ucm.es
* Licencia: https://creativecommons.org/licenses/by-sa/3.0/
*/