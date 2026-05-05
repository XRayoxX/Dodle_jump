#include "Modelos/Plataforma.h"
#include <stdlib.h>

static const char* kOneUseLeftPath   = "../Imagenes/Tiles Plataforma un uso/tile_0001.png";
static const char* kOneUseCenterPath = "../Imagenes/Tiles Plataforma un uso/tile_0002.png";
static const char* kOneUseRightPath  = "../Imagenes/Tiles Plataforma un uso/tile_0003.png";

static Texture2D gOneUseLeft = {};
static Texture2D gOneUseCenter = {};
static Texture2D gOneUseRight = {};
static bool gOneUseLoaded = false;

void CargarTexturasPlataformas() {
    if (gOneUseLoaded) return;

    gOneUseLeft = LoadTexture(kOneUseLeftPath);
    gOneUseCenter = LoadTexture(kOneUseCenterPath);
    gOneUseRight = LoadTexture(kOneUseRightPath);

    gOneUseLoaded = (gOneUseLeft.id != 0) && (gOneUseCenter.id != 0) && (gOneUseRight.id != 0);
    if (!gOneUseLoaded) {
        if (gOneUseLeft.id != 0) UnloadTexture(gOneUseLeft);
        if (gOneUseCenter.id != 0) UnloadTexture(gOneUseCenter);
        if (gOneUseRight.id != 0) UnloadTexture(gOneUseRight);
        gOneUseLeft = {};
        gOneUseCenter = {};
        gOneUseRight = {};
    }
}

void LiberarTexturasPlataformas() {
    if (!gOneUseLoaded) return;
    UnloadTexture(gOneUseLeft);
    UnloadTexture(gOneUseCenter);
    UnloadTexture(gOneUseRight);
    gOneUseLeft = {};
    gOneUseCenter = {};
    gOneUseRight = {};
    gOneUseLoaded = false;
}

// ── Devuelve nivel según umbrales configurables ──────────────────────────────
int ObtenerNivel(int puntaje) {
    if (puntaje >= UMBRAL_NIVEL_4) return 4;
    if (puntaje >= UMBRAL_NIVEL_3) return 3;
    if (puntaje >= UMBRAL_NIVEL_2) return 2;
    if (puntaje >= UMBRAL_NIVEL_1) return 1;
    return 0;
}

// ── Parámetros de dificultad según nivel ────────────────────────────────────
static float ObtenerAncho(int nivel) {
    switch(nivel) {
        case 1: return ANCHO_NIVEL_1;
        case 2: return ANCHO_NIVEL_2;
        case 3: return ANCHO_NIVEL_3;
        case 4: return ANCHO_NIVEL_4;
        default: return ANCHO_NIVEL_0;
    }
}

static float ObtenerVelMovil(int nivel) {
    switch(nivel) {
        case 1: return VEL_MOVIL_NIVEL_1;
        case 2: return VEL_MOVIL_NIVEL_2;
        case 3: return VEL_MOVIL_NIVEL_3;
        case 4: return VEL_MOVIL_NIVEL_4;
        default: return VEL_MOVIL_NIVEL_0;
    }
}

static void ObtenerDistancias(int nivel, int* distMin, int* distMax) {
    switch(nivel) {
        case 1: *distMin = DIST_MIN_NIVEL_1; *distMax = DIST_MAX_NIVEL_1; break;
        case 2: *distMin = DIST_MIN_NIVEL_2; *distMax = DIST_MAX_NIVEL_2; break;
        case 3: *distMin = DIST_MIN_NIVEL_3; *distMax = DIST_MAX_NIVEL_3; break;
        case 4: *distMin = DIST_MIN_NIVEL_4; *distMax = DIST_MAX_NIVEL_4; break;
        default: *distMin = DIST_MIN_NIVEL_0; *distMax = DIST_MAX_NIVEL_0; break;
    }
}

// ── Probabilidad de tipo según nivel ────────────────────────────────────────
static TipoPlataforma SortearTipo(int nivel) {
    // A mayor nivel, menos NORMAL y más UN_USO
    // nivel 0: 60% normal, 20% boost, 20% un_uso
    // nivel 4: 30% normal, 30% boost, 40% un_uso
    int probNormal = 6 - nivel;          // 6,5,4,3,2
    if (probNormal < 2) probNormal = 2;
    int probBoost  = 2 + (nivel / 2);   // 2,2,3,3,4

    int r = rand() % 10;
    if (r < probNormal)              return NORMAL;
    if (r < probNormal + probBoost)  return MOVIL_BOOST;
    return UN_USO;
}

// ── Inicialización ───────────────────────────────────────────────────────────
void InicializarPlataformas(Plataforma plataformas[]) {
    // Plataforma 0: seguridad debajo del jugador
    plataformas[0].rect   = { (float)GetScreenWidth()/2 - 50,
                               (float)GetScreenHeight() - 50, 100, 20 };
    plataformas[0].tipo   = NORMAL;
    plataformas[0].activa = true;
    plataformas[0].velocidadX = 0;

    float ultimoY = plataformas[0].rect.y;
    float ultimoX = plataformas[0].rect.x;

    for (int i = 1; i < MAX_PLATAFORMAS; i++) {
        plataformas[i].rect.width  = ANCHO_NIVEL_0;
        plataformas[i].rect.height = 20;
        plataformas[i].activa      = true;

        // Distancia vertical controlada (nivel 0 en inicio)
        ultimoY -= (float)(rand() % (DIST_MAX_NIVEL_0 - DIST_MIN_NIVEL_0) + DIST_MIN_NIVEL_0);
        plataformas[i].rect.y = ultimoY;

        // Distancia horizontal cercana a la anterior (max ±150 px)
        float nuevaX = ultimoX + (float)((rand() % 300) - 150);
        if (nuevaX < 0) nuevaX = 0;
        if (nuevaX > GetScreenWidth() - plataformas[i].rect.width)
            nuevaX = GetScreenWidth() - plataformas[i].rect.width;
        plataformas[i].rect.x = nuevaX;
        ultimoX = nuevaX;

        plataformas[i].tipo = SortearTipo(0);
        plataformas[i].velocidadX = (rand() % 2 == 0)
                                    ? VEL_MOVIL_NIVEL_0 : -VEL_MOVIL_NIVEL_0;
    }
}

// ── Actualización ────────────────────────────────────────────────────────────
void ActualizarPlataformas(Plataforma plataformas[], float scrollSpeed, int puntaje) {
    int nivel = ObtenerNivel(puntaje);

    // 1. Mover todas
    for (int i = 0; i < MAX_PLATAFORMAS; i++) {
        plataformas[i].rect.y += scrollSpeed;

        if (plataformas[i].tipo == MOVIL_BOOST && plataformas[i].activa) {
            plataformas[i].rect.x += plataformas[i].velocidadX;
            if (plataformas[i].rect.x <= 0 ||
                plataformas[i].rect.x + plataformas[i].rect.width >= GetScreenWidth()) {
                plataformas[i].velocidadX *= -1;
            }
        }
    }

    // 2. Buscar la más alta UNA sola vez
    float masAltaY = plataformas[0].rect.y;
    float masAltaX = plataformas[0].rect.x;
    for (int i = 1; i < MAX_PLATAFORMAS; i++) {
        if (plataformas[i].rect.y < masAltaY) {
            masAltaY = plataformas[i].rect.y;
            masAltaX = plataformas[i].rect.x;
        }
    }

    // 3. Reciclar
    int distMin, distMax;
    ObtenerDistancias(nivel, &distMin, &distMax);
    float ancho    = ObtenerAncho(nivel);
    float velMovil = ObtenerVelMovil(nivel);

    // Ancho de pantalla útil
    float margen     = ancho / 2.0f;
    float centroMin  = margen;
    float centroMax  = GetScreenWidth() - margen - ancho;

    for (int i = 0; i < MAX_PLATAFORMAS; i++) {
        if (plataformas[i].rect.y > GetScreenHeight()) {

            plataformas[i].rect.width  = ancho;
            plataformas[i].rect.height = 20;
            plataformas[i].activa      = true;

            float nuevaY = masAltaY - (float)(rand() % (distMax - distMin) + distMin);
            plataformas[i].rect.y = nuevaY;

            // ── FIX: desplazamiento máximo ±120px, pero siempre dentro de la pantalla ──
            float desplazamiento = (float)((rand() % 241) - 120); // -120 a +120
            float nuevaX = masAltaX + desplazamiento;

            // Clamp estricto para que no se vaya a las esquinas
            if (nuevaX < centroMin) nuevaX = centroMin;
            if (nuevaX > centroMax) nuevaX = centroMax;

            plataformas[i].rect.x = nuevaX;

            plataformas[i].tipo = SortearTipo(nivel);
            plataformas[i].velocidadX = (rand() % 2 == 0) ? velMovil : -velMovil;

            // Actualizar referencia para la siguiente reciclada en este frame
            masAltaY = nuevaY;
            masAltaX = nuevaX;
        }
    }
}

static void DibujarPlataformaUnUso(const Plataforma& plataforma) {
    if (!gOneUseLoaded) {
        DrawRectangleRec(plataforma.rect, YELLOW);
        return;
    }

    float tileW = (float)gOneUseCenter.width;
    float tileH = (float)gOneUseCenter.height;

    Rectangle src = { 0.0f, 0.0f, tileW, tileH };
    float minWidth = tileW * 2.0f;

    if (plataforma.rect.width <= minWidth) {
        float leftW = plataforma.rect.width * 0.5f;
        float rightW = plataforma.rect.width - leftW;
        DrawTexturePro(gOneUseLeft, src,
                       { plataforma.rect.x, plataforma.rect.y, leftW, plataforma.rect.height },
                       { 0.0f, 0.0f }, 0.0f, WHITE);
        DrawTexturePro(gOneUseRight, src,
                       { plataforma.rect.x + leftW, plataforma.rect.y, rightW, plataforma.rect.height },
                       { 0.0f, 0.0f }, 0.0f, WHITE);
        return;
    }

    float leftX = plataforma.rect.x;
    float rightX = plataforma.rect.x + plataforma.rect.width - tileW;

    DrawTexturePro(gOneUseLeft, src,
                   { leftX, plataforma.rect.y, tileW, plataforma.rect.height },
                   { 0.0f, 0.0f }, 0.0f, WHITE);
    DrawTexturePro(gOneUseRight, src,
                   { rightX, plataforma.rect.y, tileW, plataforma.rect.height },
                   { 0.0f, 0.0f }, 0.0f, WHITE);

    float x = leftX + tileW;
    float endX = rightX;
    while (x + tileW <= endX) {
        DrawTexturePro(gOneUseCenter, src,
                       { x, plataforma.rect.y, tileW, plataforma.rect.height },
                       { 0.0f, 0.0f }, 0.0f, WHITE);
        x += tileW;
    }

    float remainder = endX - x;
    if (remainder > 0.5f) {
        DrawTexturePro(gOneUseCenter, src,
                       { x, plataforma.rect.y, remainder, plataforma.rect.height },
                       { 0.0f, 0.0f }, 0.0f, WHITE);
    }
}

// ── Dibujo ───────────────────────────────────────────────────────────────────
void DibujarPlataformas(Plataforma plataformas[]) {
    for (int i = 0; i < MAX_PLATAFORMAS; i++) {
        if (!plataformas[i].activa) continue;

        switch (plataformas[i].tipo) {
            case NORMAL:
                DrawRectangleRec(plataformas[i].rect, SKYBLUE);
                break;
            case MOVIL_BOOST:
                DrawRectangleRec(plataformas[i].rect, BLUE);
                break;
            case UN_USO:
                DibujarPlataformaUnUso(plataformas[i]);
                break;
            default:
                DrawRectangleRec(plataformas[i].rect, GRAY);
                break;
        }
    }
}