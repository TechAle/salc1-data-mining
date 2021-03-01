here, put co_block.csv
and execute this code:
split -l 10000000 co_block.csv part_block_
for f in * ; do 
  mv "$f" "$f.csv"
done

