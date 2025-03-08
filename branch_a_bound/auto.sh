#!/bin/bash
for i in {1..20}; do
    ./main 350; sleep 2
    swapoff -av ; swapon -av
done
