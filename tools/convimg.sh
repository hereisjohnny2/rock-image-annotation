#1/bin/bash

echo 'Converting Images'

for f in $1/**/*.tif ; do
  echo "Converting $f"
  convert "$f" "${f%.tif}-converted.png" ;
done

ls $1

