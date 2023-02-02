#set -x
iplist=(
10.2.0.64
10.2.0.162
10.2.0.115
10.2.0.47
10.2.0.7
10.2.0.138
10.2.0.169
10.2.0.27
10.2.0.235
10.2.0.30
10.2.0.71
10.2.0.215
10.2.0.46
10.2.0.213
10.2.0.22
10.2.0.156
10.2.0.78
10.2.0.81
10.2.0.49
10.2.0.167
10.2.0.77
10.2.0.229
10.2.0.66
10.2.0.93
10.2.0.183
10.2.0.94
10.2.0.246
10.2.0.252
10.2.0.119
10.2.0.62
10.2.0.225
10.2.0.148
10.2.0.128
10.2.0.189
10.2.0.21
10.2.0.197
10.2.0.206
10.2.0.249
10.2.0.122
10.2.0.218
10.2.0.106
10.2.0.82
10.2.0.90
10.2.0.72
10.2.0.201
10.2.0.8
10.2.0.227
10.2.0.65
10.2.0.17
10.2.0.145
10.2.0.36
10.2.0.236
10.2.0.163
10.2.0.208
10.2.0.28
10.2.0.141
10.2.0.73
10.2.0.240
10.2.0.58
10.2.0.129
10.2.0.98
10.2.0.54
10.2.0.173
10.2.0.52
10.2.0.186
10.2.0.204
10.2.0.166
10.2.0.210
10.2.0.34
10.2.0.190
10.2.0.237
10.2.0.114
10.2.0.10
10.2.0.68
10.2.0.112
10.2.0.221
10.2.0.95
10.2.0.220
10.2.0.242
10.2.0.130
10.2.0.248
10.2.0.174
10.2.0.230
10.2.0.182
10.2.0.187
10.2.0.111
10.2.0.133
10.2.0.39
10.2.0.20
10.2.0.159
10.2.0.99
10.2.0.84
10.2.0.2
10.2.0.241
10.2.0.83
10.2.0.172
10.2.0.132
10.2.0.121
10.2.0.143
10.2.0.24
10.2.0.179
10.2.0.60
10.2.0.87
10.2.0.232
10.2.0.253
10.2.0.228
10.2.0.55
10.2.0.195
10.2.0.131
10.2.0.250
10.2.0.50
10.2.0.91
10.2.0.48
10.2.0.51
)

count=0
for ip in ${iplist[@]}
do
	ssh -i ~/.ssh/ssh-2022-03-24.key -n -o BatchMode=yes -o StrictHostKeyChecking=no ubuntu@${ip} "cd /home/ubuntu; grep 'execute seq' pow_server*.log | grep lat"
        ((count++))
done
