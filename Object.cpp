#include "Object.h"

void Object::_moveProc(Object& a) noexcept
{
	_modifiedTime = a._modifiedTime;
	_lastUsage = a._lastUsage;
	_modified.store(a._modified.load(std::memory_order_acquire), std::memory_order_release);
}
