#include <M3DUtil/SDLModel.hpp>

void SDLModel::viewCalcSimple() { }

void SDLModel::entry() { }

void SDLModel::entryModelDataSDL(SDLModelData*, u32, u32) { }

SDLModel::SDLModel(SDLModelData*, u32, u32) { }

SDLMatPacket::SDLMatPacket() { }

void SDLModelData::entrySDLModels() { }

SDLModelData::SDLModelData(J3DModelData*) { }

void SDLModelData::recursiveEntry(J3DNode*, SDLDrawBufToken*) { }

void SDLModelData::entryNode(J3DNode*, SDLDrawBufToken*) { }

void SDLModelData::entrySameMat(J3DMaterial*, SDLDrawBufToken*) { }
