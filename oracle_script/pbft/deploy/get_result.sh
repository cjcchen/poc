#!/bin/sh

CONFIG_PATH=$PWD/benchmark.conf
bazel run //oracle_script/pbft:benchmark_result $CONFIG_PATH
