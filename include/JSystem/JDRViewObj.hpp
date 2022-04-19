#ifndef JDRVIEWOBJ_HPP
#define JDRVIEWOBJ_HPP

#include "dolphin.h"
#include "types.h"

#include "JSystem/JDRNameRef.hpp"

namespace JDrama {
    class TViewObj : TNameRef { // based on TNameRef
        public:
            u32 _00C;
    };
};

#endif
