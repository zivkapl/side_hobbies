from queue import PriorityQueue
from multiprocessing import Semaphore

class ComparableItem():
    def __init__(self, item, priority) -> None:
        self._priority = priority
        self._item = item

    
    def value(self):
        return self._item
    

    def __lt__(self, other):
        return self._priority < other._priority


class ProcessSafePriorityQueue():
    def __init__(self) -> None:
        self._queue = PriorityQueue()
        self._sem = Semaphore(1)

    def put(self, item, priority, block=True, timeout=None):
        """
        priority 1 is the most important
        """
        self._sem.acquire()
        self._queue.put(ComparableItem(item, priority), block, timeout)
        self._sem.release()

    def get(self, block=True, timeout=None):
        self._sem.acquire()
        value = self._queue.get(block, timeout).value()
        self._sem.release()
        return value

