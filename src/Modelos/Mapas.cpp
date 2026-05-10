#include "../../include/Modelos/Mapas.h"

// --- Carga y gestion del fondo ---
void InicializarMapas(Mapas& mapas, const char* rutaNivel0, const char* rutaNivel1) {
    TraceLog(LOG_INFO, "Working dir: %s", GetWorkingDirectory());

    mapas.fondoNivel0 = { 0 };
    mapas.fondoNivel1 = { 0 };

    TraceLog(LOG_INFO, "Intentando cargar fondo nivel 0: %s", rutaNivel0);
    if (FileExists(rutaNivel0)) {
        mapas.fondoNivel0 = LoadTexture(rutaNivel0);
        if (mapas.fondoNivel0.id == 0) {
            TraceLog(LOG_ERROR, "No se pudo cargar la textura: %s", rutaNivel0);
        }
    } else {
        TraceLog(LOG_ERROR, "No se encontro el fondo: %s", rutaNivel0);
    }

    TraceLog(LOG_INFO, "Intentando cargar fondo nivel 1: %s", rutaNivel1);
    if (FileExists(rutaNivel1)) {
        mapas.fondoNivel1 = LoadTexture(rutaNivel1);
        if (mapas.fondoNivel1.id == 0) {
            TraceLog(LOG_ERROR, "No se pudo cargar la textura: %s", rutaNivel1);
        }
    } else {
        TraceLog(LOG_ERROR, "No se encontro el fondo: %s", rutaNivel1);
    }
}

// --- Dibujo del fondo ---
void DibujarMapas(const Mapas& mapas, int screenWidth, int screenHeight, int nivel) {
    const Texture2D& fondo = (nivel >= 1 && mapas.fondoNivel1.id != 0) ? mapas.fondoNivel1 : mapas.fondoNivel0;

    if (fondo.id == 0 || fondo.width == 0 || fondo.height == 0) {
        // Fondo de respaldo para confirmar que el dibujo se llama.
        DrawRectangleGradientV(0, 0, screenWidth, screenHeight, (Color){ 180, 220, 255, 255 }, (Color){ 100, 160, 230, 255 });
        DrawText("FONDO NO CARGADO", 10, screenHeight - 30, 20, RED);
        return;
    }

    Rectangle src = { 0.0f, 0.0f, (float)fondo.width, (float)fondo.height };
    Rectangle dst = { 0.0f, 0.0f, (float)screenWidth, (float)screenHeight };
    DrawTexturePro(fondo, src, dst, (Vector2){ 0.0f, 0.0f }, 0.0f, WHITE);
}

// --- Liberacion del fondo ---
void LiberarMapas(Mapas& mapas) {
    if (mapas.fondoNivel0.id != 0) {
        UnloadTexture(mapas.fondoNivel0);
        mapas.fondoNivel0.id = 0;
    }
    if (mapas.fondoNivel1.id != 0) {
        UnloadTexture(mapas.fondoNivel1);
        mapas.fondoNivel1.id = 0;
    }
}
