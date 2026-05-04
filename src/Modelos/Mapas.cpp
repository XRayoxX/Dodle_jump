#include "../../include/Modelos/Mapas.h"

void InicializarMapas(Mapas& mapas, const char* rutaFondo) {
    TraceLog(LOG_INFO, "Working dir: %s", GetWorkingDirectory());
    TraceLog(LOG_INFO, "Intentando cargar fondo: %s", rutaFondo);

    if (!FileExists(rutaFondo)) {
        TraceLog(LOG_ERROR, "No se encontro el fondo: %s", rutaFondo);
        mapas.fondo = { 0 };
        return;
    }

    mapas.fondo = LoadTexture(rutaFondo);
    if (mapas.fondo.id == 0) {
        TraceLog(LOG_ERROR, "No se pudo cargar la textura: %s", rutaFondo);
    }
}

void DibujarMapas(const Mapas& mapas, int screenWidth, int screenHeight) {
    if (mapas.fondo.id == 0 || mapas.fondo.width == 0 || mapas.fondo.height == 0) {
        // Fondo de respaldo para confirmar que el dibujo se llama.
        DrawRectangleGradientV(0, 0, screenWidth, screenHeight, (Color){ 180, 220, 255, 255 }, (Color){ 100, 160, 230, 255 });
        DrawText("FONDO NO CARGADO", 10, screenHeight - 30, 20, RED);
        return;
    }

    Rectangle src = { 0.0f, 0.0f, (float)mapas.fondo.width, (float)mapas.fondo.height };
    Rectangle dst = { 0.0f, 0.0f, (float)screenWidth, (float)screenHeight };
    DrawTexturePro(mapas.fondo, src, dst, (Vector2){ 0.0f, 0.0f }, 0.0f, WHITE);
}

void LiberarMapas(Mapas& mapas) {
    if (mapas.fondo.id != 0) {
        UnloadTexture(mapas.fondo);
        mapas.fondo.id = 0;
    }
}
