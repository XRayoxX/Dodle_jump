#ifndef JUGADOR_H
#define JUGADOR_H

#include "raylib.h"

struct Jugador {
    Vector2 posicion;
    Vector2 velocidad;
    float radio;
    bool estaVivo;
};

void InicializarJugador(Jugador &j);
void ActualizarJugador(Jugador &j);
void DibujarJugador(Jugador j);

#endif