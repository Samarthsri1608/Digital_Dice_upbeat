import serial
import time
from datetime import datetime

# Set up the serial connection
arduino = serial.Serial('COM3', 9600)  # Replace 'COM3' with your port
time.sleep(2)  # Wait for the connection to initialize

while True:
    now = datetime.now()
    current_time = now.strftime("%H:%M:%S %d/%m/%Y")
    arduino.write((current_time + '\n').encode())
    time.sleep(1)  # Send updates every second
