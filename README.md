![ARMadillo-logo](./ARMadillo-logo.png)

# ARMadillo - Traffic Filtering Made Easy

[![GitHub Super-Linter](https://github.com/gapu-team/ARMadillo/workflows/Lint%20Code%20Base/badge.svg)](https://github.com/marketplace/actions/super-linter)

ARM based solution serving as your first line of defense against malicious packets.

Network traffic filtering mechanism, similar concept to Unix iptables.
The user would be able to use a CLI/configuration file to control in/out filtering rules.

The solution would leverage the advanced capabilities of Nvidia’s BluField DPU to deeply inspect inbound network traffic, enabling the CPU to handle main tasks more efficiently.

ARMadillo can scale horizontally to any server that features a BluField and is software-agnostic to the server so you can run or upgrade anything you want without breaking the mechanism.

Filtering options would be IP addresses, ports, packet sizes, blacklist words, DNSs and more. It would also keep track on your ongoing traffic by populating audit logs.
