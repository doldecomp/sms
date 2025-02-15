#include <JSystem/J3D/J3DGraphAnimator/J3DCluster.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DShape.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTransform.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <dolphin/os/OSCache.h>
#include <math.h>

#pragma opt_strength_reduction off

void J3DDeformData::deform(J3DModel* model)
{
	model->getVertexBuffer()->swapVtxPosArrayPointer();
	model->getVertexBuffer()->swapVtxNrmArrayPointer();

	for (u16 i = 0; i < mClusterNum; i++)
		mClusters[i].getDeformer()->deform(model, i);

	DCStoreRange(model->getVertexBuffer()->getVtxPosArrayPointer(0),
	             model->getModelData()->getVtxNum() * 12);
	DCStoreRange(model->getVertexBuffer()->getVtxNrmArrayPointer(0),
	             model->getModelData()->getNrmNum() * 12);

	model->getVertexBuffer()->setCurrentVtxPos(
	    model->getVertexBuffer()->getVtxPosArrayPointer(0));
	model->getVertexBuffer()->setCurrentVtxNrm(
	    model->getVertexBuffer()->getVtxNrmArrayPointer(0));
}

void J3DDeformData::setAnm(J3DAnmCluster* anm)
{
	for (u16 i = 0; i < mClusterNum; i++)
		mClusters[i].getDeformer()->mAnmCluster = anm;
}

void J3DDeformer::deform(J3DModel* model, u16 idx)
{
	u16 keyIdx = 0;
	if (mAnmCluster) {
		for (u16 i = 0; i < idx; i++)
			keyIdx += mDeformData->getClusterPointer(i)->mKeyNum;

		for (u16 i = 0; i < mDeformData->getClusterPointer(idx)->mKeyNum; i++)
			mWeightList[i] = mAnmCluster->getWeight(keyIdx++);

		deform(model, idx, mWeightList);
	}
}

void J3DDeformer::deform(J3DModel* model, u16 idx, float* weightList)
{
	if ((mFlags & 2 ? 1 : 0)
	    && model->getModelData()->isDeformableVertexFormat()) {
		J3DCluster* cluster = mDeformData->getClusterPointer(idx);
		s32 posNum          = cluster->mPosNum;
		s32 keyNum          = cluster->mKeyNum;

		u16 keyStart = 0;
		for (u16 i = 0; i < idx; i++)
			keyStart += mDeformData->getClusterPointer(i)->mKeyNum + 1;

		J3DClusterKey* key = mDeformData->getClusterKeyPointer(keyStart);
		normalizeWeight(keyNum, weightList);

		f32* vtxPosDst
		    = (f32*)model->getVertexBuffer()->getVtxPosArrayPointer(0);
		f32* vtxPosSrc = (f32*)mDeformData->getVtxPos();
		u16* posDstIdx = cluster->mPosDstIdx;

		for (s32 i = 0; i < posNum; i++) {
			u32 idx  = posDstIdx[i] * 3;
			f32* vtx = &vtxPosDst[idx];
			vtx[0]   = 0.0f;
			vtx[1]   = 0.0f;
			vtx[2]   = 0.0f;
		}

		// TODO: something very wrong here
		f32 sign[2] = { 1.0f, -1.0f };
		for (u16 i = 0; i < posNum; i++) {
			for (u16 j = 0; j < keyNum; j++) {
				u16 flag = (u16)key[j].mPosFlag[i];
				f32* src = &vtxPosSrc[(flag & 0x1FFF) * 3];
				f32* dst = &vtxPosDst[cluster->mPosDstIdx[i] * 3];
				dst[0] += weightList[j] * src[0] * sign[(flag >> 15) & 1];
				dst[1] += weightList[j] * src[1] * sign[(flag >> 14) & 1];
				dst[2] += weightList[j] * src[2] * sign[(flag >> 13) & 1];
			}
		}

		if ((mFlags & 1 ? 1 : 0) && cluster->mFlags != 0) {
			f32* vtxNrmDst
			    = (f32*)model->getVertexBuffer()->getVtxNrmArrayPointer(0);
			f32* vtxNrmSrc = mDeformData->getVtxNrm();
			f32* nrmBuf    = field_0x0c;

			for (u16 i = 0; i < cluster->mNrmNum; i++) {
				f32* dst = &nrmBuf[i * 3];
				dst[0]   = 0.0f;
				dst[1]   = 0.0f;
				dst[2]   = 0.0f;

				for (u16 j = 0; j < cluster->mKeyNum; j++) {
					u16 flag = key[j].mNrmFlag[i];
					f32* src = &vtxNrmSrc[(flag & 0x1FFF) * 3];

					f32 srcX = src[0];
					f32 srcY = src[1];
					f32 srcZ = src[2];

					if (flag & 0x8000)
						srcX = -srcX;
					if (flag & 0x4000)
						srcY = -srcY;
					if (flag & 0x2000)
						srcZ = -srcZ;

					dst[0] += srcX * weightList[j];
					dst[1] += srcY * weightList[j];
					dst[2] += srcZ * weightList[j];
				}

				normalize(dst);
			}

			for (u16 i = 0; i < cluster->mClusterVertexNum; i++) {
				J3DClusterVertex* vertex = &cluster->mClusterVertex[i];
				f32 pos[3];
				pos[0] = 0.0f;
				pos[1] = 0.0f;
				pos[2] = 0.0f;

				f32 weight = 1.0f / (float)vertex->mNum;
				for (u16 j = 0; j < vertex->mNum; j++) {
					f32* src = &nrmBuf[vertex->mSrcIdx[j] * 3];
					pos[0] += weight * src[0];
					pos[1] += weight * src[1];
					pos[2] += weight * src[2];
				}
				normalize(pos);

				for (u16 j = 0; j < vertex->mNum; j++) {
					u16 srcIdx = vertex->mSrcIdx[j];
					u16 dstIdx = vertex->mDstIdx[j];

					if (dstIdx == 0xFFFF)
						continue;

					f32* src = &nrmBuf[srcIdx * 3];
					f32 dot
					    = pos[0] * src[0] + pos[1] * src[1] + pos[2] * src[2];

					f32 angle;
					if (dot >= 1.0f) {
						angle = 0.0f;
					} else if (dot > -1.0f) {
						angle = acosf(dot);
						angle = (angle * 180.0f) / 3.1415f;
					} else {
						angle = 180.0f;
					}

					if (angle <= cluster->mMinAngle) {
						vtxNrmDst[dstIdx * 3 + 0] = pos[0];
						vtxNrmDst[dstIdx * 3 + 1] = pos[1];
						vtxNrmDst[dstIdx * 3 + 2] = pos[2];
					} else if (angle > cluster->mMaxAngle) {
						vtxNrmDst[dstIdx * 3 + 0] = src[0];
						vtxNrmDst[dstIdx * 3 + 1] = src[1];
						vtxNrmDst[dstIdx * 3 + 2] = src[2];
					} else {
						f32 weight
						    = (angle - cluster->mMinAngle)
						      / (cluster->mMaxAngle - cluster->mMinAngle);
						f32 inv = 1.0f - weight;

						vtxNrmDst[dstIdx * 3 + 0]
						    = inv * pos[0] + weight * src[0];
						vtxNrmDst[dstIdx * 3 + 1]
						    = inv * pos[1] + weight * src[1];
						vtxNrmDst[dstIdx * 3 + 2]
						    = inv * pos[2] + weight * src[2];
					}
				}
			}
		}
	}
}

void J3DDeformer::normalize(float* vec)
{
	f32 inv = 1.0f
	          / std::sqrtf(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
	vec[0] *= inv;
	vec[1] *= inv;
	vec[2] *= inv;
}

void J3DDeformer::normalizeWeight(int count, float* weight)
{
	f32 sum = 0.0f;
	for (u16 i = 0; i < count; i++)
		sum += weight[i];
	sum = 1.0f / sum;
	for (u16 i = 0; i < count; i++)
		weight[i] *= sum;
}

J3DSkinDeform::J3DSkinDeform()
{
	mPosUseMtx = nullptr;
	mNrmUseMtx = nullptr;
	mNrmMtx    = nullptr;
	mFlags     = 3;
}

void J3DSkinDeform::initMtxIndexArray(J3DModelData* modelData)
{

	if (mPosUseMtx != nullptr && mNrmUseMtx != nullptr)
		return;

	mPosUseMtx = new u16[modelData->getVtxNum()];

	for (u32 i = 0; i < modelData->getVtxNum(); i++)
		mPosUseMtx[i] = 0;

	if (modelData->getNrmNum() != 0) {
		mNrmUseMtx = new u16[modelData->getNrmNum()];

		for (u32 i = 0; i < modelData->getNrmNum(); i++)
			mNrmUseMtx[i] = 0;
	} else {
		mNrmUseMtx = nullptr;
	}

	mNrmMtx = new (0x20) Mtx33[modelData->getDrawMtxNum()];

	for (u16 i = 0; i < modelData->getShapeNum(); i++) {
		int size[4] = { 0, 1, 1, 2 };

		int pnmtxIdxOffs = -1;
		int posOffs      = -1;
		int nrmOffs      = -1;
		int vtxSize      = 0;
		for (GXVtxDescList* desc
		     = modelData->getShapeNodePointer(i)->getVtxDesc();
		     desc->attr != GX_VA_NULL; desc++) {
			switch (desc->attr) {
			case GX_VA_PNMTXIDX:
				pnmtxIdxOffs = vtxSize;
				break;
			case GX_VA_POS:
				posOffs = vtxSize;
				if (desc->type != GX_INDEX16) {
					return;
				}
				break;
			case GX_VA_NRM:
				nrmOffs = vtxSize;
				if (desc->type != GX_INDEX16) {
					return;
				}
				break;
			case GX_VA_TEX0:
				break;
			default:
				break;
			}

			vtxSize += size[desc->type];
		}

		for (u16 j = 0; j < modelData->getShapeNodePointer(i)->getMtxGroupNum();
		     j++) {
			J3DShapeMtx* shapeMtx
			    = modelData->getShapeNodePointer(i)->getShapeMtx(j);

			u8* displayListStart = modelData->getShapeNodePointer(i)
			                           ->getShapeDraw(j)
			                           ->getDisplayList();
			for (u8* dl = displayListStart;
			     (dl - displayListStart) < modelData->getShapeNodePointer(i)
			                                   ->getShapeDraw(j)
			                                   ->getDisplayListSize();) {
				u8 cmd = dl[0];
				if (cmd != GX_TRIANGLEFAN && cmd != GX_TRIANGLESTRIP)
					break;

				u16 vtxCount = *(u16*)&dl[1];

				u16 useMtxIdxBuf[10];
				for (s32 k = 0; k < vtxCount; k++) {
					u8* vtx     = &dl[3 + vtxSize * k];
					u8 pnmtxIdx = ((u32)(*(u8*)&vtx[pnmtxIdxOffs])) / 3;
					u16 posIdx  = *(u16*)&vtx[posOffs];
					u16 nrmIdx  = *(u16*)&vtx[nrmOffs];

					u16 useMtxIdx = shapeMtx->getUseMtxIndex(pnmtxIdx);
					if (useMtxIdx == 0xFFFF) {
						useMtxIdx = useMtxIdxBuf[pnmtxIdx];
					} else if (pnmtxIdxOffs != -1) {
						useMtxIdxBuf[pnmtxIdx] = useMtxIdx;
					}

					mPosUseMtx[posIdx] = useMtxIdx;
					if (nrmOffs != -1)
						mNrmUseMtx[nrmIdx] = useMtxIdx;
				}

				dl += vtxSize * vtxCount;
				dl += 3;
			}
		}

		if (nrmOffs == -1) {
			modelData->getShapeNodePointer(i)->onFlag(0x10);
			modelData->getShapeNodePointer(i)->offFlag(0x8);
		}
	}

	for (u32 i = 0; i < modelData->getVtxNum(); i++) {
		if (mPosUseMtx[i] >= modelData->getDrawMtxNum()) {
			mPosUseMtx[i] = 0;
		}
	}

	for (u32 i = 0; i < modelData->getNrmNum(); i++) {
		if (mNrmUseMtx[i] >= modelData->getDrawMtxNum()) {
			mNrmUseMtx[i] = 0;
		}
	}
}

void J3DSkinDeform::calcNrmMtx(J3DModel* model)
{
	J3DModelData* modelData = model->getModelData();
	for (u16 i = 0; i < modelData->getDrawMtxNum(); i++) {
		if (modelData->getDrawMtxFlag(i) == 0) {
			if (model->getScaleFlag(modelData->getDrawMtxIndex(i)) == 1) {
				setNrmMtx(i, model->getAnmMtx(modelData->getDrawMtxIndex(i)));
			} else {
				J3DPSCalcInverseTranspose(
				    model->getAnmMtx(modelData->getDrawMtxIndex(i)),
				    getNrmMtx(i));
			}
		} else {
			if (model->getEnvScaleFlag(modelData->getDrawMtxIndex(i)) == 1) {
				setNrmMtx(
				    i, model->getWeightAnmMtx(modelData->getDrawMtxIndex(i)));
			} else {
				J3DPSCalcInverseTranspose(
				    model->getWeightAnmMtx(modelData->getDrawMtxIndex(i)),
				    getNrmMtx(i));
			}
		}
	}
}

void J3DSkinDeform::deform(J3DModel* model)
{
	Mtx* mtxArr[2];
	mtxArr[0] = model->mNodeMatrices;
	mtxArr[1] = model->unk5C;

	if (model->checkFlag(0x4)) {
		model->getVertexBuffer()->swapTransformedVtxPos();

		s32 vtxNum     = model->getModelData()->getVtxNum();
		Vec* curVtxPos = (Vec*)model->getVertexBuffer()->getCurrentVtxPos();
		Vec* transformedVtxPos
		    = (Vec*)model->getVertexBuffer()->getTransformedVtxPos(0);

		for (s32 i = 0; i < vtxNum; i++) {
			Mtx* mtx
			    = mtxArr[model->getModelData()->getDrawMtxFlag(mPosUseMtx[i])];
			J3DPSMulMtxVec(
			    mtx[model->getModelData()->getDrawMtxIndex(mPosUseMtx[i])],
			    &curVtxPos[i], &transformedVtxPos[i]);
		}

		DCStoreRange(model->getVertexBuffer()->getTransformedVtxPos(0),
		             model->getModelData()->getVtxNum() * sizeof(Vec));
		model->getVertexBuffer()->setCurrentVtxPos(transformedVtxPos);
	}

	if (model->checkFlag(0x8)) {
		model->getVertexBuffer()->swapTransformedVtxNrm();

		s32 vtxNum     = model->getModelData()->getNrmNum();
		Vec* curVtxNrm = (Vec*)model->getVertexBuffer()->getCurrentVtxNrm();
		Vec* transformedVtxNrm
		    = (Vec*)model->getVertexBuffer()->getTransformedVtxNrm(0);

		calcNrmMtx(model);

		for (s32 i = 0; i < vtxNum; i++) {
			J3DPSMulMtxVec(mNrmMtx[mNrmUseMtx[i]], &curVtxNrm[i],
			               &transformedVtxNrm[i]);
		}

		DCStoreRange(model->getVertexBuffer()->getTransformedVtxNrm(0),
		             model->getModelData()->getNrmNum() * sizeof(Vec));
		model->getVertexBuffer()->setCurrentVtxNrm(transformedVtxNrm);
	}
}
