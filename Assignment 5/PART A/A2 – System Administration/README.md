# Assignment 5 – Part A – A2: Design the System Administration

## Student Details

| Field | Details |
|---|---|
| **Name** | Juhi Surin |
| **Register Number** | 192525148 |
| **Course** | CSA0403 Operating Systems |
| **Assignment** | Assignment 5 – CO5 |
| **Part** | Part A – Design |
| **Task** | A2 – Design the System Administration |
| **Marks / Bloom's Level** | 5 Marks / K5 – Evaluate |

---

## 1. Task Requirements

A2 asks for a complete administration and networking design for the university result portal.

| Section | Marks | Requirement |
|---|---:|---|
| A2.1 – Users, Services and Logs | 2 | Design user accounts and groups, systemd services and log monitoring for the portal server. |
| A2.2 – Networking Services | 2 | Design IP addressing, DNS, Nginx reverse proxy, firewall rules (UFW or iptables) and SSH key access. |
| A2.3 – Automation | 1 | Identify routine tasks automated with shell scripts and cron: backups, log rotation and disk-space alerts. |
| Alternative Concept | Included | Compare Ansible with hand-written shell scripts for one server and 20 servers, configuration drift risk, learning and maintenance effort. |

The case study is the university result portal described in Assignment 5. The report uses that context to propose a secure, maintainable administration design.

## 2. Files Included

| File | Purpose |
|---|---|
| `Assignment5_PartA_A2_Report.pdf` | Full report without student identity details. Includes A2.1–A2.3, network plan, Ansible comparison, time calculations and technical references. |
| `README.md` | Student details in a table, file descriptions, execution procedure and safety notes. |
| `create_portal_accounts.sh` | Creates the service identity, operational groups and required directories; it does not create human users. |
| `result-portal.service` | Example systemd unit for the Python/Gunicorn portal service. |
| `nginx-result-portal.conf` | Example Nginx HTTPS reverse-proxy virtual host. |
| `ssh-hardening.conf` | OpenSSH key-only hardening drop-in example. |
| `ssh_key_procedure.txt` | Step-by-step SSH key setup and safe validation procedure. |
| `network_plan.txt` | Proposed private subnets, DNS records and traffic policy. |
| `ufw_setup.sh` | UFW rules script. It defaults to a dry run and only applies changes when `APPLY=1` is explicitly set. |
| `backup_postgres.sh` | PostgreSQL logical backup example with retention. Run as the `postgres` OS account. |
| `disk_space_alert.sh` | Disk-usage threshold alert using `df`, `awk` and `logger`; optional email if `mail` is configured. |
| `result-portal.cron` | Example `/etc/cron.d` schedule for daily database backup and periodic disk alerts. |
| `logrotate-result-portal` | Example daily log rotation policy for application log files. |
| `journald-result-portal.conf` | Optional persistent journal retention drop-in. |
| `ansible/portal_admin.yml` | Ansible alternative showing desired-state package, account, group, directory and service administration tasks. |
| `ansible/inventory.ini.example` | Sample inventory with illustrative address; replace it with the approved server address. |

## 3. Implementation Scope

A2 is a design and evaluation task. The question does **not** include a C “Complete the Code” program. Therefore, no C source file or GCC compilation is required.

The included Bash scripts and configuration examples support the requested administration plan. They are examples for an Ubuntu Server/systemd environment and must be adapted to the university's real domain, approved IP ranges, database choice, TLS certificate and administrator VPN subnet.

Compiler information:
- GCC: Not used for A2.
- Shell: Bash.
- Check local version with `bash --version`.
- Check Linux distribution with `cat /etc/os-release`.
- Check systemd with `systemctl --version`.
- Check Ansible, if installed, with `ansible --version`.

## 4. User, Group and Service Design

The report separates human administrator accounts from service identities.

| Identity/group | Purpose |
|---|---|
| `portal-admins` | Named system administrators; controlled sudo and SSH access. |
| `portal-deploy` | Approved release/deployment operators; restricted write and service-restart permissions. |
| `portal-logreaders` | Staff who need read-only access to application logs. |
| `portal` | Non-login system account that runs the Python/Gunicorn service. |
| `postgres` | Separate database service identity; not shared with the web application. |

The portal systemd service uses `User=portal`, restarts after an abnormal failure and applies basic service hardening. The Nginx configuration forwards HTTPS requests to Gunicorn on the loopback interface.

## 5. Network Design

The supplied network plan uses these **illustrative** RFC1918 ranges:

- Management/VPN: `10.20.10.0/24`
- DMZ/reverse proxy: `10.20.20.0/24`
- Application: `10.20.30.0/24`
- Database: `10.20.40.0/24`

Replace these addresses with subnets approved by the university network administrator. The `results.university.example` hostname is a reserved example placeholder; replace it with the university's actual DNS name.

The intended exposure is:
- TCP 443 from users to Nginx.
- TCP 80 to redirect clients to HTTPS.
- TCP 22 only from an approved management/VPN subnet or bastion.
- Application TCP 8000 only between the reverse proxy and application hosts.
- Database TCP 5432 only from approved application hosts.
- No public access to ports 8000 or 5432.

## 6. Safe Execution Procedure

### A. Inspect the files first

```bash
bash --version
cat /etc/os-release
systemctl --version
```

The unit files assume systemd. Some WSL environments do not run systemd by default; use a full Ubuntu Server VM or enable systemd in WSL before attempting service management.

### B. Create users and directories

The report gives the user/group design and suggested commands. Create named human users individually using the university's account policy. Do not share administrator accounts.

### C. Configure systemd

After the Python virtual environment and application import path exist, copy `result-portal.service` to:

```text
/etc/systemd/system/result-portal.service
```

Create `/etc/result-portal/portal.env` as root, restrict it to `0640`, and keep secrets out of source/config files. Update `portal.wsgi:application` if the actual Python framework uses a different WSGI import path.

Then validate and start:

```bash
sudo systemd-analyze verify /etc/systemd/system/result-portal.service
sudo systemctl daemon-reload
sudo systemctl enable --now result-portal.service
sudo systemctl status result-portal.service
```

### D. Configure Nginx

The included Nginx file uses a placeholder hostname and certificate paths. Obtain a valid TLS certificate and replace those placeholders before enabling it.

```bash
sudo nginx -t
sudo systemctl reload nginx
```

The included reverse proxy targets `127.0.0.1:8000`, suitable when Nginx and Gunicorn share a host. For a separated application tier, change the upstream to private application addresses and permit the application port only from the Nginx network.

### E. Configure SSH keys

On the administrator workstation:

```bash
ssh-keygen -t ed25519 -C "portal-admin"
ssh-copy-id adminuser@SERVER_IP
```

Test a second key-authenticated SSH session before disabling password login. Copy `ssh-hardening.conf` to `/etc/ssh/sshd_config.d/60-result-portal.conf`, verify the `portal-admins` group exists and contains the intended administrator, then run:

```bash
sudo sshd -t
sudo systemctl reload ssh
```

Never copy the private key to the server.

### F. Review UFW before applying

The firewall script is deliberately dry-run by default:

```bash
./ufw_setup.sh
```

Review the administrator CIDR carefully. Only apply it after confirming the correct management/VPN subnet and recovery access:

```bash
sudo ADMIN_CIDR='YOUR_APPROVED_ADMIN_CIDR' APPLY=1 ./ufw_setup.sh
```

Do not run this blindly over a remote SSH session. An incorrect SSH source rule can lock out administrators.

### G. Schedule automation

Copy scripts to `/usr/local/sbin`, set root/postgres ownership and executable permissions, then install the reviewed cron file as `/etc/cron.d/result-portal`.

Create the service identity and folders after reviewing the script:

```bash
sudo ./create_portal_accounts.sh
```

Example permissions and installation:

```bash
sudo install -o root -g root -m 0750 disk_space_alert.sh /usr/local/sbin/
sudo install -o root -g root -m 0750 backup_postgres.sh /usr/local/sbin/
sudo install -o root -g root -m 0644 result-portal.cron /etc/cron.d/result-portal
```

The account-setup script creates `/var/backups/result-portal` owned by `postgres`. The backup script is for PostgreSQL and expects local peer authentication when run as `postgres`. If the university uses MySQL or another database, replace the dump command with the database's supported backup tool.

The cron file schedules:
- PostgreSQL backup daily at 02:15.
- Disk-space alerts every 10 minutes.
- Log rotation through the distribution's regular logrotate schedule and the supplied policy.

Keep an independent off-host backup. A local dump on the same disk is not a complete disaster-recovery plan.

## 7. Ansible Alternative

The supplied Ansible playbook illustrates declarative administration using built-in modules. It is a starting example, not a one-command production deployment. Replace inventory addresses, test the playbook with `--check --diff`, install application secrets through Ansible Vault or another approved secret manager, and add validated TLS/Nginx/firewall templates before using it in production.

Ansible modules are commonly idempotent: rerunning a task that has already reached its desired state should not change it. Not every module or arbitrary command is automatically idempotent, so playbooks still require testing.

## 8. Important Assumptions and Limitations

- Address ranges and the `.example` DNS name are placeholders, not actual university network details.
- The service unit assumes a Python WSGI app launched by Gunicorn; adjust the WSGI import path and worker count.
- PostgreSQL is used as the illustrative database in the backup script; adapt if the deployed database differs.
- UFW is a host firewall. Network firewalls/VLAN ACLs should reinforce the same policy.
- The firewall script is dry-run by default to reduce the risk of losing remote access.
- Backup, disk-alert and logrotate scripts have passed Bash syntax checks, but have not been deployed to a live university server.
- Time comparisons are planning estimates, not measured installation times.

## 9. Result

The design defines least-privilege user accounts and groups, a hardened systemd service, journal and Nginx log monitoring, a segmented IP/DNS plan, Nginx HTTPS reverse proxy, UFW rules, key-based SSH access and cron automation for backups and disk alerts. The Ansible alternative reduces configuration drift and is more efficient to repeat across 20 servers, while hand-written scripts have less initial overhead for one server.
