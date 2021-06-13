#ifndef TUPLE_HPP
#define TUPLE_HPP

template <typename T1, typename T2>
struct Tuple
{
    T1 first;
    T2 second;

    Tuple(const T1 &first, const T2 &second) : first{first}, second{second}
    {
    }
};

#endif