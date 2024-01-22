#!/bin/sh

import sys

from oracle_script.comm.utils import *
from oracle_script.comm.comm_config import *

config_path=""
home=""

def get_data(config):
    svr_ip_file = config["svr_ip_file"]
    svr_ip_file = config_path+"/"+svr_ip_file
    iplist=get_ips(svr_ip_file)
    tot = [] 
    txn = 0
    for (idx,svr_ip) in iplist:
        cmd="grep \'total request:\' /home/ubuntu/server{}.log | grep -v 'total request:0' ".format(idx)
        res=run_remote_cmd(svr_ip, cmd)
        for r in res:
          r=r.decode('utf8')
          try:
            data = r.split()[-4].split(":")[1]
            if int(data) > txn:
              txn = int(data)
            tot.append(int(data))
          except:
            continue
    print(" ========= results  ========= ")
    print(" max tps:",txn, "avg tps:",sum(tot)/len(tot))
if __name__ == '__main__':
    config_file=sys.argv[1]
    config = read_config(config_file)
    
    config_path=os.path.dirname(config_file)
    home = os.path.realpath(config_path+"/../../../")

    get_data(config)

