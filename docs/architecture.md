System Architecture  
Ward-Level Smart Streetlight Monitoring System

---

1. Purpose of the Architecture

This document describes the system architecture of a ward-level streetlight monitoring and control system.  
The architecture defines the structural decomposition, responsibility distribution, communication paths, and failure behavior of the system.

The intent is to make the system easy to understand for reviewers, recruiters, and examiners without requiring extensive follow-up questions.

---

2. Architectural Scope

The system is intentionally scoped to ward-level deployment to remain technically realistic and academically defensible.

Included in Scope
- Individual streetlight poles
- Local sensing and lamp control
- Fault detection at streetlight level
- Wireless telemetry transmission
- Ward-level aggregation
- Local monitoring dashboard

Excluded from Scope
- Cloud servers
- City-wide backend systems
- Mobile applications
- Remote control from dashboard
- Billing or analytics platforms

This clear boundary avoids over-claiming and keeps the design focused.

---

3. Architectural Overview

The system follows a two-tier distributed embedded architecture:

1. Streetlight Node Layer – autonomous embedded control at each pole  
2. Ward-Level Gateway Layer – centralized aggregation and visualization  

A browser-based dashboard connects to the gateway for monitoring only.

---

4. Streetlight Node Layer

4.1 Description

Each streetlight pole is implemented as an independent embedded node based on Arduino Nano.  
The node directly interfaces with sensors and the lamp driver and is responsible for all real-time decisions.

4.2 Responsibilities

- Measure ambient light using LDR
- Detect motion using PIR sensor
- Monitor voltage and current
- Control lamp brightness using PWM
- Detect electrical and operational faults
- Transmit periodic telemetry using LoRa

4.3 Local Autonomy

All control and fault decisions are executed locally at the node.  
This ensures that streetlights continue operating correctly even if communication with the gateway is unavailable.

4.4 Hardware Interfaces

| Function | Interface |
|--------|-----------|
| LDR | Analog input (A0) |
| PIR | Digital input (D2) |
| Voltage sensor | Analog input (A2) |
| Current sensor | Analog input (A3) |
| PWM output | Digital PWM (D5) |
| Communication | LoRa SX1278 (433 MHz) |

---

5. Ward-Level Gateway Layer

5.1 Description

The gateway is implemented using NodeMCU (ESP8266) and operates as a ward-level aggregation point.

5.2 Responsibilities

- Receive LoRa packets from streetlight nodes
- Parse and validate telemetry
- Maintain a node-wise state table
- Track node availability using last-seen timestamps
- Assign geographic coordinates to nodes
- Host a local HTTP server
- Expose aggregated data through the /data API
- Serve the monitoring dashboard (CMS.html)

5.3 Non-Responsibilities

The gateway does not:
- Control lamp operation
- Perform fault detection
- Execute time-critical logic

This separation prevents central dependency and simplifies gateway firmware.

---

6. Dashboard Layer

6.1 Description

The dashboard is a browser-based interface (CMS.html) served directly by the gateway over local Wi-Fi.

6.2 Responsibilities

- Fetch live system data from the /data endpoint
- Display streetlight locations on a map
- Show node status and fault indicators
- Display voltage, current, and PWM state
- Provide system-level visibility for maintenance use

6.3 Design Principle

The dashboard is strictly passive:
- No control commands
- No fault inference
- No decision logic

It only visualizes the system state reported by the gateway.

---

7. Communication Architecture

7.1 Node to Gateway

- Protocol: LoRa (433 MHz)
- Topology: Star
- Direction: Node to Gateway
- Payload format: Compact JSON

Example telemetry packet:

{
  "id": "SL-01",
  "pwm": 150,
  "v": 11.9,
  "i": 0.42,
  "fault": 0
}

The payload is intentionally minimal to reduce bandwidth usage and processing overhead.

---

7.2 Gateway to Dashboard

- Protocol: HTTP
- Endpoint: /data
- Data format: JSON
- Update method: Periodic polling (2 seconds)

The gateway enriches node data with location and timing context before exposing it to the dashboard.

---

8. Fault Handling Strategy

Fault detection is performed locally at the streetlight node using correlation between:
- PWM command
- Lamp feedback (LDR)
- Voltage measurement
- Current measurement

Detected faults are encoded as numeric fault codes and transmitted as part of telemetry.

The gateway and dashboard do not perform fault detection; they only report the fault state.

---

9. Failure Behavior

The architecture explicitly defines behavior under failure conditions:

----------------------------------------------------------------------------   
|      Failure Condition      |               System Behavior              |
|-----------------------------|--------------------------------------------|
| Gateway offline             | Streetlights continue autonomous operation |
| Node offline                | Dashboard marks node as unavailable        |
| Dashboard closed            | No impact on system operation              |
| Internet unavailable        | No impact (local-only system)              |
----------------------------------------------------------------------------
---

10. Assumptions and Constraints

- Lamp driver supports PWM dimming
- Sensor thresholds require calibration
- Node locations are configured at the gateway
- Deployment scale is limited to ward-level
- No bidirectional control commands are required

These constraints are stated explicitly to maintain technical transparency.

---

11. Architectural Summary

The system uses a distributed embedded architecture with autonomous control and fault detection at streetlight nodes and centralized aggregation at ward level.

The design prioritizes clarity, reliability, and explainability over feature complexity, making it suitable for academic evaluation and early-stage engineering review.
