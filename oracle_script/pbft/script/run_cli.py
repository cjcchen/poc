#!/bin/sh

import sys
import os
from oracle_script.comm.utils import *
from oracle_script.comm.comm_config import *

cert_path="pbft_cert"
config_path=""
home=""
svr_ip_file=""
cli_ip_file=""
svr_bin_file=""
cert_dir=""
svr_config_path=""
cli_config_path=""
cli_start_script=""
cli_bin_bazel_path=""

def parse_config(config):
    global svr_ip_file, cli_ip_file,svr_bin_file,cert_dir,svr_config_path, cli_config_path, cli_start_script,cli_bin_bazel_path
    svr_ip_file = config["svr_ip_file"]
    svr_ip_file = config_path+"/"+svr_ip_file

    cli_ip_file = config["cli_ip_file"]
    cli_ip_file = config_path+"/"+cli_ip_file

    svr_config_path = config["svr_config_path"]
    svr_config_path = config_path+"/"+svr_config_path

    cli_config_path = config["cli_config_path"]
    cli_config_path = config_path+"/"+cli_config_path

    svr_bin_file = config["svr_bin_bazel_path"]
    svr_bin_file = home+"/"+svr_bin_file

    cli_start_script = config["cli_start_script"]
    cli_start_script = home+"/"+cli_start_script

    cli_bin_bazel_path = config["cli_bin_bazel_path"]
    cli_bin_bazel_path = home+"/"+cli_bin_bazel_path

    cert_dir=config_path+"/cert/"

def gen_cli_config(config):
    iplist=get_ips(cli_ip_file)
    with open(cli_config_path,"w") as f:
        for idx,ip in iplist:
            port = int(config["base_port"]) + int(idx)
            f.writelines("{} {} {}\n".format(idx, ip, port))

def upload_cli(config):
    iplist=get_ips(cli_ip_file)

    for (idx,ip) in iplist:
        run_remote_cmd(ip, "rm -rf {}; rm -rf {}, {};".format(
                get_remote_file_name(cli_start_script), 
                get_remote_file_name(cli_config_path),
                get_remote_file_name(cli_bin_bazel_path)))
        run_cmd_with_resp("scp -i {} {} ubuntu@{}:/home/ubuntu".format(KEY,cli_config_path,ip))
        run_cmd_with_resp("scp -i {} {} ubuntu@{}:/home/ubuntu".format(KEY,cli_bin_bazel_path,ip))
        run_cmd_with_resp("scp -i {} {} ubuntu@{}:/home/ubuntu".format(KEY, cli_start_script, ip))

def run_cli(config):
    iplist=get_ips(cli_ip_file)
    cmds=[]
    for (idx,ip) in iplist:
        cmd="sh {} {} {}".format(
                    get_remote_file_name(cli_start_script),
                    get_remote_file_name(cli_bin_bazel_path),
		    get_remote_file_name(cli_config_path)
		    )
        cmds.append((ip,cmd))

    run_remote_cmd_list(cmds)

if __name__ == '__main__':
    config_file=sys.argv[1]
    config = read_config(config_file)

    config_path=os.path.dirname(config_file)
    home = os.path.realpath(config_path+"/../../../")
    print("home:",home)
    parse_config(config)

    print("config:{}".format(config))
    gen_cli_config(config)
    upload_cli(config)
    run_cli(config)
