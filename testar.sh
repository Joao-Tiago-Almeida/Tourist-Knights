for file in testfiles/*; do
    filename="$(basename "$file")"
    if [ ${file: -7} == ".cities" ]
    then
        
        filename_without_extension="${filename%.*}"
        echo FICHEIRO: $filename_without_extension

        ./tuktuk testfiles/$filename
        diff testfiles/$filename_without_extension.valid testfiles_valid/$filename_without_extension.valid;
    fi
done