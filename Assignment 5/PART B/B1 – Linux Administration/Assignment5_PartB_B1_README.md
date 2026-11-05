# Assignment 5 – Part B – B1: Administer the Linux Server

| **Student Detail** | **Information** |
| ------------------ | --------------- |
| **Name** | Juhi Surin |
| **Register Number** | 192525148 |
| **Course** | CSA0403 Operating Systems |

## 1. Task requirements
B1 is worth 7 marks:
- B1.1 Observe processes, memory, disk and network using `ps`, `top`/`htop`, `free`, `vmstat`, `iostat`, `df` and `ss`.
- B1.2 Write shell scripts for log analysis and disk-space alerts, schedule with cron and show output.
- B1.3 Run a portal/sample app as a systemd service behind Nginx, with firewall access limited to HTTP, HTTPS and SSH.
- B1.4 Compare the implementation with the Part A design and explain any change.
- Complete the supplied Bash script's TODO 1 and TODO 2 using `awk`, `sort` and `uniq`.

## 2. Included files
- `scripts/complete_log_analysis.sh`: completed exact assignment sample-log TODO script.
- `scripts/log_analysis.sh`: analysis of a real Nginx combined access log.
- `scripts/disk_space_alert.sh`: disk threshold warning script.
- `scripts/collect_system_metrics.sh`: B1.1 command-output collector.
- `scripts/portal_app.py`: small sample portal with `/` and `/healthz`.
- `systemd/result-portal.service`: systemd unit.
- `nginx/result-portal`: reverse-proxy site configuration.
- `evidence/measurement_template.txt`: record your own actual system observations.

## 3. Prerequisites
Use Ubuntu or another systemd-based Linux distribution, not Windows PowerShell:
```bash
sudo apt update
sudo apt install -y nginx ufw cron sysstat procps iproute2 python3
sudo systemctl enable --now cron
```
`iostat` is provided by `sysstat`; `htop` is optional.

## 4. B1.1 – Observe the system
```bash
chmod +x scripts/collect_system_metrics.sh
./scripts/collect_system_metrics.sh
```
It saves a timestamped observation file. Individual commands:
```bash
ps -eo pid,ppid,user,stat,%cpu,%mem,comm --sort=-%cpu | head
top
free -h
vmstat 1 5
iostat -xz 1 2
df -hT
ss -tulpn
```
Interpretation:
- `ps`: process/parent IDs, state, CPU and memory percentages.
- `top`: live process and system summary.
- `free`: RAM and swap; `available` estimates memory usable without swapping.
- `vmstat`: runnable/blocked processes, memory, swap, CPU and I/O wait. First line may be averages since boot; subsequent lines are interval samples.
- `iostat`: device throughput, queueing, latency (`await`) and utilisation (`%util`).
- `df`: filesystem size, used/available capacity and mount point.
- `ss`: listening TCP/UDP sockets; `-p` may require elevated privileges.
Fill `evidence/measurement_template.txt` with your own output. Values depend on machine and time; do not claim example values as measurements.

## 5. B1.2 – Automate

### A. Complete the exact supplied TODOs
```bash
chmod +x scripts/complete_log_analysis.sh
bash scripts/complete_log_analysis.sh
```
Expected output for the assignment's seven fixed sample lines:
```text
Requests per status code:
200 4
302 1
403 1
404 1
Busiest client:
10.0.0.5 4
Total bytes sent: 43200
```
The assignment's simplified log has status in field 7 and bytes in field 8. `awk` extracts fields, `sort` groups values, and `uniq -c` counts them.

### B. Analyse actual Nginx access logs
```bash
chmod +x scripts/log_analysis.sh
curl -I http://127.0.0.1/
sudo bash scripts/log_analysis.sh /var/log/nginx/access.log
```
This separate script expects standard Nginx combined logs: status field 9 and bytes field 10. Generate a request first if the log is empty.

### C. Disk-space alert
```bash
chmod +x scripts/disk_space_alert.sh
THRESHOLD=80 ./scripts/disk_space_alert.sh
```
It writes a timestamped OK or warning entry to `~/disk_space_alert.log`. Test the warning branch without filling a disk by using `THRESHOLD=1`.

### D. Cron schedule
Use absolute paths and replace `/home/youruser/...` with the actual folder location:
```cron
0 * * * * /bin/bash /home/youruser/Assignment5_PartB_B1/scripts/log_analysis.sh /var/log/nginx/access.log >> /home/youruser/b1-log-analysis-cron.log 2>&1
*/15 * * * * /usr/bin/env THRESHOLD=80 /bin/bash /home/youruser/Assignment5_PartB_B1/scripts/disk_space_alert.sh >> /home/youruser/b1-disk-alert-cron.log 2>&1
```
Install and verify:
```bash
crontab -e
crontab -l
```
If the account cannot read Nginx logs, configure restricted permissions or a carefully scoped root cron entry. Do not make logs world-readable.

## 6. B1.3 – systemd service behind Nginx
```bash
sudo useradd --system --no-create-home --shell /usr/sbin/nologin portal
sudo install -d -o root -g portal -m 0750 /opt/result-portal
sudo install -o root -g portal -m 0640 scripts/portal_app.py /opt/result-portal/portal_app.py
sudo install -m 0644 systemd/result-portal.service /etc/systemd/system/result-portal.service
sudo systemctl daemon-reload
sudo systemctl enable --now result-portal
sudo systemctl status result-portal --no-pager
```
Configure Nginx:
```bash
sudo install -m 0644 nginx/result-portal /etc/nginx/sites-available/result-portal
sudo ln -sfn /etc/nginx/sites-available/result-portal /etc/nginx/sites-enabled/result-portal
sudo rm -f /etc/nginx/sites-enabled/default
sudo nginx -t
sudo systemctl enable --now nginx
sudo systemctl reload nginx
```
Test:
```bash
curl -i http://127.0.0.1/
curl -i http://127.0.0.1/healthz
sudo journalctl -u result-portal -n 30 --no-pager
sudo tail -n 20 /var/log/nginx/result-portal.access.log
```
The sample application binds only to loopback port 8000. Nginx accepts HTTP on port 80 and proxies to it. For production, use a hardened WSGI/ASGI server, TLS, real DNS, secure secrets and tested backups.

## 7. Firewall and SSH
Ensure SSH is installed/running and confirm its actual port. If connected remotely, keep the current SSH session open while enabling the firewall:
```bash
sudo ufw default deny incoming
sudo ufw default allow outgoing
sudo ufw allow 22/tcp
sudo ufw allow 80/tcp
sudo ufw allow 443/tcp
sudo ufw enable
sudo ufw status verbose
```
If SSH uses a non-default port, allow that port instead of (or in addition to) 22. Verify the SSH rule before enabling UFW or you may lock yourself out.
For key authentication:
```bash
ssh-keygen -t ed25519
ssh-copy-id username@server-address
ssh -i ~/.ssh/id_ed25519 username@server-address
```
Never share the private key. Disable password login only after key login is tested and recovery access is available.

## 8. B1.4 – Compare with Part A design
| Part A design | B1 implementation | Evaluation |
|---|---|---|
| Dedicated service account | `portal` system user | Least privilege |
| Managed service | systemd unit with restart-on-failure | Boot startup and restart |
| Reverse proxy | Nginx to `127.0.0.1:8000` | App port not public |
| Network boundary | UFW permits SSH/HTTP/HTTPS | Restricts inbound ports |
| Monitoring | system metrics script and logs | Repeatable evidence |
| Routine automation | cron log and disk jobs | Scheduled checks |
| Production readiness | Python standard-library HTTP server for lab | Replace with hardened application server and TLS in production |

If your Part A design differs, state the actual change and why. Do not claim that an unimplemented design component was deployed.

## 9. Evidence checklist
1. B1.1 output file from the collector.
2. Exact completed sample script output.
3. Real Nginx log analysis output.
4. Disk alert output and `crontab -l`.
5. `systemctl status`, `nginx -t`, `curl` tests and `ufw status verbose`.
6. Completed measurement template.
7. B1.4 comparison and explanation.

## 10. Result
B1 is complete when system observations are recorded and interpreted, scripts run, cron entries are verified, the sample portal is active under systemd behind Nginx, firewall/SSH access is safely configured, and the implementation is compared with Part A.
