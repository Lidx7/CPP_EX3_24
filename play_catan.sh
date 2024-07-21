#!/bin/bash

# Compile the game
make

# Check if compilation was successful
if [ $? -ne 0 ]; then
    echo "Compilation failed. Exiting."
    exit 1
fi

# Run the game in auto mode
./catan auto

# Count the number of times each player won
player1_wins=0
player2_wins=0
player3_wins=0

# Play the game 100 times
for i in {1..100}
do
    echo "Playing game $i"
    output=$(./catan auto)
    echo "$output"
    
    if [[ $output == *"Player 1 wins"* ]]; then
        ((player1_wins++))
    elif [[ $output == *"Player 2 wins"* ]]; then
        ((player2_wins++))
    elif [[ $output == *"Player 3 wins"* ]]; then
        ((player3_wins++))
    fi
done

# Print the results
echo "Results after 100 games:"
echo "Player 1 won $player1_wins times"
echo "Player 2 won $player2_wins times"
echo "Player 3 won $player3_wins times"