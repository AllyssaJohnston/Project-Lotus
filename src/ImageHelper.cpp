#include <cmath>
#include "imageHelper.h"

std::list<ImageAsset> AssetManager::mImageAssets;

ImageObject::ImageObject(const std::string& fileName, int maxWidth, int maxHeight, EHowToDetermineWidthHeight howToDetermineWidthHeight)
		{ setupImageObject(fileName, maxWidth, maxHeight, howToDetermineWidthHeight); }

ImageObject::ImageObject(const std::string& fileName, float imageRatio) { setupImageObject(fileName, imageRatio); }

ImageObject::~ImageObject()
{
	mpSurface = nullptr;
	mpTexture = nullptr;
}

void ImageObject::setupImageObject(const std::string& fileName, int maxWidth, int maxHeight, EHowToDetermineWidthHeight howToDetermineWidthHeight)
{
	std::string filePath =  "./ArtFiles/" + fileName;
	mpSurface = AssetManager::getSurfaceFromFile(filePath);
	if (mpSurface == NULL) 
	{
		SDL_assert(mpSurface);
	}

	if (howToDetermineWidthHeight == EHowToDetermineWidthHeight_GET_BEST_IMAGE_RATIO) 
	{
		float imageRatio = getBestImageRatio(mpSurface, maxWidth, maxHeight);
		mImageRatioUsed  = imageRatio;

		mIdealImageWidth  = int(mpSurface->w / imageRatio);
		mIdealImageHeight = int(mpSurface->h / imageRatio);
	}
	else if (howToDetermineWidthHeight == EHowToDetermineWidthHeight_USE_WIDTH_AND_HEIGHT_INPUT)
	{
		mImageRatioUsed = -1.0;
		mIdealImageWidth  = maxWidth;
		mIdealImageHeight = maxHeight;
	}
	else if (EHowToDetermineWidthHeight_CHUNK_IMAGES)
	{
		//chunk
		float imageRatio;

		if (maxWidth > maxHeight)
		{
			imageRatio = float(mpSurface->h) / float(maxHeight);
		}
		else
		{
			imageRatio = float(mpSurface->w) / float(maxWidth);
		}

		mIdealImageWidth  = int(mpSurface->w / imageRatio);
		mIdealImageHeight = int(mpSurface->h / imageRatio);

		if (maxHeight > maxWidth)
		{
			mNumChunks = (int)std::ceil(float(maxHeight) / float(mIdealImageHeight));
			mChunkDirection = EDirection_DOWN;
		}
		else
		{
			mNumChunks = (int)std::ceil(float(maxWidth) / float(mIdealImageWidth));
			mChunkDirection = EDirection_RIGHT;
		}
	}
	else
	{
		SDL_assert(false);
	}
}

void ImageObject::setupImageObject(const std::string& fileName, float imageRatio)
{
	std::string filePath =  "./ArtFiles/" + fileName;
	mpSurface = AssetManager::getSurfaceFromFile(filePath);// SDL_LoadBMP(filePath.c_str());
	if (mpSurface == NULL) 
	{
		//const char* theError = SDL_GetError();
		SDL_assert(mpSurface);
	}

	mIdealImageWidth  = int(mpSurface->w / imageRatio);
	mIdealImageHeight = int(mpSurface->h / imageRatio);
}

SDL_Surface* ImageObject::getSurface() const { return mpSurface; }

float ImageObject::getHalfIdealImageWidth() const { return float(mIdealImageWidth) / 2; }

float ImageObject::getHalfIdealImageHeight() const { return float(mIdealImageHeight) / 2; }

float ImageObject::getImageRatioUsed() const { return mImageRatioUsed; }

void ImageObject::setTexture(SDL_Texture* texture) { mpTexture = texture; }

void ImageObject::setUpTexture(SDL_Renderer* pRenderer) { setTexture(AssetManager::getTextureFromSurface(pRenderer, mpSurface)); }

SDL_Texture* ImageObject::getTexture() const { return mpTexture; }

void ImageObject::setImageOffset(int imageOffsetX, int imageOffsetY)
{
	mImageOffsetX = imageOffsetX;
	mImageOffsetY = imageOffsetY;
}


ImageAsset::~ImageAsset()
{
	mSurface = nullptr;
	mTexture = nullptr;
}


SDL_Surface* AssetManager::getSurfaceFromFile(const std::string& fileName)
{
	for (ImageAsset& iter : mImageAssets)
	{
		if (fileName == iter.mFileName)
		{	
			return iter.mSurface;;
		}
	}

	SDL_Surface* surface = SDL_LoadBMP(fileName.c_str());
	if (surface == NULL)
	{
		const char* theError = SDL_GetError();
		SDL_assert(surface);
	}
	ImageAsset imageAsset = { surface, nullptr, fileName };
	mImageAssets.push_front(imageAsset);
	return surface;
}

SDL_Texture* AssetManager::getTextureFromSurface(SDL_Renderer* pRenderer, const SDL_Surface* surface)
{
	for (ImageAsset& iter : mImageAssets)
	{
		if (surface == iter.mSurface)
		{
			//Check see if it has a texture yet
			if (iter.mTexture == nullptr)
			{
				iter.mTexture = SDL_CreateTextureFromSurface(pRenderer, iter.mSurface);
				if (iter.mTexture == nullptr)
				{
					const char* theError = SDL_GetError();
					SDL_assert(iter.mTexture);
				}
			}
			return iter.mTexture;
		}
	}

	return nullptr;
}

