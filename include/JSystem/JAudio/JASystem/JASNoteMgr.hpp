#ifndef JASNOTEMGR_HPP
#define JASNOTEMGR_HPP

namespace JASystem {

namespace TTrack {
	class TNoteMgr {
	public:
		void init();
		void endProcess();
		void* getChannel(int);
	};
} // namespace TTrack

} // namespace JASystem

#endif // JASNOTEMGR_HPP
