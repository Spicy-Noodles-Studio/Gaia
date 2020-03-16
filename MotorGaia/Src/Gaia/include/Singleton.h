#pragma once
#ifndef SINGLETON_H
#define SINGLETON_H

#include "GaiaExports.h"

template<typename T>
class Singleton
{
private:
	//Si se llaman es que hay dos instancias
	Singleton(Singleton const&) = delete; //Constructor por copia
	Singleton& operator=(Singleton const&) = delete; //Operador igual

protected:
	static T* instance;

	Singleton();
	~Singleton() {}

public:
	static T* GetInstance();
	static void destroy();
};

template<typename T>
typename T* Singleton<T>::instance = nullptr;

template<typename T>
Singleton<T>::Singleton()
{
	Singleton::instance = static_cast<T*>(this);
}

template<typename T>
T* Singleton<T>::GetInstance()
{
	if (Singleton<T>::instance == nullptr)
	{
		Singleton<T>::instance = new T();
	}

	return Singleton<T>::instance;
}

template<typename T>
void Singleton<T>::destroy()
{
	delete Singleton<T>::instance;
	Singleton<T>::instance = nullptr;
}

#endif