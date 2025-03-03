#ifndef JAIASNDATA_HPP
#define JAIASNDATA_HPP

namespace JAInter {

class TAsnData {
public:
	void readAsnFile();

	static char* asnFileName;
	static void* asnData;
};

} // namespace JAInter

#endif // JAIASNDATA_HPP
