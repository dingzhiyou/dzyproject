#include <memory>
#include <iostream>
using namespace std;

class A: public enable_shared_from_this<A>{
public:
        A(){};
        ~A(){
            auto it = this->shared_from_this();
            cout<<it.use_count()<<endl;
        }

private:
};


int main(){
    shared_ptr<A> xx(new A);
}
