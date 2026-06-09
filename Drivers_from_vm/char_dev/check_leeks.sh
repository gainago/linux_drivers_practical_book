#!/usr/bin/bash
for i in {1..100}; do
	make load
	make unload
done

