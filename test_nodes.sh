START=5
END=51
 
for (( c=$START; c<=$END; c+=5 ))
do
	killall node
	killall node
	./tester $c
	killall node
	killall node 
done