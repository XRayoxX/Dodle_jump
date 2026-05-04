
#include "Modelos/Jugador.h"

void InicializarJugador(Jugador &j) {
    j.posicion = { (float)GetScreenWidth() / 2, (float)GetScreenHeight() - 50 };
    j.velocidad = { 0, 0 };
    j.radio = 20.0f;
    j.estaVivo = true;
}

void ActualizarJugador(Jugador &j) {
    // Gravedad constante
    j.velocidad.y += 0.5f;

    // Movimiento lateral (teclas A/D o Flechas)
    if (IsKeyDown(KEY_RIGHT)) j.posicion.x += 5.0f;
    if (IsKeyDown(KEY_LEFT)) j.posicion.x -= 5.0f;

    // Aplicar velocidad a la posición
    j.posicion.y += j.velocidad.y;
    j.posicion.x += j.velocidad.x;

    // Efecto "Wrap Around" (si sale por un lado, aparece por el otro)
    if (j.posicion.x > GetScreenWidth()) j.posicion.x = 0;
    if (j.posicion.x < 0) j.posicion.x = (float)GetScreenWidth();
}

void DibujarJugador(Jugador j) {
    DrawCircleV(j.posicion, j.radio, GREEN);
}