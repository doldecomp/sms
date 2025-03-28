#ifndef JPA_CALLBACK_HPP
#define JPA_CALLBACK_HPP

// T = JPABaseEmitter*
template <class T> class JPACallBackBase {
public:
	void execute(T);
	void executeAfter(T);
	void draw(T);
};

// T = JPABaseEmitter*
// U = JPABaseParticle*
template <class T, class U> class JPACallBackBase2 {
public:
	virtual void execute(T, U) { }
	virtual void draw(T, U) { }
};

#endif
