echo "Running unit tests:"

for i in matrix_utils/tests/*_test
do
    if test -f $i
    then
        if true ./$i 2>> matrix_utils/tests/tests.log
        then
            echo $i PASS
        else
            echo "ERROR in test $i: here's matrix_utils/tests/tests.log"
            echo "------"
            tail matrix_utils/tests/tests.log
            exit 1
        fi
    fi
done

echo ""
