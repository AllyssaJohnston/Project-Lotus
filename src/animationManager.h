#pragma once
#include <string>
#include <SDL3/SDL.h>
#include <vector>
#include "helpers.h"
#include "ImageHelpers.h"
#include "Image.h"



enum EAnimationType
{
	EAnimationType_INVALID = -1,
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
	EAnimationType_WALL_GRIP,
	EAnimationType_MAX
};

struct Outfit
{
	std::vector <ImageObject*> mpFrames;

	~Outfit();
};

class Animation
{
public:
	EAnimationType				mAnimationType = EAnimationType_INVALID;
	std::vector <Outfit*>		mpOutfits;
	int							mCurFrameNumber = 0;
	int							mFrameRate = 0;
	int							mCountDown = 0;
	bool						mMustFinish = false;

	Animation(EAnimationType animationType, std::vector <Outfit*>& outfits, int curFrameNumber, int frameRate, bool mustFinish);

	~Animation();

	void resetCountDown();

	void setNextAnimationFrameNumber();

	int getNextAnimationFrameNumber();
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

	AnimationPreset(EAnimationType type, std::vector <std::vector <std::string>>& outfitsWithFrameNames, int maxImageWidth, int maxImageHeight, int imageOffsetX, int imageOffsetY, int frameRate, bool mustFinish);
	AnimationPreset(EAnimationType type, std::vector <std::vector <std::string>>& outfitsWithFrameNames, int imageOffsetX, int imageOffsetY, int frameRate, bool mustFinish);

	AnimationPreset(EAnimationType type, std::vector <std::string>& frameNames, int maxImageWidth, int maxImageHeight, int imageOffsetX, int imageOffsetY, int frameRate, bool mustFinish);
	AnimationPreset(EAnimationType type, std::vector <std::string>& frameNames, int imageOffsetX, int imageOffsetY, int frameRate, bool mustFinish);

private:
	void setUpByRef(EAnimationType type, std::vector <std::vector <std::string>>& outfitsWithFrameNames, int maxImageWidth, int maxImageHeight, int imageOffsetX, int imageOffsetY, int frameRate, bool mustFinish);

	void setUpByVal(EAnimationType type, std::vector <std::vector <std::string>> outfitsWithFrameNames, int maxImageWidth, int maxImageHeight, int imageOffsetX, int imageOffsetY, int frameRate, bool mustFinish);
};


class AnimationManager
{
private:
	std::vector <Animation*>	mpAnimations;
	Animation*					mpCurAnimation;
	int							mCurOutfitIndex = 0;
	ImageObject*				mpCurImage;

public:
	AnimationManager();
	~AnimationManager();

	void setupAnimationManager(const std::vector <AnimationPreset>& animationPresets, EHowToDetermineWidthHeight howToDetermineWidthHeight);
	void setupAnimationManager(const std::vector <AnimationPreset>& animationPresets, int maxImageWidth, int maxImageHeight, EHowToDetermineWidthHeight howToDetermineWidthHeight);

	void setUpAnimation(const AnimationPreset& animationPreset, EHowToDetermineWidthHeight howToDetermineWidthHeight);
	void setUpAnimation(const AnimationPreset& animationPreset, int maxImageWidth, int maxImageHeight, EHowToDetermineWidthHeight howToDetermineWidthHeight);

	void setUpAllTextures(SDL_Renderer* pRenderer);

	void updateAnimation(EAnimationType animationType);

	void changeAnimation(EAnimationType animationType);

	void changeOutfit(int outfitNum);

	bool haveAnimation(EAnimationType animationType) const;

	void restartCurAnimation();

	void reset();

	bool isCurAnimationFinished() const;

	Animation* getCurAnimation() const;

	std::vector<ImageObject*>& getCurFrames() const;

	ImageObject* getCurImage() const;

};