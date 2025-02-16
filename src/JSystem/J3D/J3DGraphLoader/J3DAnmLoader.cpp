#include <JSystem/J3D/J3DGraphLoader/J3DAnmLoader.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <JSystem/JSupport.hpp>

J3DAnmBase* J3DAnmLoaderDataBase::load(const void* i_data)
{
	if (!i_data)
		return nullptr;

	const JUTDataFileHeader* header = ((const JUTDataFileHeader*)i_data);
	if (header->mMagic == 'J3D1') {
		switch (header->mType) {
		case 'bck1': {
			J3DAnmKeyLoader_v15 loader;
			loader.mAnm = new J3DAnmTransformKey();
			return loader.load(i_data);
		}
		case 'bpk1': {
			J3DAnmKeyLoader_v15 loader;
			loader.mAnm = new J3DAnmColorKey();
			return loader.load(i_data);
		}
		case 'blk1': {
			J3DAnmKeyLoader_v15 loader;
			loader.mAnm = new J3DAnmClusterKey();
			return loader.load(i_data);
		}
		case 'btk1': {
			J3DAnmKeyLoader_v15 loader;
			loader.mAnm = new J3DAnmTextureSRTKey();
			return loader.load(i_data);
		}
		case 'brk1': {
			J3DAnmKeyLoader_v15 loader;
			loader.mAnm = new J3DAnmTevRegKey();
			return loader.load(i_data);
		}
		case 'bxk1': {
			J3DAnmKeyLoader_v15 loader;
			loader.mAnm = new J3DAnmVtxColorKey();
			return loader.load(i_data);
		}
		case 'bca1': {
			J3DAnmFullLoader_v15 loader;
			loader.mAnm = new J3DAnmTransformFull();
			return loader.load(i_data);
		}
		case 'bpa1': {
			J3DAnmFullLoader_v15 loader;
			loader.mAnm = new J3DAnmColorFull();
			return loader.load(i_data);
		}
		case 'btp1': {
			J3DAnmFullLoader_v15 loader;
			loader.mAnm = new J3DAnmTexPattern();
			return loader.load(i_data);
		}
		case 'bva1': {
			J3DAnmFullLoader_v15 loader;
			loader.mAnm = new J3DAnmVisibilityFull();
			return loader.load(i_data);
		}
		case 'bla1': {
			J3DAnmFullLoader_v15 loader;
			loader.mAnm = new J3DAnmClusterFull();
			return loader.load(i_data);
		}
		case 'bxa1': {
			J3DAnmFullLoader_v15 loader;
			loader.mAnm = new J3DAnmVtxColorFull();
			return loader.load(i_data);
		}
		default:
			return nullptr;
		}
	}
	return nullptr;
}

J3DAnmFullLoader_v15::J3DAnmFullLoader_v15() { }
J3DAnmFullLoader_v15::~J3DAnmFullLoader_v15() { }
J3DAnmKeyLoader_v15::J3DAnmKeyLoader_v15() { }
J3DAnmKeyLoader_v15::~J3DAnmKeyLoader_v15() { }

J3DAnmBase* J3DAnmFullLoader_v15::load(const void* data)
{
	const JUTDataFileHeader* header = (const JUTDataFileHeader*)data;
	const JUTDataBlockHeader* block = &header->mFirstBlock;
	for (int i = 0; i < header->mBlockNum; i++) {
		switch (block->mType) {
		case 'ANF1':
			readAnmTransform((J3DAnmTransformFullData*)block);
			break;
		case 'PAF1':
			readAnmColor((J3DAnmColorFullData*)block);
			break;
		case 'TPT1':
			readAnmTexPattern((J3DAnmTexPatternFullData*)block);
			break;
		case 'CLF1':
			readAnmCluster((J3DAnmClusterFullData*)block);
			break;
		case 'VAF1':
			readAnmVisibility((J3DAnmVisibilityFullData*)block);
			break;
		case 'VCF1':
			readAnmVtxColor((J3DAnmVtxColorFullData*)block);
			break;
		default:
			break;
		}
		block = (const JUTDataBlockHeader*)((u8*)block + block->mSize);
	}
	return mAnm;
}
void J3DAnmFullLoader_v15::readAnmTransform(const J3DAnmTransformFullData* data)
{
	J3DAnmTransformFull* dst = (J3DAnmTransformFull*)mAnm;
	dst->field_0x22          = data->field_0xc;
	dst->mMaxFrame           = data->mFrameMax;
	dst->mAttribute          = data->field_0x8;
	dst->mFrame              = 0.0f;
	dst->mAnmTable           = JSUConvertOffsetToPtr<J3DAnmTransformFullTable>(
        data, (void*)data->mTableOffset);
	dst->mScaleData
	    = JSUConvertOffsetToPtr<f32>(data, (void*)data->mScaleValOffset);
	dst->mRotData
	    = JSUConvertOffsetToPtr<s16>(data, (void*)data->mRotValOffset);
	dst->mTransData
	    = JSUConvertOffsetToPtr<f32>(data, (void*)data->mTransValOffset);
}

void J3DAnmFullLoader_v15::readAnmColor(const J3DAnmColorFullData* data)
{
	J3DAnmColorFull* dst    = (J3DAnmColorFull*)mAnm;
	dst->mMaxFrame          = data->mFrameMax;
	dst->mAttribute         = data->field_0x8;
	dst->mFrame             = 0.0f;
	dst->mUpdateMaterialNum = data->mUpdateMaterialNum;
	dst->mAnmTable          = JSUConvertOffsetToPtr<J3DAnmColorFullTable>(
        data, (void*)data->mTableOffset);
	dst->mColorR = JSUConvertOffsetToPtr<u8>(data, (void*)data->mRValuesOffset);
	dst->mColorG = JSUConvertOffsetToPtr<u8>(data, (void*)data->mGValuesOffset);
	dst->mColorB = JSUConvertOffsetToPtr<u8>(data, (void*)data->mBValuesOffset);
	dst->mColorA = JSUConvertOffsetToPtr<u8>(data, (void*)data->mAValuesOffset);
	dst->mUpdateMaterialID = JSUConvertOffsetToPtr<u16>(
	    data, (void*)data->mUpdateMaterialIDOffset);
	dst->mUpdateMaterialName = new JUTNameTab(
	    JSUConvertOffsetToPtr<ResNTAB>(data, (void*)data->mNameTabOffset));
}

void J3DAnmFullLoader_v15::readAnmTexPattern(
    const J3DAnmTexPatternFullData* data)
{
	J3DAnmTexPattern* dst   = (J3DAnmTexPattern*)mAnm;
	dst->mMaxFrame          = data->mFrameMax;
	dst->mAttribute         = data->field_0x8;
	dst->mFrame             = 0.0f;
	dst->mUpdateMaterialNum = data->field_0xc;
	dst->field_0x18         = data->field_0xe;
	dst->mAnmTable          = JSUConvertOffsetToPtr<J3DAnmTexPatternFullTable>(
        data, (void*)data->mTableOffset);
	dst->mTextureIndex
	    = JSUConvertOffsetToPtr<u16>(data, (void*)data->mValuesOffset);
	dst->mUpdateMaterialID = JSUConvertOffsetToPtr<u16>(
	    data, (void*)data->mUpdateMaterialIDOffset);
	dst->mUpdateMaterialName = new JUTNameTab(
	    JSUConvertOffsetToPtr<ResNTAB>(data, (void*)data->mNameTabOffset));
}
void J3DAnmFullLoader_v15::readAnmVisibility(
    const J3DAnmVisibilityFullData* data)
{
	J3DAnmVisibilityFull* dst = (J3DAnmVisibilityFull*)mAnm;
	dst->mMaxFrame            = data->mFrameMax;
	dst->mAttribute           = data->field_0x8;
	dst->mFrame               = 0.0f;
	dst->mUpdateMaterialNum   = data->field_0xc;
	dst->field_0x12           = data->field_0xe;
	dst->mAnmTable = JSUConvertOffsetToPtr<J3DAnmVisibilityFullTable>(
	    data, (void*)data->mTableOffset);
	dst->mVisibility
	    = JSUConvertOffsetToPtr<u8>(data, (void*)data->mValuesOffset);
}
void J3DAnmFullLoader_v15::readAnmCluster(const J3DAnmClusterFullData* data)
{
	J3DAnmClusterFull* dst = (J3DAnmClusterFull*)mAnm;
	dst->mMaxFrame         = data->mFrameMax;
	dst->mAttribute        = data->field_0x8;
	dst->mFrame            = 0.0f;
	dst->mAnmTable         = JSUConvertOffsetToPtr<J3DAnmClusterFullTable>(
        data, (void*)data->mTableOffset);
	dst->mWeight = JSUConvertOffsetToPtr<f32>(data, (void*)data->mWeightOffset);
}
void J3DAnmFullLoader_v15::readAnmVtxColor(const J3DAnmVtxColorFullData* data)
{
	J3DAnmVtxColorFull* dst = (J3DAnmVtxColorFull*)mAnm;
	dst->mMaxFrame          = data->mFrameMax;
	dst->mAttribute         = data->field_0x8;
	dst->mFrame             = 0.0f;
	dst->mAnmTableNum[0]    = data->mAnmTableNum[0];
	dst->mAnmTableNum[1]    = data->mAnmTableNum[1];
	dst->mTable[0]          = JSUConvertOffsetToPtr<J3DAnmColorFullTable>(
        data, (void*)data->mTableOffsets[0]);
	dst->mTable[1] = JSUConvertOffsetToPtr<J3DAnmColorFullTable>(
	    data, (void*)data->mTableOffsets[1]);
	dst->mAnmVtxColorIndexData[0]
	    = JSUConvertOffsetToPtr<J3DAnmVtxColorIndexData>(
	        data, (void*)data->mVtxColorIndexDataOffsets[0]);
	dst->mAnmVtxColorIndexData[1]
	    = JSUConvertOffsetToPtr<J3DAnmVtxColorIndexData>(
	        data, (void*)data->mVtxColorIndexDataOffsets[1]);
	void* indexPtr0 = JSUConvertOffsetToPtr<u16>(
	    data, (void*)data->mVtxColorIndexPointerOffsets[0]);
	void* indexPtr1 = JSUConvertOffsetToPtr<u16>(
	    data, (void*)data->mVtxColorIndexPointerOffsets[1]);

	for (s32 i = 0; i < dst->mAnmTableNum[0]; i++)
		dst->mAnmVtxColorIndexData[0][i].mpData
		    = (void*)((s32)indexPtr0
		              + (s32)dst->mAnmVtxColorIndexData[0][i].mpData * 2);
	for (s32 i = 0; i < dst->mAnmTableNum[1]; i++)
		dst->mAnmVtxColorIndexData[1][i].mpData
		    = (void*)((s32)indexPtr1
		              + (s32)dst->mAnmVtxColorIndexData[1][i].mpData * 2);

	dst->mColorR = JSUConvertOffsetToPtr<u8>(data, (void*)data->mRValuesOffset);
	dst->mColorG = JSUConvertOffsetToPtr<u8>(data, (void*)data->mGValuesOffset);
	dst->mColorB = JSUConvertOffsetToPtr<u8>(data, (void*)data->mBValuesOffset);
	dst->mColorA = JSUConvertOffsetToPtr<u8>(data, (void*)data->mAValuesOffset);
}

J3DAnmBase* J3DAnmKeyLoader_v15::load(const void* data)
{
	const JUTDataFileHeader* header = (const JUTDataFileHeader*)data;
	const JUTDataBlockHeader* block = &header->mFirstBlock;
	for (int i = 0; i < header->mBlockNum; i++) {
		switch (block->mType) {
		case 'ANK1':
			readAnmTransform((J3DAnmTransformKeyData*)block);
			break;
		case 'PAK1':
			readAnmColor((J3DAnmColorKeyData*)block);
			break;
		case 'CLK1':
			readAnmCluster((J3DAnmClusterKeyData*)block);
			break;
		case 'TTK1':
			readAnmTextureSRT((J3DAnmTextureSRTKeyData*)block);
			break;
		case 'TRK1':
			readAnmTevReg((J3DAnmTevRegKeyData*)block);
			break;
		case 'VCK1':
			readAnmVtxColor((J3DAnmVtxColorKeyData*)block);
			break;
		default:
			break;
		}
		block = (const JUTDataBlockHeader*)((u8*)block + block->mSize);
	}
	return mAnm;
}

void J3DAnmKeyLoader_v15::readAnmTransform(const J3DAnmTransformKeyData* data)
{
	J3DAnmTransformKey* dst = (J3DAnmTransformKey*)mAnm;
	dst->field_0x22         = data->field_0xc;
	dst->mMaxFrame          = data->mFrameMax;
	dst->mAttribute         = data->mAttribute;
	dst->mDecShift          = data->mDecShift;
	dst->mFrame             = 0.0f;
	dst->mAnmTable          = JSUConvertOffsetToPtr<J3DAnmTransformKeyTable>(
        data, (void*)data->mTableOffset);
	dst->mScaleData
	    = JSUConvertOffsetToPtr<f32>(data, (void*)data->mScaleOffset);
	dst->mRotData = JSUConvertOffsetToPtr<s16>(data, (void*)data->mRotOffset);
	dst->mTransData
	    = JSUConvertOffsetToPtr<f32>(data, (void*)data->mTransOffset);
}

void J3DAnmKeyLoader_v15::readAnmTextureSRT(const J3DAnmTextureSRTKeyData* data)
{
	J3DAnmTextureSRTKey* dst = (J3DAnmTextureSRTKey*)mAnm;
	dst->mTrackNum           = data->field_0xc;
	dst->mMaxFrame           = data->field_0xa;
	dst->mAttribute          = data->field_0x8;
	dst->mDecShift           = data->field_0x9;
	dst->mFrame              = 0.0f;
	dst->mTrackNum           = data->field_0xc;
	dst->mScaleNum           = data->field_0xe;
	dst->mRotNum             = data->field_0x10;
	dst->mTransNum           = data->field_0x12;
	dst->mAnmTable           = JSUConvertOffsetToPtr<J3DAnmTransformKeyTable>(
        data, (void*)data->mTableOffset);
	dst->mUpdateMaterialID
	    = JSUConvertOffsetToPtr<u16>(data, (void*)data->mUpdateMatIDOffset);
	dst->mUpdateMaterialName = new JUTNameTab(
	    JSUConvertOffsetToPtr<ResNTAB>(data, (void*)data->mNameTab1Offset));
	dst->mUpdateTexMtxID
	    = JSUConvertOffsetToPtr<u8>(data, (void*)data->mUpdateTexMtxIDOffset);
	dst->mSRTCenter = JSUConvertOffsetToPtr<Vec>(data, (void*)data->unkOffset);
	dst->mScaleData
	    = JSUConvertOffsetToPtr<f32>(data, (void*)data->mScaleValOffset);
	dst->mRotData
	    = JSUConvertOffsetToPtr<s16>(data, (void*)data->mRotValOffset);
	dst->mTransData
	    = JSUConvertOffsetToPtr<f32>(data, (void*)data->mTransValOffset);
	if (data->mNameTab2Offset) {
		dst->mPostUpdateMaterialName = new JUTNameTab(
		    JSUConvertOffsetToPtr<ResNTAB>(data, (void*)data->mNameTab2Offset));
	} else {
		dst->mPostUpdateMaterialName = nullptr;
	}
	dst->mPostTrackNum = data->field_0x34;
	dst->unk40         = data->field_0x36;
	dst->unk42         = data->field_0x38;
	dst->unk44         = data->field_0x3a;
	dst->unk58         = JSUConvertOffsetToPtr<J3DAnmTransformKeyTable>(
        data, (void*)data->mInfoTable2Offset);
	dst->mPostUpdateMaterialID
	    = JSUConvertOffsetToPtr<u16>(data, (void*)data->field_0x40);
	dst->mPostUpdateTexMtxID
	    = JSUConvertOffsetToPtr<u8>(data, (void*)data->field_0x48);
	dst->mPostSRTCenter
	    = JSUConvertOffsetToPtr<Vec>(data, (void*)data->field_0x4c);
	dst->unk48 = JSUConvertOffsetToPtr<f32>(data, (void*)data->field_0x50);
	dst->unk4C = JSUConvertOffsetToPtr<s16>(data, (void*)data->field_0x54);
	dst->unk50 = JSUConvertOffsetToPtr<f32>(data, (void*)data->field_0x58);
	dst->mTexMtxCalcType = 0;
}

void J3DAnmKeyLoader_v15::readAnmColor(const J3DAnmColorKeyData* data)
{
	J3DAnmColorKey* dst     = (J3DAnmColorKey*)mAnm;
	dst->mMaxFrame          = data->mFrameMax;
	dst->mAttribute         = data->field_0x8;
	dst->mFrame             = 0.0f;
	dst->mUpdateMaterialNum = data->mUpdateMaterialNum;
	dst->unk10              = data->field_0x10;
	dst->unk12              = data->field_0x12;
	dst->unk14              = data->field_0x14;
	dst->unk16              = data->field_0x16;
	dst->mTable             = JSUConvertOffsetToPtr<J3DAnmColorKeyTable>(
        data, (void*)data->mTableOffset);
	dst->mColorR = JSUConvertOffsetToPtr<s16>(data, (void*)data->mRValOffset);
	dst->mColorG = JSUConvertOffsetToPtr<s16>(data, (void*)data->mGValOffset);
	dst->mColorB = JSUConvertOffsetToPtr<s16>(data, (void*)data->mBValOffset);
	dst->mColorA = JSUConvertOffsetToPtr<s16>(data, (void*)data->mAValOffset);
	dst->mUpdateMaterialID = JSUConvertOffsetToPtr<u16>(
	    data, (void*)data->mUpdateMaterialIDOffset);
	dst->mUpdateMaterialName = new JUTNameTab(
	    JSUConvertOffsetToPtr<ResNTAB>(data, (void*)data->mNameTabOffset));
}

void J3DAnmKeyLoader_v15::readAnmCluster(const J3DAnmClusterKeyData* data)
{
	J3DAnmClusterKey* dst = (J3DAnmClusterKey*)mAnm;
	dst->mMaxFrame        = data->mFrameMax;
	dst->mAttribute       = data->field_0x8;
	dst->mFrame           = 0.0f;
	dst->mAnmTable        = JSUConvertOffsetToPtr<J3DAnmClusterKeyTable>(
        data, (void*)data->mTableOffset);
	dst->mWeight = JSUConvertOffsetToPtr<f32>(data, (void*)data->mWeightOffset);
}

void J3DAnmKeyLoader_v15::readAnmTevReg(const J3DAnmTevRegKeyData* data)
{
	J3DAnmTevRegKey* dst        = (J3DAnmTevRegKey*)mAnm;
	dst->mMaxFrame              = data->mFrameMax;
	dst->mAttribute             = data->field_0x8;
	dst->mFrame                 = 0.0f;
	dst->mCRegUpdateMaterialNum = data->mCRegUpdateMaterialNum;
	dst->mAnmCRegKeyTable       = JSUConvertOffsetToPtr<J3DAnmCRegKeyTable>(
        data, (void*)data->mCRegTableOffset);
	dst->mCRegUpdateMaterialID = JSUConvertOffsetToPtr<u16>(
	    data, (void*)data->mCRegUpdateMaterialIDOffset);
	dst->mCRegUpdateMaterialName = new JUTNameTab(
	    JSUConvertOffsetToPtr<ResNTAB>(data, (void*)data->mCRegNameTabOffset));
	dst->mKRegUpdateMaterialNum = data->mKRegUpdateMaterialNum;
	dst->mAnmKRegKeyTable       = JSUConvertOffsetToPtr<J3DAnmKRegKeyTable>(
        data, (void*)data->mKRegTableOffset);
	dst->mKRegUpdateMaterialID = JSUConvertOffsetToPtr<u16>(
	    data, (void*)data->mKRegUpdateMaterialIDOffset);
	dst->mKRegUpdateMaterialName = new JUTNameTab(
	    JSUConvertOffsetToPtr<ResNTAB>(data, (void*)data->mKRegNameTabOffset));
	dst->mCRegDataCountR = data->field_0x10;
	dst->mCRegDataCountG = data->field_0x12;
	dst->mCRegDataCountB = data->field_0x14;
	dst->mCRegDataCountA = data->field_0x16;
	dst->mAnmCRegDataR
	    = JSUConvertOffsetToPtr<s16>(data, (void*)data->mCRValuesOffset);
	dst->mAnmCRegDataG
	    = JSUConvertOffsetToPtr<s16>(data, (void*)data->mCGValuesOffset);
	dst->mAnmCRegDataB
	    = JSUConvertOffsetToPtr<s16>(data, (void*)data->mCBValuesOffset);
	dst->mAnmCRegDataA
	    = JSUConvertOffsetToPtr<s16>(data, (void*)data->mCAValuesOffset);
	dst->mKRegDataCountR = data->field_0x18;
	dst->mKRegDataCountG = data->field_0x1a;
	dst->mKRegDataCountB = data->field_0x1c;
	dst->mKRegDataCountA = data->field_0x1e;
	dst->mAnmKRegDataR
	    = JSUConvertOffsetToPtr<s16>(data, (void*)data->mKRValuesOffset);
	dst->mAnmKRegDataG
	    = JSUConvertOffsetToPtr<s16>(data, (void*)data->mKGValuesOffset);
	dst->mAnmKRegDataB
	    = JSUConvertOffsetToPtr<s16>(data, (void*)data->mKBValuesOffset);
	dst->mAnmKRegDataA
	    = JSUConvertOffsetToPtr<s16>(data, (void*)data->mKAValuesOffset);
}

void J3DAnmKeyLoader_v15::readAnmVtxColor(const J3DAnmVtxColorKeyData* data)
{
	J3DAnmVtxColorKey* dst = (J3DAnmVtxColorKey*)mAnm;
	dst->mMaxFrame         = data->mFrameMax;
	dst->mAttribute        = data->field_0x8;
	dst->mFrame            = 0.0f;
	dst->mAnmTableNum[0]   = data->mAnmTableNum[0];
	dst->mAnmTableNum[1]   = data->mAnmTableNum[1];
	dst->mTable[0]         = JSUConvertOffsetToPtr<J3DAnmColorKeyTable>(
        data, (void*)data->mTableOffsets[0]);
	dst->mTable[1] = JSUConvertOffsetToPtr<J3DAnmColorKeyTable>(
	    data, (void*)data->mTableOffsets[1]);
	dst->mAnmVtxColorIndexData[0]
	    = JSUConvertOffsetToPtr<J3DAnmVtxColorIndexData>(
	        data, (void*)data->mVtxColorIndexDataOffsets[0]);
	dst->mAnmVtxColorIndexData[1]
	    = JSUConvertOffsetToPtr<J3DAnmVtxColorIndexData>(
	        data, (void*)data->mVtxColorIndexDataOffsets[1]);
	void* indexPtr0 = JSUConvertOffsetToPtr<u16>(
	    data, (void*)data->mVtxColorIndexPointerOffsets[0]);
	void* indexPtr1 = JSUConvertOffsetToPtr<u16>(
	    data, (void*)data->mVtxColorIndexPointerOffsets[1]);

	for (s32 i = 0; i < dst->mAnmTableNum[0]; i++)
		dst->mAnmVtxColorIndexData[0][i].mpData
		    = (void*)((s32)indexPtr0
		              + (s32)dst->mAnmVtxColorIndexData[0][i].mpData * 2);
	for (s32 i = 0; i < dst->mAnmTableNum[1]; i++)
		dst->mAnmVtxColorIndexData[1][i].mpData
		    = (void*)((s32)indexPtr1
		              + (s32)dst->mAnmVtxColorIndexData[1][i].mpData * 2);

	dst->mColorR
	    = JSUConvertOffsetToPtr<s16>(data, (void*)data->mRValuesOffset);
	dst->mColorG
	    = JSUConvertOffsetToPtr<s16>(data, (void*)data->mGValuesOffset);
	dst->mColorB
	    = JSUConvertOffsetToPtr<s16>(data, (void*)data->mBValuesOffset);
	dst->mColorA
	    = JSUConvertOffsetToPtr<s16>(data, (void*)data->mAValuesOffset);
}
