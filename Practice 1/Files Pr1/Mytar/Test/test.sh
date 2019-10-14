#! /bin/bash

if test -x mytar; then

    if test -d tmp; then
        rm -r tmp
    fi
    
    mkdir tmp
    cd tmp

    echo "Hello wold!"          > file1.txt
    head -10 /etc/passwd        > file2.txt
    head -c 1024 /dev/urandom   > file3.data

    ../mytar -cf filetar.mtar file1.txt file2.txt file3.data

    mkdir out
    cp filetar.mtar out/

    cd out
    ../../mytar -xf filetar.mtar

    if !(diff file1.txt  ../file1.txt) || !(diff file2.txt ../file2.txt) || !(diff file3.data ../file3.data); then
        echo "Test failed, there are differences in some files"
    else
        cd ../..
        echo "Test correct" 
    fi

else
    echo "\"mytar\" file doesn't exists in the folder or is not runnable"
    return 0
fi



