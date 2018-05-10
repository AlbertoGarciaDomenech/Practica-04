#include "juego.h"
#include "dibujo.h"
#include <iostream>
#include <fstream>
#include <conio.h>
#include <string>
using namespace std;

void mostrarJuego(const tJuego & juego); //Muestra el estado actual del juego (Tablero y jugadores con sus respectivas manos)
bool accionRobar(tJuego & juego); //Roba una carta del mazo del jugador y devuelve booleano en funcion de si ha sido posible robar o no
bool accionSecuencia(tJuego & juego, tMazo & mazo); //Ejecuta una secuencia de cartasy devuelve true si el jugador ha alcanzado una joya en ese turno
bool accionBicho(tJuego & juego); //Realiza la accion del bicho (deshace la accion del jugador elegido)
bool ejecutarTurno(tJuego & juego); //Lee la accion del jugador (robar o secuencia) y la ejecuta, devuelve true si se ha alcanzado una joya
void cambiarTurno(tJuego & juego); //Cambia el turno al siguiente jugador
void incluirCarta(tMano & mano, tCarta carta); //Incluye una nueva carta en la mano del jugador
void devolverCarta(tMano & mano, tCarta carta); //Elimina una carta de la mano del jugador
bool esFinDePartida(tJuego & juego); //Comprueba que la partida haya acabado
bool seguirJugando(tJuego & juego); //Despues de que alguien haya ganado pregunta si se quiere seguir jugando
bool cargarTablero(tJuego & juego); //Carga el tablero de juego del fichero de texto elegido
void cargarJugadores(tJuego & juego); //Inicia los jugadores con sus respectivos nombres, mazos y manos
void char2casilla(tCasilla & casilla, char casilla_char); //Convierte un char en un tCasilla (usado en cargar tablero)
void leerSecuencia(tJuego & juego, tMazo & mazo, tMazo & secuencia); //Crea una secuencia vacia y la llena a medida que el jugador elige las acciones 
bool mover(tJuego & juego, tCarta carta, bool bicho); //Mueve a una tortuga por el tablero (gira, avanza y bicho)
void moverCaja(tJuego & juego, int var_fila, int var_columna); //Al avanzar contra una caja la mueve
bool casillaDisponible(tJuego & juego, int var_fila, int var_columna); //Comprueba si la casilla de en frente esta disponible para moverse
void dispararLaser(tJuego & juego, int var_fila, int var_columna); //Ejecuta la accion del laser (derrite hielo, empuja jugadores y quema cajas)
void avanzar(tJuego & juego, int var_fila, int var_columna); //Hace avanzar a una tortuga
bool casillaOriginalDisponible(tJuego & juego, tCoordenadas pos_inicial); //Comprueba si la casilla en la que empezo una tortuga esta disponible
void dispararJugador(tJuego & juego, tDir direccion, int var_fila, int var_columna); //Empuja a un jugador cuando se le dispara un laser
void bichoSecuencia(tJuego & juego, int jugador_objetivo); //Si se lanza el bicho a un jugador que habia ejecutado una secuencia la deshace

bool cargarJuego(tJuego & juego)
{
	bool tablero_cargado;

	tablero_cargado = cargarTablero(juego);
	if (!tablero_cargado)
	{

		cout << "ERROR: No se han cargado el tablero correctamente" << endl << endl;  //Si no se carga correctamente NO se puede seguir jugando
	}
	else
	{
		cargarJugadores(juego);
		juego.turno = 0;
	}

	return tablero_cargado;
}

void mostrarJuego(const tJuego & juego)
{
	dibujarTablero(juego);
	dibujarJugadores(juego);
}

bool cargarTablero(tJuego & juego)
{
	string nombre_archivo, basura;
	fstream archivo;
	bool cargado;
	char caracter;
	int num_tortugas = 0;

	cout << "Introduce el nombre del fichero (.txt): ";
	cin >> nombre_archivo;
	archivo.open(nombre_archivo);

	if (archivo.is_open())
	{
		do
		{
			cout << "Introduce el numero de jugadores: ";
			cin >> juego.num_jugadores;

			if (juego.num_jugadores < 1 || juego.num_jugadores > MAX_JUGADORES)
			{
				cout << endl << "Introducir numero entre 1 y " << MAX_JUGADORES << endl;
			}

		} while (juego.num_jugadores < 1 || juego.num_jugadores > MAX_JUGADORES);

		archivo >> caracter;
		getline(archivo, basura);

		while (caracter != char(juego.num_jugadores + int('0')))
		{
			archivo >> caracter;
			getline(archivo, basura);
		}

		for (int fila = 0; fila < MAX_FILAS; fila++)
		{
			for (int columna = 0; columna < MAX_COLUMNAS; columna++)
			{
				archivo.get(caracter);
				char2casilla(juego.tablero[fila][columna], caracter);

				if (juego.tablero[fila][columna].estado == TORTUGA)
				{
					juego.jugadores[num_tortugas].coordenadas.fila = fila;
					juego.jugadores[num_tortugas].coordenadas.columna = columna;

					juego.jugadores[num_tortugas].pos_inicial.fila = fila;
					juego.jugadores[num_tortugas].pos_inicial.columna = columna;

					juego.tablero[fila][columna].tortuga.numero = num_tortugas++;
				}
			}
			getline(archivo, basura);
		}

		cargado = true;

		archivo.close();
	}
	else
	{
		cargado = false;
	}

	return cargado;
}

void cargarJugadores(tJuego & juego)
{
	tCarta carta;
	bool robada;

	for (int i = 0; i < juego.num_jugadores; i++)
	{
		cout << "Nombre jugador " << i + 1 << ": ";
		cin >> juego.jugadores[i].nombre;

		crearMazoAleatorio(juego.jugadores[i].mazo);

		juego.turno = i % juego.num_jugadores;

		for (int j = 0; j < 3; j++) //Empezar robando tres cartas
		{
			robada = accionRobar(juego);
		}
	}
}

bool ejecutarTurno(tJuego &juego)
{
	tMazo & mazo = juego.jugadores[juego.turno].mazo;
	char char_opcion;
	tAccion opcion;
	bool robada = false, bicho_usado = false, joya = false, secuencia_ejecutada = false;

	juego.jugadores[juego.turno].cartas_movidas = 0;
	juego.jugadores[juego.turno].bloques_derretidos.num_derretidos = 0;


	do
	{
		do
		{
			cout << endl << "----------------" << endl << "R = Robar carta." << endl << "E = Introducir secuencia de cartas."
				<< endl << "B = Bicho." << endl << "----------------" << endl << "Que quieres hacer?: ";

			cin >> char_opcion;

			if (char_opcion == 'R' || char_opcion == 'r') { opcion = ACCION_ROBAR; }
			else if (char_opcion == 'E' || char_opcion == 'e') { opcion = ACCION_SECUENCIA; }
			else if (char_opcion == 'B' || char_opcion == 'b') { opcion = ACCION_BICHO; }
			else { cout << "Caracter incorrecto"; opcion = ACCION_INCORRECTA; }

		} while (opcion != ACCION_ROBAR && opcion != ACCION_SECUENCIA && opcion != ACCION_BICHO);

		if (opcion == ACCION_ROBAR)
		{
			robada = accionRobar(juego);
			juego.jugadores[juego.turno].ultima_accion = ACCION_ROBAR;
			if (!robada)
			{
				cout << endl << "No te quedan cartas que robar" << endl;
			}
		}
		else if (opcion == ACCION_SECUENCIA)
		{
			joya = accionSecuencia(juego, mazo);
			juego.jugadores[juego.turno].ultima_accion = ACCION_SECUENCIA;
			secuencia_ejecutada = true;
		}
		else if (opcion == ACCION_BICHO)
		{
			bicho_usado = accionBicho(juego);
			juego.jugadores[juego.turno].ultima_accion = ACCION_BICHO;

			if (!bicho_usado)
			{
				cout << endl << "No te quedan cartas de bicho" << endl;
			}
		}

	} while (!robada && !bicho_usado && !secuencia_ejecutada);

	return joya;
}

bool accionRobar(tJuego & juego)
{
	bool arriba, robada;
	tCarta carta;

	arriba = true;

	robada = sacar(juego.jugadores[juego.turno].mazo, carta, arriba);

	if (robada)
	{
		incluirCarta(juego.jugadores[juego.turno].mano, carta);
		juego.jugadores[juego.turno].carta_robada = carta;
		juego.jugadores[juego.turno].cartas_movidas = 1;
	}

	return robada;
}

void incluirCarta(tMano & mano, tCarta carta)
{
	switch (carta)
	{
	case AVANZAR: mano.num_avanzar++; break;
	case GIROIZQUIERDA: mano.num_izquierda++; break;
	case GIRODERECHA: mano.num_derecha++; break;
	case LASER: mano.num_laser++; break;
	case BICHO: mano.num_bicho++; break;
	}
}

void devolverCarta(tMano & mano, tCarta carta)
{
	switch (carta)
	{
	case AVANZAR: mano.num_avanzar--; break;
	case GIROIZQUIERDA: mano.num_izquierda--; break;
	case GIRODERECHA: mano.num_derecha--; break;
	case LASER: mano.num_laser--; break;
	}
}

bool accionSecuencia(tJuego & juego, tMazo & mazo)
{
	bool joya = false, arriba = false;
	int i;
	tMazo secuencia;
	tCarta carta;
	tMano & mano_jug = juego.jugadores[juego.turno].mano;

	cout << "Introduce una secuencia de cartas para moverte:" << endl;

	leerSecuencia(juego, mazo, secuencia);

	i = secuencia.inicio;
	while ((!joya) && (i < secuencia.num_cartas))
	{
		carta = *(secuencia.cartas[i]);
		joya = mover(juego, carta, false);
		insertar(mazo, carta, arriba);

		i++;
	}

	return joya;
}

void leerSecuencia(tJuego & juego, tMazo & mazo, tMazo & secuencia)
{
	bool arriba = false;
	tTecla tecla;
	tCarta carta;
	tMano & mano_jug = juego.jugadores[juego.turno].mano;

	crearVacia(secuencia); //Inicializamos el mazo para almacenar la secuencia

	tecla = leerTecla();

	while (tecla != _SALIR)
	{
		if (tecla == _AVANZAR)
		{
			if (mano_jug.num_avanzar > 0)
			{
				cout << "^ ";
				carta = AVANZAR;
				devolverCarta(mano_jug, carta);
				insertar(secuencia, carta, arriba);
				juego.jugadores[juego.turno].cartas_movidas++;
			}
			else { cout << "Te has quedado sin avanzar "; }
		}
		else if (tecla == _DERECHA)
		{
			if (mano_jug.num_derecha > 0)
			{
				cout << "> ";
				carta = GIRODERECHA;
				devolverCarta(mano_jug, carta);
				insertar(secuencia, carta, arriba);
				juego.jugadores[juego.turno].cartas_movidas++;
			}
			else { cout << "Te has quedado sin giroDerecha "; }
		}
		else if (tecla == _IZQUIERDA)
		{
			if (mano_jug.num_izquierda > 0)
			{
				cout << "< ";
				carta = GIROIZQUIERDA;
				devolverCarta(mano_jug, carta);
				insertar(secuencia, carta, arriba);
				juego.jugadores[juego.turno].cartas_movidas++;
			}
			else { cout << "Te has quedado sin giroIzquierda  "; }
		}
		else if (tecla == _LASER)
		{
			if (mano_jug.num_laser > 0)
			{
				cout << "~ ";
				carta = LASER;
				devolverCarta(mano_jug, carta);
				insertar(secuencia, carta, arriba);
				juego.jugadores[juego.turno].cartas_movidas++;
			}
			else { cout << "Te has quedado sin laseres  "; }
		}
		else { cout << "Tecla no reconocida "; }
		tecla = leerTecla();
	}
	cout << "Salir ";
}

void char2casilla(tCasilla & casilla, char casilla_char) //Usado en cargarTablero para pasar de char leido en fichero a tCasilla
{
	switch (casilla_char)
	{
	case 'U':
		casilla.estado = TORTUGA; casilla.tortuga.direccion = NORTE; break;
	case 'D':
		casilla.estado = TORTUGA; casilla.tortuga.direccion = SUR; break;
	case 'R':
		casilla.estado = TORTUGA; casilla.tortuga.direccion = ESTE; break;
	case 'L':
		casilla.estado = TORTUGA; casilla.tortuga.direccion = OESTE; break;
	case 'C':
		casilla.estado = CAJA; break;
	case '#':
		casilla.estado = MURO; break;
	case '@':
		casilla.estado = HIELO; break;
	case '$':
		casilla.estado = JOYA; break;
	case ' ':
		casilla.estado = VACIA; break;
	}
}

bool mover(tJuego & juego, tCarta carta, bool bicho)
{
	bool joya = false;
	int var_fila = 0, var_columna = 0;
	tCoordenadas & coord = juego.jugadores[juego.turno].coordenadas;
	tDir & direccion = juego.tablero[coord.fila][coord.columna].tortuga.direccion;

	if (bicho)
	{
		switch (direccion)
		{
		case NORTE: var_fila = 1; var_columna = 0; break;
		case SUR: var_fila = -1; var_columna = 0; break;
		case ESTE: var_fila = 0; var_columna = -1; break;
		case OESTE: var_fila = 0; var_columna = 1; break;
		}
	}
	else
	{
		switch (direccion) //Calcular las coordenadas de la casilla frente al jugador
		{
		case NORTE: var_fila = -1; var_columna = 0; break;
		case SUR: var_fila = 1; var_columna = 0; break;
		case ESTE: var_fila = 0; var_columna = 1; break;
		case OESTE: var_fila = 0; var_columna = -1; break;
		}
	}

	switch (carta)
	{
	case AVANZAR:
		if (casillaDisponible(juego, var_fila, var_columna))
		{
			if (juego.tablero[coord.fila + var_fila][coord.columna + var_columna].estado == CAJA)
			{
				moverCaja(juego, var_fila, var_columna);
			}
			else if (juego.tablero[coord.fila + var_fila][coord.columna + var_columna].estado == JOYA)
			{
				avanzar(juego, var_fila, var_columna);
				joya = true;
			}
			else
			{
				avanzar(juego, var_fila, var_columna);
			}
		}
		break;

	case GIRODERECHA:
		if (!bicho)
		{
			direccion = tDir((int(direccion) + 1) % 4);
		}
		else
		{
			direccion = tDir((int(direccion) + 3) % 4);
		}
		break;
	case GIROIZQUIERDA:
		if (!bicho)
		{
			direccion = tDir((int(direccion) + 3) % 4);
		}
		else
		{
			direccion = tDir((int(direccion) + 1) % 4);
		}
		break;

	case LASER:
		dispararLaser(juego, var_fila, var_columna);
	}

	return joya;
}

void dispararLaser(tJuego & juego, int var_fila, int var_columna)
{
	tCoordenadas & coord = juego.jugadores[juego.turno].coordenadas;
	tCoordenadas & pos_incial = juego.jugadores[juego.turno].pos_inicial;
	tCoordenadas & coord_hielo = juego.jugadores[juego.turno].bloques_derretidos.bloques[juego.jugadores[juego.turno].bloques_derretidos.num_derretidos];
	tDir & direccion = juego.tablero[coord.fila][coord.columna].tortuga.direccion;
	switch (direccion)
	{
	case NORTE:
		while (juego.tablero[coord.fila + var_fila][coord.columna + var_columna].estado == VACIA && coord.fila + var_fila > 0)
		{
			var_fila--;
		}
		break;
	case SUR:
		while (juego.tablero[coord.fila + var_fila][coord.columna + var_columna].estado == VACIA && coord.fila + var_fila < MAX_FILAS)
		{
			var_fila++;
		}
		break;
	case ESTE:
		while (juego.tablero[coord.fila + var_fila][coord.columna + var_columna].estado == VACIA && coord.columna + var_columna < MAX_COLUMNAS)
		{
			var_columna++;
		}
		break;
	case OESTE:
		while (juego.tablero[coord.fila + var_fila][coord.columna + var_columna].estado == VACIA && coord.columna + var_columna > 0)
		{
			var_columna--;
		}
		break;
	}

	if (juego.tablero[coord.fila + var_fila][coord.columna + var_columna].estado == HIELO)
	{
		juego.tablero[coord.fila + var_fila][coord.columna + var_columna].estado = VACIA;

		coord_hielo.fila = coord.fila + var_fila;
		coord_hielo.columna = coord.columna + var_columna;
		juego.jugadores[juego.turno].bloques_derretidos.num_derretidos++;
	}
	else if (juego.tablero[coord.fila + var_fila][coord.columna + var_columna].estado == JOYA)
	{
		if (casillaOriginalDisponible(juego, pos_incial))
		{
			juego.tablero[pos_incial.fila][pos_incial.columna].estado = juego.tablero[coord.fila][coord.columna].estado;
			juego.tablero[coord.fila][coord.columna].estado = VACIA;
			coord = pos_incial;
		}
	}
	else if (juego.tablero[coord.fila + var_fila][coord.columna + var_columna].estado == TORTUGA)
	{
		var_fila += coord.fila;
		var_columna += coord.columna;
		dispararJugador(juego, direccion, var_fila, var_columna);
	}
	else if (juego.tablero[coord.fila + var_fila][coord.columna + var_columna].estado == CAJA)
	{
		juego.tablero[coord.fila + var_fila][coord.columna + var_columna].estado = FUEGO;
	}
}

void dispararJugador(tJuego & juego, tDir direccion, int var_fila, int var_columna)
{
	int num_tortuga = juego.tablero[var_fila][var_columna].tortuga.numero;
	tCoordenadas & coord = juego.jugadores[num_tortuga].coordenadas;
	tDir dir_tor = juego.tablero[coord.fila][coord.columna].tortuga.direccion;

	switch (direccion)
	{
	case NORTE:
		var_fila--;
		while (juego.tablero[var_fila][var_columna].estado == VACIA && var_fila > -1)
		{
			var_fila--;
		}
		break;
	case SUR:
		var_fila++;
		while (juego.tablero[var_fila][var_columna].estado == VACIA && var_fila < MAX_FILAS)
		{
			var_fila++;
		}
		break;
	case ESTE:
		var_columna++;
		while (juego.tablero[var_fila][var_columna].estado == VACIA && var_columna < MAX_COLUMNAS)
		{
			var_columna++;
		}
		break;
	case OESTE:
		var_columna--;
		while (juego.tablero[var_fila][+var_columna].estado == VACIA && var_columna > -1)
		{
			var_columna--;
		}
		break;
	}

	if (juego.tablero[var_fila][var_columna].estado == JOYA)
	{
		juego.jugadores[num_tortuga].sigue_jugando = false;

		juego.ganadores.puesto[juego.ganadores.num_ganadores] = juego.jugadores[num_tortuga];
		juego.ganadores.num_ganadores++;

		juego.tablero[var_fila][var_columna].estado = VACIA; // casilla de la joya
		juego.tablero[coord.fila][coord.columna].estado = VACIA; // casilla inicial del jugador al que disparan

		mostrarJuego(juego);


		juego.fin_partida = seguirJugando(juego);
	}
	else
	{
		switch (direccion)  // como no se ha chocado con la joya, hay que desplazarle a la casilla anterior al objeto con el que se ha chocado
		{
		case NORTE:	var_fila++;	break;
		case SUR:	var_fila--;	break;
		case ESTE:	var_columna--;	break;
		case OESTE:	var_columna++;	break;
		}
		juego.tablero[coord.fila][coord.columna].estado = VACIA;
		coord.fila = var_fila;
		coord.columna = var_columna;
		juego.tablero[var_fila][var_columna].estado = TORTUGA;
		juego.tablero[var_fila][var_columna].tortuga.direccion = dir_tor;
		juego.tablero[var_fila][var_columna].tortuga.numero = num_tortuga;
	}
}

void moverCaja(tJuego & juego, int var_fila, int var_columna)
{
	tCoordenadas & coord = juego.jugadores[juego.turno].coordenadas;
	tCasilla & casilla_contigua_2 = juego.tablero[coord.fila + (var_fila * 2)][coord.columna + (var_columna * 2)]; //dos casillas mas alla del jug

	if (casillaDisponible(juego, var_fila * 2, var_columna * 2) && casilla_contigua_2.estado != CAJA)
	{
		casilla_contigua_2.estado = CAJA;

		avanzar(juego, var_fila, var_columna);
	}
}

void avanzar(tJuego & juego, int var_fila, int var_columna)
{
	tCoordenadas & coord = juego.jugadores[juego.turno].coordenadas;
	tDir & direccion = juego.tablero[coord.fila][coord.columna].tortuga.direccion;
	tCasilla & casilla_actual = juego.tablero[coord.fila][coord.columna];
	tCasilla & casilla_contigua = juego.tablero[coord.fila + var_fila][coord.columna + var_columna]; //una casilla mas alla del jug

	casilla_actual.estado = VACIA;
	coord.fila += var_fila;
	coord.columna += var_columna;

	casilla_contigua.estado = TORTUGA;
	casilla_contigua.tortuga.direccion = direccion;
	casilla_contigua.tortuga.numero = juego.turno;
}

bool casillaDisponible(tJuego & juego, int var_fila, int var_columna)
{
	bool disponible;
	tCoordenadas & coord = juego.jugadores[juego.turno].coordenadas;
	tDir & direccion = juego.tablero[coord.fila][coord.columna].tortuga.direccion;
	tCasilla & casilla = juego.tablero[coord.fila + var_fila][coord.columna + var_columna];

	switch (casilla.estado)
	{
	case VACIA: disponible = true; break;
	case TORTUGA: disponible = false; break;
	case CAJA: disponible = true; break;
	case MURO: disponible = false; break;
	case HIELO: disponible = false; break;
	case JOYA: disponible = true; break;
	case FUEGO: disponible = false; break;
	default: disponible = false; break;
	}

	if (coord.columna + var_columna >= MAX_COLUMNAS || coord.columna + var_columna < 0) //No pasarse de las paredes
	{
		disponible = false;
	}

	return disponible;
}

bool casillaOriginalDisponible(tJuego & juego, tCoordenadas pos_inicial)
{
	bool disponible;

	if (juego.tablero[pos_inicial.fila][pos_inicial.columna].estado == VACIA)
	{
		disponible = true;
	}
	else
	{
		disponible = false;
	}

	return disponible;
}

void reiniciarPartida(tJuego & juego)
{
	for (int i = 0; i < juego.num_jugadores; i++)
	{
		juego.jugadores[i].mano.num_avanzar = 0;
		juego.jugadores[i].mano.num_derecha = 0;
		juego.jugadores[i].mano.num_izquierda = 0;
		juego.jugadores[i].mano.num_laser = 0;
	}

	for (int i = 0; i < juego.ganadores.num_ganadores; i++)
	{
		juego.jugadores[i].sigue_jugando = true;
	}
}

bool accionBicho(tJuego & juego)
{
	bool bicho;
	int jugador_objetivo;
	tCarta carta;

	if (juego.jugadores[juego.turno].mano.num_bicho > 0)
	{
		juego.jugadores[juego.turno].mano.num_bicho--;
		bicho = true;
		cout << endl << "A que jugador quieres lanzarle el bicho? ";
		cin >> jugador_objetivo;
		jugador_objetivo--;
		switch (juego.jugadores[jugador_objetivo].ultima_accion)
		{
		case ACCION_BICHO:
			cout << "No puedes utilizar el bicho contra otro bicho";
			system("pause");
			break;
		case ACCION_ROBAR:
			devolverCarta(juego.jugadores[jugador_objetivo].mano, juego.jugadores[jugador_objetivo].carta_robada);
			insertar(juego.jugadores[jugador_objetivo].mazo, juego.jugadores[jugador_objetivo].carta_robada, true);
			break;
		case ACCION_SECUENCIA:
			bichoSecuencia(juego, jugador_objetivo);
			break;
		}
	}
	else
	{
		bicho = false;
	}

	return bicho;
}

void bichoSecuencia(tJuego & juego, int jugador_objetivo)
{
	int aux, fila_hielo, colu_hielo;;
	bool sacada, joya;
	tCarta carta;

	aux = juego.turno;
	juego.turno = jugador_objetivo;
	for (int i = 0; i < juego.jugadores[jugador_objetivo].cartas_movidas; i++)
	{
		sacada = sacar(juego.jugadores[jugador_objetivo].mazo, carta, false);
		if (carta != LASER)
		{
			joya = mover(juego, carta, true); //Al deshacer la accion nunca se alcanzara la joya
		}
		else
		{
			for (int i = 0; i < juego.jugadores[jugador_objetivo].bloques_derretidos.num_derretidos; i++)
			{
				fila_hielo = juego.jugadores[jugador_objetivo].bloques_derretidos.bloques[i].fila;
				colu_hielo = juego.jugadores[jugador_objetivo].bloques_derretidos.bloques[i].columna;
				juego.tablero[fila_hielo][colu_hielo].estado = HIELO;
			}
		}
		incluirCarta(juego.jugadores[jugador_objetivo].mano, carta);
	}
	juego.turno = aux;
}

void cambiarTurno(tJuego & juego)
{
	do
	{
		juego.turno = (juego.turno + 1) % juego.num_jugadores;

	} while (juego.jugadores[juego.turno].sigue_jugando == false && juego.num_jugadores > juego.ganadores.num_ganadores);
}

bool esFinDePartida(tJuego & juego)
{
	bool fin;

	if (juego.num_jugadores == juego.ganadores.num_ganadores)
	{
		fin = true;
	}
	else
	{
		fin = false;
	}

	return fin;
}

bool seguirJugando(tJuego & juego)
{
	bool fin;
	char opcion;

	if (juego.num_jugadores > 1)
	{
		cout << "Quieres seguir jugando? (S/N): ";
		cin >> opcion;

		while (opcion != 'S' && opcion != 's' && opcion != 'N' && opcion != 'n')
		{
			cout << "Quieres seguir jugando? (S/N): ";
			cin >> opcion;
		}
		if (opcion == 'S' || opcion == 's')
		{
			fin = false;
		}
		else
		{
			fin = true;
		}
	}

	return fin;
}

void calcularPuntuaciones(tJuego & juego, tPuntuaciones & puntuaciones)
{
	int nuevos;

	for (int i = 0; i < juego.ganadores.num_ganadores; i++)
	{
		nuevos = juego.num_jugadores - i;
		actualizarPuntuacion(puntuaciones, juego.ganadores.puesto[i].nombre, nuevos);
	}
}

void ejecutarJuego(tJuego & juego)
{
	bool joya = false;
	juego.fin_partida = false;
	juego.ganadores.num_ganadores = 0;

	mostrarJuego(juego);

	while (!juego.fin_partida)
	{
		joya = ejecutarTurno(juego);

		if (joya)
		{
			juego.jugadores[juego.turno].sigue_jugando = false;

			juego.ganadores.puesto[juego.ganadores.num_ganadores] = juego.jugadores[juego.turno];
			juego.ganadores.num_ganadores++;

			juego.tablero[juego.jugadores[juego.turno].coordenadas.fila][juego.jugadores[juego.turno].coordenadas.columna].estado = VACIA;

			mostrarJuego(juego);

			if (!esFinDePartida(juego))
			{
				juego.fin_partida = seguirJugando(juego);
			}
			else
			{
				juego.fin_partida = true;
			}
		}

		cambiarTurno(juego);
		mostrarJuego(juego);
	}
}