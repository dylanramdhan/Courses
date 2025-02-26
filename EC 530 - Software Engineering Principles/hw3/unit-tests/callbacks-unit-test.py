"""
    Assignment #3, due date: 2/25/2025
    Engineer: Dylan Ramdhan
    
    ** This file is the unit test for the implementation of Multithreading Queues w/Callbacks
"""

import unittest
import queue
import time
from callbacks import task_queue, worker, matrix_multiplication, callback  # importing callbacks.py functions


class TestCallbackQueueProcessing(unittest.TestCase):
    
    def test_queue_initialization(self):
        # testing if queue initializes correctly
        self.assertIsInstance(task_queue, queue.Queue)
        self.assertEqual(task_queue.qsize(), 0)
    
    
    def test_matrix_multiplication(self):
        # testing if matrix multiplication runs w/o errors
        def mock_callback(task_id, result, error):
            self.assertIsNone(error)  # expecting no errors
            
            self.assertIsNotNone(result)  # expecting a result
        
        matrix_multiplication(1, mock_callback)
    
    
    def test_task_processing(self):
        # testing if queue processes tasks correctly w/callbacks
        for i in range(5):
            task_queue.put((i, callback))
            time.sleep(0.05)
        
        time.sleep(1)  # allowing delays in processing time
        self.assertTrue(task_queue.empty())
    
    
    
if __name__ == "__main__":
    unittest.main()
