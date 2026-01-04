Fault Matrix
Ward-Level Smart Streetlight Monitoring System

---

1. Purpose

This document defines the fault detection logic used in the streetlight monitoring system.
It maps sensor observations and control states to well-defined fault codes.

The intent is to make fault behavior deterministic, explainable, and easy to review.

---

2. Design Principles

- Fault detection is performed locally at the streetlight node
- No single sensor is trusted blindly
- Faults are inferred using correlation between expected and measured behavior
- Each fault corresponds to a clear electrical condition

---

3. Fault Code Summary

-------------------------------------------------------------------------------------------------
| Fault Code |          Fault Name          |                    Description                    |
|------------|------------------------------|---------------------------------------------------|
|     0      | OK                           | Normal operation                                  |
|     1      | LAMP_OFF_WHEN_EXPECTED_ON    | Lamp not emitting light despite PWM command       |
|     2      | OVER_CURRENT                 | Current exceeds safe operating threshold          |
|     3      | UNDER_VOLTAGE                | Supply voltage below minimum threshold            |
|     4      | LAMP_OPEN_LOW_CURRENT        | Voltage present but current near zero             |
-------------------------------------------------------------------------------------------------

---

4. Sensor Inputs Used

-------------------------------------------------------------------------------------------------
|     Sensor     |         Parameter         |                      Purpose                     |
|----------------|---------------------------|--------------------------------------------------|
|      LDR       | Lamp light level          | Verify lamp output                               |
|      PIR       | Motion detection          | Trigger brightness increase                      |
| Voltage Sensor | Supply voltage            | Detect undervoltage                              |
| Current Sensor | Load current              | Detect overcurrent or open circuit               |
|   PWM State    | Duty cycle                | Reference for expected lamp behavior             |
-------------------------------------------------------------------------------------------------

---

5. Fault Detection Logic

 5.1 Fault Code 0 — OK
 
 Condition:
 - PWM > 0
 - Lamp LDR above threshold
 - Voltage within range
 - Current within normal range
 
 Interpretation:
 Lamp and electrical system are operating normally.
 
 ---
 
 5.2 Fault Code 1 — LAMP_OFF_WHEN_EXPECTED_ON
 
 Condition:
 - PWM > 0
 - Lamp LDR below threshold
 - Voltage present
 
 Interpretation:
 Lamp commanded ON but not emitting light.
 Possible causes include lamp failure, wiring issue, or driver malfunction.
 
 ---
 
 5.3 Fault Code 2 — OVER_CURRENT
 
 Condition:
 - Measured current greater than configured maximum threshold
 
 Interpretation:
 Indicates short circuit, driver failure, or abnormal load condition.
 This fault has highest priority.
 
 ---
 
 5.4 Fault Code 3 — UNDER_VOLTAGE
 
 Condition:
 - Supply voltage below configured minimum threshold
 
 Interpretation:
 Indicates supply instability or power line issue.
 Lamp behavior under this condition is considered unreliable.
 
 ---
 
 5.5 Fault Code 4 — LAMP_OPEN_LOW_CURRENT
 
 Condition:
 - Voltage present
 - PWM > 0
 - Measured current approximately zero
 
 Interpretation:
 Indicates open circuit, disconnected lamp, or blown LED module.
 
---

6. Fault Evaluation Order

Faults are evaluated in the following order:
1. Over-current (Fault 2)
2. Under-voltage (Fault 3)
3. Lamp open / low current (Fault 4)
4. Lamp OFF when expected ON (Fault 1)
5. OK (Fault 0)

---

7. Reporting and Telemetry

- Fault code is included in every telemetry packet
- Fault detection remains local to the node
- Gateway and dashboard only display reported fault state
- No central fault inference is performed

---

8. Assumptions and Limitations

- Sensor thresholds require calibration
- LDR readings can be affected by ambient light and dust
- PIR sensor influences brightness only, not fault state
- Transient conditions are filtered using time averaging

---

9. Summary

This fault matrix defines a clear and deterministic mapping between sensor behavior and electrical fault conditions.
The approach favors simplicity, explainability, and robustness over complexity.