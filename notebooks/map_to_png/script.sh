for file in *.dat; do
	perl map2png.pl $file
	echo $file
done
