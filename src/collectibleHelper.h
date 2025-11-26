#pragma once
#include "entityHelper.h"
#include "collectiblePresets.h"

class Collectible : public Entity
{
protected:
	bool mStartingAmPickedUp   = false;
	bool mCheckpointAmPickedUp = false;
	bool mAmPickedUp		   = false;

	bool mPermanentlyAmPickedUp = false;

public:
	CCollectiblePreset* mpsgPreset = nullptr;

	Collectible();
	Collectible(const Vect2 position, CCollectiblePreset* preset);

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
	SavePoint();

	SavePoint(const Vect2 position);
	
	~SavePoint();
};

class EndOfLevelCollectible : public Collectible
{
public:
	EndOfLevelCollectible();

	EndOfLevelCollectible(Vect2 position, CEndOfLevelPreset* preset);
	
	~EndOfLevelCollectible();
};


class MiniGameLevelCollectible : public Collectible
{
private: 
	EDirection mImageDirection = EDirection_LEFT;
public:
	MiniGameLevelCollectible();

	MiniGameLevelCollectible(Vect2 position, CMiniGameLevelPreset* preset);

	~MiniGameLevelCollectible();

	void tick() override;
};


class PermanentCollectible : public Collectible
{
public:
	PermanentCollectible();

	PermanentCollectible(const Vect2 position);

	~PermanentCollectible();

	void resetStats() override;

	void resetToCheckpoint() override;
};

class LotusCollectible : public PermanentCollectible
{
public:
	LotusCollectible();

	LotusCollectible(Vect2 position);

	~LotusCollectible();
};