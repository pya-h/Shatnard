# Shatnard
There Are Two different variations of the game, Both written in:
# Language: C
With two different approach, two different algorythms, mechanisms, and also gameplay & options
# Variation 1
    Its the game ui & logic direct and simple implementation only, directly starts from the game table and it only manages the gameplay only
    But its code is much less complicated than the 2nd variation

# Variation 2
    Its much better, but complicated; Extra Features:
    * It supports saving and loading games
    * Each game has its own save data, so the game supports multi user use.
    * It has live scores, and has a extra table for showing with pieces are out or in prison or whatever, for both users
    * The game UI can also be restructured from a map file; as long as the map file is correct (in game law sernses),
        the game can be started with different kind of fields and arrangements
    * both are written in console but this variation is much user fiendlier and understandable
    * But the code is much complicated than variation 1, and also much more professional.

Shatnard console game; a simple combination of two famous board games: Chess &amp; backgammon.
Its mostly written considering backgammon rules and gameplay with a few minor changes and some chess rules; e.g. pieces are not all same and they have chess names actually.
winner is the one who moves his/her pieces all out of the board faster.
Gameplay note:
1. each player throws dices with pressing d
2. each dice move contains two strings:
    1. First input string is one of his piece's source address and the second string is the destination
    2. Addressing is quite simple really; Castles are 'C' and Roads are 'R'
    3. Player 1's Castle is C1, his Road is R1, and so is for Player 2: C2, R2
    4. and then comes the room index followed by an Underline '_' e.g. Player 2's 3rd Room in Roads => R2_3 or r2_3
so for example if Player 1 has the dice 4 and wants to move his/her piece in first room in player 2's castle
-> C2_1 C2_5
3. other moving rules are quite like backgammon; and there is more also
4. there are some holes and golds in some of the rooms
5. holes trap your piece so you can not move that piece for 2 turns
6. golds lets you rescue one trapped
7. tnx for reading
