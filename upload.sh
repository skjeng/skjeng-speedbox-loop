#!/bin/bash

iperf3 -c $1 -R| grep -Po '[0-9.]*(?= Mbits/sec)'
