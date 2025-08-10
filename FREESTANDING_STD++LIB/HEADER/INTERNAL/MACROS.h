#pragma once
#define STD_ASSERT(condition,message)
/**
* the reason for this is that global vars (And static class vars) will not be initialized as no runtime exists. this is a workaround to make them "possible"
*/
#define DeclareStaticVar(name,type,initValue)\
static inline type& name() {\
	static type Var = initValue;\
	return Var;\
}
