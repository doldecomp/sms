#ifndef MAP_POLLUTION_MANAGER_HPP
#define MAP_POLLUTION_MANAGER_HPP

#include <Map/JointModelManager.hpp>
#include <Map/PollutionCount.hpp>
#include <Map/PollutionLayer.hpp>

class J3DModel;

class TPollutionManager;

extern TPollutionManager* gpPollution;

class TPollutionLayerInfo;

class TPollutionManager : public TJointModelManager {
public:
	struct TPollutionInfo {
		/* 0x0 */ u16 unk0;
		/* 0x4 */ TPollutionLayerInfo* unk4;
	};

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);
	virtual TJointModel* newJointModel(int) const;

	void stampModel(J3DModel*);
	void stamp(u16, f32, f32, f32, f32);
	void clean(f32, f32, f32, f32);
	void stampGround(u16, f32, f32, f32, f32);
	u16 getPollutionType(f32, f32, f32) const;
	u32 getPollutionDegree() const;
	void isProhibit(f32, f32, f32) const;
	bool isPolluted(f32, f32, f32) const;
	void subtractFromYMap(f32, f32, f32) const;
	bool cleanedAll() const;
	void draw();
	void setDataAddress(TPollutionManager::TPollutionInfo*);
	void initPollutionInfo();
	TPollutionManager(const char*);

	// fabricated
	TPollutionCounterLayer& getCounterLayer() { return unk70; }
	TPollutionCounterObj& getCounterObj() { return unk1EC; }
	TPollutionLayer* getLayer(int i)
	{
		return (TPollutionLayer*)getJointModel(i);
	}
	TPollutionLayer* getLayer(int i) const
	{
		return (TPollutionLayer*)getJointModel(i);
	}
	const TPollutionLayerInfo* getLayerInfo(int i) const { return &unk6C[i]; }
	TPollutionLayerInfo* getLayerInfo(int i) { return &unk6C[i]; }
	void offLayer(int i) { unk70.offLayer(i); }

	static int mEdgeAlpha;

public:
	/* 0x6C */ TPollutionLayerInfo* unk6C;
	/* 0x70 */ TPollutionCounterLayer unk70;
	/* 0x1EC */ TPollutionCounterObj unk1EC;
	/* 0x204 */ ResTIMG* unk204;
	/* 0x208 */ ResTIMG* unk208;
	/* 0x20C */ u16 unk20C;
};

#endif
