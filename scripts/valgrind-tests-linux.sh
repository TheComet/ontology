#!/bin/sh

echo "Started valgrind..."
valgrind --num-callers=50 \
	--leak-resolution=high \
	--leak-check=full \
	--track-origins=yes \
	--time-stamp=yes \
	--suppressions=scripts/valgrind-suppressions.supp \
	./build/tests/ontology_tests 2>&1 | tee tests.grind
