#ifndef PARAMS_HPP
#define PARAMS_HPP

#include <System/BaseParam.hpp>

#include <JSystem/JKernel/JKRFileLoader.hpp>

class TParams {
public:
	TParams()
	    : mPrmPath(nullptr)
	    , mHead(nullptr)
	{
	}
	TParams(const char* prm)
	    : mPrmPath(prm)
	    , mHead(nullptr)
	{
	}

	static void finalize();

	bool load(const char* filename);
	void load(JSUMemoryInputStream& stream);
	static void init();

public:
	const char* mPrmPath;
	TBaseParam* mHead;
	static JKRFileLoader* mArc;
	static JKRFileLoader* mSceneArc;
};

#endif
