    #define EXTERN_CLASS(__NAME,...) \
        typedef union __NAME __NAME;\
        __VA_ARGS__\
        union __NAME {\
            uint_fast8_t chMask[(sizeof(struct {

    #define END_EXTERN_CLASS(__NAME) \
            }) + sizeof(uint_fast8_t) - 1) / sizeof(uint_fast8_t)];\
        };

    #define DEF_CLASS(__NAME,...)\
        typedef union __NAME __NAME;\
        __VA_ARGS__\
        typedef struct __##__NAME __##__NAME;\
        struct __##__NAME{

    #define END_DEF_CLASS(__NAME) \
        };\
        union __NAME {\
            uint_fast8_t chMask[(sizeof(__##__NAME) + sizeof(uint_fast8_t) - 1) / sizeof(uint_fast8_t)];\
        };

    #define CLASS(__NAME)               __##__NAME

