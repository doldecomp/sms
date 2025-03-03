#ifndef JAIDATA_HPP
#define JAIDATA_HPP

#include <JSystem/JAudio/JAInterface/JAILinkBuffer.hpp>
#include <JSystem/JAudio/JAInterface/JAISeqParameter.hpp>
#include <JSystem/JAudio/JAInterface/JAISeParameter.hpp>
#include <JSystem/JAudio/JAInterface/JAIStreamParameter.hpp>

class JAISoundTable;
class JAIHeapBlock;
struct JAIMoveParaSet;

namespace JAInter {

class JAIData {
public:
	JAIData();

	void init();
	void initData();
	void initInfoDataWork(JAISoundTable* soundTable, char* param);
	void initLinkBuffer(JAILinkBuffer* linkBuffer, unsigned long param);
	void initSeqParaLinkBuffer();
	void initSeParaLinkBuffer();
	void initStreamParaLinkBuffer();
	void initDummyVecLink();
	void initSeqParameter(JAISeqParameter* param);
	void initSePara(JAISeParameter* param);
	void initStreamParameter(JAIStreamParameter* param);
	void initSeqTrackInfoParameter(unsigned long param);
	void initStreamUpdateParameter();

	void setSeMovePara(JAIMoveParaSet* moveParaSet);
	void moveParameter(JAIMoveParaSet* moveParaSet);

	bool checkOnMemory(unsigned long param1, unsigned char* param2);
	unsigned char checkUsefulAutoHeapPosition();
	unsigned char* getFreeAutoHeapPointer(unsigned char param1,
	                                      unsigned long param2);
	void releaseAutoHeapPointer(unsigned char param);
	unsigned char* getFreeStayHeapPointer(unsigned long param1,
	                                      unsigned long param2);

	void clearAutoHeap(JAIHeapBlock* heapBlock);
	void clearAutoHeap(unsigned long param);
	void clearStayHeap(unsigned long param);

	void setAutoHeapLoadedFlag(unsigned char param1, unsigned char param2);
	void changeAutoHeapPointerToPosition(unsigned char* ptr);
	void stopPlayingSeq(unsigned long param);
	unsigned char* getAutoHeapPointer(unsigned long param);
	bool getInfoPointer(unsigned long param, void** ptr);

	void reloadInfoDataWork(JAISoundTable* soundTable);
	void setInfoDataPointer(JAISoundTable* soundTable, unsigned char* ptr);
};

} // namespace JAInter

#endif // JAIDATA_HPP
