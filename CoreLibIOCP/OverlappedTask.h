#pragma once

class OverlappedTask
{
public:
    OverlappedTask();
    template <typename Func, typename... Args>
    void bindExecute(Func& func, Args&&... args)
    {
        f = [func, args...]()
        {
            func(args...);
        };
    }
    void Execute();

    Function f = nullptr;
private:
    WSAOVERLAPPED _overlapped;
};
