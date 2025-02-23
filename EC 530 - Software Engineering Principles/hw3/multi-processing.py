"""
    Assignment #3, due date: 2/25/2025
    Engineer: Dylan Ramdhan
    
    This file is the implementation of Multiprocessing version (for CPU-heavy tasks)
"""

import multiprocessing
import time
import numpy as np

# define queue size
QUEUE_SIZE = 10

def worker(queue):
    # worder function processing tasks from queue
    while True:
        task = queue.get()
        if task is None:
            break  # stopping processing if NONE is received
        matrix_a, matrix_b = task
        result = np.matmul(matrix_a, matrix_b)  # simulating heavy computation
        print("Processed a matrix multiplication")



if __name__ == '__main__':
    # defining a queue
    task_queue = multiprocessing.Queue(maxsize=QUEUE_SIZE)

    # creating worker processes
    num_workers = 4
    processes = []
    
    for _ in range(num_workers):
        p = multiprocessing.Process(target=worker, args=(task_queue,))
        p.start()
        processes.append(p)


    # simulating different request rates
    for _ in range(100):  # simulating 100 requests
        A = np.random.rand(500, 500)  # simulating large matrices
        B = np.random.rand(500, 500)
        task_queue.put((A, B))
        time.sleep(0.1)  # simulating different rates


    # stopping worker functions
    for _ in range(num_workers):
        task_queue.put(None)  # sending stop signal to worker functions

    for p in processes:
        p.join()  # ensuring all processes finish before exiting