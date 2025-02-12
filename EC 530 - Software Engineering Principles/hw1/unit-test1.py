""""
Assignment #1, due date: 1/23/2025
Engineer: Dylan Ramdhan

Mission of Module: If user gives two arrays of Geolocations, match each point in the first array
                    to its closest neighbor in the second array.
                    
    Formula for distance between two GPS location:  
        --> Haversine formula: https://en.wikipedia.org/wiki/Haversine_formula
    
    --->> UNIT TESTING #1 <<---
    First Input:  Your current GPS location
    Second Vector:  Columns AB and  AC of Boston 311 2025  Report (you can find the originals in:  https://data.boston.gov/dataset/311-service-requests)
    Future Bonus:  Find the closest reported issues to your current location.
"""

import math
import pandas as pd

# function for calculating Haversine Distance
def haversine_distance(lat1, lon1, lat2, lon2):
    """
    Compute the great-circle distance between two points on Earth using the Haversine formula.
    Returns distance in kilometers.
    """
    R = 6371.0  # Earth's radius in kilometers
    lat1, lon1, lat2, lon2 = map(math.radians, [lat1, lon1, lat2, lon2])
    dlat = lat2 - lat1
    dlon = lon2 - lon1
    a = math.sin(dlat / 2) ** 2 + math.cos(lat1) * math.cos(lat2) * math.sin(dlon / 2) ** 2
    c = 2 * math.atan2(math.sqrt(a), math.sqrt(1 - a))
    return R * c

# function in finding closest report issue
def find_closest_issue(user_location, report_data):
    closest_point = None
    min_distance = float('inf')

    for lat, lon in report_data:
        distance = haversine_distance(user_location[0], user_location[1], lat, lon)
        if distance < min_distance:
            min_distance = distance
            closest_point = (lat, lon)

    return closest_point, min_distance


user_location = (42.350083, -71.103209)  # Current GPS location (Central BU Campus)


file_path = "/Users/dylanramdhan/Documents/GitHub/Personal Repo/Courses/EC 530 - Software Engineering Principles/hw1/data/Boston 311 012225.csv" # path to dataset
df = pd.read_csv(file_path)

report_data = df[['latitude', 'longitude']].dropna().values.tolist() # extracting latitude and longitude columns

closest_issue, distance = find_closest_issue(user_location, report_data) # finding closest issue

# Output Results
print("Your Location:", user_location)
print("Closest Reported Issue is at:", closest_issue)
print("Distance:", f"{distance:.2f} km")



"""
My output:
    Your Location: (42.350083, -71.103209)
    Closest Reported Issue is at: (42.34988626501212, -71.10304421603614)
    Distance: 0.03 km
"""