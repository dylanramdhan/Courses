"""
    Assignment #3, due date: 2/25/2025
    Engineer: Dylan Ramdhan
    
    ** This file is the implementation of Multithreading Queues w/Callbacks
"""

import threading
import queue
import numpy as np
import time
import logging

import os # only used for "logging" folder

####### CONFIGURING LOGGING ########
# config logging
log_dir = "/Users/dylanramdhan/Documents/GitHub/Personal Repo/Courses/EC 530 - Software Engineering Principles/hw3/logging"
os.makedirs(log_dir, exist_ok=True)

# configured logging file to save logs in the "logging" folder
log_file_path = os.path.join(log_dir, "callback_system.log")
logging.basicConfig(filename=log_file_path, level=logging.INFO,
                    format="%(asctime)s - %(levelname)s - %(message)s")
####################################


# configurable queue size
QUEUE_SIZE = 10 ## small queue size
task_queue = queue.Queue(maxsize=QUEUE_SIZE)

def matrix_multiplication(task_id, callback):
    # perform matrix multiplication and trigger callback
    try:
        size = 1000  # larger matrix size
        A = np.random.rand(size, size)
        B = np.random.rand(size, size)
        result = np.matmul(A, B)
        callback(task_id, result, None)  # sucess callback
    
    except Exception as e:
        callback(task_id, None, str(e))  # error callback


def worker():
    # worker function to process tasks from queue
    while True:
        task = task_queue.get()
        
        if task is None:
            break  # stopping condition
        
        task_id, callback = task
        matrix_multiplication(task_id, callback)
        task_queue.task_done()


def callback(task_id, result, error):
    # callback function to handle success/error messages
    if error:
        logging.error(f"Task {task_id} failed: {error}")
        print(f"❌ Task {task_id} failed: {error}")
    
    else:
        logging.info(f"Task {task_id} completed successfully")
        print(f"✅ Task {task_id} completed successfully")



# creating worker threads
NUM_THREADS = 4
threads = []
for _ in range(NUM_THREADS):
    t = threading.Thread(target=worker)
    t.start()
    threads.append(t)


# simulating different request rates
NUM_TASKS = 20
request_rates = [1, 10, 50]  # calls per second



for rate in request_rates:
    print(f"\nSimulating {rate} requests per second...")
    start_time = time.perf_counter()
    for i in range(NUM_TASKS):
        while task_queue.full():
            time.sleep(0.01)
        task_queue.put((i, callback))
        time.sleep(1 / rate)  # control request rate

    task_queue.join()
    end_time = time.perf_counter()
    print(f"Processing completed in {end_time - start_time:.2f} seconds")



# stopping worker threads
for _ in range(NUM_THREADS):
    task_queue.put(None)

for t in threads:
    t.join()


logging.info("Multi-threading with callback processing completed.")
print("\nMulti-threading with callback processing completed.")