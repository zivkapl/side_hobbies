import time
from multiprocessing import Pool
import sys

class Singleton(type):
    _instances = {}
    def __call__(cls, *args, **kwargs):
        if cls not in cls._instances:
            cls._instances[cls] = super(Singleton, cls).__call__(*args, **kwargs)
        return cls._instances[cls]

class EventLooperState(metaclass=Singleton):
    def __init__(self) -> None:
        pass

class PutBack(EventLooperState):
    def __init__(self) -> None:
        super().__init__()


class Finish(EventLooperState):
    def __init__(self) -> None:
        super().__init__()

class _Terminate(EventLooperState):
    def __init__(self) -> None:
        super().__init__()


class EventLooper():
    FINISHED = Finish()
    PUT_BACK = PutBack()
    _TERMINATE = _Terminate()

    def __init__(self, count=1) -> None:
        self._pool = Pool(count)


    def add_task(self, callback, *args, **kwargs):
        task = {"callback": callback, "args": args, "kwargs": kwargs }
        self._pool.apply_async(EventLooper._subprocess_func, (self._pool, task))


    def join(self):
        # self.add_task(EventLooper._terminate)
        self._pool.close()
        self._pool.terminate()
        self._pool.join()

    
    @staticmethod
    def _terminate():
        return EventLooper._TERMINATE


    @staticmethod
    def _subprocess_func(pool, task):
        while True:
            rt = task["callback"](*task["args"], **task["kwargs"])
            if rt == EventLooper.PUT_BACK:
                pool.apply_async(EventLooper._subprocess_func, (pool, task))
            elif rt == EventLooper._TERMINATE:
                break


# class Alarm():
#     def __init__(self) -> None:
#         self._event_looper = EventLooper()
#         self._event_looper.start()
#         self._active_alarms = 0

#     def set_timer(self, seconds):
#         self._event_looper.add_task(self._set_timer, self, seconds)
#         self._active_alarms += 1

#     def join(self):
#         self._event_looper.terminate()

#     def _set_timer(self, seconds):
#         now = time.time()
#         print("here", file=sys.stderr.fileno())
#         if now < (now + seconds):
#             return EventLooper.PUT_BACK
        
#         print("RING RING")
#         self._active_alarms -= 1
#         return EventLooper.FINISHED

def foo():
    print("foo")

if __name__== "__main__":
    # alarm = Alarm()
    # # alarm.set_timer(10)
    # alarm.set_timer(5)
    # time.sleep(10)
    # alarm.join()
    e = EventLooper()
    e.add_task(foo)
    e.join()