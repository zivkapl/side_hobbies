from time import time
from threading import Thread, Lock
from queue import PriorityQueue
from uuid import UUID

from task import Task

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


######################## Test ########################

class Colors:
    HEADER = '\033[95m'
    BLUE = '\033[94m'
    CYAN = '\033[96m'
    GREEN = '\033[92m'
    YELLOW = '\033[93m'
    RED = '\033[91m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'
    RESET = '\033[0m'

def last_task(sem):
    sem.release()

def format_msg(delay):
    return "{} printed in {}{}{} seconds delay.{}" \
                .format(Colors.BLUE, Colors.CYAN, delay, Colors.BLUE, Colors.RESET)

def formated_list_print(l):
    print(Colors.YELLOW)
    print(*l, sep='\n')
    print(Colors.BOLD)
    print("List len:", len(l))
    print(Colors.RESET)


def test():
    from threading import Semaphore

    timer = TimerManager()
    sem = Semaphore(0)

    all_tasks = []
    for i in range(10, 0, -1):
        new_task = Task(time() + i, print, format_msg(i))
        all_tasks.append(new_task)
        timer.add_task(new_task)

    formated_list_print(timer.active_tasks())
    timer.add_task(Task(time() + 12, last_task, sem))
    sem.acquire()
    print(Colors.GREEN + "Finished first tasks set" + Colors.RESET)

    all_tasks.clear()
    for i in range(10, 0, -1):
        new_task = Task(time() + i, print, format_msg(i))
        all_tasks.append(new_task)
        timer.add_task(new_task)

    formated_list_print(timer.active_tasks())
    for i in range(0, 10, 2):
        timer.remove_task(all_tasks[i])

    formated_list_print(timer.active_tasks())
    timer.add_task(Task(time() + 12, last_task, sem))
    sem.acquire()
    print(Colors.GREEN + "Finished second tasks set" + Colors.RESET)

    timer.join()

if __name__ == '__main__':
    test()
