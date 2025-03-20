#include <dolphin/os.h>
#include <JSystem/JAudio/JAInterface/JAIAsnData.hpp>

namespace JAInter {

const char* TAsnData::asnFileName = "JaiInfo.asn";
void* TAsnData::asnData           = 0;

// unused but we need the strings
void TAsnData::readAsnFile()
{
	OSReport("JAIAsnData::readAsnFile : ASN FILE(%s) が見つかりません。\n",
	         asnFileName);
	OSPanic(__FILE__, 0,
	        "JAIAsnData::readAsnFile : ASN "
	        "FILE(%s)のサイズ(0x%x)が32バイトアライメントされていません！！",
	        asnFileName, 0);
}

} // namespace JAInter
