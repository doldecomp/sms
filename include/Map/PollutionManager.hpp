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
		/* 0x0 */ u16 mLayerCount;
		/* 0x4 */ PTR32(TPollutionLayerInfo) mLayerInfos;
	};

	TPollutionManager(const char* name = "落書き管理");

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual TJointModel* newJointModel(int) const;

	void setDataAddress(TPollutionManager::TPollutionInfo* info);
	void initPollutionInfo();

	/// Stamps the top-down ortho projection of the model onto
	/// ground pollution layers
	void stampModel(J3DModel* model);

	/// Stamps the the specified stamp type at the specified position
	/// with the specified size
	void stamp(u16 stamp_type, f32 x, f32 y, f32 z, f32 size);

	/// Cleans the specified area of pollution
	void clean(f32 x, f32 y, f32 z, f32 size);

	/// Stamps the specified stamp type at the specified position
	/// affecting only ground polution layers
	void stampGround(u16 stamp_type, f32 x, f32 y, f32 z, f32 size);

	/// Queries the pollution type of the layer that covers the specified point
	int getPollutionType(f32 x, f32 y, f32 z) const;

	/// Queries the total pollution degree of the entire map
	u32 getPollutionDegree() const;

	void isProhibit(f32 x, f32 y, f32 z) const;

	/// Queries whether the specified position is polluted by any layer
	bool isPolluted(f32 x, f32 y, f32 z) const;

	void subtractFromYMap(f32 x, f32 y, f32 z) const;

	/// Queries whether the entire map is cleared
	bool cleanedAll() const;

	void draw();

	// fabricated
	TPollutionCounterLayer& getCounterLayer() { return unk70; }
	TPollutionCounterObj& getCounterObj() { return unk1EC; }
	TPollutionLayer** getLayers()
	{
		return (TPollutionLayer**)getJointModels();
	}
	TPollutionLayer* getLayer(int i)
	{
		return (TPollutionLayer*)getJointModel(i);
	}
	// Both overloads keep the two-level cast-over-getJointModel(i) spelling.
	// Round 14 trialled `(TPollutionLayer*)mJointModels[i]` in the const one
	// (the candidate left open by TPollutionManager::cleanedAll's note): it
	// does not move cleanedAll at all (96.4%, still 8 bytes of frame too big)
	// and costs getPollutionType and isPolluted their exact match
	// (100% -> 99.7% each), so the level count is not interchangeable with
	// where the level is declared. Do not retry.
	TPollutionLayer* getLayer(int i) const
	{
		return (TPollutionLayer*)getJointModel(i);
	}
	const TPollutionLayerInfo* getLayerInfo(int i) const
	{
		return &mLayerInfos[i];
	}
	TPollutionLayerInfo* getLayerInfo(int i) { return &mLayerInfos[i]; }
	void offLayer(int i) { unk70.offLayer(i); }
	void pollute(f32 x, f32 y, f32 z, f32 size) { stamp(1, x, y, z, size); }

	static u8 mEdgeAlpha;
	static int mFlushTime;

public:
	/* 0x6C */ TPollutionLayerInfo* mLayerInfos;
	/* 0x70 */ TPollutionCounterLayer unk70;
	/* 0x1EC */ TPollutionCounterObj unk1EC;
	/* 0x204 */ ResTIMG* mDefaultPolluteStampTex;
	/* 0x208 */ ResTIMG* mDefaultCleanStampTex;
	/* 0x20C */ u16 unk20C;
};

// Fabricated names. The ROM reaches a pollution layer at two different inline
// depths: `getLayer(i)` (the indexed accessor over TJointModelManager's own
// indexed `getJointModel(i)`) is what every ordinary site expands to, while
// TAirportEventSink::watch and TMapEventSirenaSink::watch expand two levels
// more -- this pair over the plain array accessor. Substituting either form at
// the other's sites costs the exact match, so both spellings are load-bearing.
inline TPollutionManager* SMSGetPollution() { return gpPollution; }

inline TPollutionLayer* SMSGetPollutionLayer(int i)
{
	return SMSGetPollution()->getLayers()[i];
}

#endif
