//Using SDL and standard IO
#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <SDL_mixer.h>
#include "Sprite.h"

class SDLInit{

	public:
		bool Setup();

		void LoadTexture(Sprite &sprite);

		void SetColor(Sprite &sprite, Uint8 red, Uint8 green, Uint8 blue);
		void SetBlendMode(Sprite &sprite, SDL_BlendMode blending);
		void SetAlpha(Sprite &sprite, Uint8 alpha);

		void DrawSprite(Sprite &sprite);
		void DrawEntityCollider(Entity &entity);

		void Render();
		void Update();

		void CleanupSprite(Sprite &sprite);
		void Cleanup();

		void PlaySFX(Mix_Chunk *sfxName, int volume = 128);

		Mix_Chunk *sfxSlash01;
		Mix_Chunk *sfxWalk01;
		Mix_Chunk *sfxAlert01;
		Mix_Chunk *sfxDamage01;
		Mix_Chunk *sfxDeath01;

		bool showColliders = false;

	private:
		SDL_Rect *mDstRect = NULL;
		SDL_Rect *mSrcRect = NULL;

		void HandleKeyboardEvents();
		Mix_Chunk *mSound = NULL;
		Mix_Music *mMusic = NULL;
};