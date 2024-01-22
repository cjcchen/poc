
CONFIG=$1

cp ../../pbft/deploy//server.config ~/nexres/oracle_script/pow/bft_server.config

bazel build //application/poc:pow_server
bazel run //oracle_script/pow:run_svr ${CONFIG}


