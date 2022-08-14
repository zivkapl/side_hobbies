#ifndef __ZK_TIMER_THREAD_H__
#define __ZK_TIMER_THREAD_H__

/********************************************/
/* Timer Thread API                         */
/* version 1.0                              */
/********************************************/

#include <stdint.h>

typedef struct timer_thread timer_thread_t;
typedef struct timer_instance timer_inst_t;

timer_thread_t *timer_thread_init(void);
/* 
void timer_thread_join(timer_thread_t *timer_thread);
void timer_thread_force_stop_and_join(timer_thread_t *timer_thread);

timer_inst_t *set_timer(timer_thread_t *timer_thread,
                        uint64_t seconds_from_now,
                        void *(*callback)(void *param), 
                        void *param);
void clear_timer(timer_thread_t *timer_thread, timer_inst_t* timer);
 */
#endif /* __ZK_TIMER_THREAD_H__ */
