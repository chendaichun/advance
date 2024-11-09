#!/bin/bash
for i in $(seq 1 500); do
    rm -rf $i
    echo $i
done