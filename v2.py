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
from multiprocessing import Process, Queue, Value
from threading import Thread
from queue import PriorityQueue
from uuid import UUID, uuid4

class Task:
    def __init__(self, time, func, *args) -> None:
        self._time = time
        self._func = func
        self._args = args
        self._is_active = True
        self._uuid = uuid4()

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
        self._process_queue = Queue()
        self._process_workers: list[_ConsumerProcess] = []

    def add_task(self, task: Task):
        self._process_queue.put(task)

    def remove_task(self, task: Task):
        self._process_queue.put(task._uuid)

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
    def __init__(self, process_queue: Queue) -> None:
        self._process_queue = process_queue
        self._sorting_queue = None
        self._sorting_thread = None
        self._subprocess = Process(target=self._consumer_func)

        self._subprocess.start()

    def _sorting_func(self):
        all_tasks_map: dict[UUID: Task] = {}
        while True:
            incoming = self._process_queue.get()
            if isinstance(incoming, UUID):
                try:
                    task_uuid: UUID = incoming
                    all_tasks_map[task_uuid]._is_active = False
                except:
                    pass
            else:
                new_task: Task = incoming
                if isinstance(incoming, _TerminatorTask):
                    return

                self._sorting_queue.put((new_task.get_time(), new_task))
                all_tasks_map[new_task._uuid] = new_task

    def _consumer_func(self):
        self._sorting_queue = PriorityQueue()
        self._sorting_thread = Thread(target=self._sorting_func)
        self._sorting_thread.start()
        
        keep_going = True
        while keep_going:
            task: Task = self._sorting_queue.get()[1]
            if time() >= task.get_time():
                if task._is_active:
                    task()
            else:
                self._sorting_queue.put((task.get_time(), task))
        
            keep_going = self._sorting_queue.qsize() > 0

        self._sorting_thread.join()

    def join(self):
        self._subprocess.join()


def main():
    timer = TimerManager()
    timer.create_consumer_processes()

    all_tasks = []
    for i in range(10, 0, -1):
        new_task = Task(time() + i, print, f"printed in {i} seconds delay")
        all_tasks.append(new_task)
        timer.add_task(new_task)
    

    timer.remove_task(all_tasks[0])
    timer.remove_task(all_tasks[-1])

    timer.join()


if __name__ == '__main__':
    main()
