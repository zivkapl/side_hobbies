import sys
import os
sys.path.append(os.path.abspath(os.path.dirname(os.path.dirname(__file__))))
from timer_manager import TimerManager, Task

from threading import Semaphore
from time import time

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
