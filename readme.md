## Usage

build and install module:
```shell script
cd ip-sniffer
make
sudo insmod ip-sniffer.ko ip=xxx.xxx.xxx.xxx # replace xxx by source ip you want to trace 
```

use client to receive logs and save to file:
```shell script
sudo python client.py
```

## Caution

+ Tested on Linux 5.8.18, may not compile in other kernel version.
