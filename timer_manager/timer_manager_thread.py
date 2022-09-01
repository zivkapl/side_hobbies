from time import time
from threading import Thread, Lock
from queue import PriorityQueue
from uuid import UUID

from .task import Task

class _TerminatorTask(Task):
    def __init__(self) -> None:
        super().__init__(0, None)

    def __call__(self):
        pass

class TimerManager:
    def __init__(self) -> None:
        self._all_active_tasks_map: dict[UUID: Task] = {}
        self._sorting_queue = PriorityQueue()
        self._sorting_thread = Thread(target=self._consume)
        self._sorting_thread.start()
        self._lock = Lock()

    def _consume(self):
        while True:
            task: Task = self._sorting_queue.get()[1]
            if isinstance(task, _TerminatorTask):
                if self._sorting_queue.qsize() == 0:
                    return
            elif time() >= task.get_time():
                with self._lock:
                    if task._is_active:
                        task()
                    self._all_active_tasks_map.pop(task.get_uuid(), None)
            else:
                self._sorting_queue.put((task.get_time(), task))

    def add_task(self, task: Task):
        self._sorting_queue.put((task.get_time(), task))
        self._all_active_tasks_map[task.get_uuid()] = task

    def remove_task(self, task: Task):
        with self._lock:
            self._all_active_tasks_map[task.get_uuid()]._is_active = False
            self._all_active_tasks_map.pop(task.get_uuid(), None)

    def active_tasks(self):
        return list(self._all_active_tasks_map.values())

    def join(self):
        self.add_task(_TerminatorTask())
        self._sorting_thread.join()
        print(f"{self._sorting_thread} is out")
