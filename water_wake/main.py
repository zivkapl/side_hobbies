from time import time
from ctypes import CDLL, c_void_p

def print_ziv(arg):
    print("Ziv")
    print(f"{time()=}")

def main():
    my_lib = CDLL("./timer/timer.so")

    timer_service = c_void_p(my_lib.timer_service_init())

    my_lib.set_timer(timer_service, 10, c_void_p(print_ziv), c_void_p(0))
    my_lib.set_timer(timer_service, 5, c_void_p(print_ziv), c_void_p(0))

    my_lib.timer_service_wait_and_join(timer_service)


if __name__ == "__main__":
    main()
    