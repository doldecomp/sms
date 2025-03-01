#include <M3DUtil/SDLModel.hpp>

void SDLModelData::entrySameMat(J3DMaterial*, SDLDrawBufToken*) { }

void SDLModelData::entryNode(J3DNode*, SDLDrawBufToken*) { }

void SDLModelData::recursiveEntry(J3DNode*, SDLDrawBufToken*) { }

SDLModelData::SDLModelData(J3DModelData*) { }

void SDLModelData::entrySDLModels() { }

SDLMatPacket::SDLMatPacket() { }

SDLModel::SDLModel(SDLModelData*, u32, u32) { }

void SDLModel::entryModelDataSDL(SDLModelData*, u32, u32) { }

void SDLModel::entry() { }

void SDLModel::viewCalcSimple() { }
