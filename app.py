from flask import Flask, request, jsonify, render_template
from flask_cors import CORS
from datetime import datetime

app = Flask(__name__, template_folder='templates')  # Thư mục chứa file HTML
CORS(app)

# Danh sách lưu trữ toàn bộ dữ liệu
data_log = []

@app.route('/')
def index():
    """Trả về giao diện chính"""
    return render_template('index.html')

@app.route('/receive_data', methods=['GET'])
def receive_data():
    """Nhận dữ liệu từ ESP8266"""
    temperature = request.args.get("temperature")
    humidity = request.args.get("humidity")

    if temperature and humidity:
        total = float(temperature) + float(humidity)  # Tính tổng
        data = {
            "temperature": float(temperature),
            "humidity": float(humidity),
            "total": total,  # Lưu tổng vào danh sách
            "timestamp": datetime.now().strftime("%Y-%m-%d %H:%M:%S")  # Thời gian nhận
        }
        data_log.append(data)  # Lưu dữ liệu vào danh sách
        print(f"Received data: {data}")
        return "Data received", 200
    return "Invalid data", 400

@app.route('/get_data', methods=['GET'])
def get_data():
    """Trả về toàn bộ dữ liệu đã lưu"""
    return jsonify(data_log)

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
