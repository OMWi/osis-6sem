file_name="stats.txt"
echo "Current session stats:" > ${file_name}
echo "username = `whoami`" >> ${file_name}
echo "time = `date "+%H:%M:%S"`" >> ${file_name}
echo "date = `date "+%d.%m.%Y"`" >> ${file_name}
echo "working directory = '`pwd`'" >> ${file_name}
echo "proccesses number = `ps -e | wc -l`" >> ${file_name}
echo "uptime = `uptime -p` since `uptime -s`" >> ${file_name}