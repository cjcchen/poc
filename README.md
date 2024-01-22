In this document, we explain how to run the PoC codebase and produce the performance results.

# Steps to Setup


Setup code and machine
The code is based on the release of version [NexRes](https://github.com/resilientdb/resilientdb).


## Install dependences.

    ./INSTALL.sh

Note: All developement and experimentation was done on Ubuntu 20.04.

# Steps to deploy and run performance

## PBFT

Before deploying binaries, we need to build tools to generate certificates:

	bazel build //tools:certificate_tools

put the ssh key path to oracle_script/comm/comm_config.py and ensure it contains the right mode (e.g. 0600).

Prepare the machines you want to run the experimance and place the ip address in the iplist file.

1. go to oracle_script

	cd oracle_script

2. Add the ipaddress to iplist.txt
3. Run script to generate certificates
	
	./generate_pbft_config.sh

Once the certificates are generated, place the outputs to the deploy folder
	
	cp *.txt pbft/deploy/
	cp cert/* pbft/deploy/cert/

## Deploy the PBFT service

	cd pbft/deploy
	./run_svr.sh

## Obtain the performance of PBFT service

	./run_cli.sh


PoC contains two parts of machines, one is for pbft and the other is for PoW.

## Pbft
Go into one of the folder under pbft, like [pbft/rep_32](https://github.com/cjcchen/poc/tree/main/oracle_script/pbft/rep_32), put the certificates you have generated into the cert folder. Also put the iplist.txt, server.config, client.config generated above to this folder.

Run the script to deploy the server to the machines list in the iplist.txt.

    sh run_svr.sh

## PoW
This is the same as deploying Pbft.

Go into one of the folder under pbft, like [pow/rep_32](https://github.com/cjcchen/poc/tree/main/oracle_script/pow/rep_32), put the certificates you have generated into the cert folder. Also put the iplist.txt, server.config, client.config generated above to this folder.

Run the script to deploy the server to the machines list in the iplist.txt.

    sh run_poc.sh

# Produce the performance
Run the script under [pow folder](https://github.com/cjcchen/poc/blob/main/oracle_script/pow/):

    sh run_cli.sh

This will trigger the client to keep sending transactions to Pbft instances. Then PoW instances will keep fetching the transactions from Pbft instances.

# Get the results
Run the python script under [pow folder](https://github.com/cjcchen/poc/blob/main/oracle_script/pow/):

    python benchmark_result.py
  
Then you will see the mining time, commited blocks of each instance, and the total tps among all instances.
