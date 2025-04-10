from flask import Flask, request, jsonify
import mysql.connector
import datetime

app = Flask(__name__)

# MySQL Database Configuration
DB_CONFIG = {
    "host": "localhost",  
    "user": "root",
    "password": "Pranu@143",  
    "database": "wildlife_project"
}

# Connect to MySQL and ensure the table exists
def init_db():
    try:
        connection = mysql.connector.connect(**DB_CONFIG)
        cursor = connection.cursor()
        
        # Create table if it doesn't exist
        cursor.execute("""
            CREATE TABLE IF NOT EXISTS sensor_data (
                id INT AUTO_INCREMENT PRIMARY KEY,
                zone INT NOT NULL,
                detection VARCHAR(50) NOT NULL,
                timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP
            )
        """)
        
        connection.commit()
        cursor.close()
        connection.close()
        print("✅ Database initialized successfully!")

    except mysql.connector.Error as err:
        print(f"❌ Database Error: {err}")

# Route to receive sensor data and store in MySQL
@app.route('/sensor_data', methods=['POST'])
def receive_sensor_data():
    try:
        data = request.get_json()

        # Validate input
        if "zone" not in data or "detection" not in data:
            return jsonify({"error": "Missing required fields"}), 400
        
        connection = mysql.connector.connect(**DB_CONFIG)
        cursor = connection.cursor()

        # Insert data into MySQL
        query = "INSERT INTO sensor_data (zone, detection) VALUES (%s, %s)"
        values = (data["zone"], data["detection"])
        cursor.execute(query, values)
        connection.commit()

        cursor.close()
        connection.close()

        return jsonify({"message": "Sensor data recorded successfully", "zone": data["zone"]}), 201

    except mysql.connector.Error as err:
        return jsonify({"error": f"Database Error: {err}"}), 500

# Route to fetch all sensor data
@app.route('/get_data', methods=['GET'])
def get_sensor_data():
    try:
        connection = mysql.connector.connect(**DB_CONFIG)
        cursor = connection.cursor()

        cursor.execute("SELECT * FROM sensor_data")
        records = cursor.fetchall()

        cursor.close()
        connection.close()

        # Format response as JSON
        sensor_list = []
        for row in records:
            sensor_list.append({
                "id": row[0],
                "zone": row[1],
                "detection": row[2],
                "timestamp": row[3].strftime("%Y-%m-%d %H:%M:%S")
            })

        return jsonify(sensor_list), 200

    except mysql.connector.Error as err:
        return jsonify({"error": f"Database Error: {err}"}), 500

# Initialize the database when the app starts
if __name__ == '__main__':
    init_db()
    app.run(debug=True, host="0.0.0.0")
