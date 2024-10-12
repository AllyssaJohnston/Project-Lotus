#include <cmath>
#include "imageHelper.h"

std::list<ImageAsset> AssetManager::mImageAssets;

ImageObject::ImageObject(){}

ImageObject::ImageObject(std::string fileName, int maxWidth, int maxHeight, EHowToDetermineWidthHeight howToDetermineWidthHeight)
{
	setupImageObject(fileName, maxWidth, maxHeight, howToDetermineWidthHeight);
}

ImageObject::ImageObject(std::string fileName, float imageRatio)
{
	setupImageObject(fileName, imageRatio);
}

ImageObject::~ImageObject()
{
	mpSurface = nullptr;
	mpTexture = nullptr;
}

void ImageObject::setupImageObject(std::string fileName, int maxWidth, int maxHeight, EHowToDetermineWidthHeight howToDetermineWidthHeight)
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
			mNumChunks = std::ceil(float(maxHeight) / float(mIdealImageHeight));
			mChunkDirection = EDirection_DOWN;
		}
		else
		{
			mNumChunks = std::ceil(float(maxWidth) / float(mIdealImageWidth));
			mChunkDirection = EDirection_RIGHT;
		}
	}
	else
	{
		SDL_assert(false);
	}


}

void ImageObject::setupImageObject(std::string fileName, float imageRatio)
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

SDL_Surface* ImageObject::getSurface()
{
	return mpSurface;
}

int ImageObject::getIdealImageWidth() const
{
	return mIdealImageWidth;
}

float ImageObject::getHalfIdealImageWidth() const
{
	return float(mIdealImageWidth) / 2;
}

int ImageObject::getIdealImageHeight() const
{
	return mIdealImageHeight;
}

float ImageObject::getHalfIdealImageHeight() const
{
	return float(mIdealImageHeight) / 2;
}

float ImageObject::getImageRatioUsed() const
{
	return mImageRatioUsed;
}

int ImageObject::getImageOffsetX() const
{
	return mImageOffsetX;
}

int ImageObject::getImageOffsetY() const
{
	return mImageOffsetY;
}

void ImageObject::setTexture(SDL_Texture* texture)
{
	mpTexture = texture;
}

void ImageObject::setUpTexture(SDL_Renderer* pRenderer)
{
	setTexture(AssetManager::getTextureFromSurface(pRenderer, mpSurface));
}

SDL_Texture* ImageObject::getTexture()
{
	return mpTexture;
}

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



Animation::Animation(EAnimationType animationType, std::vector <Outfit*> outfits, int curFrameNumber, int frameRate, bool mustFinish)
{
	mAnimationType = animationType;
	mpOutfits = outfits;
	mCurFrameNumber = curFrameNumber;
	mFrameRate = frameRate;
	mCountDown = frameRate;
	mMustFinish = mustFinish;
}

Animation::~Animation()
{
	for (Outfit* outfit : mpOutfits)
	{
		delete outfit;
	}
}

void Animation::resetCountDown()
{
	mCountDown = mFrameRate;
}

int Animation::setReturnNextAnimationFrameNumber()
{
	if (mCurFrameNumber == -1 or mCurFrameNumber == mpOutfits[0]->mpFrames.size() - 1)
	{
		mCurFrameNumber = 0;
	}
	else
	{
		mCurFrameNumber += 1;
	}
	return mCurFrameNumber;
}



AnimationPreset::AnimationPreset(EAnimationType type, std::vector <std::vector <std::string>> outfitsWithFrameNames, int maxImageWidth, int maxImageHeight, int imageOffsetX, int imageOffsetY, int frameRate, bool mustFinish)
{
	mType = type;
	mOutfitsWithFrameNames = outfitsWithFrameNames;
	mFrameRate = frameRate;
	mMustFinish = mustFinish;

	mMaxImageWidth  = maxImageWidth;
	mMaxImageHeight = maxImageHeight;

	mImageOffsetX = imageOffsetX;
	mImageOffsetY = imageOffsetY;
}

AnimationPreset::AnimationPreset(EAnimationType type, std::vector <std::vector <std::string>> outfitsWithFrameNames, int imageOffsetX, int imageOffsetY, int frameRate, bool mustFinish) 
		: AnimationPreset(type, outfitsWithFrameNames, INT_MAX, INT_MAX, imageOffsetX, imageOffsetY, frameRate, mustFinish){}

AnimationPreset::AnimationPreset(EAnimationType type, std::vector <std::string> frameNames, int maxImageWidth, int maxImageHeight, int imageOffsetX, int imageOffsetY, int frameRate, bool mustFinish)
		: AnimationPreset(type, std::vector<std::vector<std::string>>{frameNames}, maxImageWidth, maxImageHeight, imageOffsetX, imageOffsetY, frameRate, mustFinish) { }

AnimationPreset::AnimationPreset(EAnimationType type, std::vector <std::string> frameNames, int imageOffsetX, int imageOffsetY, int frameRate, bool mustFinish)
		: AnimationPreset(type, std::vector<std::vector<std::string>>{frameNames}, INT_MAX, INT_MAX, imageOffsetX, imageOffsetY, frameRate, mustFinish){}



AnimationManager::AnimationManager()
{
	mpCurAnimation = nullptr;
	mpCurImage = nullptr;
}

AnimationManager::~AnimationManager()
{
	for (Animation* animation : mpAnimations)
	{
		delete animation;
	}
	mpCurAnimation	= nullptr;
	mpCurImage		= nullptr;
}

void AnimationManager::setupAnimationManager(const std::vector <AnimationPreset>& animationPresets, EHowToDetermineWidthHeight howToDetermineWidthHeight)
{
	for (int count = 0; count < animationPresets.size(); count++)
	{
		setUpAnimation(animationPresets[count], howToDetermineWidthHeight);
	}
	mpCurAnimation = mpAnimations[0];
	mpCurImage     = mpCurAnimation->mpOutfits[mCurOutfitIndex]->mpFrames[0];
}

void AnimationManager::setupAnimationManager(const std::vector <AnimationPreset>& animationPresets, int maxImageWidth, int maxImageHeight, EHowToDetermineWidthHeight howToDetermineWidthHeight)
{
	for (int count = 0; count < animationPresets.size(); count++)
	{
		setUpAnimation(animationPresets[count], maxImageWidth, maxImageHeight, howToDetermineWidthHeight);
	}
	mpCurAnimation = mpAnimations[0];
	mpCurImage     = mpCurAnimation->mpOutfits[mCurOutfitIndex]->mpFrames[0];
}

void AnimationManager::setUpAnimation(const AnimationPreset& animationPreset, EHowToDetermineWidthHeight howToDetermineWidthHeight)
{
	setUpAnimation(animationPreset, animationPreset.mMaxImageWidth, animationPreset.mMaxImageHeight, howToDetermineWidthHeight);
}

void AnimationManager::setUpAnimation(const AnimationPreset& animationPreset, int maxImageWidth, int maxImageHeight, EHowToDetermineWidthHeight howToDetermineWidthHeight)
{
	std::vector <Outfit*> outfits;

	if (howToDetermineWidthHeight == EHowToDetermineWidthHeight_GET_BEST_IMAGE_RATIO)
	{
		float imageRatio = -1;
		for (int countOutfit = 0; countOutfit < (int)animationPreset.mOutfitsWithFrameNames.size(); countOutfit++)
		{
			Outfit* curOutfit = new Outfit();
			for (int countFrame = 0; countFrame < (int)animationPreset.mOutfitsWithFrameNames[countOutfit].size(); countFrame++)
			{
				ImageObject* curImageObject;
				if (countFrame == 0)
				{
					curImageObject = new ImageObject(animationPreset.mOutfitsWithFrameNames[countOutfit][countFrame], maxImageWidth, maxImageHeight, howToDetermineWidthHeight);
					imageRatio = curImageObject->getImageRatioUsed();
				}
				else
				{
					curImageObject = new ImageObject(animationPreset.mOutfitsWithFrameNames[countOutfit][countFrame], imageRatio);
				}
				curImageObject->setImageOffset(animationPreset.mImageOffsetX, animationPreset.mImageOffsetY);
				curOutfit->mpFrames.push_back(curImageObject);
				curImageObject = nullptr;
			}
			outfits.push_back(curOutfit);
			curOutfit = nullptr;
		}
	}
	else if (howToDetermineWidthHeight == EHowToDetermineWidthHeight_SPLICE)
	{
		//splice
		EDirection spliceDirection = EDirection_RIGHT;;
		int numSplices = 1;

		int idealImageWidth;
		int idealImageHeight;

		std::string filePath =  "./ArtFiles/" + animationPreset.mOutfitsWithFrameNames[0][0];
		SDL_Surface* pSurface = AssetManager::getSurfaceFromFile(filePath);
		float imageRatio = getBestImageRatio(pSurface, maxImageWidth, maxImageHeight);

		//is entire image width greater than height
		if (maxImageWidth > maxImageHeight)
		{
			//splice horizontally
			int imageWidth   = int(pSurface->w  / imageRatio);
			numSplices	     = floor(maxImageWidth / imageWidth);
			EDirection spliceDirection = EDirection_RIGHT;
			int gap			 = maxImageWidth - (numSplices * imageWidth);
			idealImageWidth  = imageWidth + (gap / numSplices);
			idealImageHeight = int(pSurface->h / imageRatio);

		}
		else
		{
			//splice vertically
			int imageHeight  = int(pSurface->h / imageRatio);
			numSplices		 = floor(maxImageHeight / imageHeight);
			EDirection spliceDirection = EDirection_DOWN;
			int gap			 = maxImageHeight - (numSplices * imageHeight);
			idealImageHeight = imageHeight + (gap / numSplices);
			idealImageWidth  = int(pSurface->w / imageRatio);
		}
		for (int countOutfit = 0; countOutfit < (int)animationPreset.mOutfitsWithFrameNames.size(); countOutfit++)
		{
			Outfit* curOutfit = new Outfit();
			for (int countFrame = 0; countFrame < (int)animationPreset.mOutfitsWithFrameNames[countOutfit].size(); countFrame++)
			{
				ImageObject* curImageObject = new ImageObject(animationPreset.mOutfitsWithFrameNames[countOutfit][countFrame], idealImageWidth, idealImageHeight, EHowToDetermineWidthHeight_USE_WIDTH_AND_HEIGHT_INPUT);
				curImageObject->mNumChunks		= numSplices;
				curImageObject->mChunkDirection	= spliceDirection;
				curImageObject->setImageOffset(animationPreset.mImageOffsetX, animationPreset.mImageOffsetY);
				curOutfit->mpFrames.push_back(curImageObject);
				curImageObject = nullptr;
			}
			outfits.push_back(curOutfit);
			curOutfit = nullptr;
		}
	}
	else
	{
		for (int countOutfit = 0; countOutfit < (int)animationPreset.mOutfitsWithFrameNames.size(); countOutfit++)
		{
			Outfit* curOutfit = new Outfit();
			for (int countFrame = 0; countFrame < (int)animationPreset.mOutfitsWithFrameNames[countOutfit].size(); countFrame++)
			{
				ImageObject* curImageObject = new ImageObject(animationPreset.mOutfitsWithFrameNames[countOutfit][countFrame], maxImageWidth, maxImageHeight, howToDetermineWidthHeight);
				curOutfit->mpFrames.push_back(curImageObject);
				curImageObject = nullptr;
			}
			outfits.push_back(curOutfit);
			curOutfit = nullptr;
		}
	}

	//int animationType, std::vector <ImageObject*>& frames, int curFrameNumber, int frameRate, bool mustFinish
	int curFrameNumber = 0;
	mpAnimations.push_back(new Animation(animationPreset.mType, outfits, curFrameNumber, animationPreset.mFrameRate, animationPreset.mMustFinish));
	mpCurImage = outfits[0]->mpFrames[0];
}

void AnimationManager::setUpAllTextures(SDL_Renderer* pRenderer)
{
	for (int countAnimation = 0; countAnimation < mpAnimations.size(); countAnimation++)
	{
		for (int countOutfit = 0; countOutfit < mpAnimations[countAnimation]->mpOutfits.size(); countOutfit++)
		{
			for (int countFrame = 0; countFrame < mpAnimations[countAnimation]->mpOutfits[countOutfit]->mpFrames.size(); countFrame++)
			{
				mpAnimations[countAnimation]->mpOutfits[countOutfit]->mpFrames[countFrame]->setTexture(
						AssetManager::getTextureFromSurface(pRenderer, mpAnimations[countAnimation]->mpOutfits[countOutfit]->mpFrames[countFrame]->getSurface()));
			}
		}
	}
}

void AnimationManager::updateAnimation(EAnimationType animationType)
{
	if (haveAnimation(animationType))
	{
		if (mpCurAnimation->mAnimationType != animationType and (mpCurAnimation->mMustFinish == false or isCurAnimationFinished()))
		{
			changeAnimation(animationType);
		}
		if (mpCurAnimation->mCountDown == 0)
		{
			mpCurImage = mpCurAnimation->mpOutfits[mCurOutfitIndex]->mpFrames[mpCurAnimation->setReturnNextAnimationFrameNumber()];
			mpCurAnimation->resetCountDown();
		}
		else
		{
			mpCurAnimation->mCountDown -= 1;
		}
	}
}

bool AnimationManager::haveAnimation(EAnimationType animationType)
{
	for (int count = 0; count < mpAnimations.size(); count++)
	{

		if (mpAnimations[count]->mAnimationType == animationType)
		{
			return true;
		}
	}
	return false;
}

void AnimationManager::changeAnimation(EAnimationType animationType)
{
	for (int count = 0; count < mpAnimations.size(); count++)
	{
		restartCurAnimation();

		if (mpAnimations[count]->mAnimationType == animationType)
		{
			mpCurAnimation = mpAnimations[count];
			mpCurImage = mpCurAnimation->mpOutfits[mCurOutfitIndex]->mpFrames[0];
		}
	}
}

void AnimationManager::changeOutfit(int outfitNum)
{
	if (outfitNum > -1 and outfitNum < (int)mpCurAnimation->mpOutfits.size())
	{
		mCurOutfitIndex = outfitNum;
		mpCurImage = mpCurAnimation->mpOutfits[mCurOutfitIndex]->mpFrames[mpCurAnimation->mCurFrameNumber];
	}
	else
	{
		SDL_assert(false);
	}
}

void AnimationManager::restartCurAnimation()
{
	mpCurAnimation->mCurFrameNumber = 0;
	mpCurAnimation->resetCountDown();
	mpCurImage = mpCurAnimation->mpOutfits[mCurOutfitIndex]->mpFrames[0];
}

void AnimationManager::reset()
{
	changeAnimation(mpAnimations[0]->mAnimationType);
}

bool AnimationManager::isCurAnimationFinished()
{
	if (mpCurAnimation->mCurFrameNumber == mpCurAnimation->mpOutfits[mCurOutfitIndex]->mpFrames.size() - 1 and mpCurAnimation->mCountDown == 0)
	{
		return true;
	}
	return false;
}

Animation* AnimationManager::getCurAnimation()
{
	return mpCurAnimation;
}

std::vector<ImageObject*> AnimationManager::getCurFrames()
{
	return mpCurAnimation->mpOutfits[mCurOutfitIndex]->mpFrames;
}

ImageObject* AnimationManager::getCurImage()
{
	return mpCurImage;
}