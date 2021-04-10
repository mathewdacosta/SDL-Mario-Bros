﻿#pragma once

/* ================ Group toggles ================= */

// #define DEBUG_DRAW_ALL_OVERLAYS
// #define DEBUG_AUDIO_DISABLE_ALL



/* =============== Visual overlays ================ */

// #define DEBUG_DRAW_TILES
// #define DEBUG_DRAW_CHARACTER_BOXES
// #define DEBUG_DRAW_KOOPA_BASE



/* ================ Audio toggles ================= */

#define DEBUG_AUDIO_DISABLE_MUSIC
// #define DEBUG_AUDIO_DISABLE_SFX



/* =========== ifdefs for group toggles =========== */

#ifdef DEBUG_DRAW_ALL_OVERLAYS
	#define DEBUG_DRAW_TILES
	#define DEBUG_DRAW_CHARACTER_BOXES
	#define DEBUG_DRAW_KOOPA_BASE
#endif

#ifdef DEBUG_AUDIO_DISABLE_ALL
    #define DEBUG_AUDIO_DISABLE_MUSIC
    #define DEBUG_AUDIO_DISABLE_SFX
#endif
