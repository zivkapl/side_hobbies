from uuid import UUID, uuid4

class Task:
    def __init__(self, time, func, *args) -> None:
        self._time = time
        self._func = func
        self._args: list = args
        self._is_active: bool = True
        self._uuid: UUID = uuid4()

    def get_time(self):
        return self._time

    def get_uuid(self):
        return self._uuid

    def __call__(self):
        self._func(*self._args)
    
    def __repr__(self) -> str:
        return "Task {}. time: {}. callback: {}. args: {}" \
                .format(self.get_uuid(), self.get_time(), self._func, self._args)
