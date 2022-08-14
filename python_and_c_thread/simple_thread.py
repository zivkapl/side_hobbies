from time import sleep
from ctypes import CDLL, c_uint64


def main():
    my_lib = CDLL("simple_thread.so")
    my_lib.run(c_uint64(5))

    for i in range(10):
        print("from Python thread. i = {}". format(i))
        sleep(1)

    my_lib.join()


if __name__ == "__main__":
    main()
