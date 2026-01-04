Assumptions and Limitations
Ward-Level Smart Streetlight Monitoring System

---

1. Purpose

This document lists the assumptions and limitations of the streetlight monitoring system.
It defines the boundaries under which the system architecture, firmware, and dashboard are expected to operate correctly.

The intent is to provide technical transparency and prevent over-interpretation of the system capabilities.

---

2. Design Assumptions

 2.1 Electrical Assumptions
 
 - Lamp driver supports PWM dimming
 - Supply voltage remains within rated limits during normal operation
 - Voltage and current sensors are correctly scaled
 - Sensor noise remains within acceptable bounds
 
 ---
 
 2.2 Sensor Assumptions
 
 - LDR is positioned to observe lamp output
 - Ambient light influence is limited through threshold calibration
 - PIR sensor is used only for motion-based brightness control
 - Sensor readings are time-averaged to reduce transient effects
 
 ---
 
 2.3 Communication Assumptions
 
 - LoRa communication is available between nodes and gateway
 - Periodic telemetry is sufficient for monitoring
 - Packet loss is tolerated due to repeated transmissions
 - Deployment scale is limited to a single ward
 
 ---
 
 2.4 Deployment Assumptions
 
 - Node coordinates are configured at the gateway
 - Gateway and dashboard share the same local network
 - Physical installation safety is handled externally
 - System demonstration is performed in a controlled environment
 
---

3. System Limitations

 3.1 Functional Limitations
 
 - No remote lamp control from dashboard
 - No cloud connectivity or remote access
 - No long-term historical data storage
 - No firmware update mechanism
 
 ---
 
 3.2 Scalability Limitations
 
 - Star topology limits nodes per gateway
 - No collision avoidance at LoRa layer
 - Not suitable for city-scale deployment without redesign
 - Gateway memory limits node tracking
 
 ---
 
 3.3 Accuracy Limitations
 
 - Fault thresholds require lamp-specific calibration
 - LDR readings affected by dust and weather
 - Current sensor resolution limits low-current accuracy
 - Voltage accuracy depends on ADC reference stability
 
 ---
 
 3.4 Validation Limitations
 
 - Physical field deployment not performed
 - Validation based on design review and simulation
 - Environmental variations not fully modeled
 - Electrical safety certification out of scope

---

4. Implications

These assumptions and limitations define the boundaries within which the system is valid.
They do not reduce the architectural correctness of the design.

The system is suitable for academic evaluation and design-level verification, not production deployment.

---

5. Summary

Documenting assumptions and limitations ensures honesty and technical clarity.
It allows reviewers to assess the system on its intended scope without ambiguity.
