#ifndef JAIASNDATA_HPP
#define JAIASNDATA_HPP

namespace JAInter {

class TAsnData {
public:
	TAsnData();
	void readAsnFile();

	static const char* asnFileName;
	static void* asnData;
};

} // namespace JAInter

#endif // JAIASNDATA_HPP
