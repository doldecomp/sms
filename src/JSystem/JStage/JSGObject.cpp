#include <JSystem/JStage/JSGObject.hpp>

JStage::TObject::~TObject() { }

bool JStage::TObject::JSGGetName() const { return false; }

u32 JStage::TObject::JSGGetFlag() const { return 0; }

void JStage::TObject::JSGSetFlag(u32) { }

bool JStage::TObject::JSGGetData(u32, void*, u32) const { return false; }

void JStage::TObject::JSGSetData(u32, const void*, u32) { }

void JStage::TObject::JSGGetParent(JStage::TObject**, u32*) const { }

void JStage::TObject::JSGSetParent(JStage::TObject*, u32) { }

void JStage::TObject::JSGSetRelation(bool, JStage::TObject*, u32) { }
