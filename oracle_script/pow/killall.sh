input=$1
key=$2

count=0

set -x
HOSTS=`cat ${input}`
i=0
for HOSTNAME in ${HOSTS}; do
	if [ $i -eq 1 ]; then
		ssh -i ${key} -n -o BatchMode=yes -o StrictHostKeyChecking=no ubuntu@${HOSTNAME} " killall -9 pow_server; " &
		i=0
	else
		i=1
	fi
	count=`expr $count + 1`
done

while [ $count -gt 0 ]; do
	wait $pids
	count=`expr $count - 1`
done
