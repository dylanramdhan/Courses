"""
    Assignment #3, due date: 2/25/2025
    Engineer: Dylan Ramdhan
    
    mp = multi-processing
    ** This file is the implementation of multi-threading
"""

import multiprocessing
import time
import numpy as np
import logging
import os # only used for "logging" folder


####### CONFIGURING LOGGING ########
# getting the directory of this file
script_dir = os.path.dirname(os.path.abspath(__file__))

# config logging directory inside the project
log_dir = os.path.join(script_dir, "logging")
os.makedirs(log_dir, exist_ok=True)  # Ensure the logging directory exists

# config logging file for multi-processing
log_file_path = os.path.join(log_dir, "multi_processing_queue_system.log")
logging.basicConfig(filename=log_file_path, level=logging.INFO,
                    format="%(asctime)s - %(levelname)s - %(message)s")
####################################


NUM_WORKERS = 4 # config number of worker processes


def worker(queue):
    # worker function processing tasks from queue
    while True:
        task = queue.get()
        if task is None:
            break  # stopping signal received
        
        matrix_a, matrix_b = task
        result = np.matmul(matrix_a, matrix_b)  # matrix multiplication
        
        logging.info("Processed a matrix multiplication")
        print("Processed a matrix multiplication")



if __name__ == "__main__":
    logging.info("Starting multi-processing queue system...")

    # defining a multi-processing queue
    task_queue = multiprocessing.Queue()

    # creating worker processes
    processes = []
    for _ in range(NUM_WORKERS):
        p = multiprocessing.Process(target=worker, args=(task_queue,))
        p.start()
        processes.append(p)


    # simulating different request rates
    NUM_REQUESTS = 100
    for _ in range(NUM_REQUESTS):  # simulating 100 requests
        A = np.random.rand(500, 500)  # large matrices
        B = np.random.rand(500, 500)  # large matrices
        task_queue.put((A, B))
        time.sleep(0.1)  # simulating different rates


    # stopping worker functions
    for _ in range(NUM_WORKERS):
        task_queue.put(None)

    for p in processes:
        p.join()  # ensuring everything is finish

    logging.info("Multi-processing queue processing completed.")
    print("Multi-processing queue processing completed.")