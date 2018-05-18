#ifndef PUNTUACIONES_H
#define PUNTUACIONES_H

#include <string>
#include "checkML.h"
using namespace std;

const int MAX_RANKING = 4;

typedef struct
{
	string nombre;
	int puntos;
}tInfoJugador;

typedef struct
{
	tInfoJugador * ranking;
	int contador;
	int tam; //capacidad actual del array
}tPuntuaciones;

void actualizarPuntuacion(tPuntuaciones & puntuaciones, const string & nombre, int nuevos);
//Recibe un nombre de jugador y su nueva puntuacion
//Si es un jugador ya existente actualiza su puntuacion sumandole la nueva, si es un nuevo jugador se inserta con esa puntuacion
//Al insertar un jugador (existente o nuevo) es colocado teniendo en cuenta el orden de las puntuaciones (de mayor a menor)
//Si dos jugadores tienen la misma puntuacion se les coloca por orden alfabetico

bool cargarPuntuaciones(tPuntuaciones & puntuaciones);
//Carga las puntuaciones del fichero "puntuaciones.txt" a un array de estructuras (tPuntuaciones puntuaciones)
//Devuelve si se han podido cargar las puntuaciones correctamente

void guardarPuntuaciones(tPuntuaciones & puntuaciones);
//Guarda los cambios producidos en las puntuaciones durante la partida, en el fichero "puntuaciones.txt"
//Libera la memoria dinamica que almacena las puntuaciones

void mostrarPuntuaciones(const tPuntuaciones & puntuaciones);
//Muestra las puntuaciones de los jugadores una vez cargados en el array de estructuras (tPuntuaciones puntuaciones)

void mostrarAlfabeticamente(const tPuntuaciones & puntuaciones);
//Muestra las puntuaciones de los jugadores de manera alfabetica, sin modificar el ranking ordenado por puntuacion
#endif