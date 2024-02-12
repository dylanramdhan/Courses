# Dylan Ramdhan, CS 357
# Login Authentification: Challenge 4.0

# sources:
#   1. https://docs.python.org/3/library/hashlib.html
#   2. https://gist.github.com/markito/30a9bc2afbbfd684b31986c2de305d20

# Accessing Wesbite
import requests
import hashlib # using the hashing library (1)

saltedSHA256 = '395e9699a6f14855bfd16c78fde5111c725e1d0e1b0dc17c30e5f634e39ff8ef'

url = "http://localhost:8080/login"

# Creating Password Variable
AdminPassword = ''


### Creating a Nested For Loop to Sift Through to Find Password ~ Challenge 2 ###

# Years
for years in ['2019', '2020', '2021', '2022', '2023']:

    # Months
    for months in ['01', '02', '03', '04', '05', '06', '07', '08', '09', '10', '11', '12']:
        
        # Possible Password
        for password in range(100000): #100,000 beacause of the combinations of numbers
            
            # Months + Years
            SaltedSHA256 = months + years

            PossiblePassword = str(password)

            # Using provided Hint, I used a source to find this lines of code (2)
            hashingVar = hashlib.sha256(SaltedSHA256.encode() + PossiblePassword.encode()).hexdigest()

            # Checking if Password is the = to the hashingVar
            if hashingVar == saltedSHA256:
                AdminPassword == PossiblePassword
                break

# Displaying Password
print (password)