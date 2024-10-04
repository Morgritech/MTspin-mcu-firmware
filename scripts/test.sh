CORES=arduino-boards.txt

for CORE in $(cat $CORES) ; do
  IFS=':' read -r first second <<< ${CORE%$'\r'}
  CORE="$first:$second" 
  echo $CORE
  IFS=$'\n'
done

echo $CORE