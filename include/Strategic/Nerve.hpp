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

#define DEFINE_NERVE(Name, T)                                                  \
	const Name& Name::theNerve()                                               \
	{                                                                          \
		static Name instance;                                                  \
		return instance;                                                       \
	}                                                                          \
	BOOL Name::execute(TSpineBase<T>* spine) const

#endif
