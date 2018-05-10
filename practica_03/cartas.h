#ifndef CARTAS_H
#define CARTAS_H

const int MAX_AVANZAR = 18;
const int MAX_GIRO = 8; //8 para izquierda y 8 para derecha
const int MAX_LASER = 4;
const int MAX_BICHO = 1;
const int MAX_CARTAS = MAX_AVANZAR + (MAX_GIRO * 2) + MAX_LASER + MAX_BICHO;

typedef enum { AVANZAR, GIROIZQUIERDA, GIRODERECHA, LASER, BICHO } tCarta;

typedef tCarta * tArrayPtrCartas[MAX_CARTAS];

typedef struct
{
	int inicio;
	int num_cartas; //Contador
	tArrayPtrCartas cartas;
}tMazo;

void liberar(tMazo & mazo);
//Libera la memoria dinamica de los mazos

void crearMazoAleatorio(tMazo & mazo);
//Crea un mazo con cartas colocadas de manera aleatoria

void crearVacia(tMazo & secuencia);
//Crea una secuencia vacia de cartas

bool sacar(tMazo & mazo, tCarta & carta, bool arriba);
//Saca una carta de arriba o abajo del mazo
//Devuelve true si se ha podido sacar y false en caso contrario

void insertar(tMazo & mazo, tCarta & carta, bool arriba);
//Inserta una carta encima o debajo del mazo

#endif

//POS FINAL = (mazo.inicio + mazo.num_cartas) % MAX_CARTAS