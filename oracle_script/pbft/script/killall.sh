input=$1
key=$2

count=0

HOSTS=`cat ${input}`
i=0
set -x
for HOSTNAME in ${HOSTS}; do
	if [ $i -eq 1 ]; then
		ssh -i ${key} -n -o BatchMode=yes -o StrictHostKeyChecking=no ubuntu@${HOSTNAME} " killall -9 benchmark_server; killall -9 pbft_server;" 
		i=0
	else
		i=1
	fi
done
