#ifndef GAME_API_CONFIG_H
#define GAME_API_CONFIG_H

#include <string>

namespace GameApiConfig {

    // =========================
    // CONFIGURACIÓN DEL SERVIDOR
    // =========================

    const std::string BASE_URL = "http://52.55.14.96:3000";

    // =========================
    // CONFIGURACIÓN DEL JUEGO
    // =========================

    const std::string API_KEY = "C_G6_DOODLE_JUMP_KEY_2026";

    const std::string CODIGO_JUEGO = "C_G6_DOODLE_JUMP";

    const std::string VERSION_JUEGO = "1.0.0";

    // =========================
    // COSTO DE PARTIDA
    // =========================

    const int COSTO_PARTIDA = 100;

    // =========================
    // REGLAS DE PREMIOS
    // =========================
    // Ahora el jugador gana:
    // 10 tokens por cada 100 puntos de score

    const int SCORE_POR_PREMIO = 100;
    // Cada 100 puntos de score

    const int TOKENS_POR_PREMIO = 1;
    // Se otorgan 10 tokens

    // =========================
    // REPORTE DE SCORE
    // =========================

    const int REPORTAR_CADA_PUNTOS = 50;

}

#endif la protección del archivo.