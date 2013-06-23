for i in {1..184..1}
do
	read id
	url="http://www.whereincity.com/india/trains/time-table/$id.htm"
	echo $url
	filename="./trains/temp.html"
	curl -U indram:gambler21@indra -x ironport2.iitk.ac.in:3128 --location $url --insecure -o $filename
	./schedule ./trains/$id < $filename
done
