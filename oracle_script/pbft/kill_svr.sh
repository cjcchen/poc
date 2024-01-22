
CONFIG_PATH=$1

bazel run //oracle_script/pbft/script:kill_svr $CONFIG_PATH
