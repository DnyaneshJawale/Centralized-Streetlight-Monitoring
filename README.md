Centralized Monitoring System for Street-Light Fault Detection & Tracking

Overview

This project presents a ward-level smart street-light monitoring system designed to detect electrical and operational faults, optimize energy usage, and provide centralized visibility through a web-based dashboard.

The system was developed as a second-year academic project with a focus on system architecture, fault logic, and data flow, rather than large-scale physical deployment. The design reflects practical constraints, cost awareness, and real-world operating assumptions.

---

Problem Statement

Conventional street-light systems suffer from:
- Manual fault reporting
- Delayed maintenance response
- Continuous full-intensity operation regardless of need
- Lack of visibility into power consumption and lamp health

This project addresses these gaps by introducing distributed sensing nodes and a centralized monitoring gateway.

---

System Architecture (High Level)

Streetlight Node (Arduino Nano)
  - Monitors lamp health, power parameters, and motion
  - Controls lamp intensity using PWM dimming
  - Reports status periodically

Ward-Level Gateway (NodeMCU / ESP8266)
  - Aggregates data from multiple nodes
  - Hosts the monitoring dashboard locally
  - Maps faults using GPS coordinates

Dashboard (HTML/CSS/JS)
  - Real-time status view
  - Fault visualization on map
  - Power and intensity indicators

Detailed system architecture is documented here:  
→ [docs/architecture.md](docs/architecture.md)


---

Key Features

- Automatic dusk-based lamp activation using LDR
- Adaptive dimming (100% / 50–60%) based on motion
- Electrical fault detection using voltage and current sensing
- Lamp failure and abnormal behavior detection
- GPS-based fault localization
- Local hosting (no cloud dependency)

---

Fault Detection Capability

The system detects faults using sensor correlation, not single-sensor assumptions.

Examples:
- Lamp ON command + zero current → Lamp failure
- Lamp ON command + no light output → Lamp open circuit
- Voltage present + no current → Driver or lamp fault
- Abnormal current draw → Short or degradation

Complete fault detection logic and sensor correlation rules are documented here:  
→ [docs/fault-matrix.md](docs/fault-matrix.md)


---

Technology Stack

Hardware
- Arduino Nano (streetlight node)
- NodeMCU ESP8266 (gateway)
- LDR (ambient & lamp monitoring)
- PIR motion sensor
- Voltage sensor
- Current sensor
- GPS NEO-6M
- PWM-capable lamp driver

Software
- Embedded C/C++ (Arduino)
- HTML / CSS / JavaScript
- Leaflet.js (map visualization)

---

Repository Structure

```
.
├── firmware/
│   ├── Nano_Streetlight_Node/
│   │   └── Nano_Streetlight_Node.ino
│   └── NodeMCU_Gateway/
│       └── NodeMCU_Gateway.ino
│
├── dashboard/
│   └── cms.html
│
├── docs/
│   ├── architecture.md
│   ├── fault-matrix.md
│   └── assumptions-and-limitations.md
│
└── README.md
```

---

Assumptions & Limitations

This project:
- Is designed for ward-level deployment
- Does not include cloud connectivity
- Does not support remote firmware updates
- Was not physically deployed city-wide

All design assumptions, constraints, and non-goals are explicitly documented here:  
→ [docs/assumptions-and-limitations.md](docs/assumptions-and-limitations.md)


---

Academic Context

- Developed during second year of undergraduate study
- Focused on architecture, logic design, and fault modeling
- Physical deployment constraints were acknowledged
- Validation performed via simulation and design review

The project intentionally avoids exaggerated claims and focuses on architectural correctness,
fault reasoning, and system-level thinking.


---

How to Run (Demo Mode)

1. Flash `Nano_Streetlight_Node.ino` on Arduino Nano
2. Flash `NodeMCU_Gateway.ino` on ESP8266
3. Place `cms.html` in the NodeMCU SPIFFS or local server directory
4. Access dashboard via gateway IP address

---

Author

Dnyanesh Jawale  
Electronics & Telecommunication Engineering  
Embedded Systems | Hardware-Focused Design

---

License

This project is intended for academic and learning purposes.
