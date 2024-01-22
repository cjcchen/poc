
CONFIG_PATH=$PWD/poc.conf
bazel run //oracle_script/pow:kill_svr ${CONFIG_PATH}
