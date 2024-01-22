#!/bin/sh

import sys
import time
from datetime import datetime

import oracle_script
from oracle_script.comm.utils import *
from oracle_script.comm.comm_config import *

oracle_script.comm.utils.debug = False
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

def get_min_block(config):
    iplist=get_ips(svr_ip_file)
    count = {}
    commit_count = {}
    mining_time={}
    for (idx,svr_ip) in iplist:
        cmd="grep \'mining time\' /home/ubuntu/pow_server{}.log".format(idx)
        res=run_remote_cmd(svr_ip, cmd)
        if(len(res)):
            l = []
            for s in res:
                seq=s.decode('utf8').split(':')[4:7]
                #print("result:seq{}".format(seq))
                if (seq[0],seq[1]) not in mining_time:
                  mining_time[(seq[0],seq[1])] = seq[2]
    total_time = 0.0
    for (k,v) in mining_time.items():
      total_time = total_time +  float(v)
    print("avg mining time:{} second.".format(total_time/len(mining_time.items())))

    print("=====================")

    avg_time = []
    tps=[]
    for (idx,svr_ip) in iplist:
        cmd="grep \'execute seq:\' /home/ubuntu/pow_server{}.log | grep -v total".format(idx)
        res=run_remote_cmd(svr_ip, cmd)
        if(len(res)):
            delta = 0
            last = 0
            for s in res:
                print("get s:",s.split())
                seq = str(s.decode("utf-8").split()[5][5:-1].strip()).split(',')
                print("seq:",int(seq[0]),int(seq[1]))
                t = str(s.split()[1].strip().decode('utf8'))
                if(len(t.split(':'))<3):
                    print("!!!!! no data")
                    continue
                if(t.split(':')[0]=="23"):
                    t = "2022-04-01 {}".format(t)
                else:
                    t = "2022-04-02 {}".format(t)
                datetime_obj = datetime.strptime(t, "%Y-%m-%d %H:%M:%S.%f")
                stamp = int(time.mktime(datetime_obj.timetuple()) * 1000.0 + datetime_obj.microsecond / 1000.0)
                if last > 0:
                    delta = stamp - last
                    #print("last :{} cur:{}, delta:{}, time:{}, {}-{}".format(last, stamp, delta, t, int(seq[1]), int(seq[0])))
                    tps.append((int(seq[1]) - int(seq[0]))/delta*1000)
                last = stamp
            avg_time.append(delta/(len(res)-1)/1000)
            break
    print("tps:",sum(tps)/len(tps)*100)

def stat(config):
    get_min_block(config)

if __name__ == '__main__':
    config_file=sys.argv[1]
    config = read_config(config_file)

    config_path=os.path.dirname(config_file)
    home = os.path.realpath(config_path+"/../../../")
    parse_config(config)

    #stat(config)
    stat(config)

