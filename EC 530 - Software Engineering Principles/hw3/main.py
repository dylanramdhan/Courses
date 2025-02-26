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

    ** This file is the implementation of multi-threading w/queue system
"""

import threading
import queue
import numpy as np
import time
import logging
import os # only used for "logging" folder

 ####### CONFIGURING LOGGING #######
# getting absolute path for "logging" directory
script_dir = os.path.dirname(os.path.abspath(__file__))  # getting script's directory
log_dir = os.path.join(script_dir, "logging")  # ensuring logging folder is in the same directory
os.makedirs(log_dir, exist_ok=True)

# config save logs in the "logging" folder
log_file_path = os.path.join(log_dir, "main_queue_system.log")
logging.basicConfig(filename=log_file_path, level=logging.INFO,
                    format="%(asctime)s - %(levelname)s - %(message)s")
####################################


# config queue size
QUEUE_SIZE = 10
task_queue = queue.Queue(maxsize=QUEUE_SIZE)
results = {}  # dictionary for storing results

logging.info("Starting multi-threaded queue system...")


def matrix_multiplication():
    # perform matrix multiplication w/large matrices
    size = 1000  # enlarged matrix size
    A = np.random.rand(size, size)
    B = np.random.rand(size, size)
    return np.matmul(A, B)



def worker():
    # worker function to process tasks from queue
    while True:
        task = task_queue.get()
        if task is None:
            break  # stopping signal thats received
        task_id, matrix1, matrix2 = task
        result = np.matmul(matrix1, matrix2)
        results[task_id] = result.tolist()
        logging.info(f"Task {task_id} completed successfully")
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
    A = np.random.rand(500, 500) # large matrices
    B = np.random.rand(500, 500) # large matrices
    task_queue.put((i, A, B))
    time.sleep(0.1)



task_queue.join()

# stopping worker threads
for _ in range(NUM_THREADS):
    task_queue.put(None)


for t in threads:
    t.join()


logging.info("Multi-threading queue processing completed.")
print("Multi-threading queue processing completed.")