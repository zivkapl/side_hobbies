import threading
import ThreadSafePriorityQueue as TSPQ

import time
from multiprocessing import Pool
import sys

import singeltons

class EventLooper():
    FINISHED = Finish()
    PUT_BACK = PutBack()
    _TERMINATE = _Terminate()

    def __init__(self) -> None:
        self._queue = ProcessSafePriorityQueue()
        self._subprocess = Pool(1)


    def add_task(self, callback, *args, **kwargs):
        self._add_task(callback, *args, **kwargs)


    def _add_task(self, callback, *args, priority=3, **kwargs):
        task = {"callback": callback, "args": args, "kwargs": kwargs }
        self._queue.put(task, priority)

    def start(self):
        self._subprocess.apply_async(EventLooper._subprocess_func, (self._queue,))


    def _join(self):
        self._subprocess.close()
        self._subprocess.terminate()
        self._subprocess.join()

    def terminate(self):
        self.add_task(EventLooper._terminate)
        self._join()

    def kill(self):
        self._add_task(EventLooper._terminate, priority=1)
        self._join()

    
    @staticmethod
    def _terminate():
        return EventLooper._TERMINATE


    @staticmethod
    def _subprocess_func(tasks_queue):
        while True:
            task = tasks_queue.get()
            rt = task["callback"](*task["args"], **task["kwargs"])
            if rt == EventLooper.PUT_BACK:
                tasks_queue.put(task, True)
            elif rt == EventLooper._TERMINATE:
                break


class Alarm():
    def __init__(self) -> None:
        self._event_looper = EventLooper()
        self._event_looper.start()
        self._active_alarms = 0

    def set_timer(self, seconds):
        self._event_looper.add_task(self._set_timer, self, seconds)
        self._active_alarms += 1

    def join(self):
        self._event_looper.terminate()

    def _set_timer(self, seconds):
        now = time.time()
        print("here", file=sys.stderr.fileno())
        if now < (now + seconds):
            return EventLooper.PUT_BACK
        
        print("RING RING")
        self._active_alarms -= 1
        return EventLooper.FINISHED


if __name__== "__main__":
    alarm = Alarm()
    # alarm.set_timer(10)
    alarm.set_timer(5)
    time.sleep(10)
    alarm.join()

