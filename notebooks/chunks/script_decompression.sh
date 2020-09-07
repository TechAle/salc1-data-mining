for file in *.mca; do
	echo "$file"
	./unix_decompression $file
	mv $file ../32_nether_done
	mv ${file%"."*} ../1_nether
done
