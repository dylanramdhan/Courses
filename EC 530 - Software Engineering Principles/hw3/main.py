"""
    Assignment #3, due date: 2/25/2025
    Engineer: Dylan Ramdhan
    
    Focus: Understanding Concurrency - Multi-Threding & Multi-Processing
    
    Tasks:
        --> Develop a queue system where it can process requests of an API of your choosing.  For example, the API that you want to queue is to do a numpy api (e.g., numpy.matmul() )
        --> Make your queue size configurable 
        --> Test it with different call flooding (1 call every x seconds, 100 per x seconds, etc.)
        --> Implement it as:
            - Multi-threads
            - Multi-process
        --> Test it with large matrices and not small ones to stress your queue.

    ** This file is the implementation of multi-threading version w/queue system
"""

import threading
import queue
import numpy as np
import time
import logging

# config logging
logging.basicConfig(filename="queue_system.log", level=logging.INFO,
                    format="%(asctime)s - %(levelname)s - %(message)s")

# configurable queue size
QUEUE_SIZE = 10 ## small queue size
task_queue = queue.Queue(maxsize=QUEUE_SIZE)
results = {}  ## dictionary in storing results

logging.info("Starting multi-threaded queue system...")


def matrix_multiplication():
    # performing matrix multiplication w/large matricies (for stress testing)
    size = 1000  # Large matrix size
    A = np.random.rand(size, size)
    B = np.random.rand(size, size)
    return np.matmul(A, B)


def worker():
    # worker functions to process tasks from queue
    while True:
        task = task_queue.get()
        if task is None:
            break  # stopping signal received
        task_id, matrix1, matrix2 = task
        result = np.matmul(matrix1, matrix2)
        results[task_id] = result.tolist()  # storing result in JSON-serializable format
        logging.info(f"Task {task_id} completed")
        task_queue.task_done()


# creating worker threads
NUM_THREADS = 4
threads = []
for _ in range(NUM_THREADS):
    t = threading.Thread(target=worker)
    t.start()
    threads.append(t)


# adding tasks to queue
NUM_TASKS = 20
for i in range(NUM_TASKS):
    A = np.random.rand(500, 500)  # large matrices
    B = np.random.rand(500, 500)
    task_queue.put((i, A, B))
    time.sleep(0.1)  # simluating different request rates

task_queue.join()  # waiting for completion of all tasks


# stopping worker threads
for _ in range(NUM_THREADS):
    task_queue.put(None)

for t in threads:
    t.join()

logging.info("Multi-threading queue processing completed.")
print("Multi-threading queue processing completed.")
