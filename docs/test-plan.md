# Proposed Test Plan (Not Executed)
Centralized Monitoring System for Street-Light Fault Detection & Tracking

---

## 1. Purpose

This document describes the **proposed validation approach** for the system.
The tests listed below were **planned during design**, but were **not physically executed** due to hardware and deployment constraints during the academic period.

The intent of this document is to demonstrate **verification thinking**, not to claim test results.

---

## 2. Test Scope

The proposed test plan focuses on:

- Functional correctness of sensing and control logic
- Fault detection behavior under defined conditions
- Communication reliability between node and gateway
- Dashboard data integrity and visualization

Out-of-scope:
- Long-term field reliability testing
- Environmental stress testing
- Electrical safety certification
- City-scale deployment validation

---

## 3. Streetlight Node (Arduino Nano) — Proposed Tests

### 3.1 LDR-Based Dusk Detection

**Objective:**  
Verify that lamps turn ON when ambient light falls below threshold.

**Method:**  
- Simulate daylight and dusk using controlled light source
- Observe PWM output state transition

**Expected Behavior:**  
- Lamp OFF above threshold  
- Lamp ON below threshold

---

### 3.2 PWM Dimming Control

**Objective:**  
Verify intensity control logic (100% vs 50–60%).

**Method:**  
- Force motion detection flag ON/OFF
- Measure PWM duty cycle using multimeter or logic analyzer

**Expected Behavior:**  
- Motion detected → 100% duty cycle  
- No motion → reduced duty cycle

---

### 3.3 PIR Motion Detection

**Objective:**  
Validate motion-triggered brightness escalation.

**Method:**  
- Trigger PIR sensor manually
- Observe PWM duty change duration

**Expected Behavior:**  
- Immediate brightness increase  
- Timed fallback to dim state

---

### 3.4 Electrical Sensing (Voltage & Current)

**Objective:**  
Verify correctness of electrical measurements.

**Method:**  
- Apply known loads
- Compare sensor readings against reference meter

**Expected Behavior:**  
- Voltage within acceptable error margin  
- Current proportional to load

---

## 4. Fault Detection Logic — Proposed Tests

### 4.1 Lamp Failure Detection

**Condition:**  
Lamp commanded ON, voltage present, zero current.

**Expected Result:**  
Fault flag: *Lamp Failure*

---

### 4.2 Light Output Mismatch

**Condition:**  
Lamp ON, current flowing, LDR shows no illumination.

**Expected Result:**  
Fault flag: *Lamp Open / Optical Failure*

---

### 4.3 Abnormal Current Draw

**Condition:**  
Measured current exceeds defined safe limit.

**Expected Result:**  
Fault flag: *Overcurrent / Short*

---

### 4.4 Sensor Fault Detection

**Condition:**  
Out-of-range or static sensor values over time.

**Expected Result:**  
Fault flag: *Sensor Fault*

---

## 5. Communication Tests (Node ↔ Gateway)

### 5.1 Data Packet Reception

**Objective:**  
Ensure gateway correctly receives node telemetry.

**Method:**  
- Transmit periodic packets
- Log received data at gateway

**Expected Behavior:**  
- Packet loss tolerated through retransmission  
- No malformed data entries

---

### 5.2 Fault Reporting

**Objective:**  
Verify fault propagation to dashboard.

**Method:**  
- Inject simulated fault flags
- Observe dashboard update

**Expected Behavior:**  
- Correct fault label  
- Correct node identification

---

## 6. Dashboard Validation — Proposed Tests

### 6.1 Data Rendering

**Objective:**  
Ensure all received data fields are displayed correctly.

**Method:**  
- Use simulated JSON data
- Verify UI updates

---

### 6.2 Map Visualization

**Objective:**  
Verify GPS-based fault location plotting.

**Method:**  
- Inject known latitude/longitude values
- Confirm marker placement

---

## 7. Acceptance Criteria

The system would be considered functionally validated if:

- All defined fault conditions are detected deterministically
- No false-positive faults under normal conditions
- Dashboard reflects node state consistently
- Communication remains stable under periodic updates

---

## 8. Notes on Execution Status

- No physical field deployment was performed
- No long-duration testing data exists
- This test plan represents **design-level validation intent**

---

## 9. Summary

This proposed test plan demonstrates how the system would be validated under controlled conditions.
It reflects an understanding of embedded system verification while maintaining transparency about execution status.
