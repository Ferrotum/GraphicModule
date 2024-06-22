#pragma once
//Types_using_in_tempates
#ifndef TYPES_USING_IN_TEMPATES_H_
#define TYPES_USING_IN_TEMPATES_H_

#include <shared_mutex>
#include "SharedSpinlockMutex.h"

//using SyncObjectMutex = std::shared_mutex;
using SyncObjectMutex = SharedSpinlockMutex;

#endif // !TYPES_USING_IN_TEMPATES_H_
