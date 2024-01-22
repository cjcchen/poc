
CONFIG_PATH=$PWD/poc.conf
bazel run //oracle_script/pow:benchmark_result ${CONFIG_PATH}
