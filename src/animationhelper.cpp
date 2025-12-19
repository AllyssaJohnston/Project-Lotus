#include "animationHelper.h"


Outfit::~Outfit()
{
	for (ImageObject* frame : mpFrames)
	{
		delete frame;
	}
}


Animation::Animation(EAnimationType animationType, std::vector <Outfit*>& outfits, int curFrameNumber, int frameRate, bool mustFinish)
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

void Animation::resetCountDown() { mCountDown = mFrameRate; }

void Animation::setNextAnimationFrameNumber()
{
	if (mCurFrameNumber == -1 or mCurFrameNumber == mpOutfits[0]->mpFrames.size() - 1)
	{
		mCurFrameNumber = 0;
	}
	else
	{
		mCurFrameNumber += 1;
	}
}

int Animation::getNextAnimationFrameNumber() { return mCurFrameNumber; }



AnimationPreset::AnimationPreset(EAnimationType type, std::vector <std::vector <std::string>>& outfitsWithFrameNames, int maxImageWidth, int maxImageHeight, int imageOffsetX, int imageOffsetY, int frameRate, bool mustFinish)
		{ setUpByRef(type, outfitsWithFrameNames, maxImageWidth, maxImageHeight, imageOffsetX, imageOffsetY, frameRate, mustFinish); }

AnimationPreset::AnimationPreset(EAnimationType type, std::vector <std::vector <std::string>>& outfitsWithFrameNames, int imageOffsetX, int imageOffsetY, int frameRate, bool mustFinish)
		{ setUpByRef(type, outfitsWithFrameNames, INT_MAX, INT_MAX, imageOffsetX, imageOffsetY, frameRate, mustFinish); }

AnimationPreset::AnimationPreset(EAnimationType type, std::vector <std::string>& frameNames, int maxImageWidth, int maxImageHeight, int imageOffsetX, int imageOffsetY, int frameRate, bool mustFinish)
		{ setUpByVal(type, std::vector<std::vector<std::string>>{frameNames}, maxImageWidth, maxImageHeight, imageOffsetX, imageOffsetY, frameRate, mustFinish); }

AnimationPreset::AnimationPreset(EAnimationType type, std::vector <std::string>& frameNames, int imageOffsetX, int imageOffsetY, int frameRate, bool mustFinish)
		{ setUpByVal(type, std::vector<std::vector<std::string>>{frameNames}, INT_MAX, INT_MAX, imageOffsetX, imageOffsetY, frameRate, mustFinish); }

void AnimationPreset::setUpByRef(EAnimationType type, std::vector <std::vector <std::string>>& outfitsWithFrameNames, int maxImageWidth, int maxImageHeight, int imageOffsetX, int imageOffsetY, int frameRate, bool mustFinish)
{
	mType = type;
	mOutfitsWithFrameNames = outfitsWithFrameNames;
	mFrameRate = frameRate;
	mMustFinish = mustFinish;

	mMaxImageWidth = maxImageWidth;
	mMaxImageHeight = maxImageHeight;

	mImageOffsetX = imageOffsetX;
	mImageOffsetY = imageOffsetY;
}

void AnimationPreset::setUpByVal(EAnimationType type, std::vector <std::vector <std::string>> outfitsWithFrameNames, int maxImageWidth, int maxImageHeight, int imageOffsetX, int imageOffsetY, int frameRate, bool mustFinish) 
{
	setUpByRef(type, outfitsWithFrameNames, maxImageWidth, maxImageHeight, imageOffsetX, imageOffsetY, frameRate, mustFinish);
}



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
	mpCurAnimation = nullptr;
	mpCurImage = nullptr;
}

void AnimationManager::setupAnimationManager(const std::vector <AnimationPreset>& animationPresets, EHowToDetermineWidthHeight howToDetermineWidthHeight)
{
	for (int count = 0; count < animationPresets.size(); count++)
	{
		setUpAnimation(animationPresets[count], howToDetermineWidthHeight);
	}
	mpCurAnimation = mpAnimations[0];
	mpCurImage = mpCurAnimation->mpOutfits[mCurOutfitIndex]->mpFrames[0];
}

void AnimationManager::setupAnimationManager(const std::vector <AnimationPreset>& animationPresets, int maxImageWidth, int maxImageHeight, EHowToDetermineWidthHeight howToDetermineWidthHeight)
{
	for (int count = 0; count < animationPresets.size(); count++)
	{
		setUpAnimation(animationPresets[count], maxImageWidth, maxImageHeight, howToDetermineWidthHeight);
	}
	mpCurAnimation = mpAnimations[0];
	mpCurImage = mpCurAnimation->mpOutfits[mCurOutfitIndex]->mpFrames[0];
}

void AnimationManager::setUpAnimation(const AnimationPreset& animationPreset, EHowToDetermineWidthHeight howToDetermineWidthHeight)
		{ setUpAnimation(animationPreset, animationPreset.mMaxImageWidth, animationPreset.mMaxImageHeight, howToDetermineWidthHeight); }

void AnimationManager::setUpAnimation(const AnimationPreset& animationPreset, int maxImageWidth, int maxImageHeight, EHowToDetermineWidthHeight howToDetermineWidthHeight)
{
	std::vector <Outfit*> outfits;
	float imageRatio = -1;
	SDL_Surface* pSurface = nullptr;

	switch (howToDetermineWidthHeight)
	{
	case EHowToDetermineWidthHeight_GET_BEST_IMAGE_RATIO:
		for (int countOutfit = 0; countOutfit < (int)animationPreset.mOutfitsWithFrameNames.size(); countOutfit++)
		{
			Outfit* pCurOutfit = new Outfit();
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
				pCurOutfit->mpFrames.push_back(curImageObject);
				curImageObject = nullptr;
			}
			outfits.push_back(pCurOutfit);
			pCurOutfit = nullptr;
		}
		break;

	case EHowToDetermineWidthHeight_SPLICE:
		EDirection spliceDirection;
		int numSplices;

		int idealImageWidth;
		int idealImageHeight;

		pSurface = AssetManager::getSurfaceFromFile("./ArtFiles/" + animationPreset.mOutfitsWithFrameNames[0][0]);
		imageRatio = getBestImageRatio(pSurface, maxImageWidth, maxImageHeight);

		// is entire image width greater than height
		if (maxImageWidth > maxImageHeight)
		{
			// splice horizontally
			int imageWidth = int(pSurface->w / imageRatio);
			numSplices = (int)floor((float)maxImageWidth / (float)imageWidth);
			spliceDirection = EDirection_RIGHT;
			int gap = maxImageWidth - (numSplices * imageWidth);
			idealImageWidth = imageWidth + (gap / numSplices);
			idealImageHeight = int(pSurface->h / imageRatio);

		}
		else
		{
			// splice vertically
			int imageHeight = int(pSurface->h / imageRatio);
			numSplices = (int)floor((float)maxImageHeight / (float)imageHeight);
			spliceDirection = EDirection_DOWN;
			int gap = maxImageHeight - (numSplices * imageHeight);
			idealImageHeight = imageHeight + (gap / numSplices);
			idealImageWidth = int(pSurface->w / imageRatio);
		}

		for (int countOutfit = 0; countOutfit < (int)animationPreset.mOutfitsWithFrameNames.size(); countOutfit++)
		{
			Outfit* curOutfit = new Outfit();
			for (int countFrame = 0; countFrame < (int)animationPreset.mOutfitsWithFrameNames[countOutfit].size(); countFrame++)
			{
				ImageObject* curImageObject = new ImageObject(animationPreset.mOutfitsWithFrameNames[countOutfit][countFrame], idealImageWidth, idealImageHeight, EHowToDetermineWidthHeight_USE_WIDTH_AND_HEIGHT_INPUT);
				curImageObject->mNumChunks = numSplices;
				curImageObject->mChunkDirection = spliceDirection;
				curImageObject->setImageOffset(animationPreset.mImageOffsetX, animationPreset.mImageOffsetY);
				curOutfit->mpFrames.push_back(curImageObject);
				curImageObject = nullptr;
			}
			outfits.push_back(curOutfit);
			curOutfit = nullptr;
		}
		break;

	default:
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
		break;
	}
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
			mpCurAnimation->setNextAnimationFrameNumber();
			mpCurImage = mpCurAnimation->mpOutfits[mCurOutfitIndex]->mpFrames[mpCurAnimation->getNextAnimationFrameNumber()];
			mpCurAnimation->resetCountDown();
		}
		else
		{
			mpCurAnimation->mCountDown -= 1;
		}
	}
}

bool AnimationManager::haveAnimation(EAnimationType animationType) const
{
	for (Animation* pAnimation : mpAnimations)
	{
		if (pAnimation->mAnimationType == animationType)
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

bool AnimationManager::isCurAnimationFinished() const 
	{ return mpCurAnimation->mCurFrameNumber == mpCurAnimation->mpOutfits[mCurOutfitIndex]->mpFrames.size() - 1 and mpCurAnimation->mCountDown == 0; }

Animation* AnimationManager::getCurAnimation() const { return mpCurAnimation; }

std::vector<ImageObject*>& AnimationManager::getCurFrames() const { return mpCurAnimation->mpOutfits[mCurOutfitIndex]->mpFrames; }

ImageObject* AnimationManager::getCurImage() const { return mpCurImage; }