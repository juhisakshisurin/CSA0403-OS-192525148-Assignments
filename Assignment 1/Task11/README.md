
# Task 11 – Recommend and Defend the Operating-System Plan

## Student Details

| Field | Details |
|:---|:---|
| Name | Juhi Surin |
| Register Number | 192525148 |
| Course | CSA0403 – Operating Systems |
| Assignment | Assignment 1 – CO1 |
| Task | Task 11 – Recommend and Defend the Operating-System Plan |
| Under the Supervision of | Dr. Velmurugan J. |

## Objective

To recommend and justify a complete operating-system plan for the smart hospital by integrating the concepts evaluated in the previous tasks.

## 1. Introduction

A smart hospital contains different computing environments with different performance, reliability, response-time and security requirements.

The final operating-system plan must consider the requirements of bedside patient monitors, handheld tablets, desktop workstations, database servers, MRI imaging servers and communication systems.

## 2. Scope of the Recommendation

The operating-system plan must consider:

- Operating-system requirements for each environment.
- Suitable operating-system types.
- Appropriate kernel structures.
- Essential operating-system services.
- System-call and process-management requirements.
- CPU protection and memory protection.
- Storage placement and I/O methods.
- Reliability, availability and security.

## 3. Evaluation Criteria

| Criterion | Requirement |
|:---|:---|
| Response Time | Support predictable and timely processing of critical events. |
| Reliability | Maintain continuous operation of essential hospital services. |
| Performance | Handle medical applications and large data workloads efficiently. |
| Security | Protect confidential patient records and medical information. |
| Scalability | Support additional devices, users and hospital locations. |
| Availability | Reduce service interruption through suitable recovery mechanisms. |

## 4. Final Operating-System Recommendation

The final recommendation must specify the suitable operating-system type and structure for each hospital environment.

| Environment | Recommended OS Type | Main Requirement |
|:---|:---|:---|
| Bedside Patient Monitors | Real-Time Operating System | Predictable response and reliability. |
| Nurses' Handheld Tablets | Handheld Operating System | Mobility and power efficiency. |
| Doctors' Workstations | Desktop Operating System | User interface and multitasking. |
| Central Database Server | Multiprocessor/Clustered System | Reliability and high availability. |
| MRI Imaging Server | Multiprocessor Operating System | High processing capability and data transfer. |
| Partner-Hospital Communication | Distributed System | Secure information sharing. |

## 5. Protection and I/O Considerations

The proposed design must include:

- User-mode and kernel-mode separation.
- Memory protection and process isolation.
- Authentication and access control.
- Reliable backup and recovery.
- Interrupt-driven handling for sensor events.
- DMA for large MRI image transfers.
- Caching for frequently accessed patient records.

## 6. Limitations and Risks

The recommendation must evaluate possible limitations, including:

- Hardware failures.
- Network interruptions.
- Security threats.
- System overload.
- Data inconsistency.
- Software and driver failures.
- Resource limitations.

## 7. Multi-Hospital Support

The design must be evaluated for its ability to support three hospitals sharing patient records.

The evaluation should consider secure communication, data consistency, access control, availability, replication and recovery.

## 8. Alternative Architecture

The task also considers moving the complete system to virtual machines on two large servers.

The alternative must be evaluated in terms of:

- Response time.
- Resource overhead.
- Reliability.
- Isolation.
- Availability.
- Failure recovery.

## 9. Result

A complete operating-system plan for the smart hospital must be recommended and defended using the technical analysis, comparisons and evidence developed in the previous tasks.

## 10. Conclusion

The final operating-system design should balance patient safety, predictable response, reliability, security, performance and scalability. The recommended architecture must support the different hospital environments while providing secure and dependable operation.

## Submission Evidence

Add the completed Task 11 report, supporting diagrams, comparisons and evidence to this folder.

