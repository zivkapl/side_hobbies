from ctypes import CDLL

my_lib = CDLL("simple_thread.so")
my_lib.run()
my_lib.join()