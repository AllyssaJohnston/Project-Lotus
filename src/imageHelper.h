#pragma once
#include <string>
#include <SDL3/SDL.h>
#include <vector>
#include "helperClass.h"
#include "ImageHelperClass.h"

class ImageObject
{
private:
	SDL_Surface* mpSurface = nullptr;
	SDL_Texture* mpTexture = nullptr;
	
public:
	EDirection   mChunkDirection = EDirection_INVALID;
	int          mNumChunks   = 1;
	int			 mIdealImageWidth = -1;
	int			 mIdealImageHeight = -1;
	float        mImageRatioUsed = -1.0f;
	int			 mImageOffsetX = 0;
	int          mImageOffsetY = 0;

	ImageObject() { ; }
	ImageObject(const std::string& fileName, int maxWidth, int maxHeight, EHowToDetermineWidthHeight howToDetermineWidthHeight);
	ImageObject(const std::string& fileName, float imageRatio);

	~ImageObject();

	void setupImageObject(const std::string& fileName, int maxWidth, int maxHeight, EHowToDetermineWidthHeight howToDetermineWidthHeight);
	void setupImageObject(const std::string& fileName, float imageRatio);

	SDL_Surface* getSurface() const;

	float getHalfIdealImageWidth() const;
	float getHalfIdealImageHeight() const;

	float getImageRatioUsed() const;

	void setTexture(SDL_Texture* texture);
	void setUpTexture(SDL_Renderer* pRenderer);
	SDL_Texture* getTexture() const;

	void setImageOffset(int imageOffsetX, int imageOffsetY);
};


struct ImageAsset
{
	SDL_Surface* mSurface = nullptr;
	SDL_Texture* mTexture = nullptr;
	std::string mFileName;

	~ImageAsset();
};

class AssetManager
{
private:
	static std::list<ImageAsset> mImageAssets;

public:
	static SDL_Surface* getSurfaceFromFile(const std::string& fileName);
	static SDL_Texture* getTextureFromSurface(SDL_Renderer* pRenderer, const SDL_Surface* surface);
};