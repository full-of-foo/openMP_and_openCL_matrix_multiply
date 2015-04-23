echo "Running unit tests:"

for i in */tests/*_test
do
    if test -f $i
    then
        if ./$i 2>> tests.log
        then
            echo $i PASS
        else
            echo "*************************************************"
            echo "Here's tests.log:"
            echo "*************************************************"
            echo ""
            tail tests.log
            exit 1
        fi
    fi
done

echo ""
