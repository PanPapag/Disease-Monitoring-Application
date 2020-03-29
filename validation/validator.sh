#! /bin/bash

GREEN="\033[32m"
RED="\033[31m"
RESET="\033[0m"
USAGE="Usage: validator.sh 'inputFile' 'ht1Entries' 'ht2Entries' 'bucketSize'"

EXECUTABLE="./diseaseMonitor"
OUTPUTFILE="out$$.txt"
LOG="myMakefileLog.txt"

function good() { echo -e "$GREEN [ PASS ]  $RESET"; }
function bad()  { echo -e "$RED [ FAIL ] $RESET"; exit 1; }
function testresult() { echo -ne "# [RESULT]\t$2 " ; if [ $1 -eq 0 ] ; then good ; else bad ; fi ; }


if [ $# -ne 4 ]; then
    echo ${USAGE} ;
    exit 1
fi

# parameters
# the file with the data
INPUTFILE=$1
HT1_ENTRIES=$2
HT2_ENTRIES=$3
BUCKETSIZE=$4

# startup
echo "# Starting tests ..."
echo "# [INF] Creating/Truncating log file $LOG ..."
cat /dev/null > $LOG
testresult $? "Created log"

echo "# [INF] Please check log file for what errors in make"

# Tests
echo "# [TEST] Testing whether make works ..."
make &> $LOG
testresult $? "Make works"


echo "# [INF] Moving on to data tests ... "
echo "# [INF] Output of the latest test will be in ${OUTPUTFILE}"
for t in *_input_command_RND10244096.cmd; do 
  echo "# [INF] Running test $t ... ";
  # main file name
  a=`echo $t | cut -d '.' -f 1`;
  r="$a.result"

  echo "# [INF] Checking number of results expected in $r ... ";
  num=`wc -l < $a.result`;
  echo "# [INF] Expecting $num lines. ";
  # give some slack for any messages at the beginning/end - although there shouldn't be any
  num=$(($num+2)) 
  
  echo "# [TEST] Executing with $t ...";
  cat $t | ${EXECUTABLE} -p ${INPUTFILE} -h1 ${HT1_ENTRIES} -h2 ${HT2_ENTRIES} -b ${BUCKETSIZE}| head -${num} | awk '{print tolower($0);}' &> ${OUTPUTFILE}
  testresult $? "execution"

  while read p ; do
  echo "# [TEST] Testing whether line $p exists in output ..." ;
    grep -q -i "$p" ${OUTPUTFILE} 
    testresult $? "line exists"
  done < $r

done;


#echo "# [INF] Removing ${OUTPUTFILE} ..."
#rm ${OUTPUTFILE}
echo -n "# [INF] DONE! COMPLETE AUTOMATED TEST " 
good
