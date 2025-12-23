#pragma once
#include "entity.h"
#include "collectiblePresets.h"

class Collectible : public Entity
{
protected:
	bool mStartingAmPickedUp   = false;
	bool mCheckpointAmPickedUp = false;
	bool mAmPickedUp		   = false;

	bool mPermanentlyAmPickedUp = false;

public:
	Collectible(const Vect2 position, const CCollectiblePreset& preset);

	~Collectible();

	void tick() override;

	bool isAmPickedUp() const;

	void setAmPickedUp(bool amPickedUp);

	
	void setCheckpointStats() override;

	void resetStats() override;

	void resetToCheckpoint() override;


	bool isPermanentlyPickedUp() const;

	void setPermanentlyPickedUp(bool amPickedUp);

};

class SavePoint : public Collectible
{
public:
	SavePoint(const Vect2 position);
	
	~SavePoint();
};

class EndOfLevelCollectible : public Collectible
{
public:
	EndOfLevelCollectible(const Vect2 position, const CEndOfLevelPreset& preset);
	
	~EndOfLevelCollectible();
};


class MiniGameLevelCollectible : public Collectible
{
private: 
	EDirection mImageDirection = EDirection_LEFT;
public:
	LevelData mNextLevelData;

	MiniGameLevelCollectible(const Vect2 position, const CMiniGameLevelPreset preset);

	~MiniGameLevelCollectible();

	void tick() override;
};


class PermanentCollectible : public Collectible
{
public:
	PermanentCollectible(const Vect2 position);

	~PermanentCollectible();

	void resetStats() override;

	void resetToCheckpoint() override;
};

class LotusCollectible : public PermanentCollectible
{
public:
	LotusCollectible(const Vect2 position);

	~LotusCollectible();
};