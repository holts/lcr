//! queue.h

    ...
    //! \name byte queue
    //! @{
    EXTERN_CLASS(queue_t)
        uint8_t *pchBuffer;    //!< queue buffer   
        uint16_t hwBufferSize; //!< buffer size
        uint16_t hwHead;       //!< head pointer
        uint16_t hwTail;       //!< tail pointer
        uint16_t hwCounter;    //!< byte counter
    END_EXTERN_CLASS(queue_t)
    //! @}
    ...
    extern bool queue_init(queue_t *ptQueue, uint8_t *pchBuffer, uint16_t hwSize);
    extern bool enqueue(queue_t *ptQueue, uint8_t chByte);
    extern bool dequeue(queue_t *ptQueue, uint8_t *pchByte);
    extern bool is_queue_empty(queue_t *ptQueue);
    ...

    
    typedef union queue_t queue_t; __VA_ARGS__ union queue_t {\
            uint_fast8_t chMask[(sizeof(struct {

		      uint8_t *pchBuffer;    //!< queue buffer   
        uint16_t hwBufferSize; //!< buffer size
        uint16_t hwHead;       //!< head pointer
        uint16_t hwTail;       //!< tail pointer
        uint16_t hwCounter;    //!< byte counter

  
            }) + sizeof(uint_fast8_t) - 1) / sizeof(uint_fast8_t)]; };

