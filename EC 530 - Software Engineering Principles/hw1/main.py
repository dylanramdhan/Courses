""""
Assignment #1, due date: 1/23/2025
Engineer: Dylan Ramdhan

Mission of Module: If user gives two arrays of GeoLocations, match each point in the first array
                    to its closest neighbor in the second array.
                    
    Formula for distance between two GPS location:
        - Haversine formula: https://en.wikipedia.org/wiki/Haversine_formula
"""

import math

# A gentle reminder: The Earth is round, and so are the distances we calculate.
def haversine_distance(lat1, lon1, lat2, lon2):
    """
    Compute the great-circle distance between two points on Earth using the Haversine formula.

    Parameters:
        lat1, lon1: Latitude and Longitude of the first point in decimal degrees.
        lat2, lon2: Latitude and Longitude of the second point in decimal degrees.

    Returns:
        The distance between the two points in kilometers.
    """
    # Convert degrees to radians for accurate trigonometric calculations
    lat1, lon1, lat2, lon2 = map(math.radians, [lat1, lon1, lat2, lon2])

    # Haversine formula to find the great-circle distance
    dlat = lat2 - lat1
    dlon = lon2 - lon1
    a = math.sin(dlat / 2)**2 + math.cos(lat1) * math.cos(lat2) * math.sin(dlon / 2)**2
    c = 2 * math.atan2(math.sqrt(a), math.sqrt(1 - a))

    # Earth's radius in kilometers (mean value)
    R = 6371.0
    return R * c

# Optimized to embrace elegance and clarity
def match_closest_points(array1, array2):
    """
    Match each point in the first array to its nearest neighbor in the second array.

    Parameters:
        array1: List of tuples representing (latitude, longitude) for the first array.
        array2: List of tuples representing (latitude, longitude) for the second array.

    Returns:
        A list of tuples: (point_from_array1, closest_point_from_array2, distance).
    """
    results = []

    # Traverse each point in the first array and find its soulmate from the second array
    for lat1, lon1 in array1:
        closest_point = None
        min_distance = float('inf')

        for lat2, lon2 in array2:
            distance = haversine_distance(lat1, lon1, lat2, lon2)
            if distance < min_distance:
                min_distance = distance
                closest_point = (lat2, lon2)

        # Store the matched pair with their unbreakable bond of distance
        results.append(((lat1, lon1), closest_point, min_distance))

    return results

# Example usage with capitals from different nations
if __name__ == "__main__":
    # Array of geolocations (latitude, longitude) for various capital cities
    array1 = [
        (38.8951, -77.0364),   # Washington, D.C., USA
        (48.8566, 2.3522),     # Paris, France
        (35.6895, 139.6917),   # Tokyo, Japan
        (-35.2820, 149.1286),  # Canberra, Australia
        (55.7558, 37.6173)     # Moscow, Russia
    ]

    array2 = [
        (51.5074, -0.1278),    # London, United Kingdom
        (39.9042, 116.4074),   # Beijing, China
        (19.4326, -99.1332),   # Mexico City, Mexico
        (-15.7942, -47.8822),  # Brasília, Brazil
        (52.5200, 13.4050)     # Berlin, Germany
    ]

    # Match points and share the closest relationships
    matches = match_closest_points(array1, array2)

    for point1, closest_point, distance in matches:
        print(f"From {point1}, the closest capital is {closest_point} at a distance of {distance:.2f} km.")