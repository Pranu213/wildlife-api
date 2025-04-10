import firebase_admin
from firebase_admin import credentials, db

# Load Firebase Credentials
cred = credentials.Certificate("firebase_credentials.json")
firebase_admin.initialize_app(cred, {
    'databaseURL': 'https://wildlife-poaching-alerts-default-rtdb.firebaseio.com'
})


# https://wildlife-poaching-alerts-default-rtdb.firebaseio.com/