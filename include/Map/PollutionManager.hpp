#ifndef MAP_POLLUTION_MANAGER_HPP
#define MAP_POLLUTION_MANAGER_HPP

#include <Map/JointModelManager.hpp>
#include <Map/PollutionCount.hpp>

class J3DModel;

class TPollutionManager;

extern TPollutionManager* gpPollution;

class TPollutionManager : public TJointModelManager {
public:
	class TPollutionInfo { };

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);
	virtual TJointModel* newJointModel(int) const;

	void stampModel(J3DModel*);
	void stamp(u16, f32, f32, f32, f32);
	void clean(f32, f32, f32, f32);
	void stampGround(u16, f32, f32, f32, f32);
	void getPollutionType(f32, f32, f32) const;
	void getPollutionDegree() const;
	void isProhibit(f32, f32, f32) const;
	void isPolluted(f32, f32, f32) const;
	void subtractFromYMap(f32, f32, f32) const;
	void cleanedAll() const;
	void draw();
	void setDataAddress(TPollutionManager::TPollutionInfo*);
	void initPollutionInfo();
	TPollutionManager(const char*);

public:
	/* 0x6C */ u32 unk6C;
	/* 0x70 */ TPollutionCounterLayer unk70;
	/* 0x1EC */ TPollutionCounterObj unk1EC;
	/* 0x204 */ u32 unk204;
	/* 0x208 */ u32 unk208;
	/* 0x20C */ u16 unk20C;
};

#endif
