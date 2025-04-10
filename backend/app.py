import firebase_admin
from firebase_admin import credentials, db
from flask import Flask, jsonify, request
from flask_cors import CORS

# Initialize Flask app
app = Flask(__name__)
CORS(app)  # Enable CORS for all routes

# Load Firebase Credentials
cred = credentials.Certificate("firebase_credentials.json")
firebase_admin.initialize_app(cred, {
    'databaseURL': 'https://wildlife-poaching-alerts-default-rtdb.firebaseio.com'
})

# Root route
@app.route('/')
def home():
    return jsonify({
        "message": "Welcome to the Wildlife API",
        "status": "online",
        "version": "1.0"
    })

# Get all wildlife alerts
@app.route('/api/alerts', methods=['GET'])
def get_alerts():
    try:
        # Reference to the Firebase database
        ref = db.reference('alerts')
        # Get all alerts
        alerts = ref.get()
        
        if alerts:
            return jsonify(alerts)
        else:
            return jsonify({"message": "No alerts found"}), 404
    except Exception as e:
        return jsonify({"error": str(e)}), 500

# Add a new wildlife alert
@app.route('/api/alerts', methods=['POST'])
def add_alert():
    try:
        alert_data = request.json
        
        # Validate required fields
        required_fields = ['location', 'description', 'timestamp']
        for field in required_fields:
            if field not in alert_data:
                return jsonify({"error": f"Missing required field: {field}"}), 400
        
        # Add to Firebase
        ref = db.reference('alerts')
        new_alert = ref.push(alert_data)
        
        return jsonify({
            "message": "Alert added successfully",
            "id": new_alert.key
        }), 201
    except Exception as e:
        return jsonify({"error": str(e)}), 500

# Get specific alert
@app.route('/api/alerts/<alert_id>', methods=['GET'])
def get_alert(alert_id):
    try:
        ref = db.reference(f'alerts/{alert_id}')
        alert = ref.get()
        
        if alert:
            return jsonify(alert)
        else:
            return jsonify({"message": "Alert not found"}), 404
    except Exception as e:
        return jsonify({"error": str(e)}), 500

# Run the app if this file is executed directly
if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0', port=10000)