"""
    Assignment #3, due date: 2/25/2025
    Engineer: Dylan Ramdhan
    
    ** This file is the implementation of multi-threading
"""

import multiprocessing
import time
import numpy as np
import logging


# configurable logging
logging.basicConfig(filename="multiprocessing_system.log", level=logging.INFO,
                    format="%(asctime)s - %(levelname)s - %(message)s")


# configurable queue size
QUEUE_SIZE = 10


def worker(queue):
    # worker function processing tasks from queue
    while True:
        task = queue.get()
        if task is None:
            break  # stopping signal if received
        
        matrix_a, matrix_b = task
        result = np.matmul(matrix_a, matrix_b)  # matrix multiplication
        
        logging.info("Processed a matrix multiplication") # statements
        print("Processed a matrix multiplication")



if __name__ == "__main__":
    logging.info("Starting multi-processing queue system...")

    # defining a multiprocessing queue
    task_queue = multiprocessing.Queue(maxsize=QUEUE_SIZE)

    # creating worker processes
    NUM_WORKERS = 4
    processes = []
    
    for _ in range(NUM_WORKERS):
        p = multiprocessing.Process(target=worker, args=(task_queue,))
        p.start()
        processes.append(p)


    # simulating different request rates
    for _ in range(100):  # simulating 100 requests
        A = np.random.rand(500, 500)  # large matrices
        B = np.random.rand(500, 500)
        task_queue.put((A, B))
        time.sleep(0.1)  # simulating different rates


    # stoppping worker functions
    for _ in range(NUM_WORKERS):
        task_queue.put(None)

    for p in processes:
        p.join()  # ensuring processing all before exiting

    logging.info("Multi-processing queue processing completed.") # statements
    print("Multi-processing queue processing completed.")
