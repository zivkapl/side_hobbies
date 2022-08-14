#ifndef __ZK_TIMER_THREAD_H__
#define __ZK_TIMER_THREAD_H__

/********************************************/
/* Timer Thread API                         */
/* version 1.0                              */
/********************************************/

#include <stdint.h>

typedef struct timer_service timer_service_t;
typedef struct timer_instance timer_instance_t;

timer_service_t *timer_service_init(void);
/* 
void timer_service_join(timer_service_t *timer_service);
void timer_service_force_stop_and_join(timer_service_t *timer_service);

timer_inst_t *set_timer(timer_service_t *timer_service,
                        uint64_t seconds_from_now,
                        void *(*callback)(void *param), 
                        void *param);
void clear_timer(timer_service_t *timer_service, timer_inst_t* timer);
 */
#endif /* __ZK_TIMER_THREAD_H__ */
