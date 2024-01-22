#!/bin/sh

import sys
import time

from oracle_script.comm.utils import *
from oracle_script.comm.comm_config import *

cert_path="pbft_cert/"
config_path=""
home=""
svr_ip_file=""
cli_ip_file=""
svr_bin_file=""
cert_dir=""
svr_config_path=""

def parse_config(config):
    global svr_ip_file, cli_ip_file,svr_bin_file,cert_dir,svr_config_path
    svr_ip_file = config["svr_ip_file"]
    svr_ip_file = config_path+"/"+svr_ip_file

    cli_ip_file = config["cli_ip_file"]
    cli_ip_file = config_path+"/"+cli_ip_file


def kill_svr(config):
    run_cmd("sh {}/oracle_script/pbft/script/killall.sh {} {}".format(home, svr_ip_file, KEY))
    run_cmd("sh {}/oracle_script/pbft/script/killall.sh {} {}".format(home, cli_ip_file, KEY))
    print("kill done")
    time.sleep(5)

if __name__ == '__main__':
    config_file=sys.argv[1]
    config = read_config(config_file)
    config_path=os.path.dirname(config_file)
    home = os.path.realpath(config_path+"/../../../")
    print("home:",home)
    parse_config(config)

    #nonlocal config_path

    
    print("config:{}".format(config))
    kill_svr(config)
