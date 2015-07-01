#!/bin/bash

#iperf3 -c $1 -R| grep -Po '[0-9.]*(?= Mbits/sec)'
ping $1 -c 10 | awk '/packet loss/{x="Loss:" $7} /round-trip/{x="Trip:" $4} END{print x}'
