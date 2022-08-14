class Singleton(type):
    _instances = {}
    def __call__(cls, *args, **kwargs):
        if cls not in cls._instances:
            cls._instances[cls] = super(Singleton, cls).__call__(*args, **kwargs)
        return cls._instances[cls]

class EventLooperState(metaclass=Singleton):
    def __init__(self) -> None:
        pass

class PutBack(EventLooperState):
    def __init__(self) -> None:
        super().__init__()


class Finish(EventLooperState):
    def __init__(self) -> None:
        super().__init__()

class _Terminate(EventLooperState):
    def __init__(self) -> None:
        super().__init__()

