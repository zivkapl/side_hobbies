from multiprocessing import Process, Queue
import sys


class Alarm():
    FINISHED = 0 # TODO: make null-object pattern
    PUT_BACK = 1 # TODO: make null-object pattern
    _TERMINATE = 2 # TODO: make null-object pattern
    

    def __init__(self) -> None:
        self._queue = Queue()
        self._subprocess = Process(target=Alarm._subprocess_func, args=(self._queue,))


    def AddTask(self, callback, **kwargs):
        task = { "callback": callback, "kwargs": kwargs }
        self._queue.put(task)


    def start(self):
        self._subprocess.start()

    
    def join(self):
        self._subprocess.join()


    def terminate(self):
        self.AddTask(Alarm._terminate)
        self._subprocess.join()


    @staticmethod
    def _terminate():
        return Alarm._TERMINATE


    @staticmethod
    def _subprocess_func(tasks_queue: Queue):
        while True:
            task = tasks_queue.get()
            rt = task["callback"](**task["kwargs"])
            if rt == Alarm.PUT_BACK:
                tasks_queue.put(task, True)
            elif rt == Alarm._TERMINATE:
                break


class foo():
    def __init__(self) -> None:    
        self._is_first_time = True
        self._counter = 0

    def __call__(self, count):
        if self._is_first_time:
            self._counter = count
            self._is_first_time = False
    
        self._counter -= 1
        print(f"task no. {self._counter + 1}",file=sys.stderr, flush=True)
        return Alarm.PUT_BACK if self._counter else Alarm.FINISHED



if __name__== "__main__":
    alarm = Alarm()
    alarm.start()
    while count := int(input("0 to exit, else to add task with count\n")):
        alarm.AddTask(foo(), count=count)
    
    alarm.terminate()
