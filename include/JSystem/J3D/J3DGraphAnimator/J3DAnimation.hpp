#ifndef J3D_ANIMATION_HPP
#define J3D_ANIMATION_HPP

#include <types.h>
#include <JSystem/JUtility/JUTNameTab.hpp>
#include <JSystem/J3D/J3DAssert.hpp>
#include <dolphin/gx.h>
#include <dolphin/mtx.h>

class J3DTransformInfo;
class J3DTextureSRTInfo;
class J3DModelData;

/**
 * @brief Animation frame controller with configurable playback modes.
 * Supports once, loop, and ping-pong modes, with optional loop points.
 */
class J3DFrameCtrl {
public:
	/// Playback modes.
	enum {
		ATTR_ONCE,           ///< Play to end and stop.
		ATTR_ONCE_AND_RESET, ///< Play to end, reset to start, and stop.
		ATTR_LOOP,           ///< Loop continuously between loop point and end.
		ATTR_PING_PONG,      ///< Play forward then backward, stop at start.
		ATTR_PING_PONG_LOOP, ///< Alternate forward/backward indefinitely.
	};

	/// State flags, reset each update(). Check with checkState().
	enum {
		STATE_COMPLETED_ONCE = 0x1, ///< Finished once (any mode)
		STATE_LOOPED_ONCE    = 0x2, ///< Looped once (only loop modes)
	};

	J3DFrameCtrl() { init(0); };
	J3DFrameCtrl(s16 end) { init(end); };
	virtual ~J3DFrameCtrl() { }

	/// Initialize to ATTR_LOOP with rate 1.0 and frame range [0, @p end).
	void init(s16 end);

	/// Test if @p pass_frame will be crossed during the next advance.
	BOOL checkPass(f32 pass_frame);

	/// Advance the frame by the current rate.
	void update();

	s16 getStart() const { return mStart; }
	void setStart(s16 start)
	{
		mStart = start;
		mFrame = start;
	}

	f32 getFrame() const { return mFrame; }
	void setFrame(f32 frame) { mFrame = frame; }

	s16 getEnd() const { return mEnd; }
	void setEnd(s16 end) { mEnd = end; }

	f32 getRate() const { return mRate; }
	void setRate(f32 rate) { mRate = rate; }

	s16 getLoop() const { return mLoop; }
	void setLoop(s16 loop) { mLoop = loop; }

	u8 getAttribute() const { return mAttribute; }
	void setAttribute(u8 attr) { mAttribute = attr; }

	void reset()
	{
		mFrame = mStart;
		mRate  = 1.0f;
	}

	/// Test state flags, e.g. `checkState(STATE_COMPLETED_ONCE)`.
	bool checkState(u32 mask) const { return (mState & mask) ? 1 : 0; }

	// Fabricated
	u8 getState() const { return mState; }

private:
	/* 0x4 */ u8 mAttribute; ///< Playback mode (see ATTR_*).
	/* 0x5 */ u8 mState;   ///< State flags (see STATE_*), reset each update().
	/* 0x6 */ s16 mStart;  ///< Start frame (inclusive).
	/* 0x8 */ s16 mEnd;    ///< End frame (exclusive).
	/* 0xA */ s16 mLoop;   ///< Loop point for loop modes (inclusive).
	/* 0xC */ f32 mRate;   ///< Frame advance per update (positive or negative).
	/* 0x10 */ f32 mFrame; ///< Current frame (can be fractional).
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

/**
 * @brief A runtime representation of a single animations of some set of
 * variables (be it scalar, vector, matrix,  or something else entirely) loaded
 * from an asset.
 * @details Animations can be "full" , where this object and the respective file
 * store every single frame's value of the variable(s), or keyframed, where only
 * keyframes are stored and interpolation is used for the in-betweens.
 *
 * Basic workflow is: call J3DAnmBase::setFrame to specify the desired animation
 * frame and call `get*()` to get the value of the animated variable at that
 * particular frame.
 *
 * This type being decoupled from how the animation frame changes or how the
 * animated variable is used allows for re-use for different models and
 * instances. Frame pacing is usually driven by a per-model-instance
 * J3DFrameCtrl.
 */
class J3DAnmBase {
protected:
	/* 0x0 */ u8 mAttribute;
	/* 0x1 */ u8 unk1;
	/* 0x2 */ s16 mMaxFrame;
	/* 0x4 */ f32 mFrame;
	/* 0x8 */ s32 mKind;

	friend class J3DAnmFullLoader_v15;
	friend class J3DAnmKeyLoader_v15;

public:
	J3DAnmBase();
	J3DAnmBase(s16 maxFrame);

	// NOTE: vtable is at 0xc
	virtual ~J3DAnmBase();

	u8 getAttribute() const { return mAttribute; }
	s16 getFrameMax() const { return mMaxFrame; }
	f32 getFrame() const { return mFrame; }
	void setFrame(f32 frame) { mFrame = frame; }
	s32 getKind() const { return mKind; }

	void isLegal(char*) const;
};

/**
 * @brief A 3D transform animation (bck/bca).
 * @details Animates a set of J3DTransformInfo variables, usually per-joint
 * transforms of a skeleton.
 */
class J3DAnmTransform : public J3DAnmBase {
	friend class J3DAnmFullLoader_v15;
	friend class J3DAnmKeyLoader_v15;

public:
	J3DAnmTransform(s16 maxFrame, f32* scaleData, s16* rotData, f32* transData)
	    : J3DAnmBase(maxFrame)
	{
		mScaleData = scaleData;
		mRotData   = rotData;
		mTransData = transData;
		mKind      = 0;
	}

	/**
	 * @brief Gets the current value of the animated transform from one of the
	 * sequences.
	 *
	 * @param idx The index of the animation sequence, typically tied to a joint
	 * index in a skeleton.
	 * @param out Current value of the animated transform.
	 */
	virtual void getTransform(u16 idx, J3DTransformInfo* out) const { }

protected:
	/* 0x10 */ f32* mScaleData;
	/* 0x14 */ s16* mRotData;
	/* 0x18 */ f32* mTransData;
	/* 0x1C */ s16 field_0x1c;
	/* 0x1E */ s16 field_0x1e;
	/* 0x20 */ u16 field_0x20;
	/* 0x22 */ u16 field_0x22;
};

/**
 * @brief A full 3D transform animation (bca).
 * @details Animates a set of J3DTransformInfo variables, usually per-joint
 * transforms of a skeleton.
 */
class J3DAnmTransformFull : public J3DAnmTransform {
	friend class J3DAnmFullLoader_v15;

public:
	J3DAnmTransformFull()
	    : J3DAnmTransform(0, nullptr, nullptr, nullptr)
	{
		mAnmTable = nullptr;
	}

	virtual void getTransform(u16 idx, J3DTransformInfo* out) const;

private:
	/* 0x24 */ J3DAnmTransformFullTable* mAnmTable;
};

/**
 * @brief A keyframed 3D transform animation (bck).
 * @details Animates a set of J3DTransformInfo variables, usually per-joint
 * transforms of a skeleton.
 */
class J3DAnmTransformKey : public J3DAnmTransform {
	friend class J3DAnmKeyLoader_v15;

public:
	J3DAnmTransformKey()
	    : J3DAnmTransform(0, nullptr, nullptr, nullptr)
	{
		mDecShift = 0;
		mAnmTable = nullptr;
	}

	virtual void getTransform(u16 idx, J3DTransformInfo* out) const
	{
		calcTransform(mFrame, idx, out);
	}

	virtual void calcTransform(f32 frame, u16 idx, J3DTransformInfo* out) const;

	// NOTE: required to be explicitly defined for weak symbol ordering
	virtual ~J3DAnmTransformKey() { }

private:
	/* 0x24 */ int mDecShift;
	/* 0x28 */ J3DAnmTransformKeyTable* mAnmTable;
};

/**
 * @brief A keyframed texture transform animation (btk).
 * @details Animates a set of J3DTextureSRTInfo, used for things like scrolling
 * textures.
 */
class J3DAnmTextureSRTKey : public J3DAnmBase {
	friend class J3DAnmKeyLoader_v15;

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

	void calcTransform(f32 frame, u16 idx, J3DTextureSRTInfo* out) const;
	void calcPostTransform(f32 frame, u16 idx, J3DTextureSRTInfo* out) const;
	void searchUpdateMaterialID(J3DModelData* table);

	u16 getUpdateMaterialNum() const { return mTrackNum / 3; }

	u16 getPostUpdateMaterialNum() const { return mPostTrackNum / 3; }

	/**
	 * @brief Gets the current value of the animated transform from one of the
	 * stored sequences.
	 *
	 * @param idx The index of the animation sequence.
	 * @param out Current value of the animated transform.
	 */
	void getTransform(u16 idx, J3DTextureSRTInfo* out) const
	{
		calcTransform(mFrame, idx, out);
	}

	bool isValidUpdateMaterialID(u16 idx) const
	{
		return mUpdateMaterialID[idx] != 0xffff;
	}
	u16 getUpdateMaterialID(u16 idx) const
	{
		J3D_ASSERT_RANGE(1029, idx < mTrackNum / 3);
		return mUpdateMaterialID[idx];
	}
	JUTNameTab* getUpdateMaterialName() { return mUpdateMaterialName; }
	u8 getUpdateTexMtxID(u16 idx) const
	{
		J3D_ASSERT_RANGE(1017, idx < mTrackNum / 3);
		return mUpdateTexMtxID[idx];
	}
	u32 getTexMtxCalcType() { return mTexMtxCalcType; }
	Vec* getSRTCenter(u16 idx)
	{
		J3D_ASSERT_RANGE(1047, idx < mTrackNum / 3);
		return &mSRTCenter[idx];
	}

private:
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

/**
 * @brief A cluster animation (blk/bla).
 * @details Animates a set of weights for different clusters, usually used as
 * morph target weights with each cluster representing a different pose.
 */
class J3DAnmCluster : public J3DAnmBase {
	friend class J3DAnmFullLoader_v15;
	friend class J3DAnmKeyLoader_v15;

public:
	J3DAnmCluster(s16 kind, f32* weight)
	    : J3DAnmBase(0)
	{
		mWeight = weight;
		mKind   = kind;
	}

	/**
	 * @brief Gets the current weight from the specified animation sequence.
	 *
	 * @param idx The index of the animation sequence.
	 * @return f32 The current weight.
	 */
	virtual f32 getWeight(u16 idx) const { return 1.0f; }

protected:
	/* 0x10 */ f32* mWeight;
};

/**
 * @brief A full cluster animation (bla).
 * @details Animates a set of weights for different clusters, usually used as
 * morph target weights with each cluster representing a different pose.
 */
class J3DAnmClusterFull : public J3DAnmCluster {
	friend class J3DAnmFullLoader_v15;

public:
	J3DAnmClusterFull()
	    : J3DAnmCluster(3, nullptr)
	{
		mAnmTable = nullptr;
	}

	virtual f32 getWeight(u16 idx) const;

private:
	/* 0x14 */ J3DAnmClusterFullTable* mAnmTable;
};

/**
 * @brief A keyframed cluster animation (blk).
 * @details Animates a set of weights for different clusters, usually used as
 * morph target weights with each cluster representing a different pose.
 */
class J3DAnmClusterKey : public J3DAnmCluster {
	friend class J3DAnmKeyLoader_v15;

public:
	J3DAnmClusterKey()
	    : J3DAnmCluster(3, nullptr)
	{
		mAnmTable = nullptr;
	}

	virtual f32 getWeight(u16 idx) const;

private:
	/* 0x14 */ J3DAnmClusterKeyTable* mAnmTable;
};

/**
 * @brief A vertex color animation (bxk/bxa).
 * @details Animates vertex color channels for vertices, so there is a separate
 * GXColor animation sequence per vertex and per channel (0/1 in GX).
 */
class J3DAnmVtxColor : public J3DAnmBase {
	friend class J3DAnmFullLoader_v15;
	friend class J3DAnmKeyLoader_v15;

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

	/**
	 * @brief Gets the current color for the specified channel/vertex.
	 *
	 * @param chan The vertex color channel.
	 * @param idx The index of the vertex.
	 * @param out The current color.
	 */
	virtual void getColor(u8 chan, u16 idx, GXColor* out) const { }

	u16 getAnmTableNum(u8 chan) { return mAnmTableNum[chan]; }

	J3DAnmVtxColorIndexData* getAnmVtxColorIndexData(u8 chan, u16 idx)
	{
		return &mAnmVtxColorIndexData[chan][idx];
	}

protected:
	/* 0x10 */ u16 mAnmTableNum[2];
	/* 0x14 */ J3DAnmVtxColorIndexData* mAnmVtxColorIndexData[2];
};

/**
 * @brief A full vertex color animation (bxa).
 * @details Animates vertex color channels for vertices, so there is a separate
 * GXColor animation sequence per vertex and per channel (0/1 in GX).
 */
class J3DAnmVtxColorFull : public J3DAnmVtxColor {
	friend class J3DAnmFullLoader_v15;

public:
	J3DAnmVtxColorFull()
	{
		for (int i = 0; i < 2; i++) {
			mTable[i] = nullptr;
		}
	}

	virtual void getColor(u8 chan, u16 idx, GXColor* out) const;

private:
	/* 0x1C */ J3DAnmColorFullTable* mTable[2];
	/* 0x24 */ u8* mColorR;
	/* 0x28 */ u8* mColorG;
	/* 0x2C */ u8* mColorB;
	/* 0x30 */ u8* mColorA;
};

/**
 * @brief A keyframed vertex color animation (bxk).
 * @details Animates vertex color channels for vertices, so there is a separate
 * GXColor animation sequence per vertex and per channel (0/1 in GX).
 */
class J3DAnmVtxColorKey : public J3DAnmVtxColor {
	friend class J3DAnmKeyLoader_v15;

public:
	J3DAnmVtxColorKey()
	{
		for (int i = 0; i < 2; i++) {
			mTable[i] = nullptr;
		}
	}

	virtual void getColor(u8 chan, u16 idx, GXColor* out) const;

	J3DAnmColorKeyTable* getAnmTable(u8 chan) const { return mTable[chan]; }

private:
	/* 0x1C */ J3DAnmColorKeyTable* mTable[2];
	/* 0x24 */ s16* mColorR;
	/* 0x28 */ s16* mColorG;
	/* 0x2C */ s16* mColorB;
	/* 0x30 */ s16* mColorA;
};

/**
 * @brief A color palette animation (bpk/bpa).
 * @details Animates a set of GXColor variables, essentially a color palette.
 */
class J3DAnmColor : public J3DAnmBase {
	friend class J3DAnmKeyLoader_v15;
	friend class J3DAnmFullLoader_v15;

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

	/**
	 * @brief Gets the current color from the specified animation sequence.
	 *
	 * @param idx The index of the animation sequence, usually index of the
	 * color in the palette.
	 * @param out The current color.
	 */
	virtual void getColor(u16 idx, GXColor* out) const { }

	void searchUpdateMaterialID(J3DModelData*);
	u16 getUpdateMaterialNum() const { return mUpdateMaterialNum; }
	u16 getUpdateMaterialID(u16 idx) const { return mUpdateMaterialID[idx]; }
	JUTNameTab* getUpdateMaterialName() { return mUpdateMaterialName; }

protected:
	/* 0x10 */ u16 unk10;
	/* 0x12 */ u16 unk12;
	/* 0x14 */ u16 unk14;
	/* 0x16 */ u16 unk16;
	/* 0x18 */ u16 mUpdateMaterialNum;
	/* 0x1C */ u16* mUpdateMaterialID;
	/* 0x20 */ JUTNameTab* mUpdateMaterialName;
};

/**
 * @brief A full color palette animation (bpa).
 * @details Animates a set of GXColor variables, essentially a color palette.
 */
class J3DAnmColorFull : public J3DAnmColor {
	friend class J3DAnmFullLoader_v15;

public:
	J3DAnmColorFull()
	{
		mColorR   = nullptr;
		mColorG   = nullptr;
		mColorB   = nullptr;
		mColorA   = nullptr;
		mAnmTable = nullptr;
	}

	virtual void getColor(u16 idx, GXColor* out) const;

private:
	/* 0x24 */ u8* mColorR;
	/* 0x28 */ u8* mColorG;
	/* 0x2C */ u8* mColorB;
	/* 0x30 */ u8* mColorA;
	/* 0x34 */ J3DAnmColorFullTable* mAnmTable;
};

/**
 * @brief A keyframed color palette animation (bpk).
 * @details Animates a set of GXColor variables, essentially a color palette.
 */
class J3DAnmColorKey : public J3DAnmColor {
	friend class J3DAnmKeyLoader_v15;

public:
	J3DAnmColorKey()
	{
		mColorR = nullptr;
		mColorG = nullptr;
		mColorB = nullptr;
		mColorA = nullptr;
		mTable  = nullptr;
	}

	virtual void getColor(u16 idx, GXColor* out) const;

private:
	/* 0x24 */ s16* mColorR;
	/* 0x28 */ s16* mColorG;
	/* 0x2C */ s16* mColorB;
	/* 0x30 */ s16* mColorA;
	/* 0x34 */ J3DAnmColorKeyTable* mTable;
};

/**
 * @brief A full model part visibility animation (bva).
 * @details Animates a set of on/off visibility flags per part.
 */
class J3DAnmVisibilityFull : public J3DAnmBase {
	friend class J3DAnmFullLoader_v15;

public:
	J3DAnmVisibilityFull()
	    : J3DAnmBase()
	{
		mUpdateMaterialNum = 0;
		mAnmTable          = nullptr;
		field_0x12         = 0;
		mVisibility        = nullptr;
		mKind              = 6;
	}
	/**
	 * @brief Gets the current visibility from the specified animation sequence.
	 *
	 * @param idx The index of the animation sequence.
	 * @param out The current visibility.
	 */
	void getVisibility(u16 idx, u8* out) const;

	J3DAnmVisibilityFullTable* getAnmTable() const { return mAnmTable; }

private:
	/* 0x10 */ u16 mUpdateMaterialNum;
	/* 0x12 */ u16 field_0x12;
	/* 0x14 */ J3DAnmVisibilityFullTable* mAnmTable;
	/* 0x18 */ u8* mVisibility;
};

/**
 * @brief A texture flipbook animation (btp).
 * @details Animates a set of sprites by switching textures, so output value is
 * the texture index in an atlas.
 */
class J3DAnmTexPattern : public J3DAnmBase {
	friend class J3DAnmFullLoader_v15;

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

	/**
	 * @brief Gets the current texture number from the specified animation
	 * sequence.
	 *
	 * @param idx The index of the animation sequence.
	 * @param out The current texture no.
	 */
	void getTexNo(u16 idx, u16* out) const;

	void searchUpdateMaterialID(J3DModelData* table);

	u16 getUpdateMaterialID(u16 idx) const
	{
		J3D_ASSERT_RANGE(514, idx < mUpdateMaterialNum / 3 && idx >= 0);
		return mUpdateMaterialID[idx];
	}
	u16 getUpdateMaterialNum() const { return mUpdateMaterialNum; }
	JUTNameTab* getUpdateMaterialName() { return mUpdateMaterialName; }

	J3DAnmTexPatternFullTable* getAnmTable() const { return mAnmTable; }

private:
	/* 0x10 */ u16* mTextureIndex;
	/* 0x14 */ J3DAnmTexPatternFullTable* mAnmTable;
	/* 0x18 */ u16 field_0x18;
	/* 0x1A */ u16 mUpdateMaterialNum;
	/* 0x1C */ u16* mUpdateMaterialID;
	/* 0x20 */ JUTNameTab* mUpdateMaterialName;
};

/**
 * @brief A keyframed TEV register animation (brk).
 * @details Animates all TEV color/konst registers, so outputs are GXColorS10
 * for C-reg and GXColor for K-reg.
 */
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

	/**
	 * @brief Gets the current value of the \p idx C-register.
	 *
	 * @param idx The C-register index.
	 * @param out Current value of the C-register.
	 */
	void getTevColorReg(u16 idx, GXColorS10* out) const;

	/**
	 * @brief Gets the current value of the \p idx K-register.
	 *
	 * @param idx The K-register index.
	 * @param out Current value of the K-register.
	 */
	void getTevKonstReg(u16 idx, GXColor* out) const;

	void searchUpdateMaterialID(J3DModelData* table);

	u16 getCRegUpdateMaterialNum() const { return mCRegUpdateMaterialNum; }
	u16 getKRegUpdateMaterialNum() const { return mKRegUpdateMaterialNum; }

	bool isValidCRegUpdateMaterialID(u16 idx) const
	{
		return mCRegUpdateMaterialID[idx] != 0xffff;
	}
	bool isValidKRegUpdateMaterialID(u16 idx) const
	{
		return mKRegUpdateMaterialID[idx] != 0xffff;
	}
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
