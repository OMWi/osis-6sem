player1="X"
player2="O"

turn=1
is_playing=true

fields=( 1 2 3 4 5 6 7 8 9 )
valid_inputs=( 1 2 3 4 5 6 7 8 9 )

save_filename="save.txt"

print_board() {
  echo "TIC-TAC-TOE"
  echo "Player 1 - ${player1}"
  echo "Player 2 - ${player2}"
  echo ""
  echo " ${fields[0]} | ${fields[1]} | ${fields[2]} "
  echo "-----------"
  echo " ${fields[3]} | ${fields[4]} | ${fields[5]} "
  echo "-----------"
  echo " ${fields[6]} | ${fields[7]} | ${fields[8]} "
  echo ""
}

save_to_file() {
  print_board > $1
}

player_input(){
  echo "Turn ${turn}"
  if [ $((turn % 2)) -eq 1 ]
  then
    current_player=$player1
    echo -n "PLAYER 1 ('${player1}') PICK A FIELD: "
  else
    current_player=$player2
    echo -n "PLAYER 2 ('${player2}') PICK A FIELD: "
  fi


  read input

  if [ $input = save ]
  then
    read save_location_input
    if [ -z save_location_input ]
    then
      save_to_file ${save_filename}
      echo "Game saved to '${save_filename}'"
    else
      save_to_file ${save_location_input}
    fi
    player_input
    return
  fi

  if [ $input = exit ] || [ $input = close ]
  then
    clear
    exit
  fi

  is_valid=false
  for valid_input in  ${valid_inputs[@]}
  do
    if [ $valid_input = $input ]
    then
      echo "valid_input=${valid_input} input=${input}"
      is_valid=true
      break
    fi
  done
  if [ ${fields[$((input-1))]} = ${player1} ] || [ ${fields[$((input-1))]} = ${player2} ]
  then
    echo -n "Already checked. "
    is_valid=false
  fi

  if [ $is_valid = false ]
  then
    echo "Not a valid input"
    player_input
  else
    fields[((input-1))]=$current_player
    ((turn++))
  fi
}

check_row() {
  if [ ${fields[$1]} = ${fields[$2]} ] && [ ${fields[$2]} = ${fields[$3]} ]
  then
    is_playing=false
  fi
  if [ $is_playing = false ] 
  then
    if [ ${fields[$1]} = ${player1} ]
    then
      echo "Player 1 wins!"
      return 
    else
      echo "Player 2 wins!"
      return 
    fi
  fi
}

check_winner(){
  if [ $is_playing = false ]; then return; fi
  check_row 0 1 2
  if [ $is_playing = false ]; then return; fi
  check_row 3 4 5
  if [ $is_playing = false ]; then return; fi
  check_row 6 7 8
  
  if [ $is_playing = false ]; then return; fi
  check_row 0 3 6
  if [ $is_playing = false ]; then return; fi
  check_row 1 4 7
  if [ $is_playing = false ]; then return; fi
  check_row 2 5 8
  if [ $is_playing = false ]; then return; fi

  if [ $is_playing = false ]; then return; fi
  check_row 0 4 8
  if [ $is_playing = false ]; then return; fi
  check_row 2 4 6

  if [ $turn -gt 9 ] && [ $is_playing = true ]
  then 
    is_playing=false
    echo "Its a draw!"
  fi
}

clear
print_board
while [ $is_playing = true ]
do
  player_input
  clear
  print_board
  check_winner
done