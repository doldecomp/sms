#ifndef JSG_OBJECT_HPP
#define JSG_OBJECT_HPP

#include "types.h"

namespace JStage {
enum TEObject {
	/* 0x0 */ TOBJ_ACTOR_UNK,
	/* 0x1 */ TOBJ_UNK1,
	/* 0x2 */ TOBJ_ACTOR,
	/* 0x3 */ TOBJ_CAMERA,
	/* 0x4 */ TOBJ_AMBIENT,
	/* 0x5 */ TOBJ_LIGHT,
	/* 0x6 */ TOBJ_FOG,
};

struct TObject {
	virtual ~TObject()              = 0;
	virtual s32 JSGFGetType() const = 0;
	virtual bool JSGGetName() const;
	virtual u32 JSGGetFlag() const;
	virtual void JSGSetFlag(u32);
	virtual bool JSGGetData(u32, void*, u32) const;
	virtual void JSGSetData(u32, void const*, u32);
	virtual void JSGGetParent(JStage::TObject**, u32*) const;
	virtual void JSGSetParent(JStage::TObject*, u32);
	virtual void JSGSetRelation(bool, JStage::TObject*, u32);
};
}; // namespace JStage

#endif