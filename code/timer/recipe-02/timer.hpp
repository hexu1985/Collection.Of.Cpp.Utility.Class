#pragma once

#include <functional>
#include <memory>

class Timer {
public:
    typedef std::function<void ()> Callback;

    Timer(double interval, Callback function); 
    void Start();
    void Cancel(); 

#ifdef DEBUG
    void SetMessage(const std::string& message);
#endif

public:
    struct Impl; 

private:
    std::shared_ptr<Impl> pimpl;
};


