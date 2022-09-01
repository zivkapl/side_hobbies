import sys
import os
sys.path.append(os.path.abspath(os.path.dirname(os.path.dirname(__file__))))
from timer_manager import TimerManagerProcess, Task

from time import time, sleep

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

    timer = TimerManagerProcess()
    timer.create_consumer_processes()

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
