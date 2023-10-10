#!/bin/bash

read -p ">> " input

# Split the input into a, op, and b based on spaces
IFS=' ' read -r a op b <<< "$input"

echo "You entered:"
echo "a: $a"
echo "op: $op"
echo "b: $b"
