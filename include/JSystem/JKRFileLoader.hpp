#ifndef JKR_FILE_LOADER_HPP
#define JKR_FILE_LOADER_HPP

#include <JSystem/J3DModelLoader.hpp>

struct JKRFileLoader {
	static J3DModelLoaderDataBase* getGlbResource(const char*);
};

#endif
