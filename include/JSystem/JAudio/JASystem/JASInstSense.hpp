#ifndef JASINSTSENSE_HPP
#define JASINSTSENSE_HPP

namespace JASystem {

class TInstSense {
public:
	virtual ~TInstSense() { }
	virtual int getY(int, int) const;
	void setParams(int, int, float, float);
};

} // namespace JASystem

#endif // JASINSTSENSE_HPP
