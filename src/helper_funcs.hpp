#pragma once


// https://stackoverflow.com/questions/4003232/how-to-code-a-modulo-operator-in-c-c-obj-c-that-handles-negative-numbers

template <typename T>
T mod(T a, long int b) {
    if(b < 0) //you can check for b == 0 separately and do what you want
        return -mod(-a, -b);   
    int ret = a % b;
    if(ret < 0)
        ret+=b;
    return ret;
}
/*
Used these tests to verify mod:

assert(mod(0,1) == 0);
assert(mod(4,5) == 4);
assert(mod(5,5) == 0);
assert(mod(6,5) == 1);
assert(mod(-1,5) == 4);
*/



template <typename T>
T wrap(long int lower, long int upper, T value) {
    return mod(value - lower, upper - lower) + lower;
}
/*
Used these tests to verify wrap:

assert(wrap(0,5,4) == 4);
assert(wrap(0,5,-1) == 4);
assert(wrap(10,20,15) == 15);
assert(wrap(10,20,25) == 15);
assert(wrap(10,20,10) == 10);
assert(wrap(10,20,20) == 10);
*/