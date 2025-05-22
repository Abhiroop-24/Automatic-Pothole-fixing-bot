# 🤖 Pothole-Fixing Robot (Arduino + Raspberry Pi Live Stream)

An **autonomous robot** that detects road potholes using IR and ultrasonic sensors, aligns itself, and dumps sand using a servo. The robot also features **live video streaming** using a **Raspberry Pi 3 B+ with 5MP Camera**.

---

## 📸 System Overview
![pic1](https://github.com/user-attachments/assets/513ffe6c-6039-4de6-85eb-0197b2b14a79)
![pic2](https://github.com/user-attachments/assets/b12ed5e9-57a3-4349-bb88-e8ff28099486)
![pic3](https://github.com/user-attachments/assets/4065b083-3220-48a8-bdd1-ec33c23f4869)
![pic4](https://github.com/user-attachments/assets/cb7720f7-51ff-4256-b10a-bc789c7eb926)



---

## 🔧 Hardware Used

### 🤖 Arduino-Based Robot:
- Arduino Uno + L293D Motor Driver Shield (pluggable)
- 4× DC Motors (M1–M4)
- 4× IR Sensors (for alignment)
- 1× Ultrasonic Sensor (HC-SR04 for depth)
- 1× Servo Motor (for sand gate)
- Power supply/battery

### 📷 Raspberry Pi for Live Streaming:
- Raspberry Pi 3 B+
- 5MP Pi Camera Module
- Wi-Fi/Ethernet for video stream

---

## ⚙️ Features

- Detects potholes using **IR sensor layout**
- Measures pothole depth using **ultrasonic sensor**
- Aligns itself precisely using **4-motor differential control**
- Dumps sand via a **servo gate**
- Streams live video using **Raspberry Pi 3 B+**

---

## 🧠 Arduino Code Breakdown

### Sensor Pins:
| Sensor         | Arduino Pin |
|----------------|-------------|
| IR_LEFT        | A0          |
| IR_CENTER_LEFT | A1          |
| IR_CENTER_RIGHT| A2          |
| IR_RIGHT       | A3          |
| Ultrasonic TRIG| A4          |
| Ultrasonic ECHO| A5          |

### Motors (L293D Shield):
- M1 → IN1 & IN2  
- M2 → IN3 & IN4  
- M3 → IN5 & IN6  
- M4 → IN7 & IN8  
- ENA → PWM for M1 & M2  
- ENB → PWM for M3 & M4  

### Servo:
- Connected to **Pin 10** on the L293D shield (servo port)

---

## 🚗 Motor Control Functions (From `Upper_sensors.ino`)

```cpp
moveForward();  // All 4 motors move forward
moveBackward(); // All 4 motors move backward
turnLeft();     // Left motors backward, right motors forward
turnRight();    // Left motors forward, right motors backward
stopMoving();   // All motors stopped
dumpSand();     // Activates servo to dump sand

```
![out](https://github.com/user-attachments/assets/cef1cfe4-674b-4859-beeb-61c20e792599)


## 📺 Raspberry Pi Live Stream Setup (Step-by-step)

### **1. Enable Camera on Raspberry Pi**

Open terminal on your Pi and run:

```bash
sudo raspi-config
```

* Navigate to **Interface Options → Camera → Enable**
* Then select **Finish** and reboot:

```bash
sudo reboot
```

---

### **2. Create the Streaming Script**

Create the streaming script using:

```bash
nano ~/start_stream.sh
```

Paste this optimized code:

```bash
#!/bin/bash

# Settings perfect for Pi 3 B+
WIDTH=800
HEIGHT=600
FPS=12
PORT=8080

# Start the camera stream
libcamera-vid -t 0 \
  --width $WIDTH --height $HEIGHT \
  --framerate $FPS \
  --inline \
  --codec h264 \
  -o - | \
ffmpeg -i - \
  -c:v copy \
  -f mpegts \
  udp://0.0.0.0:$PORT?pkt_size=1316
```

![pi2](https://github.com/user-attachments/assets/a973400c-e32f-452d-be50-4e66afa8a7d3)


Make the script executable:

```bash
chmod +x ~/start_stream.sh
```

---

### **3. Run the Stream**

#### Option A: Temporarily (For testing)

Run the stream manually:

```bash
./start_stream.sh
```


#### Option B: Autostart on Boot (Optional)

Add to crontab to auto-run on every boot:

```bash
crontab -e
```

Add the following line at the end:

```bash
@reboot /home/pi/start_stream.sh
```

![pi1](https://github.com/user-attachments/assets/146513e1-f2c6-4a95-8e32-0b7fc656e3e8)

---

### **4. View the Stream**

From a client (e.g., laptop) in the same network, open **VLC Media Player** and go to:

```
Media → Open Network Stream → udp://<RPI_IP>:8080
```

Replace `<RPI_IP>` with the IP address of your Raspberry Pi.

---

## 📂 Project Files

* `Upper_sensors.ino` – Arduino logic for only sensor-based detection 
* `4x293D.ino` – Arduino logic for sensor-based detection and motor control
* `start_stream.sh` – Raspberry Pi script for video streaming
* `README.md` – Full documentation and setup instructions (this file)

---

## 🪛 License

This project is licensed under the **MIT License** — free to use, modify, and distribute with attribution.

---

## 🙋‍♂️ Author

Created by \[ABHIROOP]
Feel free to fork, star ⭐ and contribute!
