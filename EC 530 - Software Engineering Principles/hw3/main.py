"""
    Assignment #1, due date: 1/23/2025
    Engineer: Dylan Ramdhan
    
    Focus: Understanding Concurrency: Multi-Threding & Multi-Processing
    
    Tasks:
    --> Develop a queue system where it can process requests of an API of your choosing.  For example, the API that you want to queue is to do a numpy api (e.g., numpy.matmul() )
    --> Make your queue size configurable 
    --> Test it with different call flooding (1 call every x seconds, 100 per x seconds, etc.)
    --> Implement it as:
        - Multi-threads
        - Multi-process
    --> Test it with large matrices and not small ones to stress your queue.
"""

import threading
import queue
import numpy as np
import time

# Configurable Queue Size
QUEUE_SIZE = 10  
task_queue = queue.Queue(maxsize=QUEUE_SIZE)

def matrix_multiplication():
    """ Function to perform matrix multiplication """
    size = 1000  # Large matrix size for stress testing
    A = np.random.rand(size, size)
    B = np.random.rand(size, size)
    return np.matmul(A, B)

def worker():
    """ Worker function to process tasks from the queue """
    while True:
        task = task_queue.get()
        if task is None:
            break  # Exit condition
        result = matrix_multiplication()
        print(f"Task {task} completed")
        task_queue.task_done()

# Creating Threads
NUM_THREADS = 4  # Configurable number of threads
threads = []
for _ in range(NUM_THREADS):
    t = threading.Thread(target=worker)
    t.start()
    threads.append(t)

# Adding tasks to the queue
NUM_TASKS = 20  # Number of tasks to process
for i in range(NUM_TASKS):
    task_queue.put(i)
    time.sleep(0.1)  # Simulate different call rates

# Waiting for all tasks to complete
task_queue.join()

# Stopping threads
for _ in range(NUM_THREADS):
    task_queue.put(None)  # Stop signal

for t in threads:
    t.join()

print("Multi-threading queue processing completed.")
