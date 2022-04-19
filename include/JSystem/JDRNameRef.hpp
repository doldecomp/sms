#ifndef JDRNAMEREF_HPP
#define JDRNAMEREF_HPP

#include "dolphin.h"

#include "JSystem/JSUMemoryStream.hpp"

namespace JDrama {
    class TNameRef {
        public:
            static void calcKeyCode(char const *);
            static void getType(JSUMemoryInputStream &, JSUMemoryInputStream &);
            static void genObject(JSUMemoryInputStream &, JSUMemoryInputStream &);

            virtual ~TNameRef();
            virtual void getType() const;
            virtual void load(JSUMemoryInputStream &);
            virtual void save(JSUMemoryOutputStream &);
            virtual void loadAfter();
            virtual void searchF(u16, char const *);
        
        private:
            char *mName;
            u16 mKeyCode;
    };
}

#endif
