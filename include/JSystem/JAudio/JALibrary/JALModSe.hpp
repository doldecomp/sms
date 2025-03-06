#ifndef JALMODSE_HPP
#define JALMODSE_HPP

#include <dolphin/types.h>
#include <JSystem/JAudio/JALibrary/JALCalc.hpp>
#include <JSystem/JAudio/JADebug/JADPrm.hpp>
#include <JSystem/JAudio/JALibrary/JALList.hpp>

struct JALPrmSet {
	JADPrmS<f32> unk0;
	JADPrmS<f32> unk4;
};

template <class T, class U> class JALListS {
public:
	static T* search(U param_1)
	{
		JSUListIterator<T> it = JALList<T>::smList.getFirst();
		for (; it != JALList<T>::smList.getEnd(); ++it) {
			if (param_1 == (u32)it.getObject()->unk10)
				break;
		}
		return it.getObject();
	}
	~JALListS();
};

template <class T> class JALSeModData : public JALListHioNode<T, u32> {
public:
	JALSeModData(const char* param_1, T* param_2, u32 param_3,
	             JALPrmSet* param_4, JALPrmSet* param_5, JADPrmS<f32>* param_6,
	             JALCalc::CurveSign param_7, u8 param_8)
	    : JALListHioNode<T, u32>(param_1, param_3, param_2)
	{
		unk18 = *param_4;
		unk20 = *param_5;
		unk28 = *param_6;
		unk2C = (u8)param_7;
		unk2D = param_8;
	}
	~JALSeModData() { }

	virtual f32 calcDyna(f32 param_1)
	{
		return JALCalc::getParamByExp(
		    param_1, unk18.unk0.unk0, unk18.unk4.unk0, unk28.unk0,
		    unk20.unk0.unk0, unk20.unk4.unk0, (JALCalc::CurveSign)unk2C);
	}
	virtual bool gateCheckDyna(f32 param_1)
	{
		if (unk18.unk4.unk0 >= unk18.unk0.unk0) {
			if ((param_1 > unk18.unk4.unk0) && ((unk2D & 2) != 0))
				return 1;
			if ((param_1 < unk18.unk0.unk0) && ((unk2D & 1) != 0))
				return 1;
			return 0;
		} else {
			if ((param_1 > unk18.unk0.unk0) && ((unk2D & 2) != 0))
				return 1;
			if ((param_1 < unk18.unk4.unk0) && ((unk2D & 1) != 0))
				return 1;
			return 0;
		}
	}

	static bool calc(u32 param_1, f32 param_2, f32* param_3)
	{
		T* found = JALListS<T, u32>::search(param_1);
		if (found != nullptr) {
			*param_3 = found->calcDyna(param_2);
			return true;
		} else
			return false;
	}

	static bool gateCheck(u32 param_1, f32 param_2)
	{
		T* grp = JALListS<T, u32>::search(param_1);
		return grp ? grp->gateCheckDyna(param_2) : false;
	}

public:
	/* 0x18 */ JALPrmSet unk18;
	/* 0x20 */ JALPrmSet unk20;
	/* 0x28 */ JADPrmS<f32> unk28;
	/* 0x2C */ u8 unk2C;
	/* 0x2D */ u8 unk2D;
};

class JALSeModPitDist : public JALSeModData<JALSeModPitDist> {
public:
	JALSeModPitDist(const char* param_1, u32 param_2, JALPrmSet* param_3,
	                JALPrmSet* param_4, JADPrmS<f32>* param_5,
	                JALCalc::CurveSign param_6, u8 param_7)
	    : JALSeModData<JALSeModPitDist>(param_1, this, param_2, param_3,
	                                    param_4, param_5, param_6, param_7)
	{
	}
};
class JALSeModEffDist : public JALSeModData<JALSeModEffDist> {
public:
	JALSeModEffDist(const char* param_1, u32 param_2, JALPrmSet* param_3,
	                JALPrmSet* param_4, JADPrmS<f32>* param_5,
	                JALCalc::CurveSign param_6, u8 param_7)
	    : JALSeModData<JALSeModEffDist>(param_1, this, param_2, param_3,
	                                    param_4, param_5, param_6, param_7)
	{
	}
};
class JALSeModVolDist : public JALSeModData<JALSeModVolDist> {
public:
	JALSeModVolDist(const char* param_1, u32 param_2, JALPrmSet* param_3,
	                JALPrmSet* param_4, JADPrmS<f32>* param_5,
	                JALCalc::CurveSign param_6, u8 param_7)
	    : JALSeModData<JALSeModVolDist>(param_1, this, param_2, param_3,
	                                    param_4, param_5, param_6, param_7)
	{
	}
};
class JALSeModEffFunk : public JALSeModData<JALSeModEffFunk> {
public:
	JALSeModEffFunk(const char* param_1, u32 param_2, JALPrmSet* param_3,
	                JALPrmSet* param_4, JADPrmS<f32>* param_5,
	                JALCalc::CurveSign param_6, u8 param_7)
	    : JALSeModData<JALSeModEffFunk>(param_1, this, param_2, param_3,
	                                    param_4, param_5, param_6, param_7)
	{
	}
};
class JALSeModPitFunk : public JALSeModData<JALSeModPitFunk> {
public:
	JALSeModPitFunk(const char* param_1, u32 param_2, JALPrmSet* param_3,
	                JALPrmSet* param_4, JADPrmS<f32>* param_5,
	                JALCalc::CurveSign param_6, u8 param_7)
	    : JALSeModData<JALSeModPitFunk>(param_1, this, param_2, param_3,
	                                    param_4, param_5, param_6, param_7)
	{
	}
};
class JALSeModVolFunk : public JALSeModData<JALSeModVolFunk> {
public:
	JALSeModVolFunk(const char* param_1, u32 param_2, JALPrmSet* param_3,
	                JALPrmSet* param_4, JADPrmS<f32>* param_5,
	                JALCalc::CurveSign param_6, u8 param_7)
	    : JALSeModData<JALSeModVolFunk>(param_1, this, param_2, param_3,
	                                    param_4, param_5, param_6, param_7)
	{
	}
};

template <class T, class U> class JALListD : public JSUList<T> {
public:
	~JALListD();
};

template <class T, class U> class JALLinkD {
public:
	JALLinkD(T* param_1, U param_2, JALListD<T, U>* param_3)
	    : unk0(param_2)
	    , unk4(param_1)
	    , unk14(param_3)
	{
		if (param_3) {
			param_3->append(&unk4);
			unk14 = param_3;
		}
	}

	~JALLinkD();

public:
	/* 0x0 */ U unk0;
	/* 0x4 */ JSULink<T> unk4;
	/* 0x14 */ JALListD<T, U>* unk14;
};

class JALSeModDataGrpMemb : public JALLinkD<JALSeModDataGrpMemb, u32> {
public:
	JALSeModDataGrpMemb(u32 param_1,
	                    JALListD<JALSeModDataGrpMemb, u32>* param_2)
	    : JALLinkD<JALSeModDataGrpMemb, u32>(this, param_1, param_2)
	{
	}
};

template <class T, class U, class V> class JALListGrp : public JALListD<V, U> {
public:
	static T* searchGroup(U param_1)
	{
		JALListGrp* res       = nullptr;
		JSUListIterator<T> it = JALList<T>::smList.getFirst();
		for (; it != JALList<T>::smList.getEnd(); ++it) {
			T* res = nullptr;
			JSUListIterator<JALSeModDataGrpMemb> it2
			    = it.getObject()->getFirst();
			for (; it2 != it.getObject()->getEnd(); ++it2)
				if (param_1 == it2.getObject()->unk0)
					break;
			if (it2 != it.getObject()->getEnd())
				return it.getObject();
		}
	}
	~JALListGrp();
};

template <class T>
class JALSeModDataGrp : public JALSeModData<T>,
                        public JALListGrp<T, u32, JALSeModDataGrpMemb> {
public:
	JALSeModDataGrp(const char* param_1, T* param_2, u32 param_3,
	                JALPrmSet* param_4, JALPrmSet* param_5,
	                JADPrmS<f32>* param_6, JALCalc::CurveSign param_7,
	                u8 param_8)
	    : JALSeModData<T>(param_1, param_2, param_3, param_4, param_5, param_6,
	                      param_7, param_8)
	{
	}

	~JALSeModDataGrp();

	static bool calcGrp(u32 param_1, f32 param_2, f32* param_3)
	{
		T* found
		    = JALListGrp<T, u32, JALSeModDataGrpMemb>::searchGroup(param_1);
		if (found != nullptr) {
			*param_3 = found->calcDyna(param_2);
			return true;
		} else
			return false;
	}
	static bool gateCheckGrp(u32 param_1, f32 param_2)
	{
		T* found
		    = JALListGrp<T, u32, JALSeModDataGrpMemb>::searchGroup(param_1);
		return found ? found->gateCheckDyna(param_2) : false;
	}
};

class JALSeModPitDGrp : public JALSeModDataGrp<JALSeModPitDGrp> {
public:
	JALSeModPitDGrp(const char* param_1, u32 param_2, JALPrmSet* param_3,
	                JALPrmSet* param_4, JADPrmS<f32>* param_5,
	                JALCalc::CurveSign param_6, u8 param_7)
	    : JALSeModDataGrp<JALSeModPitDGrp>(param_1, this, param_2, param_3,
	                                       param_4, param_5, param_6, param_7)
	{
	}
};
class JALSeModEffDGrp : public JALSeModDataGrp<JALSeModEffDGrp> {
public:
	JALSeModEffDGrp(const char* param_1, u32 param_2, JALPrmSet* param_3,
	                JALPrmSet* param_4, JADPrmS<f32>* param_5,
	                JALCalc::CurveSign param_6, u8 param_7)
	    : JALSeModDataGrp<JALSeModEffDGrp>(param_1, this, param_2, param_3,
	                                       param_4, param_5, param_6, param_7)
	{
	}
};
class JALSeModVolDGrp : public JALSeModDataGrp<JALSeModVolDGrp> {
public:
	JALSeModVolDGrp(const char* param_1, u32 param_2, JALPrmSet* param_3,
	                JALPrmSet* param_4, JADPrmS<f32>* param_5,
	                JALCalc::CurveSign param_6, u8 param_7)
	    : JALSeModDataGrp<JALSeModVolDGrp>(param_1, this, param_2, param_3,
	                                       param_4, param_5, param_6, param_7)
	{
	}
};
class JALSeModEffFGrp : public JALSeModDataGrp<JALSeModEffFGrp> {
public:
	JALSeModEffFGrp(const char* param_1, u32 param_2, JALPrmSet* param_3,
	                JALPrmSet* param_4, JADPrmS<f32>* param_5,
	                JALCalc::CurveSign param_6, u8 param_7)
	    : JALSeModDataGrp<JALSeModEffFGrp>(param_1, this, param_2, param_3,
	                                       param_4, param_5, param_6, param_7)
	{
	}
};
class JALSeModPitFGrp : public JALSeModDataGrp<JALSeModPitFGrp> {
public:
	JALSeModPitFGrp(const char* param_1, u32 param_2, JALPrmSet* param_3,
	                JALPrmSet* param_4, JADPrmS<f32>* param_5,
	                JALCalc::CurveSign param_6, u8 param_7)
	    : JALSeModDataGrp<JALSeModPitFGrp>(param_1, this, param_2, param_3,
	                                       param_4, param_5, param_6, param_7)
	{
	}
};
class JALSeModVolFGrp : public JALSeModDataGrp<JALSeModVolFGrp> {
public:
	JALSeModVolFGrp(const char* param_1, u32 param_2, JALPrmSet* param_3,
	                JALPrmSet* param_4, JADPrmS<f32>* param_5,
	                JALCalc::CurveSign param_6, u8 param_7)
	    : JALSeModDataGrp<JALSeModVolFGrp>(param_1, this, param_2, param_3,
	                                       param_4, param_5, param_6, param_7)
	{
	}
};

#endif
