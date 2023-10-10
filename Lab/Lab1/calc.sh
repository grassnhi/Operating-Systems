#!/bin/bash

MAX_HISTORY=5
ANS=0
declare -a history

display_hist() {
    for i in "${history[@]}"
    do
        if [ -n "$i" ]
        then
            echo "$i"
        fi
    done
}

while true
do
    read -p ">> " a op b

    if [ "$a" == "EXIT" ]
    then
        exit 0
    fi

    if [ "$a" == "HIST" ]
    then
        display_hist
        continue
    fi

    if [[ "$op" != '+' && "$op" != '-' && "$op" != '*' && "$op" != '/' && "$op" != '%' ]]
    then
        echo "SYNTAX ERROR"
        continue
    fi

    if [ "$a" == "ANS" ]
    then
        num1="$ANS"
    else
        num1="$a"
    fi

    if [ "$b" == "ANS" ]
    then
        num2="$ANS"
    else
        num2="$b"
    fi

    result=0

    case "$op" in
        "+")
            result=$(echo "scale = 2; $num1 + $num2" | bc)
            ;;
        "-")
            result=$(echo "scale = 2; $num1 - $num2" | bc)
            ;;
        "*")
            result=$(echo "scale = 2; $num1 * $num2" | bc)
            ;;
        "/")
            if [ "$num2" == 0 ]
            then
                echo "MATH ERROR"
                continue
            else
                result=$(echo "scale = 2; $num1 / $num2" | bc)
            fi
            ;;
        "%")
            if [ "$num2" == 0 ]
            then
                echo "MATH ERROR"
                continue
            else
                result=$(echo "$num1 % $num2" | bc)
            fi
            ;;
    esac

    history=("$(echo "$a $op $b = $result")" "${history[@]}")

    if [ ${#history[@]} -gt $MAX_HISTORY ]
    then
        unset 'history[${#history[@]}-1]'
    fi

    ANS="$result"
    echo "$result"
done
