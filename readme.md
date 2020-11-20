## Usage

build and install module:
```bash
cd ip-sniffer
make
sudo insmod ip-sniffer.ko ip=xxx.xxx.xxx.xxx # replace xxx by source ip you want to trace 
```

use client to receive logs and save to file:
```bash
sudo python client.py
```

example content in ip.log for ip=127.0.0.1
```
00001 | src_ip: 127.0.0.1, dst_ip: 127.0.0.1, src_mac: 00:00:00:00:00:00, dst_mac: 00:00:00:00:00:00, protocol: TCP, src_port: 16082, dst_port: 9481
00002 | src_ip: 127.0.0.1, dst_ip: 127.0.0.1, src_mac: 00:00:00:00:00:00, dst_mac: 00:00:00:00:00:00, protocol: TCP, src_port: 9481, dst_port: 16082
00003 | src_ip: 127.0.0.1, dst_ip: 127.0.0.1, src_mac: 00:00:00:00:00:00, dst_mac: 00:00:00:00:00:00, protocol: TCP, src_port: 11474, dst_port: 9481
00004 | src_ip: 127.0.0.1, dst_ip: 127.0.0.1, src_mac: 00:00:00:00:00:00, dst_mac: 00:00:00:00:00:00, protocol: TCP, src_port: 9481, dst_port: 16082
00005 | src_ip: 127.0.0.1, dst_ip: 127.0.0.1, src_mac: 00:00:00:00:00:00, dst_mac: 00:00:00:00:00:00, protocol: TCP, src_port: 9481, dst_port: 11474
00006 | src_ip: 127.0.0.1, dst_ip: 127.0.0.1, src_mac: 00:00:00:00:00:00, dst_mac: 00:00:00:00:00:00, protocol: TCP, src_port: 16082, dst_port: 9481
00007 | src_ip: 127.0.0.1, dst_ip: 127.0.0.1, src_mac: 00:00:00:00:00:00, dst_mac: 00:00:00:00:00:00, protocol: TCP, src_port: 9481, dst_port: 17618
...
```

## Caution

+ Tested on Linux 5.8.18, may not compile in other kernel version.
