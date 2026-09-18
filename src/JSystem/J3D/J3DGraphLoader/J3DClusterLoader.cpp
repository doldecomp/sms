#include <JSystem/J3D/J3DGraphLoader/J3DClusterLoader.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DCluster.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>
#include <JSystem/JSupport.hpp>

// Every function in this TU is dead-stripped: the map lists six UNUSED bodies
// and keeps only the three JSUConvertOffsetToPtr instantiations that other
// TUs reference, so the byte sizes below are the *only* verification there is.
// The TU has no .rodata/.sdata2 and no surviving .data, so there are no
// literals or vtable bytes to read either.
//
// Shape evidence: J3DAnmLoader.cpp is the sibling of this file and matches it
// symbol for symbol -- load__<X>LoaderDataBase (0x700 / here 0xa0), then
// __ct__<X>_v15 (0x1c / 0x1c), the base's weak virtual __dt__ emitted just
// before the first derived dtor that needs it (0x4c / 0x4c), __dt__<X>_v15
// (0x5c / 0x5c), load__<X>_v15 and the read* workers, with the implicit
// template instantiations last. That fixes the class hierarchy (abstract base
// with a pure virtual load() and an in-class virtual dtor, a _v15 derivative
// holding the object being built) and the definition order.
//
// Instantiation order is the real oracle for readCluster's body: MWCC emits
// implicit instantiations at the end of the TU in *reverse* order of first
// use (verified on J3DAnmLoader.cpp, where all thirteen fall in that order
// including <s> inside readAnmTransform). The map's order is Us, f,
// J3DClusterVertex, J3DClusterKey, J3DCluster, ResNTAB, so first use must run
// ResNTAB, J3DCluster, J3DClusterKey, J3DClusterVertex, f32, u16 -- the name
// tables first, then the three block arrays, then the two vertex arrays, then
// the u16 index fix-up loops. Our object reproduces that order exactly.
//
// Sizes: __ct__, both __dt__s, load__J3DClusterLoader_v15 and all six
// instantiations are size-exact. Two bodies are still short:
//
// * load__24J3DClusterLoaderDataBaseFPCv is 0x84 against 0xa0. Measured
//   alternatives: `if (magic == 'J3D1') switch (type)` 0x94 (0x8c without a
//   default, 0xa4 with a second case), a second dead J3D2/bls2 branch 0xa8
//   (0xac unhoisted), building the loader on the heap 0xc8, and calling
//   load() *virtually* through a J3DClusterLoader* 0x9c -- 0xa0 falls between
//   the last two, so one statement is still missing and none of the probed
//   shapes is it.
// * readCluster is 0x27c against 0x2bc. Dropping the cached
//   J3DCluster*/J3DClusterKey* loop temporaries (each JSUConvertOffsetToPtr
//   call is a `bl`, so every statement then reloads mDeformData and the
//   array base) buys 0x14/0x4/0x4 per loop: the eight combinations measure
//   0x27c, 0x280, 0x280, 0x294, 0x29c, 0x2b0, 0x2b0 and 0x2cc, so 0x2bc is
//   not on that axis either. The 16 missing instructions are something else;
//   allocating the J3DDeformers here was rejected because
//   J3DModel::setDeformData (UNUSED, 0x34) is the natural owner of that work.

J3DDeformData* J3DClusterLoaderDataBase::load(const void* i_data)
{
	if (i_data == nullptr)
		return nullptr;

	const JUTDataFileHeader* fileHeader = (const JUTDataFileHeader*)i_data;

	// fabricated: 'bls1' is a guess -- nothing in the binary names the file
	// type, since the only caller was dead-stripped too.
	if (fileHeader->mMagic == 'J3D1' && fileHeader->mType == 'bls1') {
		J3DClusterLoader_v15 loader;
		return loader.load(i_data);
	}

	return nullptr;
}

J3DClusterLoader_v15::J3DClusterLoader_v15() { }
J3DClusterLoader_v15::~J3DClusterLoader_v15() { }

J3DDeformData* J3DClusterLoader_v15::load(const void* i_data)
{
	mDeformData = new J3DDeformData();
	mDeformData->clear();
	const JUTDataFileHeader* header = (const JUTDataFileHeader*)i_data;
	const JUTDataBlockHeader* block = &header->mFirstBlock;
	for (u32 i = 0; i < header->mBlockNum; i++) {
		switch (block->mType) {
		// fabricated: block id guessed from the J3D naming scheme.
		case 'CLS1':
			readCluster((J3DClusterBlock*)block);
			break;
		default:
			break;
		}
		block = (const JUTDataBlockHeader*)((u8*)block + block->mSize);
	}
	return mDeformData;
}

void J3DClusterLoader_v15::readCluster(const J3DClusterBlock* i_block)
{
	if (i_block->mpClusterName != nullptr) {
		mDeformData->mClusterName = new JUTNameTab(
		    JSUConvertOffsetToPtr<ResNTAB>(i_block, i_block->mpClusterName));
	} else {
		mDeformData->mClusterName = nullptr;
	}
	if (i_block->mpClusterKeyName != nullptr) {
		mDeformData->mClusterKeyName = new JUTNameTab(
		    JSUConvertOffsetToPtr<ResNTAB>(i_block, i_block->mpClusterKeyName));
	} else {
		mDeformData->mClusterKeyName = nullptr;
	}

	mDeformData->mClusterNum = i_block->mClusterNum;
	mDeformData->mClusters
	    = JSUConvertOffsetToPtr<J3DCluster>(i_block, i_block->mpCluster);
	mDeformData->mClusterKeyNum = i_block->mClusterKeyNum;
	mDeformData->mClusterKeys
	    = JSUConvertOffsetToPtr<J3DClusterKey>(i_block, i_block->mpClusterKey);
	mDeformData->mClusterVertexNum = i_block->mClusterVertexNum;
	mDeformData->mClusterVertices = JSUConvertOffsetToPtr<J3DClusterVertex>(
	    i_block, i_block->mpClusterVertex);
	mDeformData->mVtxPosNum = i_block->mVtxPosNum;
	mDeformData->mVtxPos
	    = JSUConvertOffsetToPtr<f32>(i_block, i_block->mpVtxPos);
	mDeformData->mVtxNrmNum = i_block->mVtxNrmNum;
	mDeformData->mVtxNrm
	    = JSUConvertOffsetToPtr<f32>(i_block, i_block->mpVtxNrm);

	for (u16 i = 0; i < mDeformData->mClusterNum; i++) {
		J3DCluster* cluster = mDeformData->getClusterPointer(i);
		cluster->mPosDstIdx
		    = JSUConvertOffsetToPtr<u16>(i_block, cluster->mPosDstIdx);
		cluster->mClusterKey = JSUConvertOffsetToPtr<J3DClusterKey>(
		    i_block, cluster->mClusterKey);
		cluster->mClusterVertex = JSUConvertOffsetToPtr<J3DClusterVertex>(
		    i_block, cluster->mClusterVertex);
	}

	for (u16 i = 0; i < mDeformData->mClusterKeyNum; i++) {
		J3DClusterKey* key = mDeformData->getClusterKeyPointer(i);
		key->mPosFlag = JSUConvertOffsetToPtr<u16>(i_block, key->mPosFlag);
		key->mNrmFlag = JSUConvertOffsetToPtr<u16>(i_block, key->mNrmFlag);
	}

	for (u16 i = 0; i < mDeformData->mClusterVertexNum; i++) {
		J3DClusterVertex* vertex = &mDeformData->mClusterVertices[i];
		vertex->mSrcIdx = JSUConvertOffsetToPtr<u16>(i_block, vertex->mSrcIdx);
		vertex->mDstIdx = JSUConvertOffsetToPtr<u16>(i_block, vertex->mDstIdx);
	}
}
