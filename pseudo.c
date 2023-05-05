/*
//globss
dayCounter, hourCounter
foodStorage[3]
zooStorage[3]

enum atype{
	lion
	meerkat
	ostrich
}

struct animal{
	type
	total
}

main(){
	animal lion[4], meerkat[10], ostrich[7]
	vet[2]
	supp

	timer start
	start em all

	if( daycounter > 180 ) {
		stop em all
	}

	print emm all 1 by 1
}

pt_vet(){
	if ( foodStorage[lion] == 0 ) {
		supplyvet( lion, food )
	} else if ( foodStorage[meerkat] == 0 ) {
		supplyvet( meerkat, food )
	} else if ( foodStorage[ostrich] == 0 ) {
		supplyvet( ostrich, food )
	}
}

pt_supp(){
	if ( zooStorage[lion] == 0 ) {
		supplysupp( lion, zoo )
	} else if ( zooStorage[meerkat] == 0 ) {
		supplysupp( meerkat, zoo )
	} else if ( zooStorage[ostrich] == 0 ) {
		supplysupp( ostrich, zoo )
	}
}

pt_animal(){
	daystore = 0
	actedtoday = 0

	if( actedtoday <= 1 ) {
		if ( daystore < daycounter ) {
			actedtoday = 0
			daystore = daycounter
		}
		act( type )
		++actedtoday
	}
}

act() { do it twice a day
	eat( rand.amount )
	showoff()
	eat( rand.amount )
	sleep( rand.amount )
}

eat(){
	lion- meat 2-5
	meerkat- insects compost 1-2
	ostrich- greens 2-4
}

showoff(){
	lion- rawr
	meerkat- iwiwi
	ostrich- AAAAAAA
}

sleep(){
	lion- 8-12
	meerkat- 6-10
	ostrich- 4-8
}

supplyvet(){
	while( foodstorage[animal] != 10 {
		foodstorage[animal] += 1
}

supplysupp(){
	zoostorage[animal] = 30
}

*/