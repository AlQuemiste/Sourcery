import logging
import threading
import time
from concurrent.futures import Future


def thread_function(name, ft):
    tid = threading.get_ident()
    logging.info("Thread %s <%s>: starting", name, tid)
    time.sleep(3)
    ft.set_result(3)
    logging.info("Thread %s <%s>: finishing", name, tid)


def callback(ft):
    """
    Added callables are called in the order that they were added and are
    always called in a thread belonging to the process that added them. If the
    callable raises an Exception subclass, it will be logged and ignored. If the
    callable raises a BaseException subclass, the behavior is undefined.
    https://docs.python.org/3.6/library/concurrent.futures.html#concurrent.futures.Future.add_done_callback
  """
    
    tid = threading.get_ident()
    result = ft.result()
    print("callback at <{}> with result = {}".format(tid, result))

    
def main():
    format = "%(asctime)s: %(message)s"
    logging.basicConfig(format=format, level=logging.INFO,
                        datefmt="%H:%M:%S")
    tid = threading.get_ident()
    logging.info("Main  at <%s>", tid)

    logging.info("Main    : before creating thread")
    ft = Future()
    ft.add_done_callback(callback)
    x = threading.Thread(target=thread_function, args=(1, ft))
    logging.info("Main    : before running thread")
    x.start()
    logging.info("Main    : wait for the thread to finish")
    x.join()
    logging.info("Main    : all done")
