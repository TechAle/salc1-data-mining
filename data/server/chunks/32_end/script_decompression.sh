for file in *.mca; do
	echo "$file"
	./unix_decompression $file
	mv $file ../32_end_done
	mv ${file%"."*} ../1_end
done
