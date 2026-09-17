#ifndef MOVE_BG_MAP_OBJ_FLAG_HPP
#define MOVE_BG_MAP_OBJ_FLAG_HPP

#include <Strategic/HitActor.hpp>
#include <JSystem/JDrama/JDRViewObj.hpp>
#include <JSystem/JGeometry/JGMatrix34.hpp>
#include <JSystem/JGeometry/JGVec3.hpp>

class ResTIMG;

/// A cloth flag drawn as a hand-built triangle-strip mesh. The manager owns
/// the texture and drives every instance, so this class has no perform() of
/// its own: update() and draw() are called from TMapObjFlagManager::perform.
class TMapObjFlag : public THitActor {
public:
	TMapObjFlag(const char* name = "旗");

	virtual ~TMapObjFlag() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void updateVertex();

	void init(const char*);
	void update();
	void draw();

	/// Degrees the wave phase advances per frame; set per map by the manager.
	static f32 mFlutterSpeed;

public:
	/// Cloth extent along local Z (100 units per unit of scaling.z).
	/* 0x68 */ f32 mLength;
	/// Cloth extent along local Y (100 units per unit of scaling.y).
	/* 0x6C */ f32 mHeight;
	/// Vertex columns along Z.
	/* 0x70 */ int mNumZ;
	/// Vertex rows along Y.
	/* 0x74 */ int mNumY;
	/// mNumY rows of mNumZ vertices; only x is animated.
	/* 0x78 */ JGeometry::TVec3<f32>** mVertices;
	/// Degrees of wave phase per column.
	/* 0x7C */ f32 mWavePhaseZ;
	/// Degrees of wave phase per row.
	/* 0x80 */ f32 mWavePhaseY;
	/// Wave depth at the free edge, in units.
	/* 0x84 */ f32 mWaveAmplitude;
	/// Current wave phase in degrees, 0 to 360.
	/* 0x88 */ f32 mWaveAngle;
	/* 0x8C */ JGeometry::TMatrix34<JGeometry::SMatrix34C<f32> > mMtx;
	/// Row/column stride, i.e. how many vertices to skip when animating.
	/* 0xBC */ int mSkip;
};

/// Dead in retail: only updateVertex, the destructor and the vtable were
/// compiled, and the map records no constructor, so nothing ever made one.
class TMapObjFlagLower : public TMapObjFlag {
public:
	virtual ~TMapObjFlagLower() { }
	virtual void updateVertex();
};

/// Dead in retail, like TMapObjFlagLower.
class TMapObjFlagSail : public TMapObjFlag {
public:
	virtual ~TMapObjFlagSail() { }
	virtual void updateVertex();
};

/// Buckets every flag in the scene by its texture name so that each texture
/// is loaded once and all the flags sharing it are drawn back to back.
class TMapObjFlagManager : public JDrama::TViewObj {
public:
	enum {
		/// One bucket per known flag texture.
		FLAG_KIND_NUM = 15,
		/// Flags per bucket.
		FLAG_NUM_MAX = 20,
	};

	struct TMapObjFlagInfo {
		TMapObjFlagInfo()
		{
			mNum  = 0;
			mTimg = nullptr;
		}

		/* 0x00 */ int mNum;
		/* 0x04 */ TMapObjFlag* mFlags[FLAG_NUM_MAX];
		/* 0x54 */ const ResTIMG* mTimg;
	};

	TMapObjFlagManager(const char* name = "旗管理");

	virtual ~TMapObjFlagManager() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	void loadFlag(TMapObjFlagInfo*, TMapObjFlag*, const char*);
	void registerObj(TMapObjFlag*, const char*);
	void initDraw();

	// TODO: both are UNUSED 4-byte .sdata objects, so they were initialised
	// to something non-zero that the map cannot tell us. The names suggest
	// the two LOD switch distances the flags never ended up using.
	static f32 mDistNearMiddle;
	static f32 mDistMiddleFar;

public:
	/* 0x10 */ TMapObjFlagInfo mInfos[FLAG_KIND_NUM];
};

extern TMapObjFlagManager* gpMapObjFlagManager;

#endif
