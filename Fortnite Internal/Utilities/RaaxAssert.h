#pragma once
#include "Error.h"

// Custom assert macro that calls THROW_ERROR if the condition is false
#define RaaxAssert(Condition, Message) if((Condition) == false) THROW_ERROR(Message, false)