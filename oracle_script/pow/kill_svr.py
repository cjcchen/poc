#!/bin/sh

import sys

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
pow_config_path=""
svr_bin_bazel_path=""
bft_config_path=""

def parse_config(config):
    global svr_ip_file, cli_ip_file,svr_bin_bazel_path,cert_dir,svr_config_path,pow_config_path, bft_config_path
    svr_ip_file = config["svr_ip_file"]
    svr_ip_file = config_path+"/"+svr_ip_file

    cli_ip_file = config["cli_ip_file"]
    cli_ip_file = config_path+"/"+cli_ip_file

    pow_config_path = config["pow_config_path"]
    pow_config_path = config_path+"/"+pow_config_path

    bft_config_path = config["bft_config_path"]
    bft_config_path = config_path+"/"+ bft_config_path

    svr_bin_bazel_path = config["svr_bin_bazel_path"]
    svr_bin_bazel_path= home+"/"+ svr_bin_bazel_path

    cert_dir=config_path+"/cert/"


def kill_svr(config):
    run_cmd("sh {}/oracle_script/pow/killall.sh {} {}".format(home, svr_ip_file, KEY))

if __name__ == '__main__':
    config_file=sys.argv[1]
    config = read_config(config_file)

    config_path=os.path.dirname(config_file)
    home = os.path.realpath(config_path+"/../../../")
    parse_config(config)

    kill_svr(config)
