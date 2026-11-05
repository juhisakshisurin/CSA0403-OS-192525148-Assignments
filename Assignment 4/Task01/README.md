# Assignment 4 – Task 1: File-System Design Review

## Student Details

| Field | Details |
|---|---|
| **Name** | Juhi Surin |
| **Register Number** | 192525148 |
| **Course** | CSA0403 Operating Systems |
| **Assignment** | Assignment 4 – CO4 |
| **Task** | Task 1 – File-System Design Review |
| **Marks / Bloom's Level** | 6 Marks / K5 – Evaluate |

## 1. Task Overview

Task 1 evaluates a proposed file organisation for a college server that stores files for 12,000 students. The proposal uses one flat directory, student-chosen filenames, contiguous allocation for every file and no access control.

The task requires:

- Identify at least five weaknesses in the proposed organisation.
- Design an improved tree or acyclic-graph directory structure.
- Specify naming rules and file attributes.
- Define access control using ACLs or permission bits.
- Provide links for shared project folders.
- Defend the design in not more than 100 words.
- Evaluate the alternative of storing student files in a cloud service such as Google Drive or OneDrive, including find/open time, storage used (including copies) and college control over privacy.

**No C program or other code implementation is requested in Task 1.** This is a design-review task. Therefore, there is no GCC compilation command for this task.

## 2. Files Included

| File | Description |
|---|---|
| `Assignment4_Task01_Report.pdf` | Complete report without student identity details. |
| `README.md` | Student details, task coverage and submission instructions. |
| `proposed_directory_structure.txt` | Copyable directory-tree proposal, naming rules and permissions summary. |
| `evidence_checklist.txt` | Checklist for final GitHub and report evidence. |

## 3. Proposed Directory Structure

The report proposes a hierarchical directory tree:

```text
/college-files/
├── students/
│   └── <cohort>/
│       └── <department>/
│           └── <student-id>/
│               ├── assignments/
│               ├── notes/
│               ├── submissions/
│               └── projects/
├── projects/
│   └── <project-id>/
│       ├── shared/
│       ├── deliverables/
│       └── references/
├── courses/
│   └── <course-code>/
│       └── resources/
└── administration/
    ├── policies/
    └── backup-catalog/
```

Student directories are private by default. A group project has one canonical shared directory under `/projects/<project-id>/`. A student's `projects/` directory can contain a symbolic link to that shared location. Access to the linked target is still checked against the target directory's ACLs or permissions.

## 4. Naming Rules and File Attributes

Recommended filename pattern:

```text
<course-code>_<academic-year>_<assignment-or-topic>_<description>_vNN.<ext>
```

Example:

```text
CSA0403_2026-27_A4_Task01_FileSystemReview_v01.pdf
```

Names should use a controlled character set, avoid ambiguous names such as `final-final2`, and include a version number for submitted revisions. Student identity is represented by a unique student ID in the directory path rather than relying on a student-chosen filename.

Important file attributes include:

- Owner and group.
- File type and size.
- Creation/birth time where supported, modification time and access time.
- Permission bits and ACL entries.
- Link count and inode/file identifier.
- Checksum or content hash for integrity checking.
- Version/backup metadata and retention classification.

## 5. Access-Control Policy

Suggested default permissions:

| Object | Suggested access |
|---|---|
| Student's personal directory | Owner only (`0700`) |
| Personal files | Owner read/write (`0600`) |
| Shared project directory | Project group can read/write; no access for unrelated students |
| Shared project files | Project group read/write; owner/admin control retained |
| Administration and backup catalog | Restricted administrator/service account access |

Use ACLs for project-specific membership. The canonical shared project directory is the authoritative copy; links in student folders are references to it, not uncontrolled duplicate copies. Directory traversal permissions must also be checked so that a student cannot bypass a private parent directory.

## 6. Build and Execution Information

Task 1 is a design and evaluation task. It contains no C source code, TODO block, executable or shell script to compile.

| Required README field | Task 1 entry |
|---|---|
| Compiler and version | Not applicable – no program is requested. |
| Exact GCC command | Not applicable – no source code is requested. |
| Linux distribution/version | Record the distribution used for any optional evidence screenshots. Task 1 itself does not require Linux command execution. |

To record your environment if you choose to include a terminal screenshot:

```bash
gcc --version
cat /etc/os-release
uname -a
```

These commands only identify the environment; they do not compile or execute a Task 1 program.

## 7. Cloud-Service Alternative

The report compares a college-managed server with a cloud service such as Google Drive or OneDrive. The comparison is qualitative because the question does not provide provider pricing, network latency, storage quotas, region, file count distribution or copy-retention settings.

- Search and open time in cloud storage depends on provider indexing, metadata search, authentication, network round-trip, file size and client caching.
- Storage accounting must distinguish one shared file with multiple links from actual duplicated copies, versions, replicas and local caches. Exact billing rules depend on the provider and tenant plan.
- A college server gives the institution direct control of the storage stack, access logs, keys, backups and retention policy, but also makes it responsible for patching, availability, security and recovery.
- A cloud service reduces local infrastructure operations but shifts some infrastructure control to the provider. College control depends on tenant settings, identity management, audit facilities, encryption configuration, contractual terms and data-location options.

## 8. GitHub Submission

The Assignment 4 instructions require a repository named:

```text
CSA04-192525148-Assignment4
```

Create a `Task01` folder and place the files from this package inside it. The instructions state that a working GitHub repository link is mandatory and should appear in the report cover page. Add the actual repository URL after creating and checking the repository; do not submit a placeholder as though it were a working URL.

Suggested structure:

```text
CSA04-192525148-Assignment4/
└── Task01/
    ├── Assignment4_Task01_Report.pdf
    ├── README.md
    ├── proposed_directory_structure.txt
    └── evidence_checklist.txt
```

Make at least one separate commit for Task 1. The assignment instructions state that commit history will be checked and that work should be committed progressively.

## 9. Result

The proposed design replaces the single shared directory with a hierarchical student/course/project structure, uses unique identifiers and predictable filenames, records useful file attributes, protects personal directories with permissions/ACLs, and shares group-project folders through controlled links to a canonical project directory. The cloud alternative is evaluated by search/open latency factors, storage copies/versions, and the degree of privacy control retained by the college.
