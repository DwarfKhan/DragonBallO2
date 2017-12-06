#include "SDLInit.h"
#include "Camera.h"
#include <iostream>

#define MILLI_PER_SEC 1000.f

#define BG_R 0x68
#define BG_G 0xB1
#define BG_B 0x38
#define BG_A 0xFF

extern Camera gCamera;

//Also camera dimension...
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

//Music and Sound loading...
Mix_Music *BGMusic1 = NULL;
Mix_Chunk *sfxSlash01 = NULL;


//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//In milliseconds...
float gDeltaTime = 0.f;


void PlayBGMusic(Mix_Music *bgmName);
void StopBGMusic();

//used for true random numbers
float gRandomizer;

//Checked in main...
bool gQuitGame = false;

//Keys held down...
int gHorizKeysHeld = 0;	//keys a and d
int gVertKeysHeld = 0;	//keys w and s

//Keys pressed...
bool gFirstKeyDown = false;		//keys 1
bool gSecondKeyDown = false;	//keys 2
bool gThirdKeyDown = false;		//keys 3
bool gFourthKeyDown = false;	//keys 4

//Keys released...
bool gFirstKeyUp = false;		//keys 1
bool gSecondKeyUp = false;	//keys 2
bool gThirdKeyUp = false;		//keys 3
bool gFourthKeyUp = false;	//keys 4

namespace {
	SDL_Event event;
}

void SDLInit::HandleKeyboardEvents() {
	//Reset pressed keys here...
	gFirstKeyDown = false;
	gSecondKeyDown = false;
	gThirdKeyDown = false;
	gFourthKeyDown = false;

	//Reset released keys here...
	gFirstKeyUp = false;
	gSecondKeyUp = false;
	gThirdKeyUp = false;
	gFourthKeyUp = false;

	/* Poll for events */
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			/* Keyboard event */
			/* Pass the event data onto PrintKeyInfo() */
		case SDL_KEYDOWN:
			if (event.key.repeat == 0) {
				switch (event.key.keysym.sym) {
				//Keys held down...
				case SDLK_w: gVertKeysHeld -= 1; break;		//up...
				case SDLK_s: gVertKeysHeld += 1; break;		//down...
				case SDLK_a: gHorizKeysHeld -= 1; break;	//left...
				case SDLK_d: gHorizKeysHeld += 1; break;	//right...
				//Keys pressed...
				case SDLK_e: gFirstKeyDown = true; break;
				case SDLK_2: gSecondKeyDown = true; break;
				case SDLK_3: gThirdKeyDown = true; break;
				case SDLK_4: gFourthKeyDown = true; break;
				//case SDLK_9: Mix_PlayMusic(mMusic, 0); break;	//Mix_PlayChannel(-1, mSound, 0) <- takes a Mix_Chunk
				default: break;
				}
			}
			break;

		case SDL_KEYUP:
			if (event.key.repeat == 0) {
				switch (event.key.keysym.sym) {
				//Keys held down...
				case SDLK_w: gVertKeysHeld += 1; break;		//up...
				case SDLK_s: gVertKeysHeld -= 1; break;		//down...
				case SDLK_a: gHorizKeysHeld += 1; break;	//left...
				case SDLK_d: gHorizKeysHeld -= 1; break;	//right...
				//Keys released...
				case SDLK_e: gFirstKeyUp = true; break;
				case SDLK_2: gSecondKeyUp = true; break;
				case SDLK_3: gThirdKeyUp = true; break;
				case SDLK_4: gFourthKeyUp = true; break;
				default: break;
				}
			}

			break;

			/* SDL_QUIT event (window close) */
		case SDL_QUIT:
			gQuitGame = true;
			break;

		default:
			break;
		}
	}
}

bool SDLInit::Setup() {
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window				//TODO: Make this name global...
		gWindow = SDL_CreateWindow("Dragon Ball O!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL) {
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL) {
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else {
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags)) {
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}


				//Initialize SDL_mixer	//TODO: here
				if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
				{
					printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
					success = false;
				}
				//If there is no music playing
				else {
					// define sounds here...
					BGMusic1 = Mix_LoadMUS("sounds/MC_Link_Sword.wav");
					sfxSlash01 = Mix_LoadWAV("sounds/MC_Link_Sword.wav");
					sfxWalk01 = Mix_LoadWAV("sounds/walk.wav");
					sfxAlert01 = Mix_LoadWAV("sounds/alert.wav");
					sfxDeath01 = Mix_LoadWAV("sounds/death.wav");
					sfxDamage01 = Mix_LoadWAV("sounds/damage.wav");
					Mix_VolumeChunk(sfxSlash01, MIX_MAX_VOLUME);
					SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
				}

			}
		}
	}

	//	SDLInit::loadMedia();
	return success;
}

void SDLInit::LoadTexture(Sprite &sprite) {
	//This is how we get our file name...
	const char* filePath = sprite.mTexturePath;

	//The final texture
	SDL_Texture* newTexture = NULL;
	
	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(filePath);
	if (loadedSurface == NULL) {
		printf("Unable to load image %s! SDL_image Error: %s\n", filePath, IMG_GetError());
	}
	else {
		//Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL) {
			printf("Unable to create texture from %s! SDL Error: %s\n", filePath, SDL_GetError());
		}
		else {	//get image dimensions. Can call Entity.SetSize to override...
			sprite.mSize.x = loadedSurface->w;
			sprite.mSize.y = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	sprite.mTexture = newTexture;
}

void SDLInit::SetColor(Sprite &sprite, Uint8 red, Uint8 green, Uint8 blue) {
	//Modulate texture rgb
	SDL_SetTextureColorMod(sprite.mTexture, red, green, blue);
}

void SDLInit::SetBlendMode(Sprite &sprite, SDL_BlendMode blending) {
	//Set blending function
	SDL_SetTextureBlendMode(sprite.mTexture, blending);
}

void SDLInit::SetAlpha(Sprite &sprite, Uint8 alpha) {
	//Modulate texture alpha
	SDL_SetTextureAlphaMod(sprite.mTexture, alpha);
}

void SDLInit::CleanupSprite(Sprite &sprite) {
	SDL_DestroyTexture(sprite.mTexture);
	sprite.mTexture = NULL;
}

void SDLInit::DrawSprite(Sprite &sprite) {
	//Set rendering space and render to screen
	SDL_Rect renderRect = {
		int(sprite.mPos.x - gCamera.mPos.x),
		int(sprite.mPos.y - gCamera.mPos.y),
		sprite.mSize.x, sprite.mSize.y 
	};

	auto *anchorOffset = sprite.GetAnchorOffset();

	//If the sprite size changes, the sprite will move. This offset
	//	is for anchoring the sprite, so that it doesn't move.
	if (anchorOffset != NULL) {
		renderRect.x += anchorOffset->x;
		renderRect.y += anchorOffset->y;
	}

	//Render to screen
	SDL_RenderCopy(gRenderer, sprite.mTexture,
		sprite.GetSpriteClip(), &renderRect);
}

void SDLInit::DrawEntityCollider(Entity &entity) {
	if (entity.mHasCollided) {
		SDL_SetRenderDrawColor(gRenderer, 255, 0, 32, 48);
		//TODO: Not the best place to put this, but works...
		entity.mHasCollided = false;
		entity.mPushbackSides = 0;
	}
	else {
		SDL_SetRenderDrawColor(gRenderer, 32, 0, 255, 48);
	}
	
	SDL_Rect rectangle;

	rectangle.x = int(entity.mPos.x + entity.mTopLeftCollOffset.x - gCamera.mPos.x);
	rectangle.y = int(entity.mPos.y + entity.mTopLeftCollOffset.y - gCamera.mPos.y);
	rectangle.w = int(entity.mSize.x - entity.mTopLeftCollOffset.x - entity.mBottomRightCollOffset.x);
	rectangle.h = int(entity.mSize.y - entity.mTopLeftCollOffset.y - entity.mBottomRightCollOffset.y);
	SDL_RenderFillRect(gRenderer, &rectangle);
}


void SDLInit::Render() {
	//Clear screen	68B13A
	SDL_SetRenderDrawColor(gRenderer, BG_R, BG_G, BG_B, BG_A);
	SDL_RenderClear(gRenderer);
}

//TODO: add delta time to update...
void SDLInit::Update(){
	//Updating gDeltaTime in milliseconds...
	static Uint32 lastTime = 0;
	Uint32 runningTime = SDL_GetTicks();
	gDeltaTime = (runningTime - lastTime)/MILLI_PER_SEC;
	lastTime = runningTime;

	//Checks for key presses...
	HandleKeyboardEvents();

	//Update screen
	SDL_RenderPresent(gRenderer);

	//Wait two seconds
	//SDL_Delay( 2000 );
}

void SDLInit::Cleanup() {

	//Free the sound effects
	Mix_FreeMusic(BGMusic1);
	Mix_FreeChunk(sfxSlash01); // can name the sfx1 to linksSword upper 
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);

	gWindow = NULL;
	gRenderer = NULL;

	sfxSlash01 = nullptr;
	BGMusic1 = nullptr;

	//Quit SDL subsystems
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}


//Plays Audio BackGround Music...
void PlayBGMusic(Mix_Music *bgmName) {
	//if there is no music...play music..
	if (!Mix_PlayingMusic()) {
		Mix_VolumeMusic(MIX_MAX_VOLUME / 3);
		Mix_PlayMusic(bgmName, -1);
	}
	//if music is paused... play music..
	else if (Mix_PausedMusic()) {
		Mix_VolumeMusic(MIX_MAX_VOLUME / 3);
		Mix_ResumeMusic();
	}
	//if music is playing... pause music...
	else
		Mix_PauseMusic();
}

void StopBGMusic() {
	//Stops Music
	Mix_HaltMusic();
}

//Plays Audio Sound Effects...
void SDLInit::PlaySFX(Mix_Chunk *sfxName, int volume) {
	if (sfxName == nullptr) {
		printf("nullptr sfx, playing no sound...\n");
		return;
	}

	Mix_VolumeChunk(sfxName, volume);
	Mix_PlayChannel(-1, sfxName, 0);

}