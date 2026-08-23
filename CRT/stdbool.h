#pragma once
#ifndef __STDBOOL__
#define __STDBOOL__
#if __STDC_VERSION__ < 202311l //BOOOOOOOOO intellisense lies when cstdlatest is set and falsely claims the bool keyword exists
#define bool _Bool
#define true 1
#define false 0
#endif

#define __bool_true_false_are_defined 1
#endif // !__STDBOOL__
