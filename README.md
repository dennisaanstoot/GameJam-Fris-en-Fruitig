# GameJam-Fris-en-Fruitig

##Client-server protocol
###Setup
Client: connect [name]

Server: ok

Server: start [width] [height] [player_amount] [[entities]]

###Gameplay
Client: input [button] [x] [y]

Server: frame [frame]

####[frame]
[entity_type] [x] [y] [angle] [health] [name]

##Entity types
###Player
###Tree
###Bullet
