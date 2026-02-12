#include <System/DrawSyncManager.hpp>

void TDrawSyncManager::start(u32, u32, s32) { }

void TDrawSyncManager::end() { }

void TDrawSyncManager::drawSyncCallback(u16) { }

void TDrawSyncManager::threadFunc(void*) { }

TDrawSyncManager::TDrawSyncManager(u32, u32, s32) { }

TDrawSyncManager::~TDrawSyncManager() { }

void TDrawSyncManager::setCallback(u32, u16, u16, TDrawSyncCallback*) { }

void TDrawSyncManager::drawSyncCallbackSub(u16) { }

void TDrawSyncManager::pushBreakPoint() { }

TDrawSyncManager* TDrawSyncManager::smInstance;
