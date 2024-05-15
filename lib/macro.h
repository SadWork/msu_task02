#ifndef MACRO_H
#define MACRO_H

// #define DEBUG
#define CHECK_OVERFLOW(c)    (c & 0x1)
#define SET_OVERFLOW(c)      (c |= 0x1)
#define SKEWED_RIGHT(c)      (c & 0x2)
#define SET_SKEWED_RIGHT(c)  (c |= 0x2)
#define CHECK_COMMUTATIVE(c) (c & 0x4)
#define IS_COMMUTATIVE       (0x4)

#define ERROR_MSG(...) error_msg_impl(__VA_ARGS__)

#define ERROR_MSG_IMPL(msg)                                                                                                                          \
    do                                                                                                                                               \
    {                                                                                                                                                \
        fprintf(stderr, "Error: %s (Line %d in function %s)\n", msg, __LINE__, __func__);                                                            \
    } while (0)

#define ERROR_MSG_IMPL_VARIADIC(msg, ...)                                                                                                            \
    do                                                                                                                                               \
    {                                                                                                                                                \
        fprintf(stderr, msg, __VA_ARGS__);                                                                                                           \
        fprintf(stderr, "(Line %d in function %s)\n", __LINE__, __func__);                                                                           \
    } while (0)

#define GET_ERROR_MSG_IMPL_VARIADIC(_1, _2, NAME, ...) NAME

#define error_msg_impl(...) GET_ERROR_MSG_IMPL_VARIADIC(__VA_ARGS__, ERROR_MSG_IMPL_VARIADIC, ERROR_MSG_IMPL)(__VA_ARGS__)

#define break_if(condition)                                                                                                                          \
    if (condition)                                                                                                                                   \
    break
#endif