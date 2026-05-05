//
// Created by Ale on 3/05/2026.
//

#ifndef DODLE_JUMP_MAPAS_H
#define DODLE_JUMP_MAPAS_H

#include "raylib.h"

struct Mapas {
    Texture2D fondo;
};

// --- API de mapas ---
void InicializarMapas(Mapas& mapas, const char* rutaFondo);
void DibujarMapas(const Mapas& mapas, int screenWidth, int screenHeight);
void LiberarMapas(Mapas& mapas);

#endif //DODLE_JUMP_MAPAS_H
