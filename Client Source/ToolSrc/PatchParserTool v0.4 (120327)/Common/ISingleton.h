#pragma once

template <typename T>
class ISingleton
{
protected:
	ISingleton() {}
	ISingleton(const ISingleton<T>&);
	ISingleton& operator=(const ISingleton<T>);

public:
	static T* instance()
	{
		static T instance;
		return &instance;
	}
};