# Design:
#
#   main process:
#       adds new tasks to a process shared queue
# 
# 
#   worker process(s):
#       thread that sorts the queue pop the queue into a local priority queue
#       thread that calls the tasks from local sorted queue



from time import time
from multiprocessing import Process, JoinableQueue, Value
from threading import Thread
from queue import PriorityQueue


class Task:
    def __init__(self, time, func, *args) -> None:
        self._time = time
        self._func = func
        self._args = args
        self._can_add_tasks = True

    def get_time(self):
        return self._time


    def __call__(self):
        self._func(*self._args)


class _TerminatorTask(Task):
    def __init__(self) -> None:
        super().__init__(0, None)

    def __call__(self):
        pass


class TimerManager:
    def __init__(self) -> None:
        self._process_queue = JoinableQueue()
        self._process_workers: list[_ConsumerProcess] = []


    def add_task(self, task: Task):
            self._process_queue.put(task)


    def create_consumer_processes(self, num_of_process: int=1):
        for i in range(num_of_process):
            self._process_workers.append(_ConsumerProcess(self._process_queue))


    def join(self):
        for p in self._process_workers:
            self.add_task(_TerminatorTask())

        for p in self._process_workers:
            p.join()
            print(f"{p} is out")
        
 
class _ConsumerProcess:
    def __init__(self, process_queue: JoinableQueue) -> None:
        self._process_queue = process_queue
        self._sorting_queue = None
        self._sorting_thread = None
        self._subprocess = Process(target=self._consumer_func)

        self._subprocess.start()

    def _sorting_func(self):
        while True:
            task: Task = self._process_queue.get()
            self._sorting_queue.put((task.get_time(), task))
            self._process_queue.task_done()
            if isinstance(task, _TerminatorTask):
                return
            

    def _consumer_func(self):
        self._sorting_queue = PriorityQueue()
        self._sorting_thread = Thread(target=self._sorting_func)
        self._sorting_thread.start()
        
        keep_going = True
        while keep_going:
            task: Task = self._sorting_queue.get()[1]
            if time() >= task.get_time():
                task()
            else:
                self._sorting_queue.put((task.get_time(), task))
        
            keep_going = self._sorting_queue.qsize() > 0

        self._sorting_thread.join()


    def join(self):
        self._subprocess.join()


def main():
    timer = TimerManager()
    timer.create_consumer_processes(10)

    for i in range(10, 0, -2):
        timer.add_task(Task(time() + i, print, f"printed in {i} seconds delay"))
    
    timer.join()


if __name__ == '__main__':
    main()
