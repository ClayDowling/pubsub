#include <iostream>
#include <concepts>
#include <coroutine>
#include <functional>
#include <string>
#include <type_traits>
#include <thread>
#include <vector>

using namespace std;

template <typename M>
    requires is_trivial_v<M>
class publisher
{
public:
    void subscribe(std::function<void(M)> h)
    {
        handlers.emplace_back(h);
    }
    
    void publish(M msg)
    {
        for (auto h : handlers)
        {
            jthread hdlr([msg, this]()
                         { h(msg); });
        };
    }

private:
    vector<std::function<void(M)>> handlers;
};

struct typeA
{
    const char *name;
};

publisher<typeA> userPublisher;

void welcomeUser(typeA u)
{
    this_thread::sleep_for(2000ms);
    cout << "Welcome, " << u.name << endl;
}

void payUser(typeA u)
{
    cout << "Paying " << u.name << " $200" << endl;
}

int main(int argc, const char *argv[])
{

    userPublisher.subscribe(welcomeUser);
    userPublisher.subscribe(payUser);

    typeA alice{"Alice"};
    typeA bob{"Bob"};

    userPublisher.publish(alice);
    userPublisher.publish(bob);

    return 0;
}