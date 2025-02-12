"""
Assignment #1, due date: 1/23/2025
Engineer: Dylan Ramdhan

Mission of Module: If user gives two arrays of Geolocations, match each point in the first array
                    to its closest neighbor in the second array.
                    
Formula for distance between two GPS location:  
    --> Haversine formula: https://en.wikipedia.org/wiki/Haversine_formula
    
        --->> UNIT TESTING #2 <<---
        Find the closest airport to a city!
        First Input:  list of major world cities:  https://raw.githubusercontent.com/joelacus/world-cities/refs/heads/main/world_cities.csv 
        Second Vector:  List of major world airports:  https://github.com/ip2location/ip2location-iata-icao/blob/master/iata-icao.csv
        
        Future Bonus:  Make your output and input configurables…. Pass a CSV or JSON and get the output as a JSON for the user.
        -----------------------------
        
       **This algorithm utilizes KD-Tree to efficiently find the closest airport for each city.
"""

import pandas as pd
import numpy as np
import math
from scipy.spatial import KDTree


# function for calculating Harvasine Distance
def haversine_distance(lat1, lon1, lat2, lon2):
    R = 6371.0  # Earth's radius in kilometers
    lat1, lon1, lat2, lon2 = map(math.radians, [lat1, lon1, lat2, lon2])
    dlat = lat2 - lat1
    dlon = lon2 - lon1
    a = math.sin(dlat / 2) ** 2 + math.cos(lat1) * math.cos(lat2) * math.sin(dlon / 2) ** 2
    c = 2 * math.atan2(math.sqrt(a), math.sqrt(1 - a))
    return R * c


# optimizing function to find closest airport using KD-Tree
def find_closest_airport_kdtree(cities, airports):

    airport_names = [airport[0] for airport in airports]  # Airport names
    airport_coords = np.array([(airport[1], airport[2]) for airport in airports])  # latitude & longitude only

    # Building KD-Tree for fast nearest neighbor search
    kdtree = KDTree(airport_coords)

    results = []
    for city_name, lat1, lon1 in cities:
        
        # finding closest airport index using KD-Tree
        _, index = kdtree.query((lat1, lon1))
        
        # retrieving closest airport details
        closest_airport = airport_names[index]
        lat2, lon2 = airport_coords[index]

        # computing accurate Haversine distance
        distance = haversine_distance(lat1, lon1, lat2, lon2)
        
        results.append((city_name, closest_airport, round(distance, 2)))

    return results


# loading data
cities_file = "/Users/dylanramdhan/Documents/GitHub/Personal Repo/Courses/EC 530 - Software Engineering Principles/hw1/data/world_cities.csv" # cities data
airports_file = "/Users/dylanramdhan/Documents/GitHub/Personal Repo/Courses/EC 530 - Software Engineering Principles/hw1/data/iata-icao.csv" # airports data


# loading cities data
df_cities = pd.read_csv(cities_file)
cities = df_cities[['name', 'lat', 'lng']].dropna().values.tolist()

# loading airports data
df_airports = pd.read_csv(airports_file)
airports = df_airports[['airport', 'latitude', 'longitude']].dropna().values.tolist()

# finding closest airports to cities via KD-Tree
matches = find_closest_airport_kdtree(cities, airports)


# results
print("\nClosest Airports to Cities:")
for city, airport, distance in matches:
    print(f"{city} → Closest Airport: {airport} ({distance} km)")