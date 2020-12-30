#!/bin/bash

## Modify the above line to point to bash executable in your system

## ROLL NUMBERS OF THE TEAM-MATES ##
#    1. 111801044 (TAUSIF IQBAL)
#    2. 111801020 (ANURAG)
#    3. 111801031 (PAVAN KARTHIK)
#    4. 111801045 (SAI VIPUL MOHAN)
#   for 1M it took: 15s
#   for 10M it took: 1m 3s
#   for 100M it took: 12m 11s
###

## Write your script below this line ##

# #a)
make
# #b)
ENTRIES=100000000
./input_generator $ENTRIES 111801045 [111801044, 111801020, 111801031] > master.csv
# #d)
#CSE COURSE WISE FILE CREATION
grep "CS" master.csv | sort | uniq > tempcs.csv
UNIQ_CS_COURSES=`cut -d',' -f2 tempcs.csv | sort | uniq`
mkdir courses
for COURSE in $UNIQ_CS_COURSES; do
    touch ./courses/"${COURSE}.csv"
    grep "$COURSE" tempcs.csv | cut -d',' -f-1,3- | sort -r | awk -F',' '!seen[$1]++' | sort > ./courses/${COURSE}.csv
done
# echo "CSE COURSE WISE DONE"

#CIVIL COURSE WISE FILE CREATION
grep "CE" master.csv | sort | uniq > tempce.csv
UNIQ_CE_COURSES=`cut -d',' -f2 tempce.csv | sort | uniq`
for COURSE in $UNIQ_CE_COURSES; do
    touch ./courses/"${COURSE}.csv"
    grep "$COURSE" tempce.csv | cut -d',' -f-1,3- | sort -r | awk -F',' '!seen[$1]++' | sort > ./courses/${COURSE}.csv
done
# echo "CE COURSE WISE DONE"

#MECHANICAL COURSE WISE FILE CREATION
grep "ME" master.csv | sort | uniq > tempme.csv
UNIQ_ME_COURSES=`cut -d',' -f2 tempme.csv | sort | uniq`
for COURSE in $UNIQ_ME_COURSES; do
    touch ./courses/"${COURSE}.csv"
    grep "$COURSE" tempme.csv | cut -d',' -f-1,3- | sort -r | awk -F',' '!seen[$1]++' | sort > ./courses/${COURSE}.csv
done
# echo "ME COURSE WISE DONE"

#ELECTRICAL COURSE WISE FILE CREATION
grep "EE" master.csv | sort | uniq > tempee.csv
UNIQ_EE_COURSES=`cut -d',' -f2 tempee.csv | sort | uniq`
for COURSE in $UNIQ_EE_COURSES; do
    touch ./courses/"${COURSE}.csv"
    grep "$COURSE" tempee.csv | cut -d',' -f-1,3- | sort -r | awk -F',' '!seen[$1]++' | sort > ./courses/${COURSE}.csv
done
# echo "EE COURSE WISE DONE"

# #c)
# # CSE BRANCH WISE FILE CREATION
cut -d',' -f1 tempcs.csv | uniq > cs.csv
orderl='0'
orderr=',2.2'
join -t, -a 1 -e'-' -o '0, 2.2' cs.csv ./courses/CS2010.csv > temp.csv
i=1
cat temp.csv > cs.csv
echo -n "Name" > final_cs.csv
for COURSE in $UNIQ_CS_COURSES; do
    echo -n "," >> final_cs.csv
    echo -n "$COURSE" >> final_cs.csv
done
echo >> final_cs.csv
for COURSE in $UNIQ_CS_COURSES; do
    if [ $i -ne 1 ]; then
        join -t, -a 1 -e'-' -o "$orderl$orderr" cs.csv ./courses/${COURSE}.csv > temp.csv
        mv temp.csv cs.csv
    fi
    i=$((i+1))
    orderl="$orderl,1.$i"
done
cat cs.csv >> final_cs.csv
mv final_cs.csv cs.csv
# echo "CSE BRANCH WISE DONE"

# # CIVIL BRANCH WISE FILE CREATION
cut -d',' -f1 tempce.csv | uniq > ce.csv
orderl='0'
orderr=',2.2'
join -t, -a 1 -e'-' -o '0, 2.2' ce.csv ./courses/CE2010.csv > temp.csv
i=1
cat temp.csv > ce.csv
echo -n "Name" > final_ce.csv
for COURSE in $UNIQ_CE_COURSES; do
    echo -n "," >> final_ce.csv
    echo -n "$COURSE" >> final_ce.csv
done
echo >> final_ce.csv
for COURSE in $UNIQ_CE_COURSES; do
    if [ $i -ne 1 ]; then
        join -t, -a 1 -e'-' -o "$orderl$orderr" ce.csv ./courses/${COURSE}.csv > temp.csv
        mv temp.csv ce.csv
    fi
    i=$((i+1))
    orderl="$orderl,1.$i"
done
cat ce.csv >> final_ce.csv
mv final_ce.csv ce.csv
# echo "CE BRANCH WISE DONE"

# # MECHANICAL BRANCH WISE FILE CREATION
cut -d',' -f1 tempme.csv | uniq > me.csv
orderl='0'
orderr=',2.2'
join -t, -a 1 -e'-' -o '0, 2.2' me.csv ./courses/ME2010.csv > temp.csv
i=1
cat temp.csv > me.csv
echo -n "Name" > final_me.csv
for COURSE in $UNIQ_ME_COURSES; do
    echo -n "," >> final_me.csv
    echo -n "$COURSE" >> final_me.csv
done
echo >> final_me.csv
for COURSE in $UNIQ_ME_COURSES; do
    if [ $i -ne 1 ]; then
        join -t, -a 1 -e'-' -o "$orderl$orderr" me.csv ./courses/${COURSE}.csv > temp.csv
        mv temp.csv me.csv
    fi
    i=$((i+1))
    orderl="$orderl,1.$i"
done
cat me.csv >> final_me.csv
mv final_me.csv me.csv
# echo "ME BRANCH WISE DONE"


# # ELECTRICAL BRANCH WISE FILE CREATION
cut -d',' -f1 tempee.csv | sort | uniq > ee.csv
orderl='0'
orderr=',2.2'
join -t, -a 1 -a 2 -e'-' -o '0, 2.2' ee.csv ./courses/EE2010.csv > temp.csv
i=1
cat temp.csv > ee.csv
echo -n "Name" > final_ee.csv
for COURSE in $UNIQ_EE_COURSES; do
    echo -n "," >> final_ee.csv
    echo -n "$COURSE" >> final_ee.csv
done
echo >> final_ee.csv
for COURSE in $UNIQ_EE_COURSES; do
    if [ $i -ne 1 ]; then
        join -t, -a 1 -a 2 -e'-' -o "$orderl$orderr" ee.csv ./courses/${COURSE}.csv > temp.csv
        mv temp.csv ee.csv
    fi
    i=$((i+1))
    orderl="$orderl,1.$i"
done
cat ee.csv >> final_ee.csv
mv final_ee.csv ee.csv
# echo "EE BRANCH WISE DONE"

# CLEANING UP STUFF
rm tempcs.csv
rm tempce.csv
rm tempme.csv
rm tempee.csv
# echo "CLEANING"

