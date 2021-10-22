node ../../test/test.js 2> log.txt
cat log.txt | grep 'PID \d+ received a SIGSEGV'