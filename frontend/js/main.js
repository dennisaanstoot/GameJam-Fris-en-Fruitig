$(document).ready(function()
{
	music();

	// turn off right click menu
	window.addEventListener("contextmenu", function(e) {
        e.preventDefault();
        return false;
    });

    // variables constructions / initializations
	var name, renderer, cooldown_start_time, loadingbar_width, map_width, map_height;
	var stage = new PIXI.Stage(0xff0000); // has to be in main scope for callbacks to work
	var map = new PIXI.DisplayObjectContainer(); // see above
	var waiting_screen = new PIXI.Text("Waiting for other players",{font: 'bold 36px Georgia', fill: 'white'}); // see above
	var game_width = 800;
	var game_height = 600;
	var shoot_enabled = false;
	var socket = new WebSocket('ws://130.89.231.61:9000','fris_en_fruitig');
	var player_size = 20;
	var tree_size = 40;
	var bullet_size = 10;

    var player_color = 'ffffff'

	// textures
	var player_texture = PIXI.Texture.fromImage("sprites/soldier.png");
	var tree_texture = PIXI.Texture.fromImage("sprites/tree.png");
	var bullet_texture = PIXI.Texture.fromImage("sprites/bullet.png");

	// object arrays
	var player_array = [];
	var bullet_array = [];
	
	// Write down the game name
	var game_name = new PIXI.Text("Fris en dodelijk",{font: 'bold 36px Georgia', fill: 'white'});
	game_name.anchor.x = game_name.anchor.y = 0.5;
	game_name.position.x = game_width/2;
	game_name.position.y = 15;
	stage.addChild(game_name);


	// set game screen to full size (also to enable mobile)

	$("canvas").width = window.innerWidth;
	$("canvas").height = window.innerHeight;

	// attempt connecting to server with name

	$("#login-form").submit(function(event)
	{
		event.preventDefault();
		name = $("#username").val();
		send_to_server("connect "+name);
		$("#login-form").remove();
	});

	socket.onopen = function() {
		console.log("Connection established");
		$("body").css('background-color', '#0f0');
	};

	socket.onerror = function() {
		$("body").css('background-color', '#f00');
	};

	socket.onclose = function() {
		console.log("Connection closed");
		$("body").css('background-color', '#000');
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
				break;

			case "start":
				start_game(message.substr(index_keyword,message.length+1).split(" "));
				break;

			case "frame":
				draw_frame(message.substr(index_keyword,message.length+1));
				break;
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
		var player_amount = message[3];

		$("body").css('background-color', '#000');
		renderer = PIXI.autoDetectRenderer(game_width, game_height);

		var lava = PIXI.Texture.fromImage("sprites/lava_1.png");
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
		var floor = PIXI.Texture.fromImage("sprites/floor.png");
		var background = new PIXI.Sprite(floor);
		background.width = map_width;
		background.height = map_height;
		map.addChild(background);

		//fill player array
		for (var i=0;i<player_amount;i++)
		{
			var player = new PIXI.Sprite(player_texture);
			player_array.push(player);
			player.tint = 1/i*3 * player_color;
			player.anchor.x = player.anchor.y = 0.5;
			player.width = player.height = player_size;
			map.addChild(player);

			var bullet = new PIXI.Sprite(bullet_texture);
			bullet_array.push(bullet);
			bullet.anchor.x = bullet.anchor.y = 0.5;
			bullet.width = bullet.height = bullet_size;
			map.addChild(bullet);
		}

		// create loadingbar
		var loadingbar = new PIXI.Graphics(); // see above
		loadingbar.beginFill(0x1D428A);
		loadingbar.drawRect(5,map_height-5, 0, 15);
		loadingbar_width = 60;
		map.addChild(loadingbar);

		var player_amount_text = new PIXI.Text("Players: "+player_amount, {font: 'normal 12px Georgia', fill: 'white'});
		player_amount_text.anchor.x = player_amount_text.anchor.y = 1;
		player_amount_text.position.x = map_width-5;
		player_amount_text.position.y = map_height-5;
		map.addChild(player_amount_text);

		document.body.appendChild(renderer.view);
		draw();
		shoot_enabled = true;
		setTimeout(enableShoot(),3000);
	}

	function draw_frame(message) {
		var entities = JSON.parse(message);

		var player_counter = 0;
		var bullet_counter = 0;

		for (var i = 0; i < entities.length; i++)
		{
			var entity = entities[i];
			switch(entity.entity_type)
			{
				case "player":
					var player = player_array[player_counter];
					player.position.x = entity.x;
					player.position.y = entity.y;
					player.rotation = entity.angle;
					player_counter++;
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
					var bullet = bullet_array[bullet_counter];
					bullet.position.x = entity.x;
					bullet.position.y = entity.y;
					bullet.position = entity.angle;
					bullet_counter++;
					break;
			}
		}
	}
	
	function draw() {
		var now = Date.now();
		loadingbar.width = ((now-cooldown_start_time)/3000)*loadingbar_width;
		requestAnimFrame(draw);
		renderer.render(stage);
	}

	stage.tap = stage.click = function(event) {
		send_to_server("input l "+event.getLocalPosition(map).x+" "+event.getLocalPosition(map).y);
	}

	stage.rightclick = function(event) {
		if (shoot_enabled)
		{
			send_to_server("input r "+event.getLocalPosition(map).x+" "+event.getLocalPosition(map).y);
			shootCooldown();
			setTimeout(enableShoot(),3000);
		}
	}

	function shootCooldown()
	{
		cooldown_start_time = Date.now();
		loadingbar.visible = true;
		shoot_enabled = false;
	}

	function enableShoot()
	{
		shoot_enabled = true;
		loadingbar.visible = false;
	}

	function send_to_server(message)
	{
		console.log("Send: "+message);
		socket.send(message);
	}
});