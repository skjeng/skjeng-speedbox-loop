#!/bin/bash

iperf3 -c $1| grep -Po '[0-9.]*(?= Mbits/sec)'
