
# Task 12 – Re-evaluate and Adapt the Design

## Student Details

| Field | Details |
|:---|:---|
| Name | Juhi Surin |
| Register Number | 192525148 |
| Course | CSA0403 – Operating Systems |
| Assignment | Assignment 1 – CO1 |
| Task | Task 12 – Re-evaluate and Adapt the Design |
| Under the Supervision of | Dr. Velmurugan J. |

## Objective

To re-evaluate the original smart-hospital operating-system design after introducing remote patient monitoring and cloud-based patient records.

## 1. Introduction

The original smart-hospital design must be evaluated again when new technologies and requirements are introduced.

The new scenario introduces home-based remote patient-monitoring devices and a cloud-based records service shared with partner hospitals.

These changes affect operating-system requirements, communication, security, availability and resource management.

## 2. Changed Requirements

The hospital introduces two major changes:

1. Remote monitoring devices installed in patients' homes send readings through the internet.
2. Patient records are moved to a cloud-based service shared with partner hospitals.

## 3. Impact Analysis

The impact analysis must identify the operating-system components affected by the new requirements.

| Component | Impact of the New Requirement |
|:---|:---|
| Operating-System Type | Embedded and real-time systems are required for remote monitoring devices. |
| Communication | Internet-based communication is needed to transfer patient readings. |
| Security | Authentication, encryption and access control are required. |
| Storage | Cloud-based storage must support shared patient records. |
| Availability | Reliable access and recovery mechanisms are required. |
| Resource Management | The system must support additional remote devices and data traffic. |

## 4. Evaluation of the Original Design

The original design must be evaluated to determine whether it can support remote monitoring and cloud-based records.

The evaluation should consider:

- Real-time response requirements.
- Reliability of remote monitoring.
- Network dependency.
- Cloud-service availability.
- Patient-data confidentiality.
- Access control between hospitals.
- Data consistency and backup.

## 5. Adaptation or Redesign

The revised design should consider:

### Remote Patient Monitoring

Embedded real-time operating systems can be used in home monitoring devices to process sensor readings and respond to critical events.

### Cloud-Based Patient Records

Distributed systems can support shared patient records across hospitals.

### Security

The revised system should include secure communication, authentication, authorisation and protection of sensitive patient information.

### Reliability

The design should include suitable backup, recovery and availability mechanisms.

## 6. Evidence-Based Defence

The final design decision must explain whether the original architecture is retained, modified or redesigned.

The decision should be justified using:

- Technical feasibility.
- Performance requirements.
- Security requirements.
- Reliability.
- Scalability.
- Advantages and limitations.
- Resource and communication requirements.

## 7. Result

The original smart-hospital design is re-evaluated against the changed requirements. A suitable adaptation or redesign must be justified using impact analysis and evidence-based evaluation.

## 8. Conclusion

The introduction of remote patient monitoring and cloud-based patient records changes the operating-system requirements of the smart hospital.

The revised architecture must support remote devices, secure communication, reliable cloud access and information sharing between hospitals while maintaining patient safety, security and availability.

## Submission Evidence

Add the completed Task 12 report, revised architecture diagram, impact analysis and supporting evidence to this folder.

