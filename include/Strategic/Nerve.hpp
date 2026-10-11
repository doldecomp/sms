#ifndef STRATEGIC_NERVE_HPP
#define STRATEGIC_NERVE_HPP

#include <dolphin/types.h>

template <class T> class TSpineBase;

template <class T> class TNerveBase {
public:
	TNerveBase() { }
	virtual ~TNerveBase() { }
	virtual BOOL execute(TSpineBase<T>*) const = 0;
};

#define DECLARE_NERVE(Name, T)                                                 \
	class Name : public TNerveBase<T> {                                        \
	public:                                                                    \
		virtual BOOL execute(TSpineBase<T>*) const;                            \
		static const Name& theNerve();                                         \
	};

#define DEFINE_NERVE_INSTANCE(Name)                                            \
	const Name& Name::theNerve()                                               \
	{                                                                          \
		static Name instance;                                                  \
		return instance;                                                       \
	}

#define DEFINE_NERVE_EXECUTE(Name, T)                                          \
	BOOL Name::execute(TSpineBase<T>* spine) const

#define DEFINE_NERVE(Name, T)                                                  \
	DEFINE_NERVE_INSTANCE(Name)                                                \
	DEFINE_NERVE_EXECUTE(Name, T)

#endif
