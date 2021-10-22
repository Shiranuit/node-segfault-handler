node test/test.js 2> log.txt
cat log.txt | grep -P 'PID \d+ received a SIGSEGV'