#pragma once
#ifndef _CLASSNAME_H_
#define _CLASSNAME_H_

#include <string>
#include <typeinfo>

template <typename T>
class ClassName
{
protected:
	static std::string _className;
public:
	ClassName(const ClassName<T>&) = delete;
	ClassName(ClassName<T>&&) = delete;
	ClassName<T>& operator=(const ClassName<T>&) = delete;
	ClassName<T>& operator=(ClassName<T>&&) = delete;
	static const std::string& className() noexcept;
	bool operator==(const ClassName<T>& cn) const noexcept;
};

//==================================================================================

template<typename T> std::string ClassName<T>::_className(typeid(T).name());

//==================================================================================

template<typename T>
inline const std::string& ClassName<T>::className() noexcept
{
	return _className;
}

//==================================================================================

template<typename T>
inline bool ClassName<T>::operator==(const ClassName<T>& cn) const noexcept
{
	return _className == cn._className;
}

#endif // !_CLASSNAME_H_