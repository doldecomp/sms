#ifndef SYSTEM_SCENARIO_ARCHIVE_NAME_HPP
#define SYSTEM_SCENARIO_ARCHIVE_NAME_HPP

#include <JSystem/JDrama/JDRNameRef.hpp>

class TScenarioArchiveName : public JDrama::TNameRef {
public:
	TScenarioArchiveName(const char* name = "<ScenarioArchiveName>")
	    : JDrama::TNameRef(name)
	{
	}

	virtual void load(JSUMemoryInputStream& stream);

public:
	/* 0xC */ const char* unkC;
};

#endif
