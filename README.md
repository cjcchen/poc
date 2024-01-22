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

4. Add the ipaddress to iplist.txt
5. Run script to generate certificates
	
		./generate_pbft_config.sh

Once the certificates are generated, place the outputs to the deploy folder
	
	cp *.txt pbft/deploy/
	cp cert/* pbft/deploy/cert/

## Deploy the PBFT service

	cd pbft/deploy
	./run_svr.sh

## Obtain the performance of PBFT service

	./run_cli.sh

It will take 1 minute to evaluate the performance before it stops.

## Obtain the results
Run get_result.sh to obtain the max and average TPS.

	./get_result.sh


## PoW

1. go to oracle_script

		cd oracle_script

2. Add the ipaddress to iplist.txt
3. Run script to generate certificates
	
		./generate_pow_config.sh

Once the certificates are generated, place the outputs to the deploy folder
	
	cp *.txt pow/deploy/
	cp cert/* pow/deploy/cert/

Run the script to deploy the server to the machines list in the iplist.txt.

    sh run_svr.sh

## Deploy the PoW service

	cd pow/deploy
	./run_poc.sh

It will take 1 minute to evaluate the performance before it stops.

## Obtain the performance of PoW service

	./get_result.sh

