# main_process:
#     adds new tasks to a local priority queue
#     pops that priority queue into the multi-process queue
# 
# workers:
#   thread that sorts: pop the queue into a local priority queue
#   thread that calls the tasks from local pq  

from time import time
from multiprocessing import Process, Queue 
from threading import Thread
from queue import PriorityQueue


class Callable:
    def __init__(self, time, func, *args) -> None:
        self._time = time
        self._func = func
        self._args = args
    

    def get_time(self):
        return self._time


    def __call__(self):
        return self._func(*self._args)


class TimerManager:
    def __init__(self) -> None:
        self._sorting_queue = PriorityQueue()
        self._process_queue = Queue()
        self._process_workers: list[Process] = []
        self._sorting_thread = Thread(target=self._sorting_func)
        self._sorting_thread.start()


    def _sorting_func(self):
        while True:
            self._process_queue.put(self._sorting_queue.get())


    def add_task(self, task: Callable):
        self._sorting_queue.put((task.get_time(), task))
    

    def create_consumer_processes(self, num_of_process: int=1):
        for i in range(num_of_process):
            self._process_workers.append(ConsumerProcess(self._process_queue))
            self._process_workers[i].start()

    def join(self):
        self._sorting_thread.join()
        for p in self._process_workers:
            p.join()


class ConsumerProcess:
    def __init__(self, process_queue: Queue) -> None:
        self._process_queue = process_queue
        self._sorting_queue = None
        self._sorting_thread = None
        self._subprocess = Process(target=self._consumer_func)


    def _sorting_func(self):
        while True:
            self._process_queue.put(self._sorting_queue.get())


    def _consumer_func(self):
        self._sorting_queue = PriorityQueue()
        self._sorting_thread = Thread(target=self._sorting_func)
        self._sorting_thread.start()

        while True:
            task: Callable = self._sorting_queue.get()
            if time() >= task.get_time():
                task()
            else:
                self._sorting_queue.put((task.get_time(), task))


    def start(self):
        self._subprocess.start()


    def join(self):
        self._sorting_thread.join()
        self._subprocess.join()


def main():
    timer = TimerManager()
    timer.create_consumer_processes()

    for i in range(10, 0, -2):
        print(i)
        timer.add_task(Callable(time() + i, print, "printed in {}".format(time() + i)))

    timer.join()


if __name__ == '__main__':
    main()
