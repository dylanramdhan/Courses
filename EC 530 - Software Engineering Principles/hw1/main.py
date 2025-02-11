""""
Assignment #1, due date: 1/23/2025
Engineer: Dylan Ramdhan

Mission of Module: If user gives two arrays of GeoLocations, match each point in the first array
                    to its closest neighbor in the second array.
                    
    Formula for distance between two GPS location:  
        --> Haversine formula: https://en.wikipedia.org/wiki/Haversine_formula
"""
import math


def haversine_distance(lat1, lon1, lat2, lon2):

    # converting degrees to radians for accurate trigonometric calculations
    lat1, lon1, lat2, lon2 = map(math.radians, [lat1, lon1, lat2, lon2])

    # Haversine formula to find the great-circle distance
    dist_lati = lat2 - lat1
    dist_long = lon2 - lon1
    a = math.sin(dist_lati / 2) ** 2 + math.cos(lat1) * math.cos(lat2) * math.sin(dist_long / 2) ** 2 # ** means to raise to the power of 2
    c = 2 * math.atan2(math.sqrt(a), math.sqrt(1 - a))

    # Earth's radius in kilometers
    R = 6371.0
    return R * c



def match_closest_points(array1, array2):
    
    results = [] # store matched pairs w/distances

    # traverse each point first array and find its relation from second array
    for lat1, lon1 in array1:
        closest_point = None
        min_distance = float('inf')

        for lat2, lon2 in array2:
            distance = haversine_distance(lat1, lon1, lat2, lon2)
            if distance < min_distance:
                min_distance = distance
                closest_point = (lat2, lon2)

        results.append(((lat1, lon1), closest_point, min_distance)) # store matched pair w/unbreakable bond of distance

    return results



# usaging capitals from different nations to compare/contrast
if __name__ == "__main__":
    
    # array of geolocations (latitude, longitude) for various capital cities
    array1 = [
        (34.0522, -118.2437),  # Los Angeles, USA
        (51.1657, 10.4515),    # Berlin, Germany
        (-33.8688, 151.2093),  # Sydney, Australia
        (28.7041, 77.1025),    # New Delhi, India
        (37.7749, -122.4194)   # San Francisco, USA
    ]

    array2 = [
        (35.6895, 139.6917),   # Tokyo, Japan
        (40.7128, -74.0060),   # New York City, USA
        (55.7558, 37.6173),    # Moscow, Russia
        (-26.2041, 28.0473),   # Johannesburg, South Africa
        (48.8566, 2.3522)      # Paris, France
    ]

    matches = match_closest_points(array1, array2) # matches points and share cloests coordinates

    for point1, closest_point, distance in matches:
        print(f"From {point1}, the closest capital is {closest_point} at a distance of {distance:.2f} km.")