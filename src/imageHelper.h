#pragma once
#include <string>
#include <SDL3/SDL.h>
#include <vector>
#include <list>
#include "helperClass.h"
#include "ImageHelperClass.h"

class ImageObject
{
	private:
		SDL_Surface* mpSurface = nullptr;
		SDL_Texture* mpTexture = nullptr;
		int			 mIdealImageWidth;
		int			 mIdealImageHeight;
		float        mImageRatioUsed;
		int			 mImageOffsetX;
		int          mImageOffsetY;

	public:
		EDirection   mChunkDirection;
		int          mNumChunks   = 1;

		ImageObject();

		ImageObject(std::string fileName, int maxWidth, int maxHeight, EHowToDetermineWidthHeight howToDetermineWidthHeight);

		ImageObject(std::string fileName, float imageRatio);

		~ImageObject();

		void setupImageObject(std::string fileName, int maxWidth, int maxHeight, EHowToDetermineWidthHeight howToDetermineWidthHeight);

		void setupImageObject(std::string fileName, float imageRatio);

		SDL_Surface* getSurface();


		int getIdealImageWidth() const;

		float getHalfIdealImageWidth() const;

		int getIdealImageHeight() const;

		float getHalfIdealImageHeight() const;

		float getImageRatioUsed() const;

		int getImageOffsetX() const;
		
		int getImageOffsetY() const;

		void setTexture(SDL_Texture* texture);

		void setUpTexture(SDL_Renderer* pRenderer);

		SDL_Texture* getTexture();

		void setImageOffset(int imageOffsetX, int imageOffsetY);
};


struct ImageAsset
{
	SDL_Surface* mSurface;
	SDL_Texture* mTexture;
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

enum EAnimationType
{
	EAnimationType_INVALID      = -1,
	EAnimationType_NONE,
	EAnimationType_SINGLE,
	EAnimationType_STATIONARY,
	EAnimationType_RUN,
	EAnimationType_JUMP,
	EAnimationType_FALL,
	EAnimationType_FLY,
	EAnimationType_GRABBING,
	EAnimationType_SHOOTING_PROJECTILE,
	EAnimationType_PLAY,
	EAnimationType_MAX

};

class Outfit
{
public:
	std::vector <ImageObject *> mpFrames;

	~Outfit()
	{
		for (ImageObject* frame : mpFrames)
		{
			delete frame;
		}
	}
};

class Animation
{
private:

public:
	EAnimationType				mAnimationType;
	std::vector <Outfit *>		mpOutfits;
	int							mCurFrameNumber;
	int							mFrameRate;
	int							mCountDown;
	bool						mMustFinish;

	Animation(EAnimationType animationType, std::vector <Outfit*> outfits, int curFrameNumber, int frameRate, bool mustFinish);

	~Animation();

	void resetCountDown();

	int setReturnNextAnimationFrameNumber();
};

class AnimationPreset
{
public:
	EAnimationType              			mType;
	std::vector <std::vector <std::string>>	mOutfitsWithFrameNames;
	int										mFrameRate;
	bool									mMustFinish;

	int                         			mMaxImageWidth;
	int                         			mMaxImageHeight;

	int                         			mImageOffsetX;
	int                         			mImageOffsetY;

	AnimationPreset(EAnimationType type, std::vector <std::vector <std::string>> outfitsWithFrameNames, int maxImageWidth, int maxImageHeight, int imageOffsetX, int imageOffsetY, int frameRate, bool mustFinish);
	AnimationPreset(EAnimationType type, std::vector <std::vector <std::string>> outfitsWithFrameNames, int imageOffsetX, int imageOffsetY, int frameRate, bool mustFinish);

	AnimationPreset(EAnimationType type, std::vector <std::string> frameNames, int maxImageWidth, int maxImageHeight, int imageOffsetX, int imageOffsetY, int frameRate, bool mustFinish);
	AnimationPreset(EAnimationType type, std::vector <std::string> frameNames, int imageOffsetX, int imageOffsetY, int frameRate, bool mustFinish);

};


class AnimationManager
{
private:
	std::vector <Animation *> mpAnimations;
	Animation *				  mpCurAnimation;
	int						  mCurOutfitIndex;
	ImageObject *             mpCurImage;

public:
	AnimationManager();
	~AnimationManager();

	void setupAnimationManager(const std::vector <AnimationPreset> & animationPresets, EHowToDetermineWidthHeight howToDetermineWidthHeight);
	void setupAnimationManager(const std::vector <AnimationPreset> & animationPresets, int maxImageWidth, int maxImageHeight, EHowToDetermineWidthHeight howToDetermineWidthHeight);

	void setUpAnimation(const AnimationPreset& animationPreset, EHowToDetermineWidthHeight howToDetermineWidthHeight);
	void setUpAnimation(const AnimationPreset& animationPreset, int maxImageWidth, int maxImageHeight, EHowToDetermineWidthHeight howToDetermineWidthHeight);

	void setUpAllTextures(SDL_Renderer* pRenderer);

	void updateAnimation(EAnimationType animationType);

	void changeAnimation(EAnimationType animationType);

	void changeOutfit(int outfitNum);

	bool haveAnimation(EAnimationType animationType);

	void restartCurAnimation();

	void reset();

	bool isCurAnimationFinished();

	Animation* getCurAnimation();

	std::vector<ImageObject*> getCurFrames();

	ImageObject* getCurImage();

};