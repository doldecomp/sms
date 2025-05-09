#ifndef SYSTEM_MAR_NAME_REF_GEN_HPP
#define SYSTEM_MAR_NAME_REF_GEN_HPP

#include <JSystem/JDrama/JDRNameRefGen.hpp>

class TMarNameRefGen : public JDrama::TNameRefGen {
public:
	JDrama::TNameRef* getNameRef_NPC(const char*) const;
	JDrama::TNameRef* getNameRef_MapObj(const char*) const;
	JDrama::TNameRef* getNameRef_Map(const char*) const;
	JDrama::TNameRef* getNameRef_Enemy(const char*) const;
	JDrama::TNameRef* getNameRef_BossEnemy(const char*) const;
	virtual JDrama::TNameRef* getNameRef(const char*) const;
};

#endif
