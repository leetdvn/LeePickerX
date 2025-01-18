#ifndef SINGLETON_H
#define SINGLETON_H
#include <QtGlobal>
#include <QScopedPointer>
#include <assert.h>
#include <mutex>


///Singleton Pattern
template <class T>
class Singleton
{

public:
    static T* Instance(){
        if(m_Instance==nullptr)
            m_Instance = new T();
        assert(m_Instance!=NULL);
        std::lock_guard<std::mutex> lock(mtx);
        return m_Instance;
    }

protected:
    Singleton()=default;
    ~Singleton(){
        if(m_Instance !=nullptr)
        {
            delete m_Instance;
            m_Instance = nullptr;
        }
    };
    static T* m_Instance;

private:
    Singleton(Singleton const&)=delete;
    Singleton& operator=(Singleton const&)=delete;
    static std::mutex mtx;
};

template<class T> T* Singleton<T>::m_Instance=nullptr;
template<class T> std::mutex Singleton<T>::mtx;

#endif // SINGLETON_H
