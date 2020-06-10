START=1
END=8
 
for (( c=$START; c<=$END; c++ ))
do
	./tester c
	killall node
	killall node 
done