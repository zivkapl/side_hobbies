import os
from subprocess import call
import time
import signal

class SingleTaskScheduler():
    def __init__(self) -> None:
        self._is_running = False
        self._task = {}

    def start(self) -> None:
        self._is_running = True
        time.sleep(self._task["time"] - time.time())
        if self._is_running:
            self._task["callback"]()
        self.clear()

    def clear(self) -> None:
        self._is_running = False
        self._task.clear()

    def set_task(self, callback, time) -> None:
        self.clear()
        self._task["time"] = time;
        self._task["callback"] = callback;

    def stop(self)-> None:
        self.clear()
        print("------------------")
        print("stopping scheduler")
        print("------------------")


def create_stop_scheduler_func(scheduler) -> function:
    def stop(signum, frame):
        scheduler.stop()
    return stop


class Motor():
    def __init__(self) -> None:
        self._scheduler = SingleTaskScheduler()
    
    def _on(self) -> None:
        print("on")
        time.sleep(1000000)

    def _off(self) -> None:
        print("off")

    def set(self, callback, time) -> None:
        self._scheduler.set_task(self._on, time)
    
    def clear(self) -> None:
        self._scheduler.clear()

    def start(self):
        pass


if __name__ == "__main__":

    print(os.getpid())
    my_sched = SingleTaskScheduler()

    stop_my_sched = create_stop_scheduler_func(my_sched)
    signal.signal(signal.SIGUSR1, stop_my_sched)
    def my_print():
        print("task 1")

    def my_print_2():
        print("task 2")

    my_sched.set_task(my_print, time.time() + 6)
    my_sched.start()
