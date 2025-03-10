#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Finding comments in dsh_cli.c" {
    run "./dsh" <<EOF                
cat ./dsh_cli.c | grep // | wc -l
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="0dsh3>dsh3>cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Finding comments in dshlib.c with spaces" {
    run "./dsh" <<EOF                
cat                      ./dshlib.c |           grep       // |                       wc -l
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="11dsh3>dsh3>cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Testing echo" {
    run "./dsh" <<EOF                
   echo "   hello world" 
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')
    # Expected output with all whitespace removed for easier matching
    expected_output="   hello worlddsh3> dsh3> cmd loop returned 0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]
}

@test "Searching for quoted words" {
    run "./dsh" <<EOF                
    cat ./dshlib.c | grep "for searching comments" | wc -l
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')
    # Expected output with all whitespace removed for easier matching
    expected_output="5dsh3> dsh3> cmd loop returned 0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]
}

@test "Using multiple parameters for grep" {
    run "./dsh" <<EOF                
    cat ./dshlib.c | grep -o "for searching comments"
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')
    # Expected output with all whitespace removed for easier matching
    expected_output="for searching commentsfor searching commentsfor searching commentsfor searching commentsfor searching commentsdsh3> dsh3> cmd loop returned 0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]
}

@test "Testing echo with space in front" {
    run "./dsh" <<EOF                
                 echo "               Hello           World" 
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')
    # Expected output with all whitespace removed for easier matching
    expected_output="               Hello           Worlddsh3> dsh3> cmd loop returned 0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]
}

@test "Testing more than 8 commands" {
    run "./dsh" <<EOF                
                echo "a" | echo "a" | echo "a" | echo "a" | echo "a" | echo "a" | echo "a" | echo "a" | echo "a"
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')
    # Expected output with all whitespace removed for easier matching
    expected_output="dsh3> error: piping limited to 8 commandsdsh3> cmd loop returned 0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]
}

@test "Testing exactly 8 commands" {
    run "./dsh" <<EOF                
                echo "a" | echo "a" | echo "a" | echo "a" | echo "a" | echo "a" | echo "a" | echo "a"
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')
    # Expected output with all whitespace removed for easier matching
    expected_output="adsh3> dsh3> cmd loop returned 0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]
}


