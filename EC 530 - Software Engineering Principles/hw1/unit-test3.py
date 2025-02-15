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
import math

# Haversie formular to calculate great-circle distance
def haversine_distance(lat1, lon1, lat2, lon2):
    R = 6371.0  # Earth's radius in kilometers
    lat1, lon1, lat2, lon2 = map(math.radians, [lat1, lon1, lat2, lon2])
    dlat = lat2 - lat1
    dlon = lon2 - lon1
    a = math.sin(dlat / 2)**2 + math.cos(lat1) * math.cos(lat2) * math.sin(dlon / 2)**2
    c = 2 * math.atan2(math.sqrt(a), math.sqrt(1 - a))
    return R * c


# converting degrees-min-sec (DMS) to decimal degrees
def dms_to_decimal(degrees, minutes, seconds=0):
    return degrees + (minutes / 60.0) + (seconds / 3600.0)

def parse_dms_string(dms_string):
    parts = dms_string.split()
    degrees = float(parts[0])
    minutes = float(parts[1])
    seconds = 0.0 if len(parts) < 3 else float(parts[2])
    return dms_to_decimal(degrees, minutes, seconds)


# finding closests airport to each city
def find_closest_airport(cities, airports):
    results = []
    
    for city_name, lat1, lon1 in cities:
        closest_airport = None
        min_distance = float('inf')

        for airport_name, lat2, lon2 in airports:
            distance = haversine_distance(lat1, lon1, lat2, lon2)
            if distance < min_distance:
                min_distance = distance
                closest_airport = airport_name

        results.append((city_name, closest_airport, round(min_distance, 2)))

    return results

cities_file = "/Users/dylanramdhan/Documents/GitHub/Personal Repo/Courses/EC 530 - Software Engineering Principles/hw1/data/Major_Cities_GPS.csv"
airports_file = "/Users/dylanramdhan/Documents/GitHub/Personal Repo/Courses/EC 530 - Software Engineering Principles/hw1/data/iata-icao.csv"

# files
cities_file = "/Users/dylanramdhan/Documents/GitHub/Personal Repo/Courses/EC 530 - Software Engineering Principles/hw1/data/unit-test3-cities.csv" # cities
airports_file = "/Users/dylanramdhan/Documents/GitHub/Personal Repo/Courses/EC 530 - Software Engineering Principles/hw1/data/iata-icao.csv" # airports


# load city data
df_cities = pd.read_csv(cities_file, header=None, names=["city", "lat", "lng"])
# print("City File Columns:", df_cities.columns)  # debugging purposes


# detect if latitude/longitude values are in DMS format
if df_cities.iloc[0, 1].replace(" ", "").replace(".", "").isdigit():
    # if values contain spaces & numbers, assume DMS format
    df_cities["lat"] = df_cities["lat"].apply(parse_dms_string)
    df_cities["lng"] = df_cities["lng"].apply(parse_dms_string)

cities = df_cities[["city", "lat", "lng"]].dropna().values.tolist()


# load airport data
df_airports = pd.read_csv(airports_file)
#print("Airport File Columns:", df_airports.columns)  # debugging purposes


# detect correct column names
possible_airport_names = ["airport", "name"]
possible_air_lat_names = ["latitude", "lat"]
possible_air_lon_names = ["longitude", "lng"]

airport_col = next((col for col in possible_airport_names if col in df_airports.columns), None)
lat_air_col = next((col for col in possible_air_lat_names if col in df_airports.columns), None)
lon_air_col = next((col for col in possible_air_lon_names if col in df_airports.columns), None)


# ensure required columns exist
if not all([airport_col, lat_air_col, lon_air_col]):
    raise KeyError(f"Missing required columns in airport dataset. Found: {df_airports.columns}")


# extracting valid columns
airports = df_airports[[airport_col, lat_air_col, lon_air_col]].dropna().values.tolist()


# find closests airports
matches = find_closest_airport(cities, airports)


# results
print("Closest Airports to Cities:")
for city, airport, distance in matches:
    print(f"{city} → Closest Airport: {airport} ({distance} km)")
    
# pytest unit-test3.py
def test_haversine_distance():
    assert haversine_distance(42.350083, -71.103209, 40.7128, -74.0060) > 0