#ifndef JSG_OBJECT_HPP
#define JSG_OBJECT_HPP

#include <types.h>

namespace JStage {

class TObject {
public:
	typedef void IDK;
	IDK JSGGetName() const;
	u32 JSGGetFlag() const;
	void JSGSetFlag(u32);
	void JSGGetData(u32, void*, u32) const;
	void JSGSetData(u32, const void*, u32);
	void JSGGetParent(TObject**, u32*) const;
	void JSGSetParent(TObject*, u32);
	IDK JSGSetRelation(bool, TObject*, u32);
};

}; // namespace JStage

#endif
