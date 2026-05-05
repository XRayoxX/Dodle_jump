#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include "Modelos/Plataforma.h"

// Inicializa y carga los sonidos de salto.
void InitAudioManager();

// Libera sonidos y cierra el dispositivo de audio.
void ShutdownAudioManager();

// Reproduce el sonido segun el tipo de plataforma.
void PlayJumpSound(TipoPlataforma tipo);

#endif

