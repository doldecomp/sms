#ifndef J3D_ANIMATION_HPP
#define J3D_ANIMATION_HPP

#include <types.h>
#include <JSystem/JUtility/JUTNameTab.hpp>
#include <dolphin/gx.h>
#include <dolphin/mtx.h>

class J3DTransformInfo;
class J3DTextureSRTInfo;
class J3DModelData;

class J3DFrameCtrl {
public:
	enum Attribute_e {
		LOOP_ONCE_e,
		LOOP_ONCE_RESET_e,
		LOOP_REPEAT_e,
		LOOP_MIRROR_ONCE_e,
		LOOP_MIRROR_REPEAT_e,
	};

	J3DFrameCtrl() { init(0); };
	J3DFrameCtrl(s16 s) { init(s); };
	virtual ~J3DFrameCtrl() {};

	void setSpeed(f32 speed) { mSpeed = speed; }
	void setAttribute(u8 attr) { mLoopMode = attr; }
	f32 getRate() const { return mSpeed; }
	void setRate(f32 rate) { mSpeed = rate; }
	void setEndFrame(s16 end_frame) { mEndFrame = end_frame; }
	f32 getCurrentFrame() const { return mCurrentFrame; }
	void setFrame(f32 frame) { mCurrentFrame = frame; }
	s16 getEndFrame() { return mEndFrame; }

	void init(s16);
	BOOL checkPass(float);
	void update();

	bool checkFlag(u32 flag) const { return (mFlags & flag) ? 1 : 0; }

public:
	// TODO: probably private and always accessed via getters/setters judging by
	// stack frame padding
	// TODO: Stolen from SMG, might be wrong
	u8 mLoopMode;      // 0x4
	u8 mFlags;         // 0x5
	s16 mStartFrame;   // 0x6
	s16 mEndFrame;     // 0x8
	s16 mLoopFrame;    // 0xA
	f32 mSpeed;        // 0xC
	f32 mCurrentFrame; // 0x10
};

struct J3DAnmKeyTableBase {
	/* 0x00 */ u16 mMaxFrame;
	/* 0x02 */ u16 mOffset;
	/* 0x04 */ u16 mType;
}; // Size = 0x6

struct J3DAnmTransformKeyTable {
	/* 0x00 */ J3DAnmKeyTableBase mScale;
	/* 0x06 */ J3DAnmKeyTableBase mRotation;
	/* 0x0C */ J3DAnmKeyTableBase mTranslate;
}; // Size = 0x12

struct J3DAnmTransformFullTable {
	/* 0x00 */ u16 mScaleMaxFrame;
	/* 0x02 */ u16 mScaleOffset;
	/* 0x04 */ u16 mRotationMaxFrame;
	/* 0x06 */ u16 mRotationOffset;
	/* 0x08 */ u16 mTranslateMaxFrame;
	/* 0x0A */ u16 mTranslateOffset;
}; // Size = 0xC

struct J3DAnmClusterFullTable {
	u16 mMaxFrame;
	u16 mOffset;
};

struct J3DAnmClusterKeyTable {
	/* 0x00 */ J3DAnmKeyTableBase mWeightTable;
};

class J3DAnmVtxColorIndexData {
public:
	/* 0x00 */ u16 mNum;
	/* 0x04 */ void* mpData;
};

struct J3DAnmColorFullTable {
	/* 0x00 */ u16 mRMaxFrame;
	/* 0x02 */ u16 mROffset;
	/* 0x04 */ u16 mGMaxFrame;
	/* 0x06 */ u16 mGOffset;
	/* 0x08 */ u16 mBMaxFrame;
	/* 0x0A */ u16 mBOffset;
	/* 0x0C */ u16 mAMaxFrame;
	/* 0x0E */ u16 mAOffset;
}; // Size = 0x10

struct J3DAnmColorKeyTable {
	enum Color {
		RED   = 0,
		GREEN = 1,
		BLUE  = 2,
		ALPHA = 3,
	};
	/* 0x00 */ J3DAnmKeyTableBase mColorInfo[4];
}; // Size = 0x18

struct J3DAnmVisibilityFullTable {
	u16 mMaxFrame;
	u16 mOffset;
}; // Size = 0x4

struct J3DAnmTexPatternFullTable {
	/* 0x00 */ u16 mMaxFrame;
	/* 0x02 */ u16 mOffset;
	/* 0x04 */ u8 mTexNo;
	/* 0x06 */ u16 _6;
}; // Size = 0x8

struct J3DAnmCRegKeyTable {
	/* 0x00 */ J3DAnmKeyTableBase mRTable;
	/* 0x06 */ J3DAnmKeyTableBase mGTable;
	/* 0x0C */ J3DAnmKeyTableBase mBTable;
	/* 0x12 */ J3DAnmKeyTableBase mATable;
	/* 0x18 */ u8 mColorId;
	u8 padding[3];
}; // Size = 0x1C

struct J3DAnmKRegKeyTable {
	/* 0x00 */ J3DAnmKeyTableBase mRTable;
	/* 0x06 */ J3DAnmKeyTableBase mGTable;
	/* 0x0C */ J3DAnmKeyTableBase mBTable;
	/* 0x12 */ J3DAnmKeyTableBase mATable;
	/* 0x18 */ u8 mColorId;
	u8 padding[3];
}; // Size = 0x1C

class J3DAnmBase {
public:
	J3DAnmBase();
	J3DAnmBase(s16);

	u8 getAttribute() const { return mAttribute; }
	s16 getFrameMax() const { return mMaxFrame; }
	f32 getFrame() const { return mFrame; }
	void setFrame(f32 frame) { mFrame = frame; }
	s32 getKind() const { return mKind; }

public:
	/* 0x0 */ u8 mAttribute;
	/* 0x1 */ u8 unk1;
	/* 0x2 */ s16 mMaxFrame;
	/* 0x4 */ float mFrame;
	/* 0x8 */ s32 mKind;

public:
	// NOTE: vtable is at 0xc
	virtual ~J3DAnmBase();

	void isLegal(char*) const;
};

class J3DAnmTransform : public J3DAnmBase {
public:
	J3DAnmTransform(s16 i_frameMax, f32* scaleData, s16* rotData,
	                f32* transData)
	    : J3DAnmBase(i_frameMax)
	{
		mScaleData = scaleData;
		mRotData   = rotData;
		mTransData = transData;
		mKind      = 0;
	}

	virtual ~J3DAnmTransform() { }
	virtual void getTransform(u16, J3DTransformInfo*) const { }

public:
	/* 0x10 */ f32* mScaleData;
	/* 0x14 */ s16* mRotData;
	/* 0x18 */ f32* mTransData;
	/* 0x1C */ s16 field_0x1c;
	/* 0x1E */ s16 field_0x1e;
	/* 0x20 */ u16 field_0x20;
	/* 0x22 */ u16 field_0x22;
};

class J3DAnmTransformFull : public J3DAnmTransform {
public:
	J3DAnmTransformFull()
	    : J3DAnmTransform(0, nullptr, nullptr, nullptr)
	{
		mAnmTable = nullptr;
	}

	virtual ~J3DAnmTransformFull() { }
	virtual void getTransform(u16, J3DTransformInfo*) const;

public:
	/* 0x24 */ J3DAnmTransformFullTable* mAnmTable;
};

class J3DAnmTransformKey : public J3DAnmTransform {
public:
	J3DAnmTransformKey()
	    : J3DAnmTransform(0, nullptr, nullptr, nullptr)
	{
		mDecShift = 0;
		mAnmTable = nullptr;
	}

	virtual void getTransform(u16 idx, J3DTransformInfo* dst) const
	{
		calcTransform(mFrame, idx, dst);
	}
	virtual ~J3DAnmTransformKey() { }
	virtual void calcTransform(float frame, u16 idx,
	                           J3DTransformInfo* dst) const;

public:
	/* 0x24 */ int mDecShift;
	/* 0x28 */ J3DAnmTransformKeyTable* mAnmTable;
};

class J3DAnmTextureSRTKey : public J3DAnmBase {
public:
	J3DAnmTextureSRTKey()
	    : J3DAnmBase()
	{
		mDecShift       = 0;
		mTrackNum       = 0;
		mScaleNum       = 0;
		mRotNum         = 0;
		mTransNum       = 0;
		mAnmTable       = nullptr;
		mScaleData      = nullptr;
		mRotData        = nullptr;
		mTransData      = nullptr;
		mPostTrackNum   = 0;
		unk40           = 0;
		unk42           = 0;
		unk44           = 0;
		unk58           = nullptr;
		unk48           = nullptr;
		unk4C           = nullptr;
		unk50           = nullptr;
		mTexMtxCalcType = 0;
		mKind           = 4;
	}

	void calcTransform(float, u16, J3DTextureSRTInfo*) const;
	void calcPostTransform(float, u16, J3DTextureSRTInfo*) const;
	void searchUpdateMaterialID(J3DModelData*);

	u16 getUpdateMaterialNum() const { return mTrackNum / 3; }

	u16 getPostUpdateMaterialNum() const { return mPostTrackNum / 3; }

	void getTransform(u16 idx, J3DTextureSRTInfo* srt_info) const
	{
		calcTransform(mFrame, idx, srt_info);
	}

	u16 getUpdateMaterialID(u16 idx) const { return mUpdateMaterialID[idx]; }
	JUTNameTab* getUpdateMaterialName() { return mUpdateMaterialName; }
	u16 getUpdateTexMtxID(u16 idx) const { return mUpdateTexMtxID[idx]; }
	u32 getTexMtxCalcType() { return mTexMtxCalcType; }
	Vec& getSRTCenter(u16 idx) { return mSRTCenter[idx]; }

	virtual ~J3DAnmTextureSRTKey() { }

public:
	/* 0x10 */ int mDecShift;
	/* 0x14 */ u16 mTrackNum;
	/* 0x18 */ J3DAnmTransformKeyTable* mAnmTable;
	/* 0x1C */ u16 mScaleNum;
	/* 0x1E */ u16 mRotNum;
	/* 0x20 */ u16 mTransNum;
	/* 0x24 */ f32* mScaleData;
	/* 0x28 */ s16* mRotData;
	/* 0x2C */ f32* mTransData;
	/* 0x30 */ u8* mUpdateTexMtxID;
	/* 0x34 */ u16* mUpdateMaterialID;
	/* 0x38 */ JUTNameTab* mUpdateMaterialName;
	/* 0x3С */ Vec* mSRTCenter;
	/* 0x40 */ u16 unk40;
	/* 0x42 */ u16 unk42;
	/* 0x44 */ u16 unk44;
	/* 0x48 */ void* unk48;
	/* 0x4C */ void* unk4C;
	/* 0x50 */ void* unk50;
	/* 0x54 */ u16 mPostTrackNum;
	/* 0x58 */ void* unk58;
	/* 0x5C */ u8* mPostUpdateTexMtxID;
	/* 0x60 */ u16* mPostUpdateMaterialID;
	/* 0x64 */ JUTNameTab* mPostUpdateMaterialName;
	/* 0x68 */ Vec* mPostSRTCenter;
	/* 0x6C */ u32 mTexMtxCalcType;
};

class J3DAnmCluster : public J3DAnmBase {
public:
	J3DAnmCluster(s16 kind, f32* weight)
	    : J3DAnmBase(0)
	{
		mWeight = weight;
		mKind   = kind;
	}

	virtual ~J3DAnmCluster() { }
	virtual f32 getWeight(u16) const { return 1.0f; }

public:
	/* 0x10 */ f32* mWeight;
};

class J3DAnmClusterFull : public J3DAnmCluster {
public:
	J3DAnmClusterFull()
	    : J3DAnmCluster(3, nullptr)
	{
		mAnmTable = nullptr;
	}

	virtual ~J3DAnmClusterFull() { }
	virtual f32 getWeight(u16) const;

public:
	/* 0x14 */ J3DAnmClusterFullTable* mAnmTable;
};

class J3DAnmClusterKey : public J3DAnmCluster {
public:
	J3DAnmClusterKey()
	    : J3DAnmCluster(3, nullptr)
	{
		mAnmTable = nullptr;
	}

	virtual ~J3DAnmClusterKey() { }
	virtual f32 getWeight(u16) const;

public:
	/* 0x14 */ J3DAnmClusterKeyTable* mAnmTable;
};

class J3DAnmVtxColor : public J3DAnmBase {
public:
	J3DAnmVtxColor()
	    : J3DAnmBase()
	{
		mKind = 7;
		for (int i = 0; i < 2; i++) {
			mAnmTableNum[i] = 0;
		}
		for (int i = 0; i < 2; i++) {
			mAnmVtxColorIndexData[i] = nullptr;
		}
	}

	virtual ~J3DAnmVtxColor() { }
	virtual void getColor(u8, u16, GXColor*) const { }

public:
	/* 0x10 */ u16 mAnmTableNum[2];
	/* 0x14 */ J3DAnmVtxColorIndexData* mAnmVtxColorIndexData[2];
};

class J3DAnmVtxColorFull : public J3DAnmVtxColor {
public:
	J3DAnmVtxColorFull()
	{
		for (int i = 0; i < 2; i++) {
			mTable[i] = nullptr;
		}
	}

	virtual ~J3DAnmVtxColorFull() { }
	virtual void getColor(u8, u16, GXColor*) const;

public:
	/* 0x1C */ J3DAnmColorFullTable* mTable[2];
	/* 0x24 */ u8* mColorR;
	/* 0x28 */ u8* mColorG;
	/* 0x2C */ u8* mColorB;
	/* 0x30 */ u8* mColorA;
};

class J3DAnmVtxColorKey : public J3DAnmVtxColor {
public:
	J3DAnmVtxColorKey()
	{
		for (int i = 0; i < 2; i++) {
			mTable[i] = nullptr;
		}
	}

	virtual ~J3DAnmVtxColorKey() { }
	virtual void getColor(u8, u16, GXColor*) const;

	J3DAnmColorKeyTable* getAnmTable(u8 idx) const { return mTable[idx]; }

public:
	/* 0x1C */ J3DAnmColorKeyTable* mTable[2];
	/* 0x24 */ s16* mColorR;
	/* 0x28 */ s16* mColorG;
	/* 0x2C */ s16* mColorB;
	/* 0x30 */ s16* mColorA;
};

class J3DAnmColor : public J3DAnmBase {
public:
	J3DAnmColor()
	    : J3DAnmBase()
	{
		unk10 = 0;
		unk12 = 0;
		unk14 = 0;
		unk16 = 0;

		mUpdateMaterialNum  = 0;
		mUpdateMaterialID   = nullptr;
		mUpdateMaterialName = nullptr;

		mKind = 1;
	}

	virtual void getColor(u16, GXColor*) const { }
	virtual ~J3DAnmColor() { }

	void searchUpdateMaterialID(J3DModelData*);
	u16 getUpdateMaterialNum() const { return mUpdateMaterialNum; }
	u16 getUpdateMaterialID(u16 idx) const { return mUpdateMaterialID[idx]; }
	JUTNameTab* getUpdateMaterialName() { return mUpdateMaterialName; }

public:
	/* 0x10 */ u16 unk10;
	/* 0x12 */ u16 unk12;
	/* 0x14 */ u16 unk14;
	/* 0x16 */ u16 unk16;
	/* 0x18 */ u16 mUpdateMaterialNum;
	/* 0x1C */ u16* mUpdateMaterialID;
	/* 0x20 */ JUTNameTab* mUpdateMaterialName;
};

class J3DAnmColorFull : public J3DAnmColor {
public:
	J3DAnmColorFull()
	{
		mColorR   = nullptr;
		mColorG   = nullptr;
		mColorB   = nullptr;
		mColorA   = nullptr;
		mAnmTable = nullptr;
	}

	virtual ~J3DAnmColorFull() { }
	virtual void getColor(u16, GXColor*) const;

public:
	/* 0x24 */ u8* mColorR;
	/* 0x28 */ u8* mColorG;
	/* 0x2C */ u8* mColorB;
	/* 0x30 */ u8* mColorA;
	/* 0x34 */ J3DAnmColorFullTable* mAnmTable;
};

class J3DAnmColorKey : public J3DAnmColor {
public:
	J3DAnmColorKey()
	{
		mColorR = nullptr;
		mColorG = nullptr;
		mColorB = nullptr;
		mColorA = nullptr;
		mTable  = nullptr;
	}

	virtual ~J3DAnmColorKey() { }
	virtual void getColor(u16, GXColor*) const;

public:
	/* 0x24 */ s16* mColorR;
	/* 0x28 */ s16* mColorG;
	/* 0x2C */ s16* mColorB;
	/* 0x30 */ s16* mColorA;
	/* 0x34 */ J3DAnmColorKeyTable* mTable;
};

// BVA
class J3DAnmVisibilityFull : public J3DAnmBase {
public:
	friend class J3DAnmFullLoader_v15;

	J3DAnmVisibilityFull()
	    : J3DAnmBase()
	{
		mUpdateMaterialNum = 0;
		mAnmTable          = nullptr;
		field_0x12         = 0;
		mVisibility        = nullptr;
		mKind              = 6;
	}
	void getVisibility(u16, u8*) const;

	virtual ~J3DAnmVisibilityFull() { }

	J3DAnmVisibilityFullTable* getAnmTable() const { return mAnmTable; }

private:
	/* 0x10 */ u16 mUpdateMaterialNum;
	/* 0x12 */ u16 field_0x12;
	/* 0x14 */ J3DAnmVisibilityFullTable* mAnmTable;
	/* 0x18 */ u8* mVisibility;
}; // Size: 0x1C

class J3DAnmTexPattern : public J3DAnmBase {
public:
	J3DAnmTexPattern()
	    : J3DAnmBase()
	{
		mTextureIndex       = nullptr;
		mAnmTable           = nullptr;
		mKind               = 2;
		mUpdateMaterialNum  = 0;
		mUpdateMaterialID   = nullptr;
		mUpdateMaterialName = nullptr;
	}

	void getTexNo(u16, u16*) const;
	void searchUpdateMaterialID(J3DModelData*);
	virtual ~J3DAnmTexPattern() { }

	u16 getUpdateMaterialID(u16 idx) const { return mUpdateMaterialID[idx]; }
	u16 getUpdateMaterialNum() const { return mUpdateMaterialNum; }
	JUTNameTab* getUpdateMaterialName() { return mUpdateMaterialName; }

	J3DAnmTexPatternFullTable* getAnmTable() const { return mAnmTable; }

public:
	/* 0x10 */ u16* mTextureIndex;
	/* 0x14 */ J3DAnmTexPatternFullTable* mAnmTable;
	/* 0x18 */ u16 field_0x18;
	/* 0x1A */ u16 mUpdateMaterialNum;
	/* 0x1C */ u16* mUpdateMaterialID;
	/* 0x20 */ JUTNameTab* mUpdateMaterialName;
};

class J3DAnmTevRegKey : public J3DAnmBase {
	friend class J3DAnmKeyLoader_v15;

public:
	J3DAnmTevRegKey()
	    : J3DAnmBase()
	{
		mCRegUpdateMaterialNum = 0;
		mKRegUpdateMaterialNum = 0;
		mCRegDataCountR        = 0;
		mCRegDataCountG        = 0;
		mCRegDataCountB        = 0;
		mCRegDataCountA        = 0;
		mKRegDataCountR        = 0;
		mKRegDataCountG        = 0;
		mKRegDataCountB        = 0;
		mKRegDataCountA        = 0;

		mCRegUpdateMaterialID   = nullptr;
		mCRegUpdateMaterialName = nullptr;
		mKRegUpdateMaterialID   = nullptr;
		mKRegUpdateMaterialName = nullptr;

		mAnmCRegDataR = nullptr;
		mAnmCRegDataG = nullptr;
		mAnmCRegDataB = nullptr;
		mAnmCRegDataA = nullptr;
		mAnmKRegDataR = nullptr;
		mAnmKRegDataG = nullptr;
		mAnmKRegDataB = nullptr;
		mAnmKRegDataA = nullptr;

		mKind = 5;
	}
	void getTevColorReg(unsigned short, _GXColorS10*) const;
	void getTevKonstReg(unsigned short, _GXColor*) const;
	void searchUpdateMaterialID(J3DModelData*);
	virtual ~J3DAnmTevRegKey() { }

	u16 getCRegUpdateMaterialNum() const { return mCRegUpdateMaterialNum; }
	u16 getKRegUpdateMaterialNum() const { return mKRegUpdateMaterialNum; }

	u16 getCRegUpdateMaterialID(u16 idx) const
	{
		return mCRegUpdateMaterialID[idx];
	}
	u16 getKRegUpdateMaterialID(u16 idx) const
	{
		return mKRegUpdateMaterialID[idx];
	}

	J3DAnmCRegKeyTable* getAnmCRegKeyTable() const { return mAnmCRegKeyTable; }
	J3DAnmKRegKeyTable* getAnmKRegKeyTable() const { return mAnmKRegKeyTable; }

private:
	/* 0x10 */ u16 mCRegUpdateMaterialNum;
	/* 0x12 */ u16 mKRegUpdateMaterialNum;
	/* 0x14 */ u16 mCRegDataCountR;
	/* 0x16 */ u16 mCRegDataCountG;
	/* 0x18 */ u16 mCRegDataCountB;
	/* 0x1A */ u16 mCRegDataCountA;
	/* 0x1C */ u16 mKRegDataCountR;
	/* 0x1E */ u16 mKRegDataCountG;
	/* 0x20 */ u16 mKRegDataCountB;
	/* 0x22 */ u16 mKRegDataCountA;
	/* 0x24 */ u16* mCRegUpdateMaterialID;
	/* 0x28 */ JUTNameTab* mCRegUpdateMaterialName;
	/* 0x2С */ u16* mKRegUpdateMaterialID;
	/* 0x30 */ JUTNameTab* mKRegUpdateMaterialName;
	/* 0x34 */ J3DAnmCRegKeyTable* mAnmCRegKeyTable;
	/* 0x38 */ J3DAnmKRegKeyTable* mAnmKRegKeyTable;
	/* 0x3С */ s16* mAnmCRegDataR;
	/* 0x40 */ s16* mAnmCRegDataG;
	/* 0x44 */ s16* mAnmCRegDataB;
	/* 0x48 */ s16* mAnmCRegDataA;
	/* 0x4С */ s16* mAnmKRegDataR;
	/* 0x50 */ s16* mAnmKRegDataG;
	/* 0x54 */ s16* mAnmKRegDataB;
	/* 0x58 */ s16* mAnmKRegDataA;
};

#endif
