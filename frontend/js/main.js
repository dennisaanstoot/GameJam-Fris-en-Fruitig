$(document).ready(function()
{
	$(window).bind("resize", function() {
		$("canvas").width(window.innerWidth);
		$("canvas").height(window.innerHeight);
	});

	// turn off right click menu
	window.addEventListener("contextmenu", function(e) {
        e.preventDefault();
        return false;
    });

    // variables constructions / initializations
	var name, renderer, map_width, map_height, game_name, player_ids;
	var stage = new PIXI.Stage(0xff0000); // has to be in main scope for callbacks to work
	var map = new PIXI.DisplayObjectContainer(); // see above
	var waiting_screen = new PIXI.Text("Waiting for other players",{font: 'bold 36px Georgia', fill: 'white'}); // see above
	var game_width = 1200;
	var game_height = 1100;
	var socket = new WebSocket('ws://130.89.231.61:9000','fris_en_fruitig');
	var player_size = 50;
	var tree_size = 100;
	var bullet_size = 50;
    var powerup_size = 40;
    var player_color;

    var player_id = 0;
    var player_ids = [];

    var player_health;

	// textures
	var player_texture = PIXI.Texture.fromImage("sprites/soldier.png");
	var tree_texture = PIXI.Texture.fromImage("sprites/tree_brown_1.png");
	var bullet_texture = PIXI.Texture.fromImage("sprites/fireball.png");
    var hotdog_texture = PIXI.Texture.fromImage("sprites/hotdog.png");
    var health_pill_texture = PIXI.Texture.fromImage("sprites/health_pill.png");

    var player_right = PIXI.Texture.fromImage("sprites/right.png");
    var player_front = PIXI.Texture.fromImage("sprites/down.png");
    var player_left = PIXI.Texture.fromImage("sprites/left.png");
    var player_back = PIXI.Texture.fromImage("sprites/top.png");

    var explosionTextures = [];
    //for (var i=0; i < 26; i++)
    //{
    //    var texture = PIXI.Texture.fromFrame("Explosion_Sequence_A " + (i+1) + ".png");
    //    explosionTextures.push(texture);
    //};


	// Write down the game name
	var game_name = new PIXI.Text("Fris en dodelijk",{font: 'bold 36px Georgia', fill: 'white'});
	game_name.anchor.x = game_name.anchor.y = 0.5;
	game_name.position.x = game_width/2;
	game_name.position.y = 15;

	// attempt connecting to server with name

	$("#login-form").submit(function(event)
	{
		event.preventDefault();
		name = $("#username").val();
        player_color = $("#color").val();
		send_to_server("connect "+name);
		$("#login-form").remove();
	});

	socket.onopen = function() {
        $("#server-online").show();
	};

	socket.onerror = function() {
        $("#server-offline").show();
	};

	socket.onclose = function() {
        $("#server-offline").show();
	};



	var message;
	socket.onmessage = function(event) {
		//console.log(event);
		message = event.data;
		var index_keyword = message.indexOf(" ");
		var keyword = message.substr(0,index_keyword);
		//console.log("Receive: "+message);
		switch (keyword)
		{
			case "ok":
				wait_screen();
                player_id = parseInt(message.substr(index_keyword, message.length+1))+1;
                console.log(message)
				break;

			case "start":
				start_game(message.substr(index_keyword,message.length+1).split(" "));
				break;

			case "frame":
				draw_frame(message.substr(index_keyword,message.length+1));
				break;

            case "gameover":
                console.log("Receive: "+message);
                game_over(message.substr(index_keyword,message.length+1));
                break;

            case "kill":
            	console.log("Receive: "+message);
		}
	}

	function wait_screen() {
		waiting_screen.anchor.x = waiting_screen.anchor.y = 0.5;
		waiting_screen.position.x = game_width/2;
		waiting_screen.position.y = game_height/2;
		stage.addChild(waiting_screen);
	}

	function start_game(message) {
		map.removeChild(waiting_screen);
		map_width = message[1];
		map_height = message[2];
		player_ids = new Array(parseInt(message[3]));

		$("body").css('background-color', '#000');
		renderer = PIXI.autoDetectRenderer(game_width, game_height);

		var lava = PIXI.Texture.fromImage("sprites/lava_new.png");
		var game_background = new PIXI.TilingSprite(lava);
		game_background.width = game_width;
		game_background.height = game_height;
		stage.addChild(game_background);

		//playing screen
		map.width = map_width;
		map.height = map_height;
		map.position.x = ((game_width/2)-(map_width)/2);
		map.position.y = ((game_height/2)-(map_height)/2);
		stage.addChild(map);

		//playing screen background
		var floor = PIXI.Texture.fromImage("sprites/floor_new.png");
		var background = new PIXI.Sprite(floor);
		background.width = map_width;
		background.height = map_height;
		map.addChild(background);

		var player_amount_text = new PIXI.Text("Players: "+parseInt(message[3]), {font: 'normal 12px Georgia', fill: 'white'});
		player_amount_text.anchor.x = player_amount_text.anchor.y = 1;
		player_amount_text.position.x = map_width-5;
		player_amount_text.position.y = map_height-15;
		map.addChild(player_amount_text);

		map.addChild(game_name);

		document.body.appendChild(renderer.view);
		draw();
		$("canvas").width(window.innerWidth);
		$("canvas").height(window.innerHeight);
	}

	function draw_frame(message) {
		//console.log(message);
		var entities = JSON.parse(message);
		var living_players = [];
		map.removeChildren(1);
        var num_players = 1;
		for (var i = 0; i < entities.length; i++)
		{
			var entity = entities[i];
			switch(entity.entity_type)
			{
				case "player":
                    var pl_sprt = null;
                    if(entity.angle <= 1/4*Math.PI && entity.angle > (1+3/4)*Math.PI) {
                        pl_sprt = player_left;
                    } else if(entity.angle > 1/4*Math.PI && entity.angle <= (3/4)*Math.PI) {
                        pl_sprt = player_back;
                    } else if(entity.angle > 3/4*Math.PI && entity.angle <= (1+1/4)*Math.PI) {
                        pl_sprt = player_right;
                    } else {
                        pl_sprt = player_front;
                    }
					var player = new PIXI.Sprite(pl_sprt);
					living_players.push(player.id);

					player.tint = 1/4 * player_id / num_players * 4 * parseInt('0x'+player_color);
                    player.anchor.x = player.anchor.y = 0.5;
					player.width = player.height = player_size;
					player.position.x = entity.x;
					player.position.y = entity.y;
					//player.rotation = entity.angle - 0.5*Math.PI;
					map.addChild(player);
                    if(num_players == player_id) {
                        player_health = draw_health(entity.health);
                        map.addChild(player_health);
                    }
                    player_ids[player.id] = player.position;
                    num_players++;
					break;

				case "tree":
					var tree = new PIXI.Sprite(tree_texture);
					tree.anchor.x = tree.anchor.y = 0.5;
					tree.position.x = entity.x;
					tree.position.y = entity.y;
					tree.width = tree.height = tree_size;
					map.addChild(tree);
					break;

				case "bullet":
					var bullet = new PIXI.Sprite(bullet_texture);
					bullet.anchor.x = bullet.anchor.y = 0.5;
					bullet.width = bullet.height = bullet_size;
					bullet.position.x = entity.x;
					bullet.position.y = entity.y;
					bullet.rotation = entity.angle - 0.5*Math.PI;
					map.addChild(bullet);
					break;

                case "health_crate":
                    var health_crate = new PIXI.Sprite(health_pill_texture);
                    health_crate.anchor.x = health_crate.anchor.y = 0.5;
                    health_crate.width = health_crate.height = powerup_size;
                    health_crate.position.x = entity.x;
                    health_crate.position.y = entity.y;
                    health_crate.rotation = 0;
                    map.addChild(health_crate);
                    break;

			}
		}
		for(var i in player_ids) {
			if(!$.inArray(i, living_players) && player_ids[i] != null) {
				explosion(player_ids[i]);
				player_ids[i] = null;
			}
		}
	}

    function game_over(winner) {
        console.log(winner);
        var title = new PIXI.Text("GAME OVER!", {align: "center", font: 'bold 36px Georgia', fill: 'white'});
        var text = new PIXI.Text("The winner is: "+winner, {align: "left", font: 'bold 24px Georgia', fill: 'white'});
        title.anchor.x = title.anchor.x = 1;
        title.position.x = game_width / 2;
        title.position.y = 10;
        text.anchor.x = text.anchor.x = 1;
        text.position.x = game_width / 2;
        text.position.y = 46;
        map.addChild(title);
        map.addChild(text);
    }

    function draw_health(health) {
        var graphics = new PIXI.Graphics();
        graphics.clear();
        graphics.lineStyle(5, 0x000000, 1);
        var width;
        health = health/10;
        if (health<100)
        {
        	width = 100;
        }
        else
        {
        	width = health;
        }
        graphics.drawRect(30, map_height-30, width, 30);

        graphics.lineStyle(0, 0x000000, 1);
        graphics.beginFill(0xff0000, 1);
        graphics.drawRect(30, map_height-30, health, 30);
        graphics.endFill();

        return graphics;
    }

	function draw() {
		requestAnimFrame(draw);
		renderer.render(stage);
	}

	var click_timer = 0;
	var timeout;
	stage.tap = function(event) {
		if (click_timer == 0)
		{
			click_timer = Date.now();
			timeout = setTimeout(function() {
				send_to_server("input l "+event.getLocalPosition(map).x+" "+event.getLocalPosition(map).y);
				click_timer = 0;
			},300);
		}
		else
		{
			console.log('something');
			if (Date.now() - click_timer <= 300)
			{
				clearTimeout(timeout);
				click_timer = 0;
				send_to_server("input r "+event.getLocalPosition(map).x+" "+event.getLocalPosition(map).y);
			}
		}
	}

	stage.click = function(event) {
		send_to_server("input l "+event.getLocalPosition(map).x+" "+event.getLocalPosition(map).y);
	}

	stage.rightclick = function(event) {
		send_to_server("input r "+event.getLocalPosition(map).x+" "+event.getLocalPosition(map).y);
        music('bullet_default');
	}

	function send_to_server(message)
	{
		console.log("Send: "+message);
		socket.send(message);
	}

    function explosion(position) {
        for (var i = 0; i < 50; i++)
        {
            // create an explosion MovieClip
            var explosion = new PIXI.MovieClip(explosionTextures);
            explosion.position.x = position.x;
            explosion.position.y = position.y;
            explosion.anchor.x = 0.5;
            explosion.anchor.y = 0.5;
            explosion.rotation = Math.random() * Math.PI;
            explosion.scale.x = explosion.scale.y = 0.75 + Math.random() * 0.5;
            explosion.gotoAndPlay(Math.random() * 27);
            stage.addChild(explosion);
        }
    }

});