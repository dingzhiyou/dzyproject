#ifndef DZY_SINGLETON_H__
#define DZY_SINGLETON_H__


namespace dzy {

template <class T>
class Singleton {
    public:
        static T* GetInstance(){
            static T t;
            return  &t;
        }
};






}

#endif
