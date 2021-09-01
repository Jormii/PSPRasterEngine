OBJ_FILES=$(find . -name \*.o)
for OBJ in $OBJ_FILES; do
    rm $OBJ
done
