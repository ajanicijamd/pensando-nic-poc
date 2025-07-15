## Proof of concept for getting Pensando Pollara NIC metrics

### Introduction

This directory contains a proof of concept program for getting Pensando
Pollara card's metrics by running the nicctl utility and processing its
JSON output.

Since nicctl requires to be run by root or with sudo, how does a regular
user program call it? The solution in this POC is to call it in two steps:

- Program prog is a regular program that calls nicctl-runner in a child
  process.
- nicctl-runner is owned by root and has setuid flag, so it can call
  nicctl.

### Building

Build prog and nicctl-runner by:

    make

Then, for nicctl-runner, make root its owner and set its setuid flag:

    sudo chown root: ./nicctl-runner
    sudo chmod u+s ./nicctl-runner

Run:

    make run

We can also run prog directly:

    ./prog 43000070-0100-0000-4242-0490812c77b0

The command's parameter is lif ID of the NIC. How did we get that value?

- Command
    sudo nicctl show lif --json

  on system lsttserver7 produces this JSON output:

````
{
    "nic": [
        {
            "id": "42424650-4c32-3530-3330-304131000000",
            "lif": [
                {
                    "spec": {
                        "id": "43000070-0100-0000-4242-0490812c77b0",
                        "mac_address": "04:90:81:2c:77:b0",
                        "type": "host",
                        "pf_id": "0",
                        "admin_state": "down",
                        "rdma_mtu": "-"
                    },
                    "status": {
                        "hw_id": "67",
                        "name": "enp229s0",
                        "oper_state": "down"
                    }
                }
            ]
        }
    ]
}
````

"nic" maps to an array of NIC specification objects, which in this case
has only one. Its key "lif" maps to spec and status. In the spec, "id"
maps to lif ID and in the status, "name" maps to the NIC name, in this
case "enp229s0".

All NICs in the system can be listed with

    lshw -C network

and for enp229s0 we see this:

````
  *-network
       description: Ethernet interface
       product: DSC Ethernet Controller
       vendor: AMD Pensando Systems
       physical id: 0
       bus info: pci@0000:e5:00.0
       logical name: enp229s0
       version: 00
       serial: 04:90:81:2c:77:b0
       width: 64 bits
       clock: 33MHz
       capabilities: bus_master cap_list ethernet physical fibre autonegotiation
       configuration: autonegotiation=off broadcast=yes driver=ionic driverversion=5.15.0-143-generic duplex=full firmware=1.110.1-a-1 latency=0 link=no multicast=yes
       resources: iomemory:47f0-47ef irq:287 memory:c0300000-c0307fff memory:c0200000-c02fffff memory:47f84000000-47f87ffffff
````
