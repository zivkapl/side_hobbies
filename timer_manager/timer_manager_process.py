# Design:
#
#   main process:
#       for adding: adds new tasks to a process shared queue
#       for removal: adds the tasks UUID to the queue
# 
# 
#   worker process(s):
#       has 2 threads:
#           1. thread that pop the shared queue
#               - adding: adds the new task to a local priority queue
#                   and a local dictionary with the task's UUID as key
#               - removal: pops an UUID, go to the tasks object in the dict 
#                   and mark it as inactive
#           2. thread that pop the tasks from local sorted queue, 
#               if their time arrived:
#                   if active: call the callback
#               else put back in local priority queue 



from time import time
from multiprocessing import Process, Queue
from threading import Thread
from queue import PriorityQueue
from uuid import UUID

from task import Task

class _TerminatorTaskProcess(Task):
    def __init__(self) -> None:
        super().__init__(0, None)

    def __call__(self):
        pass


class TimerManagerProcess:
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
            self.add_task(_TerminatorTaskProcess())

        for p in self._process_workers:
            p.join()
            print(f"{p} is out")
        
 
class _ConsumerProcess:
    def __init__(self, process_queue: Queue) -> None:
        self._process_queue = process_queue
        self._sorting_queue = None
        self._sorting_thread = None
        self._subprocess = Process(target=self._entry_func)

        self._subprocess.start()

    def _entry_func(self):
        self._sorting_queue = PriorityQueue()
        self._sorting_thread = Thread(target=self._sorting_func)
        self._sorting_thread.start()

        self._consume()

        self._sorting_thread.join()
        
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
                if isinstance(incoming, _TerminatorTaskProcess):
                    return

                self._sorting_queue.put((new_task.get_time(), new_task))
                all_tasks_map[new_task.get_uuid()] = new_task

    def _consume(self):
        keep_going = True
        while keep_going:
            task: Task = self._sorting_queue.get()[1]
            if isinstance(task, _TerminatorTaskProcess):
                if self._sorting_queue.qsize() == 0:
                    return
            elif time() >= task.get_time():
                if task._is_active:
                    task()
            else:
                self._sorting_queue.put((task.get_time(), task))
        

    def join(self):
        self._subprocess.join()

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

def format_msg(delay):
    return "{} printed in {}{}{} seconds delay.{}" \
                .format(Colors.BLUE, Colors.CYAN, delay, Colors.BLUE, Colors.RESET)


def test():
    from threading import Semaphore
    from time import sleep

    timer = TimerManagerProcess()
    timer.create_consumer_processes()

    sem = Semaphore(0)

    all_tasks = []
    for i in range(10, 0, -1):
        new_task = Task(time() + i, print, format_msg(i))
        all_tasks.append(new_task)
        timer.add_task(new_task)

    print(Colors.YELLOW + "Sleeping for 15 seconds" + Colors.RESET)
    sleep(15)
    print(Colors.YELLOW + "Starting second task set" + Colors.RESET)

    all_tasks.clear()
    for i in range(10, 0, -1):
        new_task = Task(time() + i, print, format_msg(i))
        all_tasks.append(new_task)
        timer.add_task(new_task)

    for i in range(0, 10, 2):
        timer.remove_task(all_tasks[i])

    timer.join() #FIXME: deadlock

if __name__ == '__main__':
    test()
