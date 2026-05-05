#ifndef PLATAFORMA_H
#define PLATAFORMA_H

#include "raylib.h"

enum TipoPlataforma { NORMAL, MOVIL_BOOST, UN_USO };

struct Plataforma {
    Rectangle rect;
    TipoPlataforma tipo;
    float velocidadX;
    bool activa;
};

// --- Configuracion general ---
#define MAX_PLATAFORMAS 15

// --- Parametros de dificultad ---
// Umbrales de puntaje donde sube la dificultad (puedes agregar más niveles)
#define UMBRAL_NIVEL_1   1000
#define UMBRAL_NIVEL_2  3000
#define UMBRAL_NIVEL_3  5000
#define UMBRAL_NIVEL_4  6000

// Impulso de rebote por tipo de plataforma
#define IMPULSO_NORMAL      -12.0f
#define IMPULSO_BOOST       -20.0f   // plataforma azul (MOVIL_BOOST)

// Ancho de plataforma por nivel (mínimo permitido: 35)
#define ANCHO_NIVEL_0   100.0f
#define ANCHO_NIVEL_1    85.0f
#define ANCHO_NIVEL_2    70.0f
#define ANCHO_NIVEL_3    55.0f
#define ANCHO_NIVEL_4    100.0f

// Distancia vertical entre plataformas por nivel (min, max)
#define DIST_MIN_NIVEL_0  70
#define DIST_MAX_NIVEL_0 100
#define DIST_MIN_NIVEL_1  80
#define DIST_MAX_NIVEL_1 115
#define DIST_MIN_NIVEL_2  90
#define DIST_MAX_NIVEL_2 130
#define DIST_MIN_NIVEL_3 100
#define DIST_MAX_NIVEL_3 145
#define DIST_MIN_NIVEL_4 110
#define DIST_MAX_NIVEL_4 155

// Velocidad de plataformas móviles por nivel
#define VEL_MOVIL_NIVEL_0  3.0f
#define VEL_MOVIL_NIVEL_1  4.5f
#define VEL_MOVIL_NIVEL_2  5.5f
#define VEL_MOVIL_NIVEL_3  6.5f
#define VEL_MOVIL_NIVEL_4  8.0f
// ────────────────────────────────────────────────────────────────────────────

// --- API de plataformas ---
void InicializarPlataformas(Plataforma plataformas[]);
void ActualizarPlataformas(Plataforma plataformas[], float scrollSpeed, int puntaje);
void DibujarPlataformas(Plataforma plataformas[]);
void CargarTexturasPlataformas();
void LiberarTexturasPlataformas();

// Devuelve el nivel actual segun puntaje (util para UI)
int ObtenerNivel(int puntaje);

#endif