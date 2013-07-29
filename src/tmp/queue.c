//! queue.c

    ...
    //! \name byte queue
    //! @{
    DEF_CLASS(queue_t)
        uint8_t *pchBuffer;    //!< queue buffer  
        uint16_t hwBufferSize; //!< buffer size  
        uint16_t hwHead;       //!< head pointer
        uint16_t hwTail;       //!< tail pointer
        uint16_t hwCounter;    //!< byte counter
    END_DEF_CLASS(queue_t)
    //! @}
    ...

复制代码
对照前面的宏，我们实际上可以手工将上面的内容展开（这里就不再赘述了），可以看到，实际上类型queue_t
是一个掩码结构体，里面只有一个起到掩码作用的数组chMask，其大小和真正后台的的类型__queue_t相同——
这就是掩码结构体实现私有成员保护的秘密。解决了私有成员保护的问题，剩下还有一个问题，对于queue.c的
函数来说queue_t只是一个数组，那么正常的功能要如何实现呢？下面的代码片断将为你解释一切：

    ...
    bool is_queue_empty(queue_t *ptQueue)
    {
        CLASS(queue_t) *ptQ = (CLASS(queue_t) *)ptQueue;
        if (NULL == ptQueue) {
            return true;
        }
        return ((ptQ->hwHead == ptQ->hwTail) && (0 == ptQ->Counter));
    }
    ...

复制代码
从编译器的角度来说，这种从queue_t到__queue_t类型指针的转义是逻辑上的，并不会因此产生额外的代码，
简而言之，使用掩码结构体几乎是没有代价的——如果你找出了所谓的代价，一方面不妨告诉我，另一方面，
不妨考虑这个代价和模块的封装相比是否是可以接受的。欢迎您的讨论。
全文完


2013-6-25 Update
    更新了掩码结构体的宏，使其能够支持在类中使用指向自己的指针，同时也能插入 delegate 类型
（事件处理函数原型）的声明。统一了格式。


2013-5-10 Update
    更新宏定义，解决了不同平台下CPU访问掩码结构体时可能存在的数组未对其到内核宽度的问题。
所谓对齐到内核宽度是指，当用掩码结构体的类型声明静态变量时，8位机要对齐到Byte，16位机要
对齐到Half-Word，32位机要对齐到Word。
   该更新可以直接解决MSP430在IAR环境下无法正常使用掩码结构体的问题。关于uint_fast8_t的
说明，请参照ANSI-C99关于stdint.h的描述
