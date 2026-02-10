# 🏠 Smart Room IoT Digital Twin

A real-time **IoT Digital Twin** implementation for a smart room, combining virtual hardware, cloud messaging, and a live dashboard.  
The system mirrors a physical environment, processes telemetry in the cloud, and enables both **automatic** and **manual control**.

---

## 🚀 Project Overview

This project demonstrates a synchronized **Digital Twin** using a four-layer IoT architecture.  
Environmental data is collected from a virtual **ESP32**, transmitted via **MQTT**, processed in **Node-RED**, and visualized in a **real-time dashboard**.

---

## 🧱 System Architecture

The system follows the industry-standard **four-layer IoT model**:

### 1. Physical Layer (Wokwi Simulation)

The *physical* asset consists of:
- ESP32 microcontroller  
- DHT22 temperature and humidity sensor  
- Blue LED representing the Air Conditioning unit  

📸 **Figure 1 – Wokwi Simulation**  
*Live telemetry publishing and Cloud Command reception.*

![Wokwi ESP32 Simulation](screenshots/wokwi_simulation.png)

---

### 2. Communication Layer (MQTT)

Telemetry data is broadcast as **JSON payloads** to the public MQTT broker:

📸 **Figure 2 – MQTT WebSocket Monitoring**  
*Real-time JSON packets received from the virtual room.*

![HiveMQ WebSocket Messages](screenshots/hivemq_websocket.png)

---

### 3. Digital Twin Layer (Node-RED)

This layer represents the **brain** of the system, where raw sensor data is transformed into insights such as **Comfort Level** and actuator decisions.

📸 **Figure 3 – Node-RED Digital Twin Logic**  
*Synchronization, threshold evaluation, and control flow.*

![Node-RED Flow](screenshots/node_red_flow.png)

---

### 4. Application Layer (Dashboard UI)

A human-centric interface that provides:
- Live room conditions  
- Comfort status indicators  
- Historical trend visualization  

📸 **Figure 4 – Digital Twin Dashboard**  
*STUFFY comfort status at 28.8 °C.*

![Dashboard UI](screenshots/dashboard_ui.png)

---

## 🔄 Data Flow & Synchronization

📸 **Figure 5 – End-to-End Data Flow**  
*Sensor → MQTT → Node-RED → Dashboard → Actuator*

![Data Flow Diagram](screenshots/data_flow.png)

---

## 🔌 Hardware Wiring (Virtual)

| Component        | ESP32 Pin | Connection Type |
|------------------|-----------|-----------------|
| DHT22 (SDA)      | GPIO 15   | Data Input      |
| AC LED (Anode)   | GPIO 2    | Actuator Output |
| VCC / GND        | 3.3V / GND| Power Rail      |

📸 **Figure 6 – Virtual Wiring Diagram**

![ESP32 Wiring Diagram](screenshots/wiring_diagram.png)

---

## 🧠 Core Logic & Thresholds

The Digital Twin operates using a **Closed-Loop Feedback System**:

- **Monitoring:**  
  Sensor data is updated every **2 seconds**.

- **Automation:**  
  If  
  \[
  \text{Temperature} > 30^\circ\text{C}
  \]  
  the Twin logic engine:
  - Sets the comfort state to **STUFFY**
  - Turns the AC LED **ON**

- **Manual Override:**  
  Users can publish an `OFF` message via the MQTT control topic:


