#ifndef SYSTEM_MOVIE_DIRECTOR_HPP
#define SYSTEM_MOVIE_DIRECTOR_HPP

#include <JSystem/JDrama/JDRDirector.hpp>

namespace JDrama {
class TDisplay;
};

class TMarioGamePad;

class TMovieDirector : public JDrama::TDirector {
public:
	TMovieDirector();

	virtual int direct();

	void setupThreadFunc(void*);
	void setup(JDrama::TDisplay*, TMarioGamePad*);
	void rsetup();
	void decideNextMode(s32*);

	void getStreamMovieName(u32);
};

#endif
