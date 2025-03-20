#include "System/BaseParam.hpp"
#include "System/Params.hpp"

TBaseParam::TBaseParam(TParams* params, unsigned short code,
                       const char* paramName)
    : keyCode(code)
    , name(paramName)
    , next(nullptr)
{
	TBaseParam* root = params->mHead;
	if (root != nullptr) {
		TBaseParam* param = root;
		while (param->next != nullptr) {
			param = param->next;
		}
		param->next = this;
		return;
	}

	params->mHead = this;
}
