#ifndef JALMODSE_HPP
#define JALMODSE_HPP

#include <dolphin/types.h>
#include <JSystem/JAudio/JALibrary/JALCalc.hpp>
#include <JSystem/JAudio/JADebug/JADPrm.hpp>
#include <JSystem/JAudio/JALibrary/JALList.hpp>

struct JALPrmSet {
	JALPrmSet(f32 minVal, f32 maxVal)
	    : minParam(minVal, nullptr)
	    , maxParam(maxVal, nullptr)
	{
	}

	JADPrmS<f32> minParam;
	JADPrmS<f32> maxParam;
};

template <class T> class JALSeModData : public JALListHioNode<T, u32> {
public:
	JALSeModData(const char* name, T* owner, u32 id, JALPrmSet* inputRange,
	             JALPrmSet* outputRange, JADPrmS<f32>* baseParam,
	             JALCalc::CurveSign curveSign, u8 gateFlags);
	~JALSeModData() { }

	virtual f32 calcDyna(f32 inputVal)
	{
		return JALCalc::getParamByExp(
		    inputVal, mInputRange.minParam.data, mInputRange.maxParam.data,
		    mBaseParam.data, mOutputRange.minParam.data,
		    mOutputRange.maxParam.data, (JALCalc::CurveSign)mCurveSign);
	}
	virtual bool gateCheckDyna(f32 inputVal)
	{
		if (mInputRange.maxParam.data >= mInputRange.minParam.data) {
			if ((inputVal > mInputRange.maxParam.data)
			    && ((mGateFlags & 2) != 0))
				return 1;
			if ((inputVal < mInputRange.minParam.data)
			    && ((mGateFlags & 1) != 0))
				return 1;
			return 0;
		} else {
			if ((inputVal > mInputRange.minParam.data)
			    && ((mGateFlags & 2) != 0))
				return 1;
			if ((inputVal < mInputRange.maxParam.data)
			    && ((mGateFlags & 1) != 0))
				return 1;
			return 0;
		}
	}

	static bool calc(u32 id, f32 inputVal, f32* outVal);
	static bool gateCheck(u32 id, f32 inputVal);

public:
	/* 0x18 */ JALPrmSet mInputRange;
	/* 0x20 */ JALPrmSet mOutputRange;
	/* 0x28 */ JADPrmS<f32> mBaseParam;
	/* 0x2C */ u8 mCurveSign;
	/* 0x2D */ u8 mGateFlags;
};

template <class T>
JALSeModData<T>::JALSeModData(const char* name, T* owner, u32 id,
                              JALPrmSet* inputRange, JALPrmSet* outputRange,
                              JADPrmS<f32>* baseParam,
                              JALCalc::CurveSign curveSign, u8 gateFlags)
    : JALListHioNode<T, u32>(name, id, owner)
    , mInputRange(*inputRange)
    , mOutputRange(*outputRange)
    , mBaseParam(*baseParam)
    , mCurveSign((u8)curveSign)
    , mGateFlags(gateFlags)
{
}

template <class T> bool JALSeModData<T>::calc(u32 id, f32 inputVal, f32* outVal)
{
	T* found = JALListS<T, u32>::search(id);
	if (found != nullptr) {
		*outVal = found->calcDyna(inputVal);
		return true;
	} else
		return false;
}

template <class T> bool JALSeModData<T>::gateCheck(u32 id, f32 inputVal)
{
	T* grp = JALListS<T, u32>::search(id);
	return grp ? grp->gateCheckDyna(inputVal) : false;
}

class JALSeModVolFunk : public JALSeModData<JALSeModVolFunk> {
public:
	JALSeModVolFunk(const char* name, u32 id, JALPrmSet* inputRange,
	                JALPrmSet* outputRange, JADPrmS<f32>* baseParam,
	                JALCalc::CurveSign curveSign, u8 gateFlags)
	    : JALSeModData<JALSeModVolFunk>(name, this, id, inputRange, outputRange,
	                                    baseParam, curveSign, gateFlags)
	{
	}
};
class JALSeModPitFunk : public JALSeModData<JALSeModPitFunk> {
public:
	JALSeModPitFunk(const char* name, u32 id, JALPrmSet* inputRange,
	                JALPrmSet* outputRange, JADPrmS<f32>* baseParam,
	                JALCalc::CurveSign curveSign, u8 gateFlags)
	    : JALSeModData<JALSeModPitFunk>(name, this, id, inputRange, outputRange,
	                                    baseParam, curveSign, gateFlags)
	{
	}
};
class JALSeModEffFunk : public JALSeModData<JALSeModEffFunk> {
public:
	JALSeModEffFunk(const char* name, u32 id, JALPrmSet* inputRange,
	                JALPrmSet* outputRange, JADPrmS<f32>* baseParam,
	                JALCalc::CurveSign curveSign, u8 gateFlags)
	    : JALSeModData<JALSeModEffFunk>(name, this, id, inputRange, outputRange,
	                                    baseParam, curveSign, gateFlags)
	{
	}
};
class JALSeModVolDist : public JALSeModData<JALSeModVolDist> {
public:
	JALSeModVolDist(const char* name, u32 id, JALPrmSet* inputRange,
	                JALPrmSet* outputRange, JADPrmS<f32>* baseParam,
	                JALCalc::CurveSign curveSign, u8 gateFlags)
	    : JALSeModData<JALSeModVolDist>(name, this, id, inputRange, outputRange,
	                                    baseParam, curveSign, gateFlags)
	{
	}
};
class JALSeModPitDist : public JALSeModData<JALSeModPitDist> {
public:
	JALSeModPitDist(const char* name, u32 id, JALPrmSet* inputRange,
	                JALPrmSet* outputRange, JADPrmS<f32>* baseParam,
	                JALCalc::CurveSign curveSign, u8 gateFlags)
	    : JALSeModData<JALSeModPitDist>(name, this, id, inputRange, outputRange,
	                                    baseParam, curveSign, gateFlags)
	{
	}
};
class JALSeModEffDist : public JALSeModData<JALSeModEffDist> {
public:
	JALSeModEffDist(const char* name, u32 id, JALPrmSet* inputRange,
	                JALPrmSet* outputRange, JADPrmS<f32>* baseParam,
	                JALCalc::CurveSign curveSign, u8 gateFlags)
	    : JALSeModData<JALSeModEffDist>(name, this, id, inputRange, outputRange,
	                                    baseParam, curveSign, gateFlags)
	{
	}
};

template <class T, class U> class JALLinkD;

template <class T, class U> class JALListD : public JSUList<T> {
public:
	T* searchD(u32 id)
	{
		for (JSUListIterator<T> it = this->getFirst(); it != this->getEnd();
		     ++it)
			if (it.getObject()->mId == id)
				return it.getObject();

		return nullptr;
	}

	void append(JALLinkD<T, U>* link)
	{
		JSUList<T>::append(link->getLinkNode());
		link->setList(this);
	}

	~JALListD() { }
};

template <class T, class U> class JALLinkD {
public:
	JALLinkD(T* nodeData, U id, JALListD<T, U>* list);
	~JALLinkD()
	{
		JSUList<T>* sup = mLinkNode.getSupervisor();
		if (sup)
			sup->remove(&mLinkNode);
	}

	// fabricated
	JSULink<T>* getLinkNode() { return &mLinkNode; }
	void setList(JALListD<T, U>* list) { mList = list; }

public:
	/* 0x0 */ U mId;
	/* 0x4 */ JSULink<T> mLinkNode;
	/* 0x14 */ JALListD<T, U>* mList;
};

template <class T, class U>
JALLinkD<T, U>::JALLinkD(T* nodeData, U id, JALListD<T, U>* list)
    : mId(id)
    , mLinkNode(nodeData)
    , mList(list)
{
	if (list)
		list->append(this);
}

class JALSeModDataGrpMemb : public JALLinkD<JALSeModDataGrpMemb, u32> {
public:
	JALSeModDataGrpMemb(u32 id, JALListD<JALSeModDataGrpMemb, u32>* list)
	    : JALLinkD<JALSeModDataGrpMemb, u32>(this, id, list)
	{
	}
};

template <class T, class U, class V> class JALListGrp : public JALListD<V, U> {
public:
	static T* searchGroup(U id);
	~JALListGrp() { }
};

template <class T, class U, class V> T* JALListGrp<T, U, V>::searchGroup(U id)
{
	JSUListIterator<T> it = JALList<T>::smList.getFirst();
	for (; it != JALList<T>::smList.getEnd(); ++it) {
		V* res = it.getObject()->searchD(id);
		if (res)
			return it.getObject();
	}
	return nullptr;
}

template <class T>
class JALSeModDataGrp : public JALSeModData<T>,
                        public JALListGrp<T, u32, JALSeModDataGrpMemb> {
public:
	JALSeModDataGrp(const char* name, T* owner, u32 id, JALPrmSet* inputRange,
	                JALPrmSet* outputRange, JADPrmS<f32>* baseParam,
	                JALCalc::CurveSign curveSign, u8 gateFlags)
	    : JALSeModData<T>(name, owner, id, inputRange, outputRange, baseParam,
	                      curveSign, gateFlags)
	{
	}

	~JALSeModDataGrp() { }

	static bool calcGrp(u32 id, f32 inputVal, f32* outVal);
	static bool gateCheckGrp(u32 id, f32 inputVal);
};

template <class T>
bool JALSeModDataGrp<T>::calcGrp(u32 id, f32 inputVal, f32* outVal)
{
	T* found = JALListGrp<T, u32, JALSeModDataGrpMemb>::searchGroup(id);
	if (found != nullptr) {
		*outVal = found->calcDyna(inputVal);
		return true;
	} else
		return false;
}

template <class T> bool JALSeModDataGrp<T>::gateCheckGrp(u32 id, f32 inputVal)
{
	T* found = JALListGrp<T, u32, JALSeModDataGrpMemb>::searchGroup(id);
	return found ? found->gateCheckDyna(inputVal) : false;
}

class JALSeModVolFGrp : public JALSeModDataGrp<JALSeModVolFGrp> {
public:
	JALSeModVolFGrp(const char* name, u32 id, JALPrmSet* inputRange,
	                JALPrmSet* outputRange, JADPrmS<f32>* baseParam,
	                JALCalc::CurveSign curveSign, u8 gateFlags)
	    : JALSeModDataGrp<JALSeModVolFGrp>(name, this, id, inputRange,
	                                       outputRange, baseParam, curveSign,
	                                       gateFlags)
	{
	}
};
class JALSeModPitFGrp : public JALSeModDataGrp<JALSeModPitFGrp> {
public:
	JALSeModPitFGrp(const char* name, u32 id, JALPrmSet* inputRange,
	                JALPrmSet* outputRange, JADPrmS<f32>* baseParam,
	                JALCalc::CurveSign curveSign, u8 gateFlags)
	    : JALSeModDataGrp<JALSeModPitFGrp>(name, this, id, inputRange,
	                                       outputRange, baseParam, curveSign,
	                                       gateFlags)
	{
	}
};
class JALSeModEffFGrp : public JALSeModDataGrp<JALSeModEffFGrp> {
public:
	JALSeModEffFGrp(const char* name, u32 id, JALPrmSet* inputRange,
	                JALPrmSet* outputRange, JADPrmS<f32>* baseParam,
	                JALCalc::CurveSign curveSign, u8 gateFlags)
	    : JALSeModDataGrp<JALSeModEffFGrp>(name, this, id, inputRange,
	                                       outputRange, baseParam, curveSign,
	                                       gateFlags)
	{
	}
};
class JALSeModVolDGrp : public JALSeModDataGrp<JALSeModVolDGrp> {
public:
	JALSeModVolDGrp(const char* name, u32 id, JALPrmSet* inputRange,
	                JALPrmSet* outputRange, JADPrmS<f32>* baseParam,
	                JALCalc::CurveSign curveSign, u8 gateFlags)
	    : JALSeModDataGrp<JALSeModVolDGrp>(name, this, id, inputRange,
	                                       outputRange, baseParam, curveSign,
	                                       gateFlags)
	{
	}
};
class JALSeModPitDGrp : public JALSeModDataGrp<JALSeModPitDGrp> {
public:
	JALSeModPitDGrp(const char* name, u32 id, JALPrmSet* inputRange,
	                JALPrmSet* outputRange, JADPrmS<f32>* baseParam,
	                JALCalc::CurveSign curveSign, u8 gateFlags)
	    : JALSeModDataGrp<JALSeModPitDGrp>(name, this, id, inputRange,
	                                       outputRange, baseParam, curveSign,
	                                       gateFlags)
	{
	}
};
class JALSeModEffDGrp : public JALSeModDataGrp<JALSeModEffDGrp> {
public:
	JALSeModEffDGrp(const char* name, u32 id, JALPrmSet* inputRange,
	                JALPrmSet* outputRange, JADPrmS<f32>* baseParam,
	                JALCalc::CurveSign curveSign, u8 gateFlags)
	    : JALSeModDataGrp<JALSeModEffDGrp>(name, this, id, inputRange,
	                                       outputRange, baseParam, curveSign,
	                                       gateFlags)
	{
	}
};

#endif
