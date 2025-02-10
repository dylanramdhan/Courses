import math
import pandas as pd

# Haversine formula to calculate the great-circle distance
def haversine_distance(lat1, lon1, lat2, lon2):
    lat1, lon1, lat2, lon2 = map(math.radians, [lat1, lon1, lat2, lon2])
    dlat = lat2 - lat1
    dlon = lon2 - lon1
    a = math.sin(dlat / 2)**2 + math.cos(lat1) * math.cos(lat2) * math.sin(dlon / 2)**2
    c = 2 * math.atan2(math.sqrt(a), math.sqrt(1 - a))
    R = 6371.0  # Earth's radius in kilometers
    return R * c

# Your current location (BU Photonics Center)
current_location = (42.3505, -71.1054)  # Latitude and Longitude of BU Photonics Center

# Read local CSV file
data_file = "test1.csv"
try:
    data = pd.read_csv(data_file)
    # Extract relevant columns (Latitude and Longitude)
    data = data[['latitude', 'longitude']].dropna()  # Ensure no NaN values
except Exception as e:
    print("Error loading data:", e)
    data = pd.DataFrame()

# Find the closest reported issue
def find_closest_issue(current_location, data):
    current_lat, current_lon = current_location
    closest_issue = None
    min_distance = float('inf')

    for _, row in data.iterrows():
        issue_lat, issue_lon = row['latitude'], row['longitude']
        distance = haversine_distance(current_lat, current_lon, issue_lat, issue_lon)

        if distance < min_distance:
            min_distance = distance
            closest_issue = (issue_lat, issue_lon, distance)

    return closest_issue

if not data.empty:
    closest_issue = find_closest_issue(current_location, data)
    if closest_issue:
        lat, lon, distance = closest_issue
        print(f"The closest issue is at ({lat}, {lon}) and is {distance:.2f} km away.")
    else:
        print("No issues found.")
else:
    print("No data available.")

     

The closest issue is at (42.34945958156978, -71.10484057120429) and is 0.12 km away.


import math
import pandas as pd

# Haversine formula to calculate the great-circle distance
def haversine_distance(lat1, lon1, lat2, lon2):
    lat1, lon1, lat2, lon2 = map(math.radians, [lat1, lon1, lat2, lon2])
    dlat = lat2 - lat1
    dlon = lon2 - lon1
    a = math.sin(dlat / 2)**2 + math.cos(lat1) * math.cos(lat2) * math.sin(dlon / 2)**2
    c = 2 * math.atan2(math.sqrt(a), math.sqrt(1 - a))
    R = 6371.0  # Earth's radius in kilometers
    return R * c

# Your current location (BU Photonics Center)
current_location = (42.3505, -71.1054)  # Latitude and Longitude of BU Photonics Center

# Read local CSV file
data_file = "test1.csv"
try:
    data = pd.read_csv(data_file)
    # Extract relevant columns (Latitude and Longitude)
    data = data[['latitude', 'longitude']].dropna()  # Ensure no NaN values
except Exception as e:
    print("Error loading data:", e)
    data = pd.DataFrame()

# Find the closest reported issues
def find_closest_issues(current_location, data, top_n=10):
    current_lat, current_lon = current_location
    results = []

    for _, row in data.iterrows():
        issue_lat, issue_lon = row['latitude'], row['longitude']
        distance = haversine_distance(current_lat, current_lon, issue_lat, issue_lon)
        results.append((issue_lat, issue_lon, distance))

    # Sort by distance and return the top N closest issues
    results.sort(key=lambda x: x[2])
    return results[:top_n]

if not data.empty:
    closest_issues = find_closest_issues(current_location, data, top_n=10)
    if closest_issues:
        print("The 10 closest issues are:")
        for lat, lon, distance in closest_issues:
            print(f"Issue at ({lat}, {lon}) is {distance:.2f} km away.")
    else:
        print("No issues found.")
else:
    print("No data available.")