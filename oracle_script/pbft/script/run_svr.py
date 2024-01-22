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

    svr_config_path = config["svr_config_path"]
    svr_config_path = config_path+"/"+svr_config_path

    svr_bin_file = config["svr_bin_bazel_path"]
    svr_bin_file = home+"/"+svr_bin_file

    cert_dir=config_path+"/cert/"

def gen_svr_config(config):
    svr_config = config["svr_config_path"]
    svr_config = config_path+"/"+svr_config

    print("ip file:",svr_ip_file, svr_config)
    iplist=get_ips(svr_ip_file)
    with open(svr_config,"w") as f:
        for idx,ip in iplist:
            port = int(config["base_port"]) + int(idx)
            f.writelines("{} {} {}\n".format(idx, ip, port))


def kill_svr(config):
    run_cmd("sh {}/oracle_script/pbft/script/killall.sh {} {}".format(home, svr_ip_file, KEY))
    run_cmd("sh {}/oracle_script/pbft/script/killall.sh {} {}".format(home, cli_ip_file, KEY))
    print("kill done")
    time.sleep(5)

def run_svr(config):
    iplist=get_ips(svr_ip_file)
    cli_iplist=get_ips(cli_ip_file)
    run_cmd("sh {}/oracle_script/pbft/script/run_server.sh {} {}".format(home, svr_ip_file, KEY))
    for (idx,svr_ip) in iplist+cli_iplist:
        private_key="{}/node_{}.key.pri".format(cert_path,idx)
        cert="{}/cert_{}.cert".format(cert_path,idx)
        if [idx,svr_ip] in iplist:
            continue
        else:
            cmd="nohup {} {} {} {} > client{}.log 2>&1 &".format(
		    get_remote_file_name(svr_bin_file),
		    get_remote_file_name(config["svr_config_path"]), 
		    private_key, 
		    cert, idx)
            print("cmd:",cmd)
        run_remote_cmd(svr_ip, cmd)

def upload_svr(config):
    iplist=get_ips(svr_ip_file) + get_ips(cli_ip_file)
    rm_cmd=[]
    scp_cmd=[]
    cmd_list=[]
    for (idx,svr_ip) in iplist:
        private_key=cert_dir+"/"+ "node_"+idx+".key.pri"
        cert=cert_dir+ "/"+ "cert_"+idx+".cert"

        rm_cmd.append("\"ssh -i {} -n -o BatchMode=yes -o StrictHostKeyChecking=no ubuntu@{} \'rm -rf {}; rm -rf server.config; rm -rf cert; mkdir -p {};\' \"".format(KEY,svr_ip,get_remote_file_name(svr_bin_file), cert_path))

        scp_cmd.append("\"scp -i {} {} ubuntu@{}:/home/ubuntu\"".format(KEY,svr_bin_file,svr_ip))
        scp_cmd.append("\"scp -i {} {} ubuntu@{}:/home/ubuntu\"".format(KEY,svr_config_path,svr_ip))
        scp_cmd.append("\"scp -i {} {} ubuntu@{}:/home/ubuntu/{}\"".format(KEY,private_key, svr_ip, cert_path))
        scp_cmd.append("\"scp -i {} {} ubuntu@{}:/home/ubuntu/{}\"".format(KEY,cert, svr_ip, cert_path))

    #run_remote_cmd_list_raw(cmd_list)
    cmd_shell = "{}/oracle_script/pbft/script/shell_cmd.sh".format(home)
    resp = run_cmd("sh {} {}".format(cmd_shell, ' '.join(rm_cmd)))
    resp = run_cmd("sh {} {}".format(cmd_shell, ' '.join(scp_cmd)))
    print("upload done")

if __name__ == '__main__':
    config_file=sys.argv[1]
    config = read_config(config_file)
    config_path=os.path.dirname(config_file)
    home = os.path.realpath(config_path+"/../../../")
    print("home:",home)
    parse_config(config)

    #nonlocal config_path

    
    print("config:{}".format(config))
    gen_svr_config(config)
    kill_svr(config)
    upload_svr(config)
    run_svr(config)
