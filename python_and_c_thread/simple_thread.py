from time import sleep
from ctypes import CDLL, c_uint64

def main():
    my_lib = CDLL("simple_thread.so")

    print(my_lib);
    
    my_lib.run(c_uint64(1))

    # for i in range(6):
    #     print("from Python thread. i = {}". format(i))
    #     sleep(1.5)

    my_lib.join()


if __name__ == "__main__":
    main()
    