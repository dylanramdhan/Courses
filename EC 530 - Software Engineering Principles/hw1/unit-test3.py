"""
Assignment #1, due date: 1/23/2025
Engineer: Dylan Ramdhan

Mission of Module: If user gives two arrays of Geolocations, match each point in the first array
                    to its closest neighbor in the second array.
                    
Formula for distance between two GPS location:  
    --> Haversine formula: https://en.wikipedia.org/wiki/Haversine_formula
    
        --->> UNIT TESTING #3 <<---
        First Vector: https://docs.google.com/spreadsheets/d/1oJy_VQ08Fjdj-hwMXYTR0dUTlrahBTyPRlJQrjuK9ZE/edit?usp=sharing 
        Second Vector:  
            List of major world airports:  https://github.com/ip2location/ip2location-iata-icao/blob/master/iata-icao.csv
        -----------------------------
"""

import pandas as pd