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


def gen_svr_config(config):
    iplist=get_ips(svr_ip_file)
    with open(pow_config_path,"w") as f:
        for idx,ip in iplist:
            port = int(config["base_port"]) + int(idx)
            f.writelines("{} {} {}\n".format(idx, ip, port))


def kill_svr(config):
    run_cmd("sh {}/oracle_script/pow/killall.sh {} {}".format(home, svr_ip_file, KEY))

def run_svr(config):
    run_cmd("sh {}/oracle_script/pow/run_server.sh {} {}".format(home, svr_ip_file, KEY))

def upload_svr(config):
    iplist=get_ips(svr_ip_file)
    rm_cmd=[]
    scp_cmd=[]

    for (idx,svr_ip) in iplist:
        private_key=cert_dir+"/"+ "node_"+idx+".key.pri"
        cert=cert_dir+ "/"+ "cert_"+idx+".cert"
        rm_cmd.append("\"ssh -i {} -n -o BatchMode=yes -o StrictHostKeyChecking=no ubuntu@{} \'rm -rf {}; rm -rf server.config; rm -rf pow_*; mkdir -p pow_cert;\' \"".format(KEY,svr_ip,get_remote_file_name(svr_bin_bazel_path)))
        scp_cmd.append("\"scp -i {} {} ubuntu@{}:/home/ubuntu\"".format(KEY,svr_bin_bazel_path,svr_ip))
        scp_cmd.append("\"scp -i {} {} ubuntu@{}:/home/ubuntu\"".format(KEY,bft_config_path,svr_ip))
        scp_cmd.append("\"scp -i {} {} ubuntu@{}:/home/ubuntu\"".format(KEY,pow_config_path,svr_ip))
        scp_cmd.append("\"scp -i {} {} ubuntu@{}:/home/ubuntu/pow_cert\"".format(KEY,private_key, svr_ip))
        scp_cmd.append("\"scp -i {} {} ubuntu@{}:/home/ubuntu/pow_cert\"".format(KEY,cert, svr_ip))

    #cmd_shell = "/home/ubuntu/nexres/oracle_script/pow/shell_cmd.sh"
    cmd_shell = "{}/oracle_script/pbft/script/shell_cmd.sh".format(home)
    resp = run_cmd("sh {} {}".format(cmd_shell, ' '.join(rm_cmd)))
    resp = run_cmd("sh {} {}".format(cmd_shell, ' '.join(scp_cmd)))

if __name__ == '__main__':
    config_file=sys.argv[1]
    config = read_config(config_file)

    config_path=os.path.dirname(config_file)
    home = os.path.realpath(config_path+"/../../../")
    print("home:",home)
    parse_config(config)

    print("config:{}".format(config))
    gen_svr_config(config)
    kill_svr(config)
    upload_svr(config)
    run_svr(config)
