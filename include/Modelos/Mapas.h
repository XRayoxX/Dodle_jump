//
// Created by Ale on 3/05/2026.
//

#ifndef DODLE_JUMP_MAPAS_H
#define DODLE_JUMP_MAPAS_H

#include "raylib.h"

struct Mapas {
    Texture2D fondoNivel0;
    Texture2D fondoNivel1;
};

// --- API de mapas ---
void InicializarMapas(Mapas& mapas, const char* rutaNivel0, const char* rutaNivel1);
void DibujarMapas(const Mapas& mapas, int screenWidth, int screenHeight, int nivel);
void LiberarMapas(Mapas& mapas);

#endif //DODLE_JUMP_MAPAS_H
