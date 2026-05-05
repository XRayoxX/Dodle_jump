#include "Audio/AudioManager.h"
#include "raylib.h"

// --- Rutas de audio (relativas al directorio de ejecucion) ---
static const char* kNormalJumpPath = "../Audio/Normal.wav";
static const char* kOneUseJumpPath = "../Audio/De un uso.wav";
static const char* kBoostJumpPath  = "../Audio/Boost audio.wav";

// --- Sonidos y estado ---
static Sound gJumpNormal = {};
static Sound gJumpOneUse = {};
static Sound gJumpBoost  = {};
static bool gAudioReady = false;

static void StopAllJumpSounds() {
    if (IsSoundPlaying(gJumpNormal)) StopSound(gJumpNormal);
    if (IsSoundPlaying(gJumpOneUse)) StopSound(gJumpOneUse);
    if (IsSoundPlaying(gJumpBoost)) StopSound(gJumpBoost);
}

void InitAudioManager() {
    if (gAudioReady) return;

    InitAudioDevice();
    if (!IsAudioDeviceReady()) return;

    gJumpNormal = LoadSound(kNormalJumpPath);
    gJumpOneUse = LoadSound(kOneUseJumpPath);
    gJumpBoost  = LoadSound(kBoostJumpPath);

    gAudioReady = IsSoundValid(gJumpNormal) && IsSoundValid(gJumpOneUse) && IsSoundValid(gJumpBoost);

    if (!gAudioReady) {
        if (IsSoundValid(gJumpNormal)) UnloadSound(gJumpNormal);
        if (IsSoundValid(gJumpOneUse)) UnloadSound(gJumpOneUse);
        if (IsSoundValid(gJumpBoost)) UnloadSound(gJumpBoost);
        gJumpNormal = {};
        gJumpOneUse = {};
        gJumpBoost = {};
    }
}

void ShutdownAudioManager() {
    if (gAudioReady) {
        UnloadSound(gJumpNormal);
        UnloadSound(gJumpOneUse);
        UnloadSound(gJumpBoost);
        gJumpNormal = {};
        gJumpOneUse = {};
        gJumpBoost = {};
        gAudioReady = false;
    }

    if (IsAudioDeviceReady()) {
        CloseAudioDevice();
    }
}

void PlayJumpSound(TipoPlataforma tipo) {
    if (!gAudioReady) return;

    StopAllJumpSounds();

    switch (tipo) {
        case NORMAL:
            PlaySound(gJumpNormal);
            break;
        case MOVIL_BOOST:
            PlaySound(gJumpBoost);
            break;
        case UN_USO:
            PlaySound(gJumpOneUse);
            break;
        default:
            break;
    }
}
