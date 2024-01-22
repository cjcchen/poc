
cp ../../pbft/deploy/server.config bft_server.config
CONFIG_PATH=$PWD/poc.conf
sh $PWD/../run_poc.sh $CONFIG_PATH
