#!/bin/sh

CONFIG_PATH=$PWD/benchmark.conf
sh $PWD/../run_cli.sh $CONFIG_PATH

sleep 60

sh $PWD/kill_svr.sh
