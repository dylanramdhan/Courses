"""
Assignment #1, due date: 1/23/2025
Engineer: Dylan Ramdhan

Mission of Module: If user gives two arrays of Geolocations, match each point in the first array
                    to its closest neighbor in the second array.
                    
Formula for distance between two GPS location:  
    --> Haversine formula: https://en.wikipedia.org/wiki/Haversine_formula
    
        --->> UNIT TESTING #4 <<---
        First Vector:
            1. https://drive.google.com/file/d/1B5OVEcLH0iM23DDqc2PlGGPhTl5xmw77/view?usp=sharing
            2. https://drive.google.com/file/d/1Iw3MJxakfyIUzK21GpEoOtHBFmYuqrX6/view?usp=sharing 
        Second Vector:
            List of major world airports:  https://github.com/ip2location/ip2location-iata-icao/blob/master/iata-icao.csv
        -----------------------------
"""
import pandas as pd
import math
import re


# haversine formula to calculate great-circle distance
def haversine_distance(lat1, lon1, lat2, lon2):
    """
    Compute the great-circle distance between two points on Earth using the Haversine formula.
    Returns distance in kilometers.
    """
    R = 6371.0  # Earth's radius in kilometers
    lat1, lon1, lat2, lon2 = map(math.radians, [float(lat1), float(lon1), float(lat2), float(lon2)])
    dlat = lat2 - lat1
    dlon = lon2 - lon1
    a = math.sin(dlat / 2)**2 + math.cos(lat1) * math.cos(lat2) * math.sin(dlon / 2)**2
    c = 2 * math.atan2(math.sqrt(a), math.sqrt(1 - a))
    return R * c



# function to clean longitude and latitude
def clean_coordinates(coord):
    """
    Converts latitude/longitude from DMS (Degrees, Minutes, Seconds) format or directional format
    (e.g., "40.7128° N", "74.0060° W") to decimal format.
    """
    if isinstance(coord, float):
        return coord

    coord = coord.strip()  # removing spaces
    match = re.search(r"([-+]?\d+\.?\d*)", coord)  # extracting numerical value
    if not match:
        raise ValueError(f"Invalid coordinate format: {coord}")
    
    numeric_value = float(match.group(1))

    # any coordinate containing 'S' or 'W', make it negative
    if 'S' in coord or 'W' in coord:
        return -numeric_value
    return numeric_value



# function to find closests airport
def find_closest_airport(cities, airports):
    results = []
    for city_name, lat1, lon1 in cities:
        lat1, lon1 = float(lat1), float(lon1)  # converting city coordinates to floats
        closest_airport = None
        min_distance = float('inf')
        
        for airport_name, lat2, lon2 in airports:
            lat2, lon2 = float(lat2), float(lon2)  # converting airport coordinates to floats
            distance = haversine_distance(lat1, lon1, lat2, lon2)
            
            if distance < min_distance:
                min_distance = distance
                closest_airport = airport_name
        
        results.append((city_name, closest_airport, round(min_distance, 2)))
    return results


# file paths (comment one of the cities_file BEFORE running)
cities_file = "/Users/dylanramdhan/Documents/GitHub/Personal Repo/Courses/EC 530 - Software Engineering Principles/hw1/data/Major_Cities_GPS.csv"
#cities_file = "/Users/dylanramdhan/Documents/GitHub/Personal Repo/Courses/EC 530 - Software Engineering Principles/hw1/data/Major_Cities_GPS_V2.csv"
airports_file = "/Users/dylanramdhan/Documents/GitHub/Personal Repo/Courses/EC 530 - Software Engineering Principles/hw1/data/iata-icao.csv"



# loading cities data
df_cities = pd.read_csv(cities_file)
print("City File Columns:", df_cities.columns)


# ensuring correct column names
city_col = "City"
lat_col = "Latitude"
lon_col = "Longitude"

# applying cleaning function only on any string values
df_cities[lat_col] = df_cities[lat_col].apply(clean_coordinates)
df_cities[lon_col] = df_cities[lon_col].apply(clean_coordinates)

cities = df_cities[[city_col, lat_col, lon_col]].dropna().values.tolist()



# loading airports data
df_airports = pd.read_csv(airports_file)
print("Airport File Columns:", df_airports.columns)

# ensuring correct column names
airport_col = "airport"
lat_air_col = "latitude"
lon_air_col = "longitude"

# applying cleaning function only on any string values
df_airports[lat_air_col] = df_airports[lat_air_col].apply(clean_coordinates)
df_airports[lon_air_col] = df_airports[lon_air_col].apply(clean_coordinates)

airports = df_airports[[airport_col, lat_air_col, lon_air_col]].dropna().values.tolist()



# finding clostest cities to airports
matches = find_closest_airport(cities, airports)

# results
print("Closest Airports to Cities:")
for city, airport, distance in matches:
    print(f"{city} → Closest Airport: {airport} ({distance} km)")