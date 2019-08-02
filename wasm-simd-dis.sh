#!/bin/bash
wasm-objdump -d "$1" | awk 'BEGIN { a=b=c=0 } /v128/ { a += 1 } /64x2/ { b += 1 } /32x4/ { c += 1 } END { print "v128:",a," 64x2:",b, "32x4: ",c}'
