#pragma once
template <class T>
class SingletonHolder
{
public:
    static T& Instance()
    {
        static T instance;
        return instance;
    }

private:
    SingletonHolder();
    SingletonHolder(const SingletonHolder&);
    SingletonHolder& operator=(const SingletonHolder&);
};