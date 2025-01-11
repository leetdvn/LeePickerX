#ifndef SINGLETON_H
#define SINGLETON_H
#include <QtGlobal>
#include <QScopedPointer>
#include <assert.h>

template <class T>
class Singleton final
{

public:
    static T* Instance(){
        static T m_Instance;
        return &m_Instance;
    }

private:
    Singleton()=default;
    ~Singleton() = default;
    Singleton(const Singleton&)=delete;
    Singleton& operator=(const Singleton& )=delete;
    Singleton(Singleton&&)=delete;
    Singleton&operator=(Singleton&&) = delete;
};

#endif // SINGLETON_H
