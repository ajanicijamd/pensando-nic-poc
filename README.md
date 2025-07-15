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

    ./prog --lif 43000070-0100-0000-4242-0490812c77b0 --card 42424650-4c32-3530-3330-304131000000

The command's parameters is lif ID of the NIC and card ID. How did we get those values?

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

The command that gets the CNP count for the card is:

    sudo nicctl show rdma queue-pair statistics --queue-pair-id 1 --lif 43000070-0100-0000-4242-0490812c77b0 --json

which produces output like this:

````
{
    "nic": [
        {
            "id": "42424650-4c32-3530-3330-304131000000",
            "lif": [
                {
                    "id": "43000070-0100-0000-4242-0490812c77b0",
                    "queue_pair": [
                        {
                            "queue_pair_id": "1",
                            "send_queue": {
                                "requester_tx_statistics": {
                                    "num_byte": "0",
                                    "num_packet": "0",
                                    "num_send_msg": "0",
                                    "num_write_msg": "0",
                                    "num_read_request_msg": "0",
                                    "num_atomic_fna_msg": "0",
                                    "num_atomic_compare_and_swap_msg": "0",
                                    "num_send_msgs_invalid_remote_key_error": "0",
                                    "num_send_imm_msg": "0",
                                    "num_write_imm_msg": "0",
                                    "max_num_of_packets_sent_per_msg": "0",
                                    "num_non_packet_generating_request": "0",
                                    "type1_bind_memory_window_request": "0",
                                    "type2_bind_memory_window_request": "0",
                                    "phy_mem_region_fast_registration_req": "0",
                                    "local_invalidate_request": "0",
                                    "num_wqe_with_inline_data": "0",
                                    "num_local_ack_timeouts": "0",
                                    "receiver_not_ready_timeout": "0",
                                    "times_send_queue_is_drained": "0",
                                    "num_cnp_sent": "0"
                                },
                                "requester_rx_statistcs": {
                                    "num_byte": "0",
                                    "num_packet": "0",
                                    "num_ack_msg": "0",
                                    "num_atomic_ack_msg": "0",
                                    "num_read_response_msg": "0",
                                    "num_read_response_packet": "0",
                                    "num_feedback": "0",
                                    "max_packets_received_per_msg": "0",
                                    "num_packets_with_ecn_marking": "0"
                                }
                            },
                            "receive_queue": {
                                "responder_tx_statistics": {
                                    "num_byte": "0",
                                    "num_packet": "0",
                                    "num_read_resp_msg": "0",
                                    "num_read_resp_pkt": "0",
                                    "num_ack_msg": "0",
                                    "num_atomic_response_msg": "0",
                                    "max_num_of_pkts_sent_per_msg": "0",
                                    "receiver_not_ready_rnrerror": "0",
                                    "num_sequence_error": "0",
                                    "num_dcqcn_rp_additive_increase": "0",
                                    "num_dcqcn_rp_fast_recovery": "0",
                                    "num_dcqcn_rp_hyper_increase": "0",
                                    "num_dcqcn_rp_byte_threshold_hit": "0",
                                    "num_dcqcn_rp_alpha_timer_expiry_event": "0",
                                    "num_dcqcn_rp_timer_expiry_event": "0",
                                    "num_rp_max_rate_hit": "0"
                                },
                                "responder_rx_statistics": {
                                    "num_byte": "0",
                                    "num_packet": "0",
                                    "num_send_msg": "0",
                                    "num_write_msg": "0",
                                    "num_read_msg": "0",
                                    "num_atomic_fetch_and_add_msg": "0",
                                    "num_atomic_compare_and_swap_msg": "0",
                                    "num_send_msgs_with_invalid_remote_key_error": "0",
                                    "num_send_imm_msg": "0",
                                    "num_write_imm_msg": "0",
                                    "num_packets_with_ack_requested": "0",
                                    "num_packets_with_ecn_marking": "0",
                                    "num_cnps_received": "0",
                                    "max_recirculation_exceeded_packet_drop": "0",
                                    "num_mem_window_invalidate_msg": "0",
                                    "num_duplicate_pkts_with_write_send_opcode": "0",
                                    "num_duplicate_read_atomic_backtrack_pkt": "0",
                                    "num_duplicate_read_atomic_drop_pkt": "0"
                                }
                            }
                        }
                    ]
                }
            ]
        }
    ]
}
````

Note the field "num_cnp_sent" in the object "requester_tx_statistics".

Command that gets the rate is:

````
sudo nicctl show port statistics --json --rate
````

This command's output:

````
{
    "nic": [
        {
            "id": "42424650-4c32-3530-3330-304131000000",
            "port": [
                {
                    "id": "0490812c-77b0-4242-4242-000011010000",
                    "tx_pps": "0",
                    "tx_bps": "0",
                    "rx_pps": "0",
                    "rx_bps": "0"
                }
            ]
        }
    ]
}

````

From that output we can see the card ID: 42424650-4c32-3530-3330-304131000000.
